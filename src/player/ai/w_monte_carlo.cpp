/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2011  by Diether Knof and Borg Enders
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
 ********************************************************************/

#include "constants.h"
#include <fstream>

#include "w_monte_carlo.h"


#include "VirtualGamesInterface.h"
#include "trickweighting.h"
#include "heuristics.h"
#include "ai.h"
#include "../ai.random/ai.h"
#include "../../party/rule.h"
#include "../../game/game.h"
#include "../../game/game_summary.h"
#include "../../game/exception.h"
#include "../../card/trick.h"
#include "../../misc/setting.h"
#include "../../ui/ui.h"

// whether to save the runtime
//#define SAVE_RUNTIME

#ifdef RELEASE
#undef SAVE_RUNTIME
#endif
/**
 ** constructor
 **
 ** @param      vgi            the vgi
 ** @param      future_limit   the future limit
 ** @param      rating_type    the rating type
 **
 ** @return     -
 **
 ** @author     Diether Knof
 **
 ** @version    0.7.3
 **/
WMonteCarlo::WMonteCarlo(VirtualGamesInterface const& vgi,
                         AiType const ai_type,
                         unsigned const& future_limit,
                         Rating::Type const& rating_type) :
  vgi_p(&vgi),
  ai_type_(ai_type),
  future_limit_(future_limit),
  rating_type_(rating_type),
  player_virt_p(),
  hand_(),
  weighting_p()
{ }

/**
 ** destructor
 **
 ** @param      -
 **
 ** @return     -
 **
 ** @author     Diether Knof
 **
 ** @version    0.4.4
 **/
WMonteCarlo::~WMonteCarlo()
{
  for (vector<Player*>::iterator player = this->player_virt_p.begin();
       player != this->player_virt_p.end();
       player++)
    delete *player;
} // WMonteCarlo::~WMonteCarlo()

/**
 ** -> result
 **
 ** @param      -
 **
 ** @return     the vgi
 **
 ** @author     Diether Knof
 **
 ** @version    0.7.3
 **/
VirtualGamesInterface const&
WMonteCarlo::vgi() const
{
  return *this->vgi_p;
} // MonteCarloInterface const& MonteCarlo::vgi() const

/**
 ** creates the game and the players.
 **
 ** @param      -
 **
 ** @return     -
 **
 ** @exception	InvalidGameException	the virtual game is invalid
 **
 ** @author     Borg Enders
 ** @author     Diether Knof
 **
 ** @version    0.5.4
 **
 ** @todo	future limit for the virtual players
 **/
void
WMonteCarlo::init()
{
  // free the memory
  for (vector<Player*>::iterator player = this->player_virt_p.begin();
       player != this->player_virt_p.end();
       player++)
    delete *player;
  this->player_virt_p.clear();

  // set the cards to the valid cards
  if (this->ai_type() == AITYPE::MONTE_CARLO_JAB_OR_SERVE) {
    if (this->vgi().game().trick_current().isempty()) {
      this->hand_ = this->vgi().hand().validcards(this->vgi().game().trick_current());
    } else {
      this->hand_ = HandCards();
      Ai const& ai = dynamic_cast<Ai const&>(this->vgi().game().player(this->vgi().no()));
      {
        HandCard const card(this->vgi().hand(),
                            Heuristics::jabbing_card(this->vgi().game().trick_current(), ai));
        if (!card.is_empty()
            && !(   this->vgi().same_team(this->vgi().game().trick_current().winnerplayer())
                 // ToDo: check whether the opposite playes behind can have a card between the winnercard and 'card'
                 && (card == this->vgi().game().trick_current().winnercard())) )
          this->hand_.add(card);
      }
      {
        HandCard const card(this->vgi().hand(),
                            Heuristics::lowest_serving_card(this->vgi().game().trick_current(), ai));
        if (!card.is_empty())
          this->hand_.add(card);
      }
      if (this->hand_.cardsnumber() == 0)
        this->hand_ = this->vgi().hand().validcards(this->vgi().game().trick_current());
    } // if (this->vgi().game().trick_current().isempty())

  } else {
    this->hand_ = this->vgi().hand().validcards(this->vgi().game().trick_current());
  }
  // set the weightings to 0
  this->weighting_p = vector<long long int>(this->hand().cardsnumber(), 0);

  // create the virtual game and the players
  // (the players have a hand independent of the hand of the vgi, so that
  //  reordering the card is no problem)
  for (vector<Player*>::const_iterator player
       = this->vgi().game().players_begin();
       player != this->vgi().game().players_end();
       player++) {

    this->player_virt_p.push_back(new AiRandom(static_cast<AiRandom const&>(this->vgi().game().player(this->vgi().no()))));

    // if the player is not the vgi, some values have to changed
    if (**player != this->vgi().game().player(this->vgi().no())) {
      unsigned const no = this->player_virt_p.size() - 1;
      AiRandom& ai_virtual = static_cast<AiRandom&>(*this->player_virt_p.back());

      // set the playernumber
      ai_virtual.set_no(no);

      // set the hand
      ai_virtual.set_hand(this->vgi().handofplayer(**player) );
      DEBUG_ASSERTION((ai_virtual.hand().cardsnumber() + 1
                       // hack (if the player has already played a card in the trick)
                       >= this->vgi().game().tricks_remaining_no()),
                      "WMonteCarlo::init():\n"
                      "  the hand of the virtual player " << ai_virtual.no()
                      << " is too small: "
                      << ai_virtual.hand().cardsnumber()
                      << " < " << this->vgi().game().tricks_remaining_no()
                      << ":\n"
                      << ai_virtual.hand());


    } // if (**player != this->vgi())
  } // for (player)

  return ;
} // void WMonteCarlo::init()


