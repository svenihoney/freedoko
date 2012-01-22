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

#include "ui.h"
#include "main_window.h"
#include "menu.h"
#include "table.h"
#include "trick.h"

#include "../../card/trick.h"

namespace UI_GTKMM_NS {

  /**
   **
   ** a new trick is opened
   **
   ** @param     trick   trick that is opened
   **
   ** @return    -
   **
   ** @version   0.6.3
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::trick_open(::Trick const& trick)
    {
      this->thrower.inc_depth();

      this->UI::trick_open(trick);

      this->main_window->menu->trick_open();

      this->table->draw_all();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::trick_open(::Trick const& trick)

  /**
   **
   ** the trick is full.
   ** show the 'full trick'-window or wait
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::trick_full()
    {
      this->thrower.inc_depth();

      this->main_window->menu->announcements_update();
      this->table->trick_full();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::trick_full()

  /**
   **
   ** move the trick in the pile of the winnerplayer
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.2
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::trick_move_in_pile()
    {
      this->thrower.inc_depth();

      this->table->trick_move_in_pile();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::trick_move_in_pile(trick)

  /**
   **
   ** the trick is closed
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.2
   **
   ** @author    Diether Knof
   **
   ** @todo	all
   **
   **/
  void
    UI_GTKMM::trick_close()
    {
      this->thrower.inc_depth();

      this->UI::trick_close();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::trick_close()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
