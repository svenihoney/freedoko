/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2013  by Diether Knof and Borg Enders
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
 **********************************************************************/

#ifdef USE_UI_GTKMM

#ifndef GTKMM_FULL_TRICK_HEADER
#define GTKMM_FULL_TRICK_HEADER

#include "base.h"
#include "widgets/sticky_dialog.h"

class Trick;
class Player;

namespace Gtk {
  class Image;
}; // namespace Gtk

namespace UI_GTKMM_NS {
  class Table;
  class TrickSummary;

  /**
   **
   ** @brief	the game finished window
   **
   ** @author	Diether Knof
   **
   **/
  class FullTrick : public Base, public Gtk::StickyDialog {
    public:
      FullTrick(Table* const table);
      ~FullTrick();

      void show_information(::Trick const& trick);
      void close();

      void name_changed(Player const& player);

    private:
      void init();

      // on timeout close the window
      bool on_timeout();

    private:
      TrickSummary* trick_summary;
      Gtk::Image* winnercard;
      ::Trick const* trick;

    private: // unused
      FullTrick();
      FullTrick(FullTrick const&);
      FullTrick& operator=(FullTrick const&);
  }; // class FullTrick : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_FULL_TRICK_HEADER

#endif // #ifdef USE_UI_GTKMM
