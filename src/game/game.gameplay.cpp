/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2012  by Diether Knof and Borg Enders
 *
 *   This program is free software; you can redistribute it and/or 
 *   modify it under the terms of the GNU General Public License as 
 *   published by the Free Software Foundation; either version 2 of 
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details. 
 *   You can find this license in the file 'gpl.txt'.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *   MA  02111-1307  USA
 *
 *  Contact:
 *    Diether Knof dknof@gmx.de
 *    Borg Enders  borg@borgsoft.de
 *
 *********************************************************************/

#include "constants.h"

#include "game.h"

#include "gameplay_actions.h"

#include "../party/party.h"
#include "../party/rule.h"
#include "../player/playersDb.h"
#include "../player/ai/ai.h"
#include "../player/ai/heuristics.h"
#include "../card/trick.h"
#include "../ui/ui.h"
#include "../misc/setting.h"
#include "../os/bug_report_replay.h"
extern OS_NS::BugReportReplay* bug_report_replay;

#ifdef DKNOF
#include "../card/weighted_card_list.h"
#endif

/**
 **
 ** plays a game (get reservation, make the tricks, give the result)
 **
 ** @param       -
 **
 ** @return      -
 **
 ** @version     0.6.7
 **
 ** @author      Borg Enders
 ** @author      Diether Knof
 **
 **/
void
Game::play()
{
  using namespace GAMESTATUS;

  if (::game_status != GAME_NEW)
    return ;

redistribute: // if the cards have to be redistributed
  for (vector<Player*>::const_iterator p = this->players().begin();
       p != this->players().end();
       ++p)
    (*p)->game_open(*this);

  if (::game_status == GAME_REDISTRIBUTE) {
    ::game_status = GAMESTATUS::GAME_NEW;
    this->teaminfo_reset();
    this->set_type(GAMETYPE::NORMAL);
  } else { // if !(::game_status == GAME_REDISTRIBUTE)
    if (!this->isvirtual())
      ::ui->game_open(*this);
  } // if !(::game_status == GAME_REDISTRIBUTE)

  if (::game_status != GAME_NEW) {
    if (!this->isvirtual()) {
      ::ui->game_close();
    }
    return ;
  } // if (::game_status != GAME_NEW)

  ::game_status = GAME_INIT;
  this->init();
  if (FAST_PLAY & FAST_NS::SEED_INFO)
    return ;

  if (   (::game_status != GAME_RESERVATION)
      && (::game_status != GAME_REDISTRIBUTE))
    return ;

  if (::game_status == GAME_RESERVATION)
    ::game_status = GAME_START;

  // set the startplayer
  if (   this->is_duty_solo()
      || (   this->rule()(Rule::LUSTSOLO_PLAYER_LEADS)
          && GAMETYPE::is_solo(this->type())) )
    this->player_current_ = &this->soloplayer();
  else
    this->player_current_ = const_cast<Player*>(&this->startplayer());

  if (!this->isvirtual())
    ::ui->game_start();
  for (vector<Player*>::iterator player = this->players().begin();
       player != this->players().end();
       player++)
    (*player)->game_start();

  if (::game_status == GAME_REDISTRIBUTE) {
    this->poverty_cards_.clear();
    this->poverty_shifted_ = false;

    this->set_seed(this->party().seed_next());

    if (!this->isvirtual())
      ::ui->game_redistribute();

    goto redistribute;
  } // if (::game_status == GAME_REDISTRIBUTE)

  // testing swines
  this->test_swines_from_reservations();

  if (::game_status != GAME_START)
    return ;

  // resort for not announced swines/hyperswines
  this->players_hands_sort();

  ::game_status = GAME_PLAY;

  // the reserve is needed because the players keep pointers to the tricks
  // in their trickpiles
  for (unsigned t = 0; t < this->trickno(); t++) {
    try {
      this->tricks_.push_back(new Trick(this->player_current()));
      for (vector<Player*>::iterator player = this->players().begin();
           player != this->players().end();
           player++)
        (*player)->trick_open(this->trick_current());
      if (!this->isvirtual()) {
        ::ui->trick_open(this->trick_current());
        ::ui->gameplay_action(GameplayAction::TrickOpen());
      }
      // request announcement of the players
      this->announcements_request();

      while(!this->trick_current().isfull()) {
        this->nextplayer();
        if (::game_status != GAMESTATUS::GAME_PLAY)
          throw(::game_status);
      } // while(!this->trick_current().isfull())

      ::game_status = GAMESTATUS::GAME_FULL_TRICK;
      this->evaluatetrick();
      ::game_status = GAME_PLAY;
    } catch (...) {
      if (!this->isvirtual()) {
        ::ui->trick_close();
      }
      throw;
    } // try

    if (!this->isvirtual()) {
      ::ui->trick_close();
    }
  } // for (t < this->trickno())

  return ;
} // void Game::play()


/**
 ** initializes a new game -- get reservations and set the game type
 **
 ** @param       -
 **
 ** @return      -
 **
 ** @author      Borg Enders
 ** @author      Diether Knof
 **
 ** @version     0.4.4
 **/
