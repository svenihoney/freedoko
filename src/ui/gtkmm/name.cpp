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

#include "name.h"
#include "table.h"
#include "hand.h"
#include "icongroup.h"

#include "ui.h"
#include "cards.h"

#include "../../player/player.h"
#include "../../game/game.h"
#include "../../game/game.h"
#include "../../party/rule.h"
#include "../../card/trick.h"

#include "../../utils/string.h"

namespace UI_GTKMM_NS {

#define NAME_TRICKS_STEP_BIG_PART \
	static_cast<int>(this->ui->cards->height() / 15.0)
#define NAME_TRICKS_STEP_SMALL_PART \
	static_cast<int>(this->ui->cards->width() / 30.0)
#define NAME_CARDS_STEP_BIG_PART \
	static_cast<int>(this->ui->cards->height() * 0)
#define NAME_CARDS_STEP_SMALL_PART \
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
  Name::Name(Table* const table, Player const* const player) :
    HTIN(table, player)
  {
    return ;
  } // Name::Name(Table* const table, Player const* const player)

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
  Name::~Name()
  {
    return ;
  } // Name::~Name()

  /**
   ** draws the name
   **
   ** @param     update   whether to update the area (default: true)
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.4
   **
   ** @bug	the rectangle does not take the 'italic' font into account
   **/
  void
    Name::draw(bool const update)
    {
      Glib::RefPtr<Gdk::GC> gc;

      // test whether it is the active player
      bool is_active = false;
      switch (::game_status) {
      case GAMESTATUS::GAME_RESERVATION:
        // in the reservation show the current player
        if (this->player->game().player_current().type() == Player::HUMAN) {
          // for the human show the startplayer
          if (   GAMETYPE::is_solo(this->player->game().type())
              && (this->player->game().rule()(Rule::LUSTSOLO_PLAYER_LEADS)))
            is_active = (*this->player
                         == this->player->game().player_current());
          else
            is_active = (*this->player
                         == this->player->game().startplayer());
        } else {
          // for all other players show the player to wait for
          // (for the network game)
          is_active = (*this->player
                       == this->player->game().player_current());
        }
        break;
      case GAMESTATUS::GAME_START:
      case GAMESTATUS::GAME_INIT:
      case GAMESTATUS::GAME_POVERTY_SHIFT:
        is_active = (*this->player
                     == this->player->game().player_current());

        break;
      case GAMESTATUS::GAME_PLAY:
      case GAMESTATUS::GAME_FULL_TRICK:
      case GAMESTATUS::GAME_TRICK_TAKEN:
        is_active = ((*this->player)
                     == this->player->game().player_current());
        break;
      case GAMESTATUS::GAME_FINISHED:
        if (this->table().in_game_review())
          is_active = (   this->table().game_review_trick_visible()
                       && (*this->player
                       == this->table().game_review_trick().winnerplayer()));
        else
          is_active = (   (this->player->team()
                           == this->player->game().winnerteam())
                       || (   (this->player->game().winnerteam()
                               == TEAM::NOTEAM)
                           && (this->player->team() == TEAM::RE)) );
#ifdef POSTPONED
        if (this->gui().playfield().table().last_trick().trickno()
            == UINT_MAX) {
          is_active = (this->player().team()
                       == this->player().game().winnerteam());
        } else { // if (show a specific trick)
          is_active = (this->player()
                       == this->player().game().trick(this->gui().playfield().table().last_trick().trickno()).winnerplayer());
        } // if (show a specific trick)
#endif
        break;

      default:
        break;
      } // switch (::game_status)

      if (is_active)
        gc = this->table().name_active_gc;
      else if (   (::game_status == GAMESTATUS::GAME_RESERVATION)
               && this->ui->game().has_made_reservation(*this->player)
               && (this->ui->game().reservation(*this->player).game_type
                   != GAMETYPE::NORMAL) )
        gc = this->table().name_reservation_gc;
      else
        gc = this->table().name_gc;

      this->table().name_layout->set_text(this->player->name());
      {
        int layout_width, layout_height, layout_width_border;
        this->table().name_layout->get_pixel_size(layout_width,
                                                  layout_height);
        // bug: there seems to be a problem with italic characters
        layout_width_border = layout_height / 5;
        if (update) {
          // redrawing only the name leads to graphical errors (transparency?)
          this->table().clear(Gdk::Rectangle(this->pos_x() - layout_width_border,
                                             this->pos_y(),
                                             layout_width
                                             + 2 * layout_width_border,
                                             layout_height));
          // ToDo: remove
          switch(this->position()) {
          case NORTH:
          case SOUTH:
            this->icongroup().draw(false);
            break;
          case EAST:
            if (this->width() > this->hand().width())
              this->icongroup(SOUTH).draw(false);
            break;
          case WEST:
            break;
          case CENTER:
            DEBUG_ASSERTION(false,
                            "Name::draw(update)\n"
                            "  wrong position 'CENTER' "
                            << "(" << this->position() << ")");
            break;
          } // switch(this->position)
        } // if (update)

        if (   is_active
            && this->ui->in_progress()
            && this->ui->progress() != 1
            ) {
          {
            gc = this->table().name_reservation_gc;
            Gdk::Rectangle r(this->pos_x() - layout_width_border,
                             this->pos_y(),
                             static_cast<int>(layout_width
                                              * this->ui->progress())
                             + layout_width_border,
                             layout_height);
            gc->set_clip_rectangle(r);
          }
          this->table().pixmap->draw_layout(gc,
                                            this->pos_x(), this->pos_y(),
                                            this->table().name_layout);
          {
            gc = this->table().name_active_gc;
            Gdk::Rectangle r(this->pos_x()
                             + static_cast<int>(layout_width
                                                * this->ui->progress()),
                             this->pos_y(),
                             static_cast<int>(layout_width
                                              * (1 - this->ui->progress()))
                             + 2 * layout_width_border,
                             layout_height);
            gc->set_clip_rectangle(r);
          }
        } else { // if !(progress and active player)
          Gdk::Rectangle r(this->pos_x() - layout_width_border,
                           this->pos_y(),
                           layout_width + 2 * layout_width_border,
                           layout_height);
          gc->set_clip_rectangle(r);
        } // if !(progress and active player)
        this->table().pixmap->draw_layout(gc,
                                          this->pos_x(), this->pos_y(),
                                          this->table().name_layout);

        if (update)
          this->table().update(Gdk::Rectangle(this->pos_x()
                                              - layout_width_border,
                                              this->pos_y(),
                                              layout_width
                                              + 2 * layout_width_border,
                                              layout_height));
      }

      return ;
    } // void Name::draw(bool const update = false)

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    starting x position of the name
   ** 
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  int
    Name::pos_x() const
    {
      int value = this->hand().pos_x();

      if (this->position() == EAST) {
        if (this->width() > this->hand().width())
          value -= (this->width() - this->hand().width());
      } // if (this->position() == EAST)

      return value;
    } // int Name::pos_x() const

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    starting y position of the name
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    Name::pos_y() const
    {
      switch(this->position()) {
      case NORTH:
        return (this->hand().pos_y()
                + this->hand().height()
                + this->hand().margin_y());
      case WEST:
        return (this->hand().pos_y()
                - this->hand().margin_y()
                - this->height());
      case SOUTH:
        return (this->hand().pos_y()
                - this->hand().margin_y()
                - this->height());
      case EAST:
        return (this->hand().pos_y()
                + this->hand().height()
                + this->hand().margin_y());
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Name::pos_y()\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(position)

      return 0;
    } // int Name::pos_y() const

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    width of the name
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    Name::width() const
    {
      this->table().name_layout->set_text(this->player->name());

      int layout_width, layout_height;
      this->table().name_layout->get_pixel_size(layout_width,
                                                layout_height);

      return layout_width;
    } // int Name::width() const

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    height of the name
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    Name::height() const
    {
      this->table().name_layout->set_text(this->player->name());

      int layout_width, layout_height;
      this->table().name_layout->get_pixel_size(layout_width,
                                                layout_height);

      return layout_height;
    } // int Name::height() const

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
