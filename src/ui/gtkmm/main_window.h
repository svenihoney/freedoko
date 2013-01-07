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

#ifndef GTKMM_MAIN_WINDOW_HEADER
#define GTKMM_MAIN_WINDOW_HEADER

#include "base.h"
#include <gtkmm/window.h>
namespace Gtk {
  class VBox;
} // class Gtk

namespace UI_GTKMM_NS {
  class Menu;

  /**
   **
   ** @brief	the main window
   **
   ** @author	Diether Knof
   **
   ** @todo	drawing area
   **
   **/
  class MainWindow : public Base, public Gtk::Window {
    public:
      MainWindow(Base* const parent);
      ~MainWindow();

      void start_new_party_event();
      void end_party_event();
      void quit_program_event();
      void generate_error_event();
      void set_minimal_size();

      // a key press
      bool on_key_press_event(GdkEventKey* key);


    private:
      void on_hide();
      void on_show();

      // this is called p.e. by iconfy/deiconify
      bool on_window_state_event(GdkEventWindowState* state);

    public:
      Menu* menu;
      Gtk::VBox* container;

    private: // unused
      MainWindow();
      MainWindow(MainWindow const& main_window);
      MainWindow& operator=(MainWindow const& main_window);
  }; // class MainWindow : public Base, public Gtk::Window

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_MAIN_WINDOW_HEADER

#endif // #ifdef USE_UI_GTKMM