void
Game::init()
{
  using namespace GAMESTATUS;

  // reset some variables
  this->soloplayer_ = NULL;

  this->marriage_selector_ = MARRIAGE_SELECTOR::TEAM_SET;
  this->marriage_determination_trickno_ = 0;
  this->poverty_cards_.clear();
  this->poverty_cards_returned_.clear();
  this->poverty_shifted_ = false;
  this->announcement_
    = vector<vector<AnnouncementWithTrickno> >(this->playerno(),
                                               vector<AnnouncementWithTrickno>(1));
#ifdef USE_NETWORK
  this->announcement_over_network_
    = vector<Announcement>(this->playerno(), ANNOUNCEMENT::NOANNOUNCEMENT);
#endif
  this->swines_owner_ = NULL;
  this->first_fox_catcher_ = NULL;
  this->swines_announced_ = false;
  this->hyperswines_owner_ = NULL;
  this->hyperswines_announced_ = false;
  this->finished_ = false;

  this->player_current_ = const_cast<Player*>(&(this->startplayer()));

  this->set_type(GAMETYPE::NORMAL);

  // initialise the players
  for (vector<Player*>::iterator player = this->players().begin();
       player != this->players().end();
       player++) {
    (*player)->new_game(*this);
  }

  // distribute the cards
  this->distributecards();

  if (game_status != GAME_INIT)
    return ;

  ::game_status = GAME_RESERVATION;
  ::ui->game_cards_distributed();

  { // get the reservation
    ::ui->reservation_ask(this->player_current());
    this->set_type(GAMETYPE::NORMAL);
    ::ui->reservation_ask(this->player_current());
    this->set_type(GAMETYPE::NORMAL);
    this->player_current().reservation_get(this->party().is_duty_soli_round());
    ::ui->gameplay_action(GameplayAction::Reservation(this->player_current().no(),
                                                      this->player_current().reservation()));
    ::ui->reservation_got(this->player_current().reservation(),
                          this->player_current());
    if (game_status != GAME_RESERVATION)
      return ;

    if (this->party().is_duty_soli_round())
      DEBUG_ASSERTION(this->startplayer().is_remaining_duty_solo(this->startplayer().reservation().game_type),
                      "Game::init()\n"
                      "  Duty solo round and player still has to player a solo:\n"
                      << "  free duty solo:    " << this->startplayer().remaining_duty_free_soli() << '\n'
                      << "  free picture solo: " << this->startplayer().remaining_duty_picture_soli() << '\n'
                      << "  free color solo:   " << this->startplayer().remaining_duty_color_soli() << '\n'
                      << "  But he has selected no duty solo:\n"
                      << this->startplayer().reservation());


    if (!(   this->party().is_duty_soli_round()
          && !this->reservation(this->startplayer()).offer_duty_solo) ) {
      for (this->player_current_
           = &this->player_following(this->player_current());
           this->player_current() != this->startplayer();
           this->player_current_
           = &this->player_following(this->player_current())) {
        this->set_type(GAMETYPE::NORMAL);
        ::ui->reservation_ask(this->player_current());
        this->set_type(GAMETYPE::NORMAL);
        this->player_current().reservation_get();
        ::ui->gameplay_action(GameplayAction::Reservation(this->player_current().no(),
                                                          this->player_current().reservation()));
        ::ui->reservation_got(this->player_current().reservation(),
                              this->player_current());
        if (game_status != GAME_RESERVATION)
          return ;
      } // for (this->player_current_)
    } // if (duty solo selected)
    this->set_type(GAMETYPE::NORMAL);
  } // get the reservation

  if (FAST_PLAY & FAST_NS::SEED_INFO) {
    this->print_seed_statistics();
    return ;
  }

  // testing a solo game
  for (unsigned i = 0;
       i < this->playerno();
       i++,
       this->player_current_
       = &(this->player_following(this->player_current()))) {
    if (GAMETYPE::is_solo(this->reservation(this->player_current()).game_type)
        && this->rule()(this->reservation(this->player_current()).game_type)) {
      // the player 'p' is playing a solo
      this->set_type(this->reservation(this->player_current()).game_type);
      this->soloplayer_ = &this->player_current();

      // no other player can play a solo
      if (!(   this->party().is_duty_soli_round()
            && (this->player_current() == this->startplayer())
            && this->reservation(this->player_current()).offer_duty_solo) )
        break;
    }
  } // for (i < this->playerno())

  if (   (this->type() == GAMETYPE::NORMAL)
      || this->rule()(Rule::THROWING_BEFORE_SOLO) ) {
    // testing fox highest trump
    if (this->rule()(Rule::THROW_WHEN_FOX_HIGHEST_TRUMP)) {
      for (unsigned i = 0;
           i < this->playerno();
           i++,
           this->player_current_
           = &(this->player_following(this->player_current()))) {
        if ( (this->reservation(this->player_current()).game_type
              == GAMETYPE::FOX_HIGHEST_TRUMP)) {
          // look, whether there is a card that is higher than the fox
          unsigned c;
          for (c = 0; c < this->player_current().hand().cardsnumber(); c++)
            // check with diamond ten because of swines!
            if (   Card(Card::DIAMOND, Card::TEN).less(this->player_current().hand().card(c))
                && !this->player_current().hand().card(c).istrumpace())
              break;
          if (c == this->player_current().hand().cardsnumber()) {
            this->set_type(GAMETYPE::FOX_HIGHEST_TRUMP);
            ::game_status = GAMESTATUS::GAME_REDISTRIBUTE;
            this->soloplayer_ = &this->player_current();
            break;
          } // if (c < this->player(p).hand().cardsnumber())
        } // if (reservation)
      } // for (p < playerno())
    } // if (rule()(Rule::THROW_WHEN_FOX_HIGHEST_TRUMP))

    // testing thrown nines
    if (this->rule()(Rule::THROW_WITH_NINES)) {
      for (unsigned i = 0;
           i < this->playerno();
           i++,
           this->player_current_
           = &(this->player_following(this->player_current()))) {
        if ( ((this->reservation(this->player_current()).game_type
               == GAMETYPE::THROWN_NINES)
             )
            && (this->player_current().hand().numberofnines()
                >= this->rule()(Rule::MIN_NUMBER_OF_THROWING_NINES))) {
          this->set_type(GAMETYPE::THROWN_NINES);
          ::game_status = GAMESTATUS::GAME_REDISTRIBUTE;
          this->soloplayer_ = &this->player_current();
          break;
        } // if (reservation)
      } // for (p < playerno())
    } // if (rule()(Rule::THROW_WITH_NINES))

    // testing thrown kings
    if (this->rule()(Rule::THROW_WITH_KINGS)) {
      for (unsigned i = 0;
           i < this->playerno();
           i++,
           this->player_current_
           = &(this->player_following(this->player_current()))) {
        if ( ((this->reservation(this->player_current()).game_type
               == GAMETYPE::THROWN_KINGS)
             )
            && (this->player_current().hand().numberofkings()
                >= this->rule()(Rule::MIN_NUMBER_OF_THROWING_KINGS))) {
          this->set_type(GAMETYPE::THROWN_KINGS);
          ::game_status = GAMESTATUS::GAME_REDISTRIBUTE;
          this->soloplayer_ = &this->player_current();
          break;
        } // if (reservation)
      } // for (p < playerno())
    } // if (rule()(Rule::THROW_WITH_KINGS))

    // testing thrown nines and kings
    if (this->rule()(Rule::THROW_WITH_NINES_AND_KINGS)) {
      for (unsigned i = 0;
           i < this->playerno();
           i++,
           this->player_current_
           = &(this->player_following(this->player_current()))) {
        if ( ((this->reservation(this->player_current()).game_type
               == GAMETYPE::THROWN_NINES_AND_KINGS)
             )
            && (this->player_current().hand().numberofnines()
                + this->player_current().hand().numberofkings()
                >= this->rule()(Rule::MIN_NUMBER_OF_THROWING_NINES_AND_KINGS))) {
          this->set_type(GAMETYPE::THROWN_NINES_AND_KINGS);
          ::game_status = GAMESTATUS::GAME_REDISTRIBUTE;
          this->soloplayer_ = &this->player_current();
          break;
        } // if (reservation)
      } // for (p < playerno())
    } // if (rule()(Rule::THROW_WITH_NINES_AND_KINGS))

    // testing thrown richness
    if (this->rule()(Rule::THROW_WITH_RICHNESS)) {
      for (unsigned i = 0;
           i < this->playerno();
           i++,
           this->player_current_
           = &(this->player_following(this->player_current()))) {
        if ( ((this->reservation(this->player_current()).game_type
               == GAMETYPE::THROWN_RICHNESS)
             )
            && (this->player_current().hand().points()
                >= this->rule()(Rule::MIN_RICHNESS_FOR_THROWING))) {
          this->set_type(GAMETYPE::THROWN_RICHNESS);
          ::game_status = GAMESTATUS::GAME_REDISTRIBUTE;
          this->soloplayer_ = &this->player_current();
          break;
        } // if (reservation)
      } // for (p < playerno())
    } // if (rule()(Rule::THROW_WITH_RICHNESS))

  } // if (this->type() == GAMETYPE::NORMAL)

  if (this->type() == GAMETYPE::NORMAL) {
    // testing a marriage
    for (unsigned i = 0;
         i < this->playerno();
         i++,
         this->player_current_
         = &(this->player_following(this->player_current()))) {
      if ( ((this->reservation(this->player_current()).game_type
             == GAMETYPE::MARRIAGE)
           )
          && (this->player_current().hand().numberofclubqueens() == 2)) {
        this->set_type(GAMETYPE::MARRIAGE);
        this->soloplayer_ = &this->player_current();
        this->marriage_selector_
          = this->reservation(this->player_current()).marriage_selector;
        DEBUG_ASSERTION(this->rule()(this->marriage_selector()),
                        "Game::init():\n"
                        "  marriage selector '" << this->marriage_selector_
                        << "' is not valid.\n");
      }
    } // for (p < playerno())
  } // if (type() == GAMETYPE::NORMAL)

  if (   (this->type() == GAMETYPE::NORMAL)
      || (   this->is_solo()
          && this->rule()(Rule::THROWING_BEFORE_SOLO) )
      || (   (this->type() == GAMETYPE::MARRIAGE)
          && !this->rule()(Rule::MARRIAGE_BEFORE_POVERTY) )
     ) {
    // testing a poverty
    if (this->rule()(Rule::POVERTY)) {
      for (unsigned i = 0;
           i < this->playerno();
           i++,
           this->player_current_
           = &(this->player_following(this->player_current()))) {
        if (   (   (this->reservation(this->player_current()).game_type
                    == GAMETYPE::POVERTY)
               )
            && this->player_current().hand().has_poverty() ) {
          if (   this->rule()(Rule::THROWING_BEFORE_SOLO)
              && (this->type() != GAMETYPE::NORMAL)
              && this->rule()(Rule::POVERTY_SHIFT)
              && !(   (this->player_current().hand().numberoftrumps() > 1)
                   && this->rule()(Rule::THROW_WITH_ONE_TRUMP) )
             )
            continue;
          this->soloplayer_ = &this->player_current();
          this->set_type(GAMETYPE::POVERTY);
          this->test_swines_from_reservations();
          this->poverty_shift();
          if (::game_status != GAMESTATUS::GAME_RESERVATION)
            return ;
          break;
        } // if (reservation)
      } // for (p < playerno())
    } // if (rule()(Rule::POVERTY))
  } // if (type() == GAMETYPE::NORMAL)

  if (this->type() != GAMETYPE::MARRIAGE)
    this->marriage_selector_ = MARRIAGE_SELECTOR::TEAM_SET;

  this->check_for_silent_marriage();
  this->players_hands_sort();

  // update the team info in 'game' and in the players
  this->teaminfo_set_at_gamestart();

  return ;
} // void Game::init()

