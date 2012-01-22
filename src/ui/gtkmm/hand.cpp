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

#ifdef USE_UI_GTKMM

#include "hand.h"
#include "table.h"
#include "card_suggestion.h"
#include "ui.h"

#include "cards.h"
#include "icongroup.h"

#include "../../game/game.h"
#include "../../party/rule.h"
#include "../../player/player.h"
#include "../../player/human/human.h"
#include "../../player/ai/cards_information.of_player.h"
#include "../../card/trick.h"
#include "../../misc/setting.h"

namespace UI_GTKMM_NS {

  /**
   ** Constructor
   **
   ** @param     table   the table
   ** @param     player   the player
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Hand::Hand(Table* const table, Player const* const player) :
    HTIN(table, player)
  {
    return ;
  } // Hand::Hand(Table* const table, Player const* const player)

  /**
   ** Destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Hand::~Hand()
  {
    return ;
  } // Hand::~Hand()

  /**
   ** draws the hand
   **
   ** @param     update   whether to update the area
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  void
    Hand::draw(bool const update)
    {
      if (!(   (::game_status >= GAMESTATUS::GAME_INIT)
          && (::game_status <= GAMESTATUS::GAME_FINISHED)) )
        return ;

      if (   (::game_status == GAMESTATUS::GAME_FINISHED)
          && !this->table().in_game_review()) {
        this->draw_tricks(update);
        return ;
      }

      int pos_x = this->pos_x();
      int pos_y = this->pos_y();
      int width = this->width();
      int height = this->height();
      Game const& game = this->player->game();

      // emphasize suggested card
      if (   this->table().card_suggestion_
          && (this->player->type() == Player::HUMAN)) {
        switch (this->position()) {
        case SOUTH:
          pos_y -= (this->margin_y() * 2 / 3);
          height += (this->margin_y() * 2 / 3);
          break;
        case NORTH:
          height += (this->margin_y() * 2 / 3);
          break;
        case WEST:
          width += (this->margin_x() * 2 / 3);
          break;
        case EAST:
          pos_x -= (this->margin_x() * 2 / 3);
          width += (this->margin_x() * 2 / 3);
          break;
        case CENTER:
          DEBUG_ASSERTION(false,
                          "Hand::update()\n"
                          "  wrong position 'CENTER' "
                          << "(" << this->position() << ")");
          break;
        } // switch (this->position())
      } // if (this->table().card_suggestion_)

      if (update)
        this->table().clear(Gdk::Rectangle(pos_x, pos_y, width, height));

      bool const show = (   ( 
#if defined(RELEASE) && !defined(DKNOF)
                             (this->player->type() == Player::HUMAN)
#else
                             // show also ai information hands of the human
                             //&& (   (game.player_current() == *this->player) ||
                                 (this->position() == SOUTH)
#endif
                            )
                         || (::setting(Setting::SHOW_ALL_HANDS)
                             && game.rule()(Rule::SHOW_ALL_HANDS))
                         || (::game_status == GAMESTATUS::GAME_FINISHED)
                         || (   (::game_status
                                 == GAMESTATUS::GAME_REDISTRIBUTE)
                             && (*this->player == game.soloplayer()) )
                        );

      // the ai to show the cards information of
      Ai const* const ai
        = (  (game.humanno() == 1)
#if !defined(RELEASE) && defined(DKNOF)
           && false
#endif
           ? dynamic_cast<Ai const*>(game.human_player())
           : dynamic_cast<Ai const*>(this->hand(SOUTH).player));
      bool const show_ai_information_hand
        = (   !show
           && (::game_status >= GAMESTATUS::GAME_PLAY)
           && ::setting(Setting::SHOW_AI_INFORMATION_HANDS)
           && (ai != NULL));

      // the hand to draw
      ::Hand* const hand(!show_ai_information_hand
                         ? NULL
                         : new ::Hand(ai->handofplayer(*this->player)));
      SortedHand const sorted_hand(!show_ai_information_hand
                                   ? this->player->sorted_hand()
                                   : SortedHand(*hand) );
      // for showing the 'must have' cards
      Card last_card;
      unsigned last_card_no = 0;

      // draw the cards
      for (unsigned c = 0; c < sorted_hand.cardsnumber_all(); c++) {
        if (this->table().in_game_review())
          if (sorted_hand.played_trick(c) < this->table().game_review_trickno())
            continue;

        if ( (show || show_ai_information_hand
              ? !sorted_hand.played(c)
              : !sorted_hand.hand().played(c) )
            || (::game_status == GAMESTATUS::GAME_FINISHED) ) {

          Glib::RefPtr<Gdk::Pixbuf>
            card = (show || show_ai_information_hand
                    ? this->ui->cards->card(sorted_hand.card_all(c),
                                            this->rotation())
                    : this->ui->cards->back(this->rotation())
                   );
          int pos_x = (this->start_x() +
                       static_cast<int>(this->cards_step_x(sorted_hand.cardsnumber_all()) * c));
          int pos_y = (this->start_y() +
                       static_cast<int>(this->cards_step_y(sorted_hand.cardsnumber_all()) * c));
          if (last_card != sorted_hand.card_all(c)) {
            last_card_no = 0;
            last_card = sorted_hand.card_all(c);
          }
          if (show_ai_information_hand
              && !(last_card_no < ai->cards_information().of_player(*this->player).must_have(sorted_hand.card_all(c))) ) {
            Glib::RefPtr<Gdk::Pixbuf> copy = card->copy();
            card->saturate_and_pixelate(copy, 5, true);
            card = copy;
          } else {
            last_card_no += 1;
          }

          // emphasize the valid cards
          if (   ::setting(Setting::EMPHASIZE_VALID_CARDS)
              && game.rule()(Rule::SHOW_IF_VALID)
              && (::game_status == GAMESTATUS::GAME_PLAY)
              && (game.player_current() == *this->player)
              && (this->player->type() == Player::HUMAN)) {
            if (!game.trick_current().isvalid(sorted_hand.card_all(c),
                                              sorted_hand.hand())) {
              Glib::RefPtr<Gdk::Pixbuf> copy = card->copy();
              card->saturate_and_pixelate(copy, 0, true);
              card = copy;
            } // if (!card valid)
          } // if (emphasize valid cards)
          if (   this->table().card_suggestion_
              && (this->player == &game.player_current())
              && (c == sorted_hand.requested_position_all()) ) {
            switch (this->position()) {
            case NORTH:
              pos_y += (this->margin_y() * 2 / 3);
              break;
            case SOUTH:
              pos_y -= (this->margin_y() * 2 / 3);
              break;
            case WEST:
              pos_x += (this->margin_x() * 2 / 3);
              break;
            case EAST:
              pos_x -= (this->margin_x() * 2 / 3);
              break;
            case CENTER:
              DEBUG_ASSERTION(false,
                              "Hand::draw(update)\n"
                              "  wrong position 'CENTER' "
                              << "(" << this->position() << ")");
              break;
            } // switch (this->position())
          } // if (suggested card)

          this->table().pixmap->draw_pixbuf(this->table().gc,
                                            card,
                                            0, 0,
                                            pos_x, pos_y,
                                            -1, -1,
                                            Gdk::RGB_DITHER_NONE,
                                            0, 0);
        } // if (!sorted_hand.played(c))
      } // for (c < sorted_hand.cardsnumber_all()

      if (show_ai_information_hand)
        delete hand;
      if (update)
        this->table().update(Gdk::Rectangle(pos_x, pos_y, width, height));

      return ;
    } // void Hand::draw(bool update = true)

  /**
   ** draw the tricks the player has got
   **
   ** @param     update   whether to update the area
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  void
    Hand::draw_tricks(bool const update)
    {
      if (!(   (::game_status >= GAMESTATUS::GAME_INIT)
            && (::game_status <= GAMESTATUS::GAME_FINISHED)) )
        return ;

      int pos_x = this->pos_x();
      int pos_y = this->pos_y();
      int width = this->width();
      int height = this->height();
      Game const& game = this->player->game();
      ::TrickPile const& trickpile = this->player->trickpile();

      // gap between two tricks
      int const trick_gap = 1;
      // number of gaps
      unsigned const gapsno
        = (trickpile.size() * (game.playerno() + trick_gap) - trick_gap - 1);
      switch (this->position()) {
      case SOUTH:
        pos_y -= (this->margin_y() * 2 / 3);
        height += (this->margin_y() * 2 / 3);
        break;
      case NORTH:
        height += (this->margin_y() * 2 / 3);
        break;
      case WEST:
        width += (this->margin_x() * 2 / 3);
        break;
      case EAST:
        pos_x -= (this->margin_x() * 2 / 3);
        width += (this->margin_x() * 2 / 3);
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::update()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch (this->position())

      if (update)
        this->table().clear(Gdk::Rectangle(pos_x, pos_y, width, height));

      int step_x = ((   trickpile.empty()
                     || (this->position() == WEST)
                     || (this->position() == EAST))
                    ? 0
                    : min(this->ui->cards->width() / 4,
                          (width - this->ui->cards->width()) / gapsno)
                   );
      int step_y = ((   trickpile.empty()
                     || (this->position() == SOUTH)
                     || (this->position() == NORTH))
                    ? 0
                    : min(this->ui->cards->width() / 4,
                          (height - this->ui->cards->width()) / gapsno)
                   );
      for (unsigned t = 0; t < trickpile.size(); ++t) {
        ::Trick const& trick = *trickpile[t];
        for (unsigned c = 0; c < trick.actcardno(); ++c) {
          int pos_x
            = ( (   (this->position() == WEST)
                 || (this->position() == EAST))
               ? this->start_x()
               : static_cast<int>(this->start_x()
                                  + (width - this->ui->cards->width()
                                     - step_x * gapsno) / 2)
              ) + step_x * (t * (game.playerno() + trick_gap) + c);
          int pos_y
            = ( (   (this->position() == SOUTH)
                 || (this->position() == NORTH))
               ? this->start_y()
               : static_cast<int>(this->start_y()
                                  + (height - this->ui->cards->width()
                                     - step_y * gapsno) / 2)
              ) + step_y * (t * (game.playerno() + trick_gap) + c);
          if (this->position() == EAST) {
            pos_y = (static_cast<int>(this->start_y()
                                      + (height - this->ui->cards->width()
                                         - step_y * gapsno) / 2)
                    ) + step_y * (gapsno + 1
                                  - (t * (game.playerno() + trick_gap) + c));
          }
          if (trick.is_specialpoints_card(c)) {
            switch (this->position()) {
            case NORTH:
              pos_y += (this->margin_y() * 2 / 3);
              break;
            case SOUTH:
              pos_y -= (this->margin_y() * 2 / 3);
              break;
            case WEST:
              pos_x += (this->margin_x() * 2 / 3);
              break;
            case EAST:
              pos_x -= (this->margin_x() * 2 / 3);
              break;
            case CENTER:
              DEBUG_ASSERTION(false,
                              "Hand::draw(update)\n"
                              "  wrong position 'CENTER' "
                              << "(" << this->position() << ")");
              break;
            } // switch (this->position())
          } // if (trick.is_specialpoints_card(c))

          Glib::RefPtr<Gdk::Pixbuf>
            card = this->ui->cards->card(trick.card(c),
                                         this->rotation());
          if (c != trick.winnercardno()) {
            Glib::RefPtr<Gdk::Pixbuf> copy = card->copy();
            card->saturate_and_pixelate(copy, 0, true);
            card = copy;
          } // if (c != trick.winnercardno())

          this->table().pixmap->draw_pixbuf(this->table().gc,
                                            card,
                                            0, 0,
                                            pos_x, pos_y,
                                            -1, -1,
                                            Gdk::RGB_DITHER_NONE,
                                            0, 0);
        } // for (c < trick.actcardno())
      } // for (t < trickpile.size())
      if (update)
        this->table().update(Gdk::Rectangle(pos_x, pos_y, width, height));

      return ;
    } // void Hand::draw_tricks(bool update = true)

  /**
   ** -> result
   **
   ** @param     position   Position of the hand
   **
   ** @return    upper left x position of the hand at 'position'
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.6.1
   **/
  int
    Hand::pos_x(Position const position) const
    {
      int value = 0; // return value

      switch(position) {
      case NORTH:
      case SOUTH:
        value = (this->table().border_x()
                 + this->width(WEST)
                 + this->margin_x(WEST)
                 + this->icongroup().width(WEST)
                 + this->icongroup().margin_x(WEST));
        break;
      case WEST:
        value = this->table().border_x();
        break;
      case EAST:
        value = (this->table().width() - this->table().border_x()
                 - this->width());
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::pos_x(position)\n"
                        "  wrong position 'CENTER' "
                        << "(" << position << ")");
        break;
      } // switch(position)

