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
 *********************************************************************/

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "trickpile.h"
#include "table.h"
#include "hand.h"

#include "ui.h"
#include "cards.h"

#include "../../player/player.h"
#include "../../game/game.h"
#include "../../game/game.h"
#include "../../party/rule.h"
#include "../../card/trick.h"
#include "../../misc/setting.h"

#include "../../utils/string.h"

namespace UI_GTKMM_NS {

#define TRICKPILE_TRICKS_STEP_BIG_PART \
  static_cast<int>(this->ui->cards->height() / 15.0)
#define TRICKPILE_TRICKS_STEP_SMALL_PART \
  static_cast<int>(this->ui->cards->width() / 30.0)
#define TRICKPILE_CARDS_STEP_BIG_PART \
  static_cast<int>(this->ui->cards->height() * 0)
#define TRICKPILE_CARDS_STEP_SMALL_PART \
  static_cast<int>(this->ui->cards->width() / 20.0)

  /**
   **
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
   **
   **/
  TrickPile::TrickPile(Table* const table, Player const* const player) :
    HTIN(table, player)
  {
    return ;
  } // TrickPile::TrickPile(Table* const table, Player const* const player)

  /**
   **
   ** Destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  TrickPile::~TrickPile()
  {
    return ;
  } // TrickPile::~TrickPile()

  /**
   **
   ** draws the trickpile
   **
   ** @param     update   whether to update the area
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    TrickPile::draw(bool const update)
    {
      if ((::game_status >= GAMESTATUS::GAME_INIT)
          && (::game_status <= GAMESTATUS::GAME_FINISHED)) {
        int pos_x = this->pos_x();
        int pos_y = this->pos_y();
        int width = this->width();
        int height = this->height();

        // for the specialpoints update the values
        // I hope, there cannot be more specialpoints than cards, since that
        // is not tested here.
        switch(this->player->no()) {
        case 0:
          pos_y += this->ui->cards->back().get_width();
          pos_y -= this->ui->cards->back().get_height();
          height -= this->ui->cards->back().get_width();
          height += this->ui->cards->back().get_height();
          break;
        case 1:
          width -= this->ui->cards->back().get_width();
          width += this->ui->cards->back().get_height();
          break;
        case 2:
          height -= this->ui->cards->back().get_width();
          height += this->ui->cards->back().get_height();
          break;
        case 3:
          pos_x += this->ui->cards->back().get_width();
          pos_x -= this->ui->cards->back().get_height();
          width -= this->ui->cards->back().get_width();
          width += this->ui->cards->back().get_height();
          break;
        default:
          DEBUG_ASSERTION(false,
                          "TrickPile::draw_tricks():\n"
                          "  player->no not valid: "
                          << this->player->no());
          break;
        } // switch(this->player->no())

        if (update)
          this->table().clear(Gdk::Rectangle(pos_x, pos_y, width, height));

        // draw the cards
        this->draw_tricks();
        if (this->player->game().rule()(Rule::SHOW_TRICKPILES_POINTS)
            && (::setting(Setting::SHOW_TRICKPILES_POINTS)
                || (::game_status == GAMESTATUS::GAME_FINISHED)))
          this->draw_points();

        if (update)
          this->table().update(Gdk::Rectangle(pos_x, pos_y, width, height));

      } // if (GAME_RESERVATION <= game_status < GAME_FINISHED)

      return ;
    } // void TrickPile::draw(bool const update = true)

  /**
   **
   ** draws the tricks in the trickpile
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   ** @todo	skip the cards, the human player(s) do know not to make a special points (because of further team informations)
   **
   **/
  void
    TrickPile::draw_tricks()
    {
      if (!(::game_status & GAMESTATUS::GAME))
        return ;
      if (   (::game_status == GAMESTATUS::GAME_FINISHED)
          && !this->table().in_game_review())
        return ;

      unsigned trickno = this->player->number_of_tricks();

      if (this->table().in_game_review()) {
        trickno = 0;
        for (::TrickPile::const_iterator
             t = this->player->trickpile().begin();
             t != this->player->trickpile().end();
             ++t)
          if ((*t)->no() + 1 < this->table().game_review_trickno())
            trickno += 1;
          else
            break;
      } // if (this->table().in_game_review())

      for (unsigned t = 0; t < trickno; t++) {
        HandCards cards
          = this->player->trickpile()[t]->specialpoints_cards(); 
        unsigned const specialpoints_no = cards.size();
        // ToDo: skip the cards, the human player(s) do know not to make a special points (because of further team informations)

        if (this->player->game().rule()(Rule::SHOW_TRICKS_IN_TRICKPILES)) {
          while (cards.size() < this->player->game().playerno())
            cards.push_back(HandCard());
        } else {
          cards.push_back(HandCard());
        }
        for (unsigned c = 0; c < cards.size(); ++c) {
          HandCard const& card = cards[c];
          Rotation rotation = this->pile_rotation();
          int x = (this->start_x()
                   + this->tricks_step_x() * t
                   + this->cards_step_x() * c);
          int y = (this->start_y()
                   + this->tricks_step_y() * t
                   + this->cards_step_y() * c);
          if (!card.is_empty()) {
            switch (this->position()) {
            case SOUTH:
              y += this->ui->cards->back(rotation).get_height();
              rotation = ROTATION::counterclockwise(rotation);
              y -= this->ui->cards->card(card, rotation).get_height();
              x -= this->cards_step_y() * c;
              break;
            case WEST:
              rotation = ROTATION::clockwise(rotation);
              y += this->cards_step_x() * c;
              break;
            case EAST:
              x += this->ui->cards->back(rotation).get_width();
              rotation = ROTATION::counterclockwise(rotation);
              x -= this->ui->cards->card(card, rotation).get_width();
              x -= this->cards_step_y() * c;
              break;
            case NORTH:
              x += this->ui->cards->back(rotation).get_width();
              rotation = ROTATION::clockwise(rotation);
              x -= this->ui->cards->card(card, rotation).get_width();
              y -= this->cards_step_x() * c;
              break;
            case CENTER:
              DEBUG_ASSERTION(false,
                              "TrickPile::draw_tricks()\n"
                              "  wrong position 'CENTER' "
                              << "(" << this->position() << ")");
              break;
            } // switch (this->position())
          } else { // if !(card)
            x -= (specialpoints_no * this->cards_step_x());
            y -= (specialpoints_no * this->cards_step_y());
          } // if !(card)
          this->table().pixmap->draw_pixbuf(this->table().gc,
                                            this->ui->cards->card(card,
                                                                  rotation),
                                            0, 0,
                                            x, y,
                                            -1, -1,
                                            Gdk::RGB_DITHER_NONE,
                                            0, 0);
        } // for (c < cards.size())
      } // for (t < this->player->number_of_tricks())

      return ;
    } // void TrickPile::draw_tricks()

