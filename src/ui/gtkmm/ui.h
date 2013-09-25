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

#ifndef UI_GTKMM_HEADER
#define UI_GTKMM_HEADER

#include "../ui.h"
#include "base.h"
#include "thrower.h"

#include "../../misc/translation.h"

namespace Gtk {
  class Main;
  class Label;
  class StockButton;
  class Button;
  class Window;
  class TextView;
  class Object;
  class Widget;
} // namespace Gtk
#include <glibmm/refptr.h>
#include <gdkmm/pixbuf.h>

namespace UI_GTKMM_NS {
  class UI_GTKMM_Wrap;
  class Translations;
  class Cards;
  class Icons;
  class FirstRun;
  class ProgramUpdated;
  class SplashScreen;
  class MainWindow;
  class PartySummary;
  class PartySettings;
  class PlayersDB;
  class GameDebug;
  class Chatter;
  class Help;
  class License;
  class CardsetLicense;
  class ChangeLog;
  class Support;
  class About;
  class Table;
  class Preferences;
#ifdef USE_NETWORK
  class Network;
  class NetworkLog;
#endif
  class BugReport;
  class BugReportReplay;

  /**
   **
   ** @brief	the UI, created with gtkmm
   **
   ** @author	Diether Knof
   **
   ** @todo	replay game
   ** @todo	help
   ** @todo	messages (status line)
   ** @todo	drag'n drop (cardset, cards back, background)
   ** @todo	game log window
   **
   **/
  class UI_GTKMM : public UI, public Base {
    friend class Base;

    public:
    // detect whether the string is an utf8 text
    static bool is_utf8(string const& text);
    // converts the text in utf8 format
    static Glib::ustring to_utf8(string const& text);

    public:
    UI_GTKMM();

    ~UI_GTKMM();


    // initialize the ui
    void init(int& argc, char**& argv);

    // updates the UI
    void update();
    // sleeps 'sleep_msec' thousands of seconds
    // (< 0 for infinity)
    void sleep(unsigned const sleep_msec);

    // quit the main loop
    void main_quit();
    // stop sleeping (for timeout events)
    static bool stop_sleeping();

    // a key press
    bool key_press(GdkEventKey const* const key);

    // start new party
    void start_new_party();
    // end the party
    void end_party();
    // quit the program
    void quit_program();

    // the language has changed
    void language_changed();


    // the parts of a party
    void party_open(Party& party);
    void party_get_settings();
    void party_loaded();
    void party_seed_changed();
    void party_startplayer_changed();
    void party_start();
    void party_start_round(unsigned const round);
    void party_finish();
    void party_close();

    // a gameplay action
    void gameplay_action(GameplayAction const& action);

    // the parts of a game
    void game_open(Game& game);
    void game_cards_distributed();
    void game_redistribute();
    void reservation_ask(Player const& player);
    ::Reservation reservation_get(Player const& player);
    void game_start();
    void game_finish();
    void game_close();

    // the parts of a trick
    void trick_open(::Trick const& trick);
    void trick_full();
    void trick_move_in_pile();
    void trick_close();
    // get a card
    HandCard card_get(Player const& player);
    // the card is played
    void card_played(HandCard const& card);

    // 'player' has a poverty and shifts
    void poverty_shifting(Player const& player);
    // 'player' shifts 'cardno' cards
    void poverty_shift(Player const& player, unsigned const cardno);
    // ask 'player' whether to accept the poverty
    void poverty_ask(Player const& player, unsigned const cardno);
    // the player 'player' has denied the poverty trumps
    void poverty_take_denied(Player const& player);
    // all players have denied to take the cards
    void poverty_take_denied_by_all();
    // the player 'player' has accepted the poverty trumps
    // and has returned 'cardno' cards with 'trumpno' trumps
    void poverty_take_accepted(Player const& player,
			       unsigned const cardno,
			       unsigned const trumpno);
    // 'player' shifts cards
    HandCards poverty_shift(Player& player);
    // returns whether 'player' accepts the poverty
    bool poverty_take_accept(Player& player,
			     unsigned const cardno);
    // the player changes the cards of the poverty
    HandCards poverty_cards_change(Player& player,
				   HandCards const& cards);
    // the poverty player 'player' gets 'cards'
    void poverty_cards_get_back(Player& player,
				HandCards const& cards);

