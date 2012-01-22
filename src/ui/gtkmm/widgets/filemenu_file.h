/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2012  by Diether Knof and Borg Enders
 *
 *   This program is free software; you can redistribute it and/or 
 *   modify it under the terms of the GNU General Public FileMenuFilterFile as 
 *   published by the Free Software Foundation; either version 2 of 
 *   the FileMenuFilterFile, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public FileMenuFilterFile for more details. 
 *   You can find this license in the file 'gpl.txt'.
 *
 *   You should have received a copy of the GNU General Public FileMenuFilterFile
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

#ifndef GTKMM_WIDGET_FILE_MENU_FILTER_FILE_HEADER
#define GTKMM_WIDGET_FILE_MENU_FILTER_FILE_HEADER

#include "filemenu.h"

namespace Gtk {
  /**
   ** @brief	a file menu, only with directories with the file 'file'
   **
   ** @author	Diether Knof
   **/
  class FileMenuFilterFile : public FileMenu {
    public:
      FileMenuFilterFile(sigc::slot1<void, string> signal_slot,
                         string const file) :
        FileMenu(signal_slot),
        file(file)
    { }

      virtual ~FileMenuFilterFile()
      { }

      virtual FileMenu* create_new(string const directory) const
      {
        FileMenu* file_menu = new FileMenuFilterFile(this->signal_slot_,
                                                     this->file);
        file_menu->set_directory(directory);
        return file_menu;
      }

      virtual bool accept(string const filename) const
      {
        return (Glib::file_test(filename, Glib::FILE_TEST_IS_DIR)
                && Glib::file_test(filename + "/" + this->file,
                                   Glib::FILE_TEST_IS_REGULAR));
      }

    private:
      string file;

    private: // unused
      FileMenuFilterFile();
      FileMenuFilterFile(FileMenuFilterFile const&);
      FileMenuFilterFile& operator=(FileMenuFilterFile const&);
  }; // class FileMenuFilterFile : public Menu

} // namespace Gtk

#endif // #ifdef GTKMM_WIDGET_FILE_MENU_FILTER_FILE_HEADER

#endif // #ifdef USE_UI_GTKMM