/**
 **
 ** -> result
 **
 ** @param      -
 **
 ** @return     the card, that makes the most points
 **
 ** @version    0.5.4
 **
 ** @author     Borg Enders
 **
 **/
Card
WMonteCarlo::best_card()
{
  // create the virtual game and the virtual players
  this->init();

  if (this->hand_.cardsnumber() == 1)
    return this->hand_.card(0);

  // calculate the weightings
  this->weightings_calc();


  // search the best valid card

  // the number of the best card
  HandCard best_card = this->hand().card(0);
  int best_weighting = this->weighting_p[0];
  // search a better valid card

#ifdef BENDERS
#ifndef RELEASE
  if (this->vgi().no() == 1) {
    COUT << this->vgi().no() << std::endl;
    COUT << this->hand().card(0) << " " << this->weighting_p[0] << std::endl;
  }
#endif
#endif
  for (unsigned c = 1; c < this->hand().cardsnumber(); c++) {
#ifdef BENDERS
#ifndef RELEASE
    if (this->vgi().no() == 1) {
      COUT << this->hand().card(c) << " " << this->weighting_p[c] << std::endl;
    }
#endif
#endif

    // never take a dolle as first card in the rule 'second over first'
    if (   this->vgi().game().rule()(Rule::DOLLEN_SECOND_OVER_FIRST)
        && this->vgi().game().trick_current().isstartcard()
        && this->hand().card(c).isdolle()
        && this->vgi().cards_information().remaining_others(Card::DOLLE)
       )
      continue;
    
    // never take a fox but in the last three tricks
    if (this->vgi().game().tricks_remaining_no() > 3) {
      if (this->hand().card(c).isfox())
        continue;
      if (best_card.isfox()) {
        best_card = this->hand().card(c);
        best_weighting = this->weighting_p[c];
      }
    }
    if(   (this->weighting_p[c] > best_weighting) // find best weighting
       || (   (this->weighting_p[c] == best_weighting) // there is not one best
           &&(   (   this->hand().card(c).less(best_card) // smallest card
                  && (!this->hand().card(c).isfox() || this->weighting_p[c]>0 ) // no Fox
                 )
              || (   best_card.isfox()
                  && best_weighting < 0) // replace fox
             )
          )
      )	    	           
    {
      best_card = this->hand().card(c);
      best_weighting = this->weighting_p[c];
    } // if (better card found)
  } // for (c < this->hand().cardsnumber())

  if(   (   best_card.isdolle() 
         || best_card.isswine() ) 
     && !this->hand().highest_card().is_empty()
     && this->vgi().game().trick_current().isstartcard() ) {
    best_card = this->hand().highest_card();
  }

  // optimize the result a bit
  if (   (best_card.value() >= 10)
      && best_card.istrump()
      && !best_card.is_special())
    return Heuristics::lowest_trump_card(this->vgi().hand());

  return Heuristics::optimized_card_to_play(best_card,
                                            this->vgi().game().trick_current(),
                                            dynamic_cast<Ai const&>(this->vgi().game().player(this->vgi().no())));
} // Card WMonteCarlo::best_card()