    // returns the selected partner for a genscher
    Player const* genscher_partner();

    // redrawing
    void redraw_all();
    void gametype_changed();
    void players_switched(Player const& player_a,
			  Player const& player_b);
    void player_changed(Player const& player);
    void name_changed(Player const& player);
    void voice_changed(Player const& player);
    void hand_changed(Player const& player);
    void teaminfo_changed(Player const& player);
    void aiconfig_changed(Aiconfig const& aiconfig);

    void cards_changed();
    void cards_back_changed();
    void icons_changed();

    // change of the rules
    void rule_changed(int const type, void const* const old_value);
    // changes of the settings
    void setting_changed(int const type, void const* const old_value);

    // load the help page
    void help_load(string const& page);


    // whether the ui is busy
    void set_busy();
    void set_not_busy();

    // 'player' has chatted 'message'
    void chat(string const& player, string const& message,
	      bool const self = true);
#ifdef USE_NETWORK
    // a new listener was created
    void network_listener_new(::Network::Listener const& listener);
    // a listener was deleted
    void network_listener_deleted(::Network::Listener const& listener);
    // a new connection was established
    void network_connection_new(::Network::Connection const& connection);
    // a connection has changed
    void network_connection_changed(::Network::Connection const& connection);
    // a connection is deleted
    void network_connection_deleted(::Network::Connection const& connection);
    // there was network traffic over the given connection
    void network_traffic(::Network::Connection const& connection,
			 ::Network::TrafficType const traffic_type,
			 string const& text);
#endif

    // the status message has changed
    void status_message_changed(Translator::Translation const& status_message);

    // the progress has changed
    void progress_changed(double const progress_max);
    // the progress is finished
    void progress_finished();


    // the first run of the program
    void first_run(Translator::Translation const& message);
    // an update of the version
    void program_updated(DK::Utils::Version const& old_version);
    // information for the user
    void information(Translator::Translation const& message,
		     INFORMATION::Type const type,
                     bool const force_show = false);
    // show the help
    void show_help(string const& location);

    // an error has occured
    void error(string const& message);


    // generate an error
    void generate_error(string const error_message);


    // internal functions

    // add a part to the list
    void add_part(Base& base);
    // add a window to the window list
    void add_window(Gtk::Window& window);

    // iconifies all windows
    void iconify_all_windows();
    // deiconifies all windows
    void deiconify_all_windows();

    public: // the gtkmm part
    Thrower thrower;

    Gtk::Main* kit;
    Glib::RefPtr<Gdk::Colormap> colormap;

    Glib::RefPtr<Gdk::Pixbuf> logo;

    Glib::RefPtr<Gdk::Pixbuf> icon;

    Translations* translations;

    Cards* cards;
    Icons* icons;

    FirstRun* first_run_window;
    ProgramUpdated* program_updated_window;
    SplashScreen* splash_screen;

    MainWindow* main_window;

    Table* table;

    PartySummary* party_summary;
    PartySettings* party_settings;

    PlayersDB* players_db;

    GameDebug* game_debug;

    Help* help;
    License* license;
    ChangeLog* changelog;
    Chatter* chatter;
    Support* support;
    About* about;

    Preferences* preferences;
#ifdef USE_NETWORK
    Network* network;
    NetworkLog* network_log;
#endif

    BugReport* bug_report;
    BugReportReplay* bug_report_replay;

    static bool sleeping;

    private:
    // all elements of the ui
    std::list<Base*> parts;

    // all windows
    std::list<Gtk::Window*> windows;

    private: // unused
    UI_GTKMM(UI_GTKMM const& ui);
    UI_GTKMM& operator=(UI_GTKMM const& ui);
  }; // class UI_GTKMM : public UI, public Base

} // namespace UI_GTKMM_NS

#endif // #ifdef UI_GTKMM_HEADER

#endif // #ifdef USE_UI_GTKMM
