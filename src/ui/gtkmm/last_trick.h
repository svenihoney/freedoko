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
 **********************************************************************/

#ifdef USE_UI_GTKMM

#ifndef GTKMM_LAST_TRICK_HEADER
#define GTKMM_LAST_TRICK_HEADER

#include "base.h"
#include "widgets/sticky_dialog.h"

class Trick;
class Player;

namespace Gtk {
  class Button;
  class VBox;
}; // namespace Gtk

namespace UI_GTKMM_NS {
  class Table;
  class TrickSummary;

  /**
   **
   ** @brief	the last trick window
   **
   ** @author	Diether Knof
   **
   **/
  class LastTrick : public Base, public Gtk::StickyDialog {
    public:
      LastTrick(Table* const table);
      ~LastTrick();

      void show_last_trick(::Trick const& last_trick);
      void close();

      unsigned trickno() const;
      void set_trickno(unsigned const trickno);

      void current_trick_closed();

      void name_changed(::Player const& player);

      bool key_press(GdkEventKey* key);


    private:
      void init();

      void previous_trick();
      void following_trick();

      void on_hide();

    private:
      ::Trick const* last_trick;

      Gtk::HBox* navigation_container;
      Gtk::Button* previous_trick_button;
      Gtk::Button* following_trick_button;
      
      TrickSummary* trick_summary;

    private: // unused
      LastTrick();
      LastTrick(LastTrick const&);
      LastTrick& operator=(LastTrick const&);
  }; // class LastTrick : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_LAST_TRICK_HEADER

#endif // #ifdef USE_UI_GTKMM