/**
 ** calculates the weightings
 ** and writes the values into the vector 'weighting_p;
 **
 ** @param      -
 **
 ** @return     -
 **
 ** @author     Borg Enders
 **
 ** @version    0.4.4
 **/
void
WMonteCarlo::weightings_calc()
{
  ::ui->update();

#ifdef SAVE_RUNTIME
  // for measuring the runtime
  ofstream ostr("MonteCarlo.time", ios::app);
  clock_t time_begin = 0;
  if (!this->vgi().game().isvirtual())
    time_begin = clock();
#endif

  // some statistics
  static unsigned counter = 0;
  static unsigned virtual_depth = 0;
  static unsigned virtual_depth_min = 0;
  static unsigned counter_last = 0;
  if (!this->vgi().game().isvirtual()) {
    counter = 0;
    virtual_depth = 0;
    virtual_depth_min = 0;
    counter_last = 0;
  }


  virtual_depth += 1;

  if (INFO_W_VIRTUAL_GAMES) {
    if (counter >= counter_last + 500) {
      counter_last = counter;
      virtual_depth_min = virtual_depth;
      cout << "MonteCarlo: depth: " << virtual_depth
        << "\t\t(counter: " << counter << ")" << endl;
    }
    if (virtual_depth < virtual_depth_min) {
      virtual_depth_min = virtual_depth;
      cout << "MonteCarlo: depth: " << virtual_depth << endl;
    }
  } // if (INFO_W_VIRTUAL_GAMES)

  HandCards const hand = this->hand();
  if (!this->vgi().game().isvirtual())
    ::ui->set_max_progress(hand.size());
  try {

    for (unsigned c = 0; c < hand.cardsnumber(); c++) {
      ::ui->ai_test_card(hand.card(c), this->vgi().no());
      unsigned trickCnt = 0; // number of calculated tricks
      Rating* rating = Rating::new_(this->rating_type());
      while ( trickCnt < this->future_limit() / hand.cardsnumber() ) {
        ::ui->ai_test_card(hand.card(c), this->vgi().no());
        ::ui->update();

        // this->hand() has only validcards
        if (INFO_W_VIRTUAL_GAMES)
          if (!this->vgi().game().isvirtual())
            cout << "MonteCarlo: calculating card "
              << this->hand().card(c)
              << "\t(" << c + 1 << " / " << this->hand().cardsnumber() << ")" << endl;

        if (INFO_W_VIRTUAL_GAMES)
          if (!this->vgi().game().isvirtual())
            cout << "  " << hand.card(c) << endl;

        // create the players
        vector<Player*> ai_virt;
        for (vector<Player*>::const_iterator ai
             = this->player_virt_p.begin();
             ai != this->player_virt_p.end();
             ai++) {
          ai_virt.push_back(new AiRandom(**ai));
        }
        // create the virtual game
        Game game_virt(this->vgi().game(), ai_virt);


        for (unsigned p = 0; p < this->vgi().game().playerno(); p++)  {
          if (this->vgi().guessedteamofplayer(game_virt.player(p)) == TEAM::UNKNOWN)
            game_virt.set_teaminfo(game_virt.player(p),
                                   TEAM::opposite(this->vgi().team()));
          else
            game_virt.set_teaminfo(game_virt.player(p),
                                   this->vgi().guessedteamofplayer(game_virt.player(p)));
        }


        { // play the card 'c'
          HandCard const card(game_virt.player_current().hand(), hand.card(c));
          game_virt.player_current().hand().playcard(card);
          game_virt.trick_current() += card;
          game_virt.teaminfo_update();
          game_virt.player_current_
            = &game_virt.player_following(game_virt.player_current());
        }


        try {
          // the other virtual players play a card
          while(!game_virt.trick_current().isfull()) {
            counter += 1;
            game_virt.nextplayer();
          }

          // first trick is full
          game_virt.evaluatetrick();

          // play tricks till the end depth

          while( (game_virt.trick_current_no() < game_virt.trickno()) 
                && game_virt.tricks_remaining_no() != 0) {
            if (INFO_W_VIRTUAL_GAMES)
              if (!this->vgi().game().isvirtual())
                cout << "  trick depth " << game_virt.trick_current_no() << endl;

            game_virt.tricks().push_back(new Trick(game_virt.player_current()));
            while(!game_virt.trick_current().isfull()) {
              counter += 1;
              game_virt.nextplayer();
            }
            game_virt.evaluatetrick();

          } // while()

          long long int weighting = 0;
          // finished with creating tricks,
          // now calc the modi for each trick
          for (unsigned t = this->vgi().game().trick_current_no();
               t < game_virt.trick_current_no();
               t++)
          {
            // add the modi to the weighting
            weighting
              += TrickWeighting::modi(this->vgi(), game_virt.trick(t),
                                      game_virt.player(this->vgi().no()).team(),
                                      game_virt.trick(t).card_of_player(game_virt.player(this->vgi().no()) ) );
            trickCnt ++;
          }


          // add some points, if the own team is in the back,
          if (game_virt.trick_current_no() < game_virt.trickno() - 1) {
            weighting
              += TrickWeighting::backhand(this->vgi(), this->hand().card(c),
                                          game_virt );      	
          } // if (game_virt.trick_current_no() < game_virt.trickno() - 1)


          // game finished, the main reason is the final result...

          if( game_virt.tricks_remaining_no() == 0 )
          {
            game_virt.finish();
            GameSummary game_summary( game_virt );

            if ( game_summary.winnerteam() == this->vgi().team() )
              weighting += game_summary.points() * 10000;
            else
              weighting -= game_summary.points() * 10000;
          }
#ifdef OUTDATED
          // 0.7.3 by rating
          this->weighting_p[c] += weighting;
#endif
          rating->add(weighting);

          ::ui->ai_card_weighting(weighting);
        } catch (InvalidGameException e) {
          // ToDo: test whether the exception is thrown

          // if the game is invalid, don't take the card
          this->weighting_p[c] = INT_MIN;
          trickCnt += game_virt.trick_current_no();
          ::ui->ai_card_weighting(INT_MIN);

          DEBUG_CAUGHT();
        } catch (...) {
          // free the memory
          for (vector<Player*>::iterator ai = ai_virt.begin();
               ai != ai_virt.end();
               ai++)
            delete *ai;

          delete rating;
          throw;
        } // try

        // free the memory
        for (vector<Player*>::iterator ai = ai_virt.begin();
             ai != ai_virt.end();
             ai++)
          delete *ai;

        if (INFO_W_VIRTUAL_GAMES)
          if (!this->vgi().game().isvirtual())
            cout << "weighting " << hand.card(c) << ": "
              << this->weighting_p[c] << "\n";

        if (!this->vgi().game().isvirtual()) {
          ::ui->add_progress(min(floor(::ui->progress()) + 1 - ::ui->progress(), 
                                 trickCnt
                                 / static_cast<double>(this->future_limit()
                                                       / hand.cardsnumber())
                                 - (::ui->progress() * hand.size()
                                    - floor(::ui->progress() * hand.size()))
                                ));
        } // if (!this->vgi().game().isvirtual())
      } // while (trickCnt)
      if (!this->vgi().game().isvirtual()) {
        ::ui->add_progress(c + 1 - ::ui->progress() * hand.size());
      } // if (!this->vgi().game().isvirtual())

      this->weighting_p[c] = rating->value();
      ::ui->ai_card_weighting(this->weighting_p[c]);
      delete rating;
    } // for (c < hand.cardsnumber())

  } catch (...) {
    if (!this->vgi().game().isvirtual())
      ::ui->finish_progress(false);
    throw;
  }
  if (!this->vgi().game().isvirtual())
    ::ui->finish_progress();

  if (INFO_W_VIRTUAL_GAMES)
    if (!this->vgi().game().isvirtual())
      cout << "MonteCarlo: counter " << counter 
        << "  (" << this->future_limit() << ")"
        << endl;

#ifdef POSTPONED
  DEBUG_ASSERTION(((this->end_depth() == 1)
                   || (counter <= this->future_limit())),
                  "WMonteCarlo::weightings_calc():\n"
                  "  counter is greater than the future limit:\n"
                  "  " << counter << " > " << this->future_limit()
                  << "\t\t(end_depth: " << this->end_depth() << ")");
#endif

#ifdef SAVE_RUNTIME
  if (!this->vgi().game().isvirtual())
    if (counter >= 2000) {
      unsigned const used_time = ((clock() - time_begin)
                                  / (CLOCKS_PER_SEC / 1000));
      ostr << setw(8) << counter
        << "\t" << setw(8) << used_time
        << "\t" << setw(8) << (used_time * 1000 / counter) 
        << endl;
    } // if (counter >= 2000)
#endif // #ifdef SAVE_RUNTIME


  virtual_depth -= 1;

  return ;
} // void WMonteCarlo::weightings_calc()