/**
 ** resets the game to the first trick
 ** Does not reset the hands of the players.
 ** This function is to be used to create a virtual copy of game that can be used to replay the game from the beginning (that is after shifting of the poverty)
 **
 ** @param       -
 **
 ** @return      -
 **
 ** @author      Diether Knof
 **
 ** @version     0.7.12
 **/
void
Game::reset_to_first_trick()
{

  this->player_current_ = &this->player(this->startplayer().no());

  this->marriage_selector_ = MARRIAGE_SELECTOR::TEAM_SET;
  this->marriage_determination_trickno_ = UINT_MAX;
  this->tricks_.clear();
  this->teaminfo_set_at_gamestart();
  this->human_teaminfo_ = this->teaminfo_;
  // ToDo: add human to info
  this->announcement_.clear();
  this->swines_owner_ = NULL;
  this->first_fox_catcher_ = NULL;
  this->swines_announced_ = false;
  this->hyperswines_owner_ = NULL;
  this->hyperswines_announced_ = false;
  this->finished_ = false;

  // gameplay 

  return ;
} // void Game::reset_to_first_trick()

/**
 ** -> result
 **
 ** @param     player   the player
 **
 ** @return    whether the player has made a reservation
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.4
 **/
bool
Game::has_made_reservation(Player const& player) const
{
  if (::game_status < GAMESTATUS::GAME_RESERVATION)
    return false;
  if (::game_status > GAMESTATUS::GAME_RESERVATION)
    return true;

  return ( (player.no()
            + this->playerno()
            - this->startplayer().no()
           ) % this->playerno()
          <  (this->player_current().no()
              + this->playerno()
              - this->startplayer().no()
             ) % this->playerno()
         );
} // bool Game::has_made_reservation(Player player) const

