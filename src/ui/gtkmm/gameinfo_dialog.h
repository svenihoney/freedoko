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

#ifndef GTKMM_GAMEINFO_DIALOG_HEADER
#define GTKMM_GAMEINFO_DIALOG_HEADER

#include "base.h"

class Player;

#include "widgets/sticky_dialog.h"
namespace Gtk {
  class StockButton;
}; // namespace Gtk

namespace UI_GTKMM_NS {
  class Table;
  class GameInfo;

  /**
   ** @brief    dialog that shows game informations
   **
   ** @author   Diether Knof
   **/
  class GameInfoDialog : public Base, public Gtk::StickyDialog {
    public:
      class Information;

    public:
      // constructor
      GameInfoDialog(Table* const table);
      // destructor
      virtual ~GameInfoDialog();

      // the corresponding table
      Table& table();

      // show the information (and manage it)
      void show(Information* const game_info,
                bool const close_automatically,
                unsigned const close_delay);
      // show the information (and manage it)
      void show(Information* const game_info);

      // close the window
      void close();

      // update the announcement reply sensitivity
      void update_announcement_reply();
      // the name of 'player' has changed
      void name_changed(Player const& player);

      // on timeout close the window
      bool on_timeout();

    private:
      // initialize the window
      void init();
      // reply to the last announcement
      void announcement_reply();
      // announce hyperswines
      void announce_hyperswines();

      // add the information
      void add_info(Information* const info);
      // remove all informations
      void clear();

      // whether the dialog is blocking the gameplay
      bool is_blocking() const;

    protected:
      // close button
      Gtk::StockButton* close_button;
      // announcement reply button
      Gtk::StockButton* announcement_reply_button;
      // announce hyperswines
      Gtk::StockButton* hyperswines_announce_button;
      // variable for the timeout signal
      sigc::connection timeout_connection;

      // the shown informations
      vector<Information*> infos;

    private: // unused
      GameInfoDialog();
      GameInfoDialog(GameInfoDialog const&);
      GameInfoDialog& operator=(GameInfoDialog const&);
  }; // class GameInfoDialog : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_GAMEINFO_DIALOG_HEADER

#endif // #ifdef USE_UI_GTKMM