      return value;
    } // int Hand::pos_x(Position position)

  /**
   ** -> result
   **
   ** @param     position   Position of the hand
   **
   ** @return    upper left y position of the hand at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::pos_y(Position const position) const
    {
      int value = 0; // return value

      switch(position) {
      case NORTH:
        value = this->table().border_y();
        break;
      case SOUTH:
        value = (this->table().height()
                 - this->table().border_y()
                 - this->height());
        break;
      case WEST:
      case EAST:
        value = (this->table().border_y()
                 + this->height(NORTH)
                 + this->margin_y(NORTH)
                 + this->icongroup().height(NORTH)
                 + this->icongroup().margin_y(NORTH));
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::pos_y(position)\n"
                        "  wrong position 'CENTER' "
                        << "(" << position << ")");
        break;
      } // switch(position)

      return value;
    } // int Hand::pos_y(Position position)

  /**
   ** -> result
   **
   ** @param     position   Position of the hand
   **
   ** @return    width of the hand at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::width(Position const position) const
    {
      int value = 0; // return value

      switch(position) {
      case NORTH:
      case SOUTH:
        value = (this->table().width()
                 - this->table().border_x()
                 - this->width(WEST)
                 - this->margin_x(WEST)
                 - this->icongroup().width(WEST)
                 - this->icongroup().margin_x(WEST)
                 - this->table().border_x()
                 - this->width(EAST)
                 - this->margin_x(EAST)
                 - this->icongroup().width(EAST)
                 - this->icongroup().margin_x(EAST));

        if (value
            < static_cast<int>(this->ui->cards->width(this->rotation(position))))
          value = this->ui->cards->width(this->rotation(position));

        break;
      case WEST:
      case EAST:
        value = this->ui->cards->width(this->rotation(position));
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::width(position)\n"
                        "  wrong position 'CENTER' "
                        << "(" << position << ")");
        break;
      } // switch(position)

      return value;
    } // int Hand::width(Position position)

  /**
   ** -> result
   **
   ** @param     position   Position of the hand
   **
   ** @return    height of the hand at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::height(Position const position) const
    {
      int value = 0; // return value

      switch(position) {
      case NORTH:
      case SOUTH:
        value = this->ui->cards->height(this->rotation(position));
        break;
      case WEST:
      case EAST:
        value = (this->table().height()
                 - this->table().border_y()
                 - this->height(NORTH)
                 - this->margin_y(NORTH)
                 - this->icongroup().height(NORTH)
                 - this->icongroup().margin_y(NORTH)
                 - this->table().border_y()
                 - this->height(SOUTH)
                 - this->margin_y(SOUTH)
                 - this->icongroup().height(SOUTH)
                 - this->icongroup().margin_y(SOUTH));

        if (value
            < static_cast<int>(this->ui->cards->height(this->rotation(position))))
          value = this->ui->cards->height(this->rotation(position));

        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::height(position)\n"
                        "  wrong position 'CENTER' "
                        << "(" << position << ")");
        break;
      } // switch(position)

      return value;
    } // int Hand::height(Position position)

  /**
   ** -> result
   **
   ** @param     position   Position of the hand
   **
   ** @return    starting x position of the hand at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::start_x(Position const position) const
    {
      return this->pos_x(position);
    } // int Hand::start_x(Position const position)

  /**
   ** -> result
   **
   ** @param     position   Position of the hand
   **
   ** @return    starting y position of the hand at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::start_y(Position const position) const
    {
      return this->pos_y(position);
    } // int Hand::start_y(Position const position)

  /**
   ** -> result
   **
   ** @param     position   Position of the hand
   **
   ** @return    the x margin at 'position
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::margin_x(Position const position) const
    {
      int value = 0;

      switch(position) {
      case NORTH:
      case SOUTH:
        value = (this->ui->cards->height() / 10);
        break;
      case WEST:
      case EAST:
        value = (this->ui->cards->height() / 10);
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::margin_x(position)\n"
                        "  wrong position 'CENTER' "
                        << "(" << position << ")");
        break;
      } // switch(position)

      return value;
    } // int Hand::margin_x(Position position)

  /**
   ** -> result
   **
   ** @param     position   Position of the hand
   **
   ** @return    the y margin at 'position
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::margin_y(Position const position) const
    {
      int value = 0;

      switch(position) {
      case NORTH:
      case SOUTH:
        value = (this->ui->cards->height() / 10);
        break;
      case WEST:
      case EAST:
        value = (this->ui->cards->height() / 10);
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::margin_y(position)\n"
                        "  wrong position 'CENTER' "
                        << "(" << position << ")");
        break;
      } // switch(position)

      return value;
    } // int Hand::margin_y(Position position)

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    upper left x position of the hand
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::pos_x() const
    {
      return this->pos_x(this->position());
    } // int Hand::pos_x()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    upper left y position of the hand
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::pos_y() const
    {
      return this->pos_y(this->position());
    } // int Hand::pos_y()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    width of the hand
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::width() const
    {
      return this->width(this->position());
    } // int Hand::width()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    height of the hand
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::height() const
    {
      return this->height(this->position());
    } // int Hand::height()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    starting x position of the hand
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::start_x() const
    {
      return this->start_x(this->position());
    } // int Hand::start_x()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    starting y position of the hand
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::start_y() const
    {
      return this->start_y(this->position());
    } // int Hand::start_y()

  /**
   ** -> result
   **
   ** @param     cards_no   number of cards (all cards)
   **
   ** @return    x step of the cards
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.6.1
   **/
  double
    Hand::cards_step_x(unsigned const cards_no) const
    {
      if (cards_no == 0)
        return 0;

      double value = 0;

      switch(this->position()) {
      case NORTH:
      case SOUTH:
        value = ((this->width(this->position())
                  - this->ui->cards->width(this->rotation(this->position())))
                 / static_cast<double>(cards_no - 1));
        break;
      case WEST:
      case EAST:
        value = 0;
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::cards_step_x(cards_no)\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return value;
    } // double Hand::cards_step_x(unsigned cards_no)

  /**
   ** -> result
   **
   ** @param     cards_no   number of cards (all cards)
   **
   ** @return    y step of the cards
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  double
    Hand::cards_step_y(unsigned const cards_no) const
    {
      if (cards_no <= 1)
        return 0;

      double value = 0;

      switch(this->position()) {
      case NORTH:
      case SOUTH:
        value = 0;
        break;
      case WEST:
      case EAST:
        value = ((this->height(this->position())
                  - this->ui->cards->height(this->rotation(this->position())))
                 / static_cast<double>(cards_no - 1));
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Hand::cards_step_y(cards_no)\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return value;
    } // double Hand::cards_step_y(unsigned cards_no)

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the horizontal margin of the hand (to the center of the table)
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::margin_x() const
    {
      return this->margin_x(this->position());
    } // int Hand::margin_x()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the vertical margin of the hand (to the center of the table)
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Hand::margin_y() const
    {
      return this->margin_y(this->position());
    } // int Hand::margin_y()

  /**
   ** -> result
   **
   ** @param     x   x position
   ** @param     y   y position
   **
   ** @return    the number of the card at the given position,
   **		UINT_MAX, if there is no card
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   ** @todo	check for transparency
   **/
  unsigned
    Hand::cardno_at_position(int const x, int const y) const
    {
      // note: the position of the hand is complex to calculate
      // (see Hand::draw()), so since it does not take much time to get
      // through all cards I just check all cards.
      for (unsigned cb = this->player->sorted_hand().cardsnumber_all();
           cb > 0;
           cb--) {
        unsigned const c = cb - 1;
        if (!this->player->sorted_hand().played(c)) {
          int pos_x = (this->start_x() +
                       static_cast<int>(this->cards_step_x(this->player->sorted_hand().cardsnumber_all()) * c));
          int pos_y = (this->start_y() +
                       static_cast<int>(this->cards_step_y(this->player->sorted_hand().cardsnumber_all()) * c));

          // emphasize suggested card
          if (   this->table().card_suggestion_
              && (this->player == &this->player->game().player_current())
              && (c == this->player->sorted_hand().requested_position()) ) {
            switch (this->position()) {
            case NORTH:
              pos_y += (this->margin_y() * 2 / 3);
              break;
            case SOUTH:
              pos_y -= (this->margin_y() * 2 / 3);
              break;
            case WEST:
              pos_x += (this->margin_x() * 2 / 3);
              break;
            case EAST:
              pos_x -= (this->margin_x() * 2 / 3);
              break;
            case CENTER:
              DEBUG_ASSERTION(false,
                              "Hand::cardno_at_position(x, y)\n"
                              "  wrong position 'CENTER' "
                              << "(" << this->position() << ")");
              break;
            } // switch (this->position())
          } // if (suggested card

          if ((x >= pos_x)
              && (x < (pos_x
                       + static_cast<int>(this->ui->cards->width(this->rotation()))))
              && (y >= pos_y)
              && (y < (pos_y
                       + static_cast<int>(this->ui->cards->height(this->rotation()))))
             )
            // the position is over the card
            // ToDo: check for transparency
            return c;
        }
      }

      return UINT_MAX;
    } // unsigned Hand::cardno_at_position(int x, int y) const

  /**
   ** -> result
   **
   ** @param     x   x position
   ** @param     y   y position
   **
   ** @return    the card at the given position
   **		Card(), if there is no card
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.6.5
   **/
  Card
    Hand::card_at_position(int const x, int const y) const
    {
      unsigned const cardno = this->cardno_at_position(x, y);

      if (cardno == UINT_MAX)
        return Card();

      return this->player->sorted_hand().card_all(cardno);
    } // Card Hand::card_at_position(int const x, int const y) const

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