/**
 ** -> result
 **
 ** @param     player   the player
 **
 ** @return    the reservation of 'player'
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.4
 **/
Reservation const&
Game::reservation(Player const& player) const
{
  return player.reservation();
} // Reservation const& Game::reservation(Player const& player) const


/**
 ** make the shifting of the poverty cards
 ** distribute the cards randomly at the players
 **
 ** @param      -
 **
 ** @return     -
 **
 ** @author     Diether Knof
 **
 ** @version    0.4.5
 **/
void
Game::poverty_shift()
{
  if ((this->type() != GAMETYPE::POVERTY)
      || !this->rule()(Rule::POVERTY_SHIFT)
      || ((this->soloplayer().hand().numberoftrumps() <= 1)
          && this->rule()(Rule::THROW_WITH_ONE_TRUMP)
         )
     ) {
    this->player_current_ = &(this->soloplayer());
    ::game_status = GAMESTATUS::GAME_REDISTRIBUTE;
    return ;
  } // if (not poverty shift)

  ::game_status = GAMESTATUS::GAME_POVERTY_SHIFT;

  this->set_teaminfo(this->soloplayer(), TEAM::RE);
  this->soloplayer().set_team(TEAM::RE);

  this->test_swines_from_reservations();
  // resort for not announced swines/hyperswines
  this->players_hands_sort();

  if (!this->isvirtual())
    ::ui->poverty_shifting(this->soloplayer());

  this->poverty_cards_ = this->soloplayer().poverty_shift();

  DEBUG_ASSERTION((this->poverty_cards().player()
                   == this->soloplayer()),
                  "Game::poverty_shift():\n"
                  "  the soloplayer '" << this->soloplayer().no()
                  << "' shifts cards from player '"
                  << this->poverty_cards().player().no() << "'");

  DEBUG_ASSERTION((this->soloplayer().hand().numberoftrumps()
                   == 0),
                  "Game::poverty_shift():\n"
                  "  Poverty: the soloplayer still has trump on the hand.\n"
                  "remaining trumps: "
                  << this->soloplayer().hand().numberoftrumps() << '\n'
                  << "Hand:\n"
                  << this->soloplayer().hand()
                  << "shifted cards:\n"
                  << this->poverty_cards());

  DEBUG_ASSERTION( (this->poverty_cards().size()
                    + this->soloplayer().hand().cardsnumber()
                    == this->rule()(Rule::NUMBER_OF_TRICKS_IN_GAME)),
                  "Game::poverty_shift():\n"
                  "  wrong number of cards shifted: "
                  << this->poverty_cards().size());

#ifdef DEBUG
  if (this->rule()(Rule::POVERTY_SHIFT_ONLY_TRUMP))
    DEBUG_ASSERTION( (this->poverty_cards().numberoftrumps()
                      == this->poverty_cards().cardsnumber()),
                    "Game::poverty_shift():\n"
                    "  wrong number of cards shifted: "
                    << "trumps = " << this->poverty_cards().numberoftrumps()
                    << " != " << this->poverty_cards().size() << " = cards shifted");
  else if (!this->rule()(Rule::POVERTY_FOX_DO_NOT_COUNT)
           || this->swines_announced() )
    // normal case: shift at max 3 cards
    DEBUG_ASSERTION( (this->poverty_cards().cardsnumber()
                      == this->rule()(Rule::MAX_NUMBER_OF_POVERTY_TRUMPS)),
                    "Game::poverty_shift():\n"
                    "  wrong number of cards shifted: "
                    << "poverty cards = " << this->rule()(Rule::MAX_NUMBER_OF_POVERTY_TRUMPS)
                    << " != " << this->poverty_cards().size() << " = cards shifted");
  else if (!this->rule()(Rule::POVERTY_FOX_SHIFT_EXTRA))
    // at max 3 cards or number of trumps
    DEBUG_ASSERTION( (   (this->poverty_cards().numberoftrumps()
                          == this->poverty_cards().cardsnumber())
                      || (this->poverty_cards().cardsnumber()
                          == this->rule()(Rule::MAX_NUMBER_OF_POVERTY_TRUMPS)) ),
                    "Game::poverty_shift():\n"
                    "  wrong number of cards shifted: "
                    << "trumps or "
                    << "poverty cards = " << this->rule()(Rule::MAX_NUMBER_OF_POVERTY_TRUMPS)
                    << " != " << this->poverty_cards().size() << " = cards shifted");
  else
    // fox are shifted extra
    DEBUG_ASSERTION( (this->poverty_cards().cardsnumber()
                      - this->poverty_cards().numberoftrumpaces()
                      == this->rule()(Rule::MAX_NUMBER_OF_POVERTY_TRUMPS)),
                    "Game::poverty_shift():\n"
                    "  wrong number of cards shifted: "
                    << "poverty cards = " << this->rule()(Rule::MAX_NUMBER_OF_POVERTY_TRUMPS)
                    << " != " << this->poverty_cards().size() << " - "
                    << this->poverty_cards().numberoftrumpaces()
                    << " = cards shifted - fox");
#endif // #ifdef DEBUG

  for (vector<Player*>::iterator p = this->players().begin();
       p != this->players().end();
       p++)
    (*p)->poverty_shift(this->soloplayer(), this->poverty_cardno());

  //  this->soloplayer().hand().remove(this->poverty_cards());
  if (!this->isvirtual()) {
    ::ui->poverty_shift(this->soloplayer(), this->poverty_cardno());
    ::ui->gameplay_action(GameplayAction::PovertyShift(this->soloplayer().no(),
                                                       this->poverty_cards()));
  }

  // ask all players, whether they accept the poverty or not
  bool accept = false;
  for (this->player_current_ = &this->player_following(this->soloplayer());
       this->player_current() != this->soloplayer();
       this->player_current_
       = &(this->player_following(this->player_current()))) {
    if (!this->isvirtual()) {
      ::ui->poverty_ask(this->player_current(), this->poverty_cardno());
      if (   !(FAST_PLAY & FAST_NS::PAUSE)
          && !(   ::bug_report_replay
               && ::bug_report_replay->auto_action())) {
        if (this->player_current_->type() != Player::HUMAN)
          ::ui->sleep(::setting(Setting::CARD_PLAY_DELAY));
      }
      if (::game_status != GAMESTATUS::GAME_POVERTY_SHIFT)
        return ;
    } // if (!this->isvirtual())


    accept = this->player_current().poverty_take_accept(this->poverty_cardno());
    if (::game_status != GAMESTATUS::GAME_POVERTY_SHIFT)
      return ;

    if (accept) {
      { // test for swines and hyperswines before taking the cards
        if (this->rule()(Rule::SWINES)
            && this->rule()(Rule::SWINES_ANNOUNCEMENT_BEGIN)
            && this->reservation(this->player_current()).swines)
          this->swines_announce(this->player_current());
        if (this->rule()(Rule::HYPERSWINES)
            && this->rule()(Rule::HYPERSWINES_ANNOUNCEMENT_BEGIN)
            && this->reservation(this->player_current()).hyperswines)
          this->hyperswines_announce(this->player_current());
      } // test for swines and hyperswines before taking the cards
      ::ui->gameplay_action(GameplayAction::PovertyAccepted(this->player_current().no()));
      break;
    } else { // if !(accept)
      for (vector<Player*>::iterator p = this->players().begin();
           p != this->players().end();
           p++)
        (*p)->poverty_take_denied(this->player_current());
      if (!this->isvirtual()) {
        ::ui->poverty_take_denied(this->player_current());
        ::ui->gameplay_action(GameplayAction::PovertyDenied(this->player_current().no()));
      }
    } // if !(accept)
  } // for (this->player_current_)

  if (!accept) {
    this->player_current_ = &(this->soloplayer());

    for (vector<Player*>::iterator p = this->players().begin();
         p != this->players().end();
         p++)
      (*p)->poverty_take_denied_by_all();
    if (!this->isvirtual()) {
      ::ui->poverty_take_denied_by_all();
      ::ui->gameplay_action(GameplayAction::PovertyDeniedByAll());
    }

    this->soloplayer().sorted_hand().add(this->poverty_cards_);
    this->soloplayer().hand_sort();

    ::game_status = GAMESTATUS::GAME_REDISTRIBUTE;
  } else { // if !(!accept)
    // set the teaminfo
    this->set_teaminfo(this->player_current(), TEAM::RE);
    this->player_current().set_team(TEAM::RE);

    for (vector<Player*>::iterator p = this->players().begin();
         p != this->players().end();
         p++) {
      if (this->teaminfo(**p) == TEAM::UNKNOWN) {
        this->set_teaminfo(**p, TEAM::CONTRA);
        (*p)->set_team(TEAM::CONTRA);
      }
    } // for (p)
    this->teaminfo_set_at_gamestart();

    this->poverty_cards_returned_
      = this->player_current().poverty_cards_change(this->poverty_cards());
    if (::game_status != GAMESTATUS::GAME_POVERTY_SHIFT)
      return ;

    // check for shifted swines
    if (this->rule()(Rule::SWINES)
        && this->swines_announced()
        && (this->swines_owner() == &this->soloplayer())) {
      this->swines_announce(this->player_current());
    }
    if (this->rule()(Rule::HYPERSWINES)
        && this->hyperswines_announced()
        && (this->hyperswines_owner() == &this->soloplayer()))
      this->hyperswines_announce(this->player_current());

    DEBUG_ASSERTION((this->poverty_cards_returned().player()
                     == this->player_current()),
                    "Game::poverty_shift():\n"
                    "  the current player '" << this->player_current().no()
                    << "' shifts back cards from player '"
                    << this->poverty_cards_returned().player().no() << "'");

    DEBUG_ASSERTION((this->poverty_cards_returned().size()
                     == this->poverty_cardno()),
                    "Game::poverty_shift():\n"
                    "  number of cards returned ("
                    << this->poverty_cards_returned().size()
                    << ") is not the same as are shifted ("
                    << this->poverty_cardno() << ").");

    for (vector<Player*>::iterator p = this->players().begin();
         p != this->players().end();
         p++)
      (*p)->poverty_take_accepted(this->player_current(),
                                  this->poverty_cards_returned().size(),
                                  this->poverty_cards_returned().numberoftrumps());
    if (!this->isvirtual()) {
      ::ui->poverty_take_accepted(this->player_current(),
                                  this->poverty_cards_returned().size(),
                                  this->poverty_cards_returned().numberoftrumps());
      ::ui->gameplay_action(GameplayAction::PovertyReturned(this->player_current().no(),
                                                            this->poverty_cards_returned()));
    }


    // add the cards to the player
    this->player_current_ = &this->soloplayer();
    this->soloplayer().poverty_cards_get_back(this->poverty_cards_returned());
    if (!this->isvirtual())
      ::ui->gametype_changed();

    if (::game_status != GAMESTATUS::GAME_POVERTY_SHIFT)
      return ;

  } // if !(!accept)

  this->poverty_shifted_ = true;

  if (accept) {
    // check, that the swines announcement is correct
    if (this->swines_owner()) {
      if (this->swines_owner()->team() == TEAM::RE) {
        vector<Player*>::iterator p;
        for (p = this->players().begin();
             p != this->players().end();
             ++p) {
          if ((*p)->hand().numberoftrumpaces()
              == this->rule()(Rule::NUMBER_OF_SAME_CARDS)) {
            this->swines_announce(**p);
            break;
          }
        } // for (p \in this->players())
        if (p == this->players().end()) {
          this->swines_owner_ = NULL;
          this->swines_announced_ = false;
        }
      } // if (this->swines_owner()->team() == TEAM::RE)

      if (this->hyperswines_owner()) {
        if (this->hyperswines_owner()->team() == TEAM::RE) {
          vector<Player*>::iterator p;
          for (p = this->players().begin();
               p != this->players().end();
               ++p) {
            if ((*p)->hand().numberoftrumpaces()
                == this->rule()(Rule::NUMBER_OF_SAME_CARDS)) {
              this->hyperswines_announce(**p);
              break;
            }
          } // for (p \in this->players())
          if (p == this->players().end()) {
            this->hyperswines_owner_ = NULL;
            this->hyperswines_announced_ = false;
          }
        } // if (this->hyperswines_owner()->team() == TEAM::RE)
      } // if (this->hyperswines_owner())
    } // if (this->swines_owner())
  } // if (accept)

  if (::game_status == GAMESTATUS::GAME_POVERTY_SHIFT)
    ::game_status = GAMESTATUS::GAME_RESERVATION;

  return ;
} // void Game::poverty_shift()

