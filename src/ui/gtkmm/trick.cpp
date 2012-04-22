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

#include "trick.h"
#include "table.h"
#include "icongroup.h"

#include "../../party/party.h"
#include "../../game/game.h"
#include "../../card/trick.h"
#include "../../player/player.h"

namespace UI_GTKMM_NS {

  /**
   **
   ** Constructor
   **
   ** @param     table   corresponding table
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  Trick::Trick(Table* const table) :
    TrickDrawing(table)
    { }

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
  Trick::~Trick()
  { }

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the table
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Table&
    Trick::table()
    {
      return static_cast<Table&>(*this->parent);
    } // Table& Trick::table()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the table
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Table const&
    Trick::table() const
    {
      return static_cast<Table&>(*this->parent);
    } // Table const& Trick::table() const

  /**
   ** draws the trick
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
    Trick::draw(bool const update)
    {
      if (   (::game_status >= GAMESTATUS::GAME_PLAY)
          && (   (::game_status < GAMESTATUS::GAME_FINISHED)
              || (this->table().in_game_review()) ) ) {
        this->set_center(Gdk::Point(this->table().width() / 2,
                                    this->table().height() / 2));

        if (this->table().in_game_review()) {
          if (this->table().game_review_trick_visible())
            this->set_trick(this->table().game_review_trick());
          else
            this->remove_trick();
        } else {
          this->set_trick(::party.game().trick_current());
        }

        if (update)
          this->table().clear(this->outline());

        // only show the trick if it is in the middle
        // (and not in the trickpile)
        if (this->trick
            && (!this->trick->isfull()
                || this->trick->winnerplayer().trickpile().empty()
                || (this->trick->winnerplayer().trickpile().back()->no()
                    < this->trick->no())
                || this->table().in_game_review()))
          this->TrickDrawing::draw(this->table().pixmap);

        if (update)
          this->table().update(this->outline());

      } // if (GAME_RESERVATION <= game_status < GAME_FINISHED)

      return ;
    } // void Trick::draw(bool const update = true)

  /**
   ** ->result
   **
   ** @param     -
   **
   ** @return    whether the mouse is over the trick
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  bool
    Trick::mouse_over_cards() const
    {
      if (!this->trick)
        return false;

      int x, y;
      this->table().get_pointer(x, y);
      return this->TrickDrawing::mouse_over_cards(x, y);
    } // bool Trick::mouse_over_cards() const

  /**
   ** ->result
   **
   ** @param     -
   **
   ** @return    whether the mouse is over the trick (sloppy version)
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  bool
    Trick::mouse_over_cards_sloppy() const
    {
      if (!this->trick)
        return false;

      Gdk::Rectangle const outline = this->TrickDrawing::outline();

      Gdk::Rectangle rec
        = Gdk::Rectangle(outline.get_x() / 2
                         + (this->table().icongroup(WEST).announcement_start_x(0)
                            +  this->table().icongroup(WEST).width()
                           ) / 2,
                         outline.get_y() / 2
                         + (this->table().icongroup(NORTH).announcement_start_y(0)
                            +  this->table().icongroup(NORTH).height()
                           ) / 2,
                         0, 0);
      rec.set_width(outline.get_width()
                    + 2 * (outline.get_x() - rec.get_x()) );
      rec.set_height(outline.get_height()
                     + 2 * (outline.get_y() - rec.get_y()) );

      int x, y;
      this->table().get_pointer(x, y);

      return (   (x >= rec.get_x())
              && (x < rec.get_x() + rec.get_width())
              && (y >= rec.get_y())
              && (y < rec.get_y() + rec.get_height()) );
    } // bool Trick::mouse_over_cards_sloppy() const

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
