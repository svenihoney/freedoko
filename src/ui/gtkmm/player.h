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

#ifndef GTKMM_PLAYER_HEADER
#define GTKMM_PLAYER_HEADER

#include "base.h"
#include "players.h"
#include "../../player/player.h"
class Aiconfig;
typedef ::Player::Type PlayerType;

#include <sigc++/object.h>
namespace Gtk {
  class Label;
  class Button;
  class StockButton;
  class Entry;
  class RadioButton;
  class FileMenu;
} // namespace Gtk

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the configuration of one player (without aiconfig)
   **
   ** @author	Diether Knof
   **
   **/
  class Players::Player : public Base, public sigc::trackable {
    public:
      friend class Players;

    public:
      class AiConfig;

    public:
      Player(Players* const players, unsigned const playerno);
      ~Player();

      ::Player const& player() const;
      ::Player& player();
      unsigned playerno() const;

      AiConfig& aiconfig();

      void create_backup();
      void reset();

      bool name_changed_event(GdkEventFocus*);
      void random_name();
      void voice_selected(string const& voice);
      void type_change(PlayerType const type);
      void aiconfig_update(::Aiconfig const& aiconfig);

      void sensitivity_update();
      void update();
      void name_update();
      void voice_update();
      void language_update();

    private:
      void init();

    private:
      Players* const players;

      AiConfig* aiconfig_;

      unsigned const playerno_;
      ::Player const* player_bak;

    protected:
      Gtk::Label* name_label;
      Gtk::Entry* name_entry;
      Gtk::Button* random_name_button;
      Gtk::Button* voice_button;
      Gtk::FileMenu* voice_menu;
      vector<Gtk::RadioButton*> type_button;
      Gtk::StockButton* reset_button;

    private: // unused
      Player();
      Player(Player const& player);
      Player& operator=(Player const& player);
  }; // class Players::Player : public Base, public Gtk::Alignment

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_PLAYER_HEADER

#endif // #ifdef USE_UI_GTKMM