/**
 ** distribute the cards randomly at the players
 **
 ** @param       -
 **
 ** @return      -
 **
 ** @author      Borg Enders
 ** @author      Diether Knof
 **
 ** @version     0.4.4
 **/
void
Game::distributecards()
{
  ::pseudo_rand_set(this->seed());
  if (!(FAST_PLAY & FAST_NS::SEED_INFO))
    cout << "seed: " << this->seed() << "\n";

  vector<Card> all_cards = this->rule().cards();
  for (unsigned i = 1; i < this->rule()(Rule::NUMBER_OF_SAME_CARDS); ++i)
    all_cards.insert(all_cards.end(),
                     this->rule().cards().begin(),
                     this->rule().cards().end());

  vector<vector<Card> > hands(this->playerno());

  while (!all_cards.empty()) {
    for (vector<vector<Card> >::iterator h = hands.begin();
         h != hands.end();
         ++h) {
      DEBUG_ASSERTION(!all_cards.empty(),
                      "Game::distributecards():\n"
                      "  valid cards cannot be distributed between the players");
      vector<Card>::iterator
        c = (all_cards.begin() + RAND(all_cards.size()));
      h->push_back(*c);
      all_cards.erase(c);
    } // for (h \in hands)
  } // while (!cards.empty())

  // create the hands for the players
  for (unsigned p = 0;
       p < this->playerno();
       p++) {
    this->players_[(p + this->startplayer().no()) % this->playerno()]
      ->set_hand(hands[p]);
  }

  return ;
} // void Game::distributecards()

