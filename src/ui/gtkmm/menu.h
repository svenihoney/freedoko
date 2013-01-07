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

#ifndef GTKMM_MENU_HEADER
#define GTKMM_MENU_HEADER

#include "base.h"

#include "../../basistypes.h"

#include "../../misc/setting.theme.h"

#include <gtkmm/menubar.h>
#include <sigc++/connection.h>
namespace Gtk {
  class Menu;
  class MenuItem;
  class CheckMenuItem;
  class Label;
  class FileChooserDialog;
} // namespace Gtk

namespace UI_GTKMM_NS {
  class MainWindow;
  class Preferences;

  /**
   **
   ** @brief	the main window
   **
   ** party
   **   +- new party		C-n
   **   +- load party		C-l
   **   +- save party		C-s
   **   +- end party
   **   `- quit			C-q
   ** information
   **   +- card suggestion	h
   **   +- last trick		l
   **   +- rules		r
   **   +- players
   **   `- party points		p
   ** announcement
   **   `-  ...			n	m
   ** preferences
   **   +- show all hands
   **   +- show valid cards
   **   +- cards order
   **   `- preferences		F2
   ** network
   **   +- chatter		C
   **   +- log
   **   `- settings		F3
   ** bug report
   ** help
   **   +- help index		F1
   **   +- license
   **   +- cardset license
   **   +- changelog
   **   `- about
   ** 
   ** Note: the translation of the announcements are managed internally
   ** 
   **
   ** @author	Diether Knof
   **
   ** @todo	replace 'party_points_*_connection' (see sigc-doc)
   ** @todo	update the player names in the announcement
   **
   **/
  class Menu : public Base, public Gtk::MenuBar {
    friend class Preferences; // for changing the preferences menu items
    friend class UI_GTKMM; // for shortcuts

    public:
    Menu(MainWindow* const main_window);
    ~Menu();

    void set_signals();
    void show_all_hands_set();
    void emphasize_valid_cards_set();

    void party_open();
    void party_start();
    void game_start();
    void trick_open();
    void card_get();
    void card_got();
    void game_finish();
    void game_close();
    void party_finish();
    void party_close();

    void announcements_update();
    void language_update();
    private:
    void init();

    public:
    void announcement_event(unsigned const playerno, int const announcement);
    void swines_announcement_event(unsigned const playerno);
    void hyperswines_announcement_event(unsigned const playerno);

    private:
    void save_party_event();
    void load_party_event();

    void update_themes_sensitivity();
    void apply_theme_event(Setting::Theme const* const theme);

    private:
    MainWindow* main_window;

    Gtk::Menu* party_menu;
    Gtk::MenuItem* party;
    Gtk::ImageMenuItem* new_party;
    Gtk::ImageMenuItem* load_party;
    Gtk::ImageMenuItem* save_party;
    Gtk::ImageMenuItem* end_party;
    Gtk::ImageMenuItem* quit;

    Gtk::Menu* information_menu;
    Gtk::MenuItem* information;
    Gtk::ImageMenuItem* card_suggestion;
    Gtk::ImageMenuItem* last_trick;
    Gtk::ImageMenuItem* game_debug;
    Gtk::ImageMenuItem* bug_report_replay;
    Gtk::ImageMenuItem* rules;
    Gtk::ImageMenuItem* party_points;
    Gtk::ImageMenuItem* players;
    Gtk::ImageMenuItem* players_db;

    Gtk::MenuItem* announcements;
    vector<Gtk::MenuItem*> announcements_player;
    vector<Gtk::Menu*> announcements_player_menu;
    vector<vector<Gtk::MenuItem*> > announcements_announcement;
    vector<Gtk::MenuItem*> swines_separator;
    vector<Gtk::MenuItem*> swines_announcement;
    vector<Gtk::MenuItem*> hyperswines_announcement;

    Gtk::Menu* preferences_menu;
    Gtk::MenuItem* preferences;
    protected:
    Gtk::CheckMenuItem* emphasize_valid_cards;
    Gtk::CheckMenuItem* show_all_hands;
    private:
    Gtk::ImageMenuItem* cards_order;
    Gtk::Menu* themes_menu;
    Gtk::MenuItem* themes_item;
    vector<Gtk::ImageMenuItem*> themes;
    Gtk::ImageMenuItem* preferences_preferences;

#ifdef USE_NETWORK
    Gtk::Menu* network_menu;
    Gtk::MenuItem* network;
    Gtk::ImageMenuItem* chatter;
    Gtk::ImageMenuItem* network_log;
    Gtk::ImageMenuItem* network_settings;
#endif

    Gtk::ImageMenuItem* bug_report;

    Gtk::Menu* help_menu;
    Gtk::MenuItem* help;
    Gtk::ImageMenuItem* help_index;
    Gtk::ImageMenuItem* license;
    Gtk::ImageMenuItem* cardset_license;
    Gtk::ImageMenuItem* changelog;
    Gtk::ImageMenuItem* support;
    Gtk::ImageMenuItem* about;
    Gtk::ImageMenuItem* generate_error;

    sigc::connection party_points_present_connection;

    Gtk::FileChooserDialog* load_party_file_chooser;
    Gtk::FileChooserDialog* save_party_file_chooser;

    private: // unused
    Menu();
    Menu(Menu const& main_window);
    Menu& operator=(Menu const& main_window);
  }; // class Menu : public Base, public Gtk::MenuBar

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_MENU_HEADER

#endif // #ifdef USE_UI_GTKMM
