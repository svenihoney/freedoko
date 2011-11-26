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

#ifndef GTKMM_PARTY_SETTINGS_HEADER
#define GTKMM_PARTY_SETTINGS_HEADER

#include "base.h"
#include "widgets/sticky_dialog.h"
namespace Gtk {
  class Notebook;
  class StockButton;
  class Label;
  class SpinButton;
  class CheckButton;
  class RadioButton;
} // namespace Gtk

class Setting;
class Player;
class Aiconfig;

namespace UI_GTKMM_NS {
  class Players;
  class Rules;

  /**
   **
   ** @brief	the party settings dialog
   **
   ** @author	Diether Knof
   **
   **/
  class PartySettings : public Base, public Gtk::StickyDialog {
    public:
      PartySettings(Base* const parent);

      ~PartySettings();

      void get();
      void on_show();

      void load_bug_report();

      void sensitivity_update();
      void seed_value_sensitivity_update();

      void update();

      // update the rules (value, sensitivity)
      void rules_update();
      // the rules have changed
      void rule_change(int const type);

      void players_switched(::Player const& player_a,
			    ::Player const& player_b);
      void player_update(::Player const& player);
      void name_update(::Player const& player);
    private:
      void name_update_local(::Player const& player);
    public:
      void voice_update(::Player const& player);
      void aiconfig_update(::Aiconfig const& aiconfig);

    private:
      void init();

      void start_party_event();

      void seed_change_event();
      void startplayer_change_event();

      void switch_players_event(unsigned const p);


      // a key press
      bool key_press(GdkEventKey* key);

#ifndef OUTDATED
      // remove, when the network is (mostly) stable
      void show_network_unstable_warning();
#endif

    public:
      Players* players;
      Rules* rules;

    private:
      unsigned seed_bak;
      unsigned startplayer_first_bak;

    private:
      Gtk::StockButton* load_bug_report_button;
      Gtk::StockButton* start_party_button;
      Gtk::StockButton* close_button;

      Gtk::SpinButton* seed_value;
      Gtk::CheckButton* seed_random;

      Gtk::CheckButton* rule_number_of_rounds_limited;
      Gtk::SpinButton* rule_number_of_rounds;
      Gtk::CheckButton* rule_points_limited;
      Gtk::SpinButton* rule_points;

      vector<Gtk::RadioButton*> startplayer;
      Gtk::RadioButton* startplayer_random;
      vector<Gtk::Button*> switch_players_buttons;

      Gtk::StockButton* configure_players;
      Gtk::StockButton* configure_rules;
#ifdef USE_NETWORK
      Gtk::StockButton* configure_network;
#endif

    private: // unused
      PartySettings();
      PartySettings(PartySettings const&);
      PartySettings& operator=(PartySettings const&);
  }; // class PartySettings : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_PARTY_SETTINGS_HEADER

#endif // #ifdef USE_UI_GTKMM
