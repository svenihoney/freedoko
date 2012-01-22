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
 **********************************************************************/

#ifdef USE_UI_GTKMM

#ifndef GTKMM_PLAYERS_HEADER
#define GTKMM_PLAYERS_HEADER

#include "base.h"
#include "widgets/sticky_dialog.h"

#include "../../player/player.h"

class Player;
class Aiconfig;
#include <gtkmm/notebook.h>

namespace Gtk {
  class StockButton;
  class Table;
  class ToggleButton;
  class Entry;
}; // namespace Gtk
namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the window with all players
   **
   ** @author	Diether Knof
   **
   ** @todo	export into a window
   **
   **/
  class Players : public Base, public Gtk::StickyDialog {
    public:
      class Player;

    public:
      Players(Base* const parent);
      ~Players();

      Player& player(unsigned const playerno);

      void create_backup();
      void reset();

      void sensitivity_update();
      void update();
      void players_switched(::Player const& player_a, ::Player const& player_b);
      void player_update(::Player const& player);
      void name_update(::Player const& player);
      void voice_update(::Player const& player);
      void aiconfig_update(::Aiconfig const& aiconfig);

      bool name_changed_event(GdkEventFocus*);
      void random_name(unsigned const playerno);
      void type_change(unsigned const playerno, ::Player::Type const type);

    private:
      void init();

      Gtk::ToggleButton* add_group(string const& name,
				   Gtk::Table* table, int& current_row,
				   int const depth,
			Gtk::ToggleButton* dependent_button);
      void add_aiconfig(int const type,
			Gtk::Table* table, int& current_row,
			Gtk::ToggleButton* dependent_button);

      void notebook_switch_page_signal(GtkNotebookPage*, guint pageno,
				       Gtk::Notebook* notebook);

      // a key was pressed
      bool on_key_press_event(GdkEventKey* key);

    private:
      vector<Player*> player_;
      Gtk::Notebook* players_notebook;

    private: // unused
      Players();
      Players(Players const& main_window);
      Players& operator=(Players const& main_window);
  }; // class Players : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_PLAYERS_HEADER

#endif // #ifdef USE_UI_GTKMM