/**
 ** the next player has to play a card
 **
 ** @param       -
 **
 ** @return      -
 **
 ** @author      Borg Enders
 ** @author      Diether Knof
 **
 ** @version     0.4.4
 **
 ** @bug         uses processor time, not real time
 */
void
Game::nextplayer()
{
  DEBUG_ASSERT(this->self_check());

#ifdef POSTPONED
  DEBUG_ASSERTION((this->player_current().hand().cardsnumber()
                   == this->trickno() - this->trick_current_no()),
                  "Game::nextplayer():\n"
                  "  current player has not the right number of cards on the hand\n"
                  "trick " << this->trick_current_no() << "\n"
                  << this->player_current());
#endif


  // ***ToDo DK this uses the processor time
  const clock_t time_before_playing_card = ::clock();

  HandCard const played_card = this->player_current().card_get();

  if (!(::game_status & GAMESTATUS::GAME))
    return ;

  DEBUG_ASSERTION(!played_card.is_empty(),
                  "Game::nextplayer():\n"
                  "  result of 'Player::card_get()' is an empty card.\n"
                  "  Player number: " << this->player_current().no() << "\n"
                  "  Player type:   " << this->player_current().type());
  // here always update,
  // so that the user can do something during the calculation of the ai
  ::ui->update();

  // testing, whether the card is valid.
  // if it is not, take the first valid card of the hand.
  if (!this->trick_current().isvalid(played_card)
      && !(   this->isvirtual()
           && (this->player_current().hand().cardsnumber()
               > this->player_current().cards_to_play()))
     ) {
#ifndef RELEASE
    cerr << "ai type = " << played_card.player().type() << endl;
    cerr << "heuristic = " << static_cast<Ai const&>(this->player_current()
                                                    ).lastHeuristic_
      << endl;
    cerr << *this;
#endif
#ifndef POSTPONED
    cerr << "hand:   " << this->player_current().hand() << endl;
    cerr << "current trick:\n" << this->trick_current() << endl;
    cerr << "played card: " << played_card << endl;
    cerr << "heuristic:   " << static_cast<Ai const&>(this->player_current()).lastHeuristic_ << endl;
    DEBUG_ASSERTION(false,
                    "Game::nextplayer()\n"
                    "  card \"" << played_card << "\" is not valid"
                    << endl);
#else
    // *** the opposite team has won
#endif
  }

  if (   !(FAST_PLAY & FAST_NS::PAUSE)
      && !(   ::bug_report_replay
           && ::bug_report_replay->auto_action())) {

    // wait a bit, before the card is played
    // if it is the human who has to play
    // Bug: DK
    // When the time wraps around, it is assumed, that the calculation
    // has taken no time

    if (this->player_current().type() & Player::AI) {
      if (!this->isvirtual()) {

#ifdef POSTPONED
        // ToDo: cannot use this code when using 'clock()'
        while (time_before_playing_card
               + (::setting(Setting::CARD_PLAY_DELAY)
                  * static_cast<double>(CLOCKS_PER_SEC)
                  / 1000)
               > ::clock() ) {
#endif

          ::ui->sleep(max(long(0),
                          long(::setting(Setting::CARD_PLAY_DELAY)
                               - ((::clock()
                                   - time_before_playing_card)
                                  / (static_cast<double>(CLOCKS_PER_SEC)
                                     / 1000) ))));

#ifdef POSTPONED
          // ToDo: cannot use this code when using 'clock()'
        } // while (still have to wait)
#endif
      } // if (!this->isvirtual())
    } // if (this->player_current().type() & Player::AI)
  } // if (!(FAST_PLAY & FAST_NS::PAUSE))

  this->check_for_genscher(played_card);

  // remove the card from the hand, add it to the trick
  // and give the message to the ui
  this->player_current().hand().playcard(played_card);
  this->trick_current() += played_card;
  // set the next player as current player
  if (this->trick_current().isfull())
    this->player_current_ = const_cast<Player*>(&(this->trick_current().winnerplayer()));
  else
    this->player_current_ = &this->player_following(this->player_current());

  this->teaminfo_update();
  for (vector<Player*>::iterator player = this->players().begin();
       player != this->players().end();
       player++) {
    (*player)->card_played(played_card);
  }

  this->determine_silent_marriage();
  if (this->isvirtual())
    ::ui->virtual_card_played(played_card);
  else {
    ::ui->card_played(played_card);
    Aiconfig::Heuristic heuristic = Aiconfig::NO_HEURISTIC;
    switch (played_card.player().type()) {
    case Player::HUMAN:
    case Player::AI:
      heuristic = static_cast<Ai const&>(played_card.player()).lastHeuristic_;
      break;
    case Player::UNSET:
    case Player::AI_DUMMY:
    case Player::AI_RANDOM:
      heuristic = Aiconfig::NO_HEURISTIC;
      break;
    case Player::NETWORK:
      heuristic = Aiconfig::NETWORK;
      break;
    } // switch (played_card.player().type())

    ::ui->gameplay_action(GameplayAction::CardPlayed(played_card.player().no(),
                                                     played_card,
                                                     heuristic));
  }

  // request announcement of the players
  this->announcements_request();

  return ;
} // void Game::nextplayer();

