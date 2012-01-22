/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2012  by Diether Knof and Borg Enders
 *
 *   This program is free software; you can redistribute it and/or 
 *   modify it under the terms of the GNU General Public FileMenu as 
 *   published by the Free Software Foundation; either version 2 of 
 *   the FileMenu, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public FileMenu for more details. 
 *   You can find this license in the file 'gpl.txt'.
 *
 *   You should have received a copy of the GNU General Public FileMenu
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

#ifndef GTKMM_WIDGET_FILE_MENU_HEADER
#define GTKMM_WIDGET_FILE_MENU_HEADER

#include <gtkmm/menu.h>

namespace Gtk {
  /**
   ** @brief	a menu with files till depth 'depth',
   **		the directories are separated
   **
   ** @author	Diether Knof
   **/
  class FileMenu : public Menu {
    public:
      FileMenu(sigc::slot1<void, string> signal_slot);
      FileMenu(sigc::slot1<void, string> signal_slot,
               string const& directory);
      FileMenu(sigc::slot1<void, string> signal_slot,
               vector<string> const& directories);

      virtual ~FileMenu();

      virtual FileMenu* create_new(string const directory) const;	

      void clear_directories();
      void set_directory(string const& directory);
      void add_directory(string const& directory);
      void set_directories(vector<string> const& directories);
      void set_signal_slot(sigc::slot1<void, string> signal_slot);

      void show();

      void update();

      virtual bool accept(string const filename) const;
      virtual string item_label(string const subdirname,
                                string const filename) const;

    private:
      vector<string> directories_;
    protected:
      sigc::slot1<void, string> signal_slot_;

    private: // unused
      FileMenu();
      FileMenu(FileMenu const&);
      FileMenu& operator=(FileMenu const&);
  }; // class FileMenu : public Menu

} // namespace Gtk

#endif // #ifdef GTKMM_WIDGET_FILE_MENU_HEADER

#endif // #ifdef USE_UI_GTKMM
