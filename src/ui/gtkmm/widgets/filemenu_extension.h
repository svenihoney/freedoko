/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2011  by Diether Knof and Borg Enders
 *
 *   This program is free software; you can redistribute it and/or 
 *   modify it under the terms of the GNU General Public FileMenuFilterExtension as 
 *   published by the Free Software Foundation; either version 2 of 
 *   the FileMenuFilterExtension, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public FileMenuFilterExtension for more details. 
 *   You can find this license in the file 'gpl.txt'.
 *
 *   You should have received a copy of the GNU General Public FileMenuFilterExtension
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

#ifndef GTKMM_WIDGET_FILE_MENU_FILTER_EXTENSION_HEADER
#define GTKMM_WIDGET_FILE_MENU_FILTER_EXTENSION_HEADER

#include "filemenu.h"

namespace Gtk {
  /**
   ** @brief	a file menu, only with files with extension 'extension'
   **
   ** @author	Diether Knof
   **/
  class FileMenuFilterExtension : public FileMenu {
    public:
      FileMenuFilterExtension(sigc::slot1<void, string> signal_slot,
                              string const extension) :
        FileMenu(signal_slot),
        extension(extension)
    { }

      virtual ~FileMenuFilterExtension()
      { }

      virtual FileMenu* create_new(string const directory) const
      {
        FileMenu* file_menu = new FileMenuFilterExtension(this->signal_slot_,
                                                          this->extension);
        file_menu->set_directory(directory);
        return file_menu;
      }

      virtual bool accept(string const filename) const
      {
        if (filename.size() < 4)
          return false;

        return (Glib::file_test(filename, Glib::FILE_TEST_IS_REGULAR)
                && (string(filename.end() - 1 - this->extension.size(),
                           filename.end())
                    == "." + this->extension));
      }

      virtual string item_label(string const subdirname,
                                string const filename) const
      {
        return (string(filename,
                       0, filename.size() - 1 - this->extension.size()));
      }

    private:
      string extension;

    private: // unused
      FileMenuFilterExtension();
      FileMenuFilterExtension(FileMenuFilterExtension const&);
      FileMenuFilterExtension& operator=(FileMenuFilterExtension const&);
  }; // class FileMenuFilterExtension : public Menu

} // namespace Gtk

#endif // #ifdef GTKMM_WIDGET_FILE_MENU_FILTER_EXTENSION_HEADER

#endif // #ifdef USE_UI_GTKMM