/**
 ** evaluates who is startplayer for nextcard and updates player
 **
 ** @param       -
 **
 ** @return      -
 **
 ** @author      Borg Enders
 ** @author      Diether Knof
 **
 ** @version     0.4.4
 **/
void
Game::evaluatetrick()
{
  // Winnerplayer has as result the number of the winner
  Player const& winnerplayer = this->trick_current().winnerplayer();

  // test for marriage
  this->determine_marriage();
  if (!(::game_status & GAMESTATUS::GAME))
    return ;

  // test for catcher of the first fox of swines
  if (this->rule()(Rule::SWINE_ONLY_SECOND)
      && !this->first_fox_catcher()) {
    for (vector<Player*>::const_iterator p = this->players_begin();
         p != this->players_end();
         ++p) {
      if (this->trick_current().card_of_player(**p).istrumpace()) {
        this->first_fox_catcher_ = &winnerplayer;
        (*p)->hand_sort();
        break;
      } // if (is trump ace)
    } // for (p \in this->player)
  } // if (fox in trick)

  // test whether the player has to make an announcement
  if (   (this->type() == GAMETYPE::NORMAL)
      && (this->trick_current_no() == 1)
      && (this->rule()(Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS))
      && (this->trick_current().points() >= 30)
      && (this->announcement_of_team(this->trick_current().winnerplayer().team())
          != ANNOUNCEMENT::NO0)
      && (    !this->rule()(Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS_ONLY_FIRST)
          || (   (this->announcement_of_team(TEAM::RE)
                  == ANNOUNCEMENT::NOANNOUNCEMENT)
              && (this->announcement_of_team(TEAM::CONTRA)
                  == ANNOUNCEMENT::NOANNOUNCEMENT) )
         )
      && (   !this->rule()(Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS_ONLY_RE_CONTRA)
          || (this->announcement_of_team(this->trick_current().winnerplayer().team())
              == ANNOUNCEMENT::NOANNOUNCEMENT) )
      && (   !GAMETYPE::is_marriage(this->type()) 
          || this->rule()(Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS_IN_MARRIAGE))
     ) {
    this->make_announcement(ANNOUNCEMENT::next(this->announcement_of_team(this->trick_current().winnerplayer().team())),
                            this->trick_current().winnerplayer());
  } // if (player must make an announcement)

  // tell the players and the ui, that the trick is full
  for (vector<Player*>::const_iterator player = this->players_begin();
       player != this->players_end();
       player++)
    (*player)->trick_full(this->trick_current());
  if (this->isvirtual()) {
    ::ui->virtual_trick_full();

    // request announcement of the players
    this->announcements_request();

  } else { // if !(this->isvirtual())
    ::ui->gameplay_action(GameplayAction::TrickFull(this->trick_current()));
    ::ui->trick_full();

    // request announcement of the players
    this->announcements_request();

    ::game_status = GAMESTATUS::GAME_TRICK_TAKEN;
    ::ui->gameplay_action(GameplayAction::TrickTaken());
    // this is needed for the network
    for (vector<Player*>::const_iterator player = this->players_begin();
         player != this->players_end();
         player++)
      (*player)->trick_taken();
  } // if !(this->isvirtual())
  if (!(::game_status & GAMESTATUS::GAME))
    return ;

  // move the trick in the trickpile
  this->trick_current().move_in_trickpile();

  // Startplayer for next round 
  this->player_current_ = &this->player(winnerplayer.no());

  return ;
} // void Game::evaluatetrick();