  /**
   **
   ** draws the points in the trickpile
   **
   ** @param     update   whether to update the area (default: false)
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    TrickPile::draw_points(bool const update)
    {
      unsigned number_of_tricks = this->player->number_of_tricks();
      this->table().trickpile_points_layout->set_text(DK::Utils::String::to_string(this->player->points_in_trickpile()));

      if (::game_status == GAMESTATUS::GAME_FINISHED) {
        unsigned points = this->player->points_in_trickpile();
        if (this->table().in_game_review()) {
          number_of_tricks = 0;
          points = 0;

          for (::TrickPile::const_iterator
               t = this->player->trickpile().begin();
               t != this->player->trickpile().end();
               ++t)
            if ((*t)->no() + 1 < this->table().game_review_trickno())
              points += (*t)->points(),
                     number_of_tricks += 1;
            else
              break;
        } // if (this->table().in_game_review())

        this->table().trickpile_points_layout->set_text(DK::Utils::String::to_string(points));
      } // if (::game_status == GAMESTATUS::GAME_FINISHED)

      if (number_of_tricks == 0)
        return ;

      int number_of_cards
        = (  this->player->game().rule()(Rule::SHOW_TRICKS_IN_TRICKPILES)
           ? (this->player->game().playerno()
              - this->player->trickpile().back()->specialpoints_cards().size())
           : 1);
      if (number_of_cards < 1)
        number_of_cards = 1;

      int layout_width, layout_height;
      this->table().trickpile_points_layout->get_pixel_size(layout_width,
                                                            layout_height);
      int x = (this->start_x()
               + (this->tricks_step_x()
                  * (number_of_tricks - 1))
               + (this->cards_step_x()
                  * (number_of_cards - 1))
               + this->ui->cards->width(this->pile_rotation()) / 2
               - (layout_width / 2)
              );
      int y = (this->start_y()
               + (this->tricks_step_y()
                  * (number_of_tricks - 1))
               + (this->cards_step_y()
                  * (number_of_cards - 1))
               + this->ui->cards->height(this->pile_rotation()) / 2
               - (layout_height / 2)
              );
      Glib::RefPtr<Gdk::GC> gc = this->table().trickpile_points_gc;
      if (   (::game_status == GAMESTATUS::GAME_FINISHED)
          && !this->table().in_game_review()
         ) {
        if (   (this->player->team()
                == this->player->game().winnerteam())
            || (   (this->player->game().winnerteam()
                    == TEAM::NOTEAM)
                && (this->player->team() == TEAM::RE)) )
          gc = this->table().name_active_gc;
        else
          gc = this->table().name_gc;
        switch (this->position()) {
        case SOUTH:
          x = (this->hand().start_x()
               + this->hand().width()
               + this->hand().margin_x());
          y = (this->hand().start_y()
               + this->hand().height() / 2
               - layout_height / 2);
          break;
        case NORTH:
          x = (this->hand().start_x()
               + this->hand().width()
               + this->hand().margin_x());
          y = (this->hand().start_y()
               + this->hand().height() / 2
               - layout_height / 2);
          break;
        case WEST:
          x = (this->hand().start_x()
               + this->hand().width() / 2
               - layout_width / 2);
          y = (this->hand().start_y()
               + this->hand().height()
               + this->hand().margin_y());
          break;
        case EAST:
          x = (this->hand().start_x()
               + this->hand().width() / 2
               - layout_width / 2);
          y = (this->hand().start_y()
               - this->hand().margin_y()
               - layout_height);
          break;
        case CENTER:
          DEBUG_ASSERTION(false,
                          "Trick::draw_points()\n"
                          "  wrong position 'CENTER' "
                          << "(" << this->position() << ")");
          break;
        } // switch (this->position())
        Gdk::Rectangle r(x - layout_width / 2,
                         y - layout_height / 2,
                         2 * layout_width, 2 * layout_height);
        gc->set_clip_rectangle(r);
      } // if ()

      this->table().pixmap->draw_layout(gc,
                                        x, y,
                                        this->table().trickpile_points_layout);

      if (update)
        this->table().update(Gdk::Rectangle(x, y, layout_width, layout_height));

      return ;
    } // void TrickPile::draw_points(bool const update = false)

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    rotation of the cards in the pile
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.2
   **/
  Rotation
    TrickPile::pile_rotation() const
    {
      switch(this->position()) {
      case NORTH:
        return LEFT;
      case SOUTH:
        return RIGHT;
      case WEST:
        return UP;
      case EAST:
        return DOWN;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::pile_rotation()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return UP;
    } // Rotation TrickPile::pile_rotation() const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    starting x position of the trickpile
   ** 
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  int
    TrickPile::pos_x() const
    {
      int value = this->start_x();

      switch(this->position()) {
      case NORTH:
      case EAST:
        value -= (this->width()
                  - this->ui->cards->width(this->pile_rotation()));
        break;
      case SOUTH:
      case WEST:
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::pos_x()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(position)

      return value;
    } // int TrickPile::pos_x()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    starting y position of the trickpile
   ** 
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  int
    TrickPile::pos_y() const
    {
      int value = this->start_y();

      switch(this->position()) {
      case NORTH:
      case WEST:
        break;
      case SOUTH:
      case EAST:
        value -= (this->height()
                  - this->ui->cards->height(this->pile_rotation()));
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::pos_y()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(position)

      return value;
    } // int TrickPile::pos_y()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    width of the trickpile
   ** 
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  int
    TrickPile::width() const
    {
      int const value = (this->ui->cards->width(this->pile_rotation())
                         + ((this->player->number_of_tricks() - 1)
                            * std::abs(this->tricks_step_x()))
                         + ((this->player->game().playerno() - 1)
                            * std::abs(this->cards_step_x()))
                        );

      return value;
    } // int TrickPile::width()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    height of the trickpile
   ** 
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  int
    TrickPile::height() const
    {
      int const value = (this->ui->cards->height(this->pile_rotation())
                         + ((this->player->number_of_tricks() - 1)
                            * std::abs(this->tricks_step_y()))
                         + ((this->player->game().playerno() - 1)
                            * std::abs(this->cards_step_y()))
                        );

      return value;
    } // int TrickPile::height()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    starting x position of the trickpile
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    TrickPile::start_x() const
    {
      switch(this->position()) {
      case NORTH:
        return (this->hand().start_x()
                - this->ui->cards->width(this->pile_rotation())
                - this->hand().margin_x());
      case SOUTH:
        return (this->hand().start_x()
                + this->hand().width()
                + this->hand().margin_x());
      case WEST:
        return this->hand().start_x();
      case EAST:
        return (this->hand().start_x()
                + this->hand().width()
                - this->ui->cards->width(this->pile_rotation()));
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::start_x()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int TrickPile::start_x()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    starting y position of the trickpile
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    TrickPile::start_y() const
    {
      switch(this->position()) {
      case NORTH:
        return this->hand().start_y();
      case SOUTH:
        return (this->hand().start_y()
                + this->hand().height()
                - this->ui->cards->height(this->pile_rotation()));
      case WEST:
        return (this->hand().start_y()
                + this->hand().height()
                + this->hand().margin_y());
      case EAST:
        return (this->hand().start_y()
                - this->hand().margin_y()
                - this->ui->cards->height(this->pile_rotation()));
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::start_y()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int TrickPile::start_y()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    x step of the tricks
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    TrickPile::tricks_step_x() const
    {
      if (!this->player->game().rule()(Rule::SHOW_TRICKS_IN_TRICKPILES))
        return 0;

      switch(this->position()) {
      case NORTH:
        return -TRICKPILE_TRICKS_STEP_BIG_PART;
      case SOUTH:
        return TRICKPILE_TRICKS_STEP_BIG_PART;
      case WEST:
        return TRICKPILE_TRICKS_STEP_SMALL_PART;
      case EAST:
        return -TRICKPILE_TRICKS_STEP_SMALL_PART;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::tricks_step_x()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int TrickPile::tricks_step_x()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    y step of the tricks
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    TrickPile::tricks_step_y() const
    {
      if (!this->player->game().rule()(Rule::SHOW_TRICKS_IN_TRICKPILES))
        return 0;

      switch(this->position()) {
      case NORTH:
        return TRICKPILE_TRICKS_STEP_SMALL_PART;
      case SOUTH:
        return -TRICKPILE_TRICKS_STEP_SMALL_PART;
      case WEST:
        return TRICKPILE_TRICKS_STEP_BIG_PART;
      case EAST:
        return -TRICKPILE_TRICKS_STEP_BIG_PART;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::tricks_step_y()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int TrickPile::tricks_step_y()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    x step of the cards
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    TrickPile::cards_step_x() const
    {
      switch(this->position()) {
      case NORTH:
        return -TRICKPILE_CARDS_STEP_BIG_PART;
      case SOUTH:
        return TRICKPILE_CARDS_STEP_BIG_PART;
      case WEST:
        return TRICKPILE_CARDS_STEP_SMALL_PART;
      case EAST:
        return -TRICKPILE_CARDS_STEP_SMALL_PART;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::cards_step_x()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int TrickPile::cards_step_x()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    y step of the cards
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    TrickPile::cards_step_y() const
    {
      switch(this->position()) {
      case NORTH:
        return TRICKPILE_CARDS_STEP_SMALL_PART;
      case SOUTH:
        return -TRICKPILE_CARDS_STEP_SMALL_PART;
      case WEST:
        return TRICKPILE_CARDS_STEP_BIG_PART;
      case EAST:
        return -TRICKPILE_CARDS_STEP_BIG_PART;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "TrickPile::cards_step_y()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int TrickPile::cards_step_y()

  /**
   **
   ** -> result
   **
   ** @param     x   x position
   ** @param     y   y position
   **
   ** @return    the pointer to the trick under the mouse curser
   **		NULL if the pointer is over no trick
   **			or the trick cannot be shown (-> rules)
   ** 
   ** @version   0.6.6
   **
   ** @author    Diether Knof
   **
   ** @todo	transparency
   **
   **/
  ::Trick const*
    TrickPile::over_trick(int const x, int const y) const
    {
      if (::in_running_game()) {
        ::TrickPile const& trickpile = this->player->trickpile();

        for (int t = trickpile.size() - 1; t >= 0; --t) {
          if (trickpile[t]->no()
              + this->player->game().rule()(Rule::SHOW_TRICKS_NUMBER)
              < this->player->game().real_trick_current_no())
            break;

          int const c = this->player->game().playerno() - 1;

          int const step_x = ((this->cards_step_x() > 0)
                              ? this->cards_step_x()
                              : -this->cards_step_x());
          int const step_y = ((this->cards_step_y() > 0)
                              ? this->cards_step_y()
                              : -this->cards_step_y());
          int const pos_x = (this->start_x()
                             + t * this->tricks_step_x()
                             + c * ((this->cards_step_x() > 0)
                                    ? 0
                                    : -step_x));

          int const pos_y = (this->start_y()
                             + t * this->tricks_step_y()
                             + c * ((this->cards_step_y() > 0)
                                    ? 0
                                    : -step_y));

          if ( (x >= pos_x)
              && (x < (pos_x
                       + step_x * c
                       + this->ui->cards->back(this->pile_rotation()
                                              ).get_width()))
              && (y >= pos_y)
              && (y < (pos_y
                       + step_y * c
                       + this->ui->cards->back(this->pile_rotation()
                                              ).get_height()))
             ) {
            return trickpile[t];
          }
        } // if (this->player has made the last trick)
      } // if (::in_running_game())

      return NULL;
    } // ::Trick const* TrickPile::over_trick(int const x, int const y)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