/**
 ** the game is finished
 **
 ** @param       -
 **
 ** @return      -
 **
 ** @author      Borg Enders
 ** @author      Diether Knof
 **
 ** @version     0.4.4
 **/
void
Game::finish()
{
  if (this->is_undetermined_marriage()) {
    this->marriage_determination_trickno_ = this->trick_current_no();
    this->set_type(GAMETYPE::MARRIAGE_SOLO);
    for (vector<Player*>::const_iterator player = this->players_begin();
         player != this->players_end();
         player++)
      this->teaminfo_[(*player)->no()]
        = ((**player == this->soloplayer())
           ? TEAM::RE
           : TEAM::CONTRA);
    this->marriage_selector_ = MARRIAGE_SELECTOR::TEAM_SET;
    // update the teaminfo of the players
    for (vector<Player*>::iterator player = this->players().begin();
         player != this->players().end();
         player++)
      (*player)->marriage(this->soloplayer(), this->soloplayer());
  } // if (undetermined marriage)

  this->finished_ = true;

  return ;
} // void Game::finish()

/**
 ** print seed information on stdout
 **
 ** @param       -
 **
 ** @return      -
 **
 ** @author      Diether Knof
 **
 ** @version     0.7.3
 **/
void
Game::print_seed_statistics() const
{
  // format (for each player)
  // seed
  // playerno
  // playername
  // number trumps
  // number club queens
  // number dollen
  // number queens
  // number jacks
  // number swines
  // number hyperswines
  // number fehl colors
  // number color aces
  // reservation (gametype)

  // next line must be called in 'freedoko.cpp'
  //cout << "seed;no;name;trumpno;club queens;spade queens;dollen;queens;jacks;diamond aces;hyperswines;fehl colors;color aces;reservation;hand value\n";
  for (unsigned p = 0; p < this->playerno(); ++p) {
    Player const& player = this->player(p);
    Hand const& hand = player.hand();
    cout << this->seed() << ';';
    cout << p << ';' << player.name() << ';';
    cout << hand.numberoftrumps() << ';';
    cout << hand.numberofclubqueens() << ';';
    cout << hand.numberof(Card::DOLLE) << ';';
    cout << hand.numberof(Card(Card::SPADE, Card::QUEEN)) << ';';
    cout << hand.numberof(Card::QUEEN) << ';';
    cout << hand.numberof(Card::JACK) << ';';
    cout << hand.numberof(Card(Card::DIAMOND, Card::ACE)) << ';';
    cout << hand.numberof(Card(Card::DIAMOND, Card::NINE)) << ';';
    // cout << (hand.has_hyperswines() ? "hyperswines" : "") << ';';
    cout << 3 - (  (hand.contains(Card::CLUB) ? 1 : 0)
                 + (hand.contains(Card::SPADE) ? 1 : 0)
                 + (hand.contains(Card::HEART) ? 1 : 0)) << ';';
    cout << hand.numberof(Card::ACE) - hand.numberoftrumpaces() << ';';
    cout << player.reservation().game_type << ';';
    cout << Heuristics::CalcHandValue(static_cast<Ai const&>(player),
                                      *this) << ';';
    cout << '\n';
  } // for (p)

  return ;
} // void Game::print_seed_statistics() const
