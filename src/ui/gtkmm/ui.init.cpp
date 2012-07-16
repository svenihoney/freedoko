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
 *********************************************************************/

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "ui.h"
#include <gtkmm/main.h>

#include "translations.h"
#include "cards.h"
#include "icons.h"
#include "first_run.h"
#include "program_updated.h"
#include "splash_screen.h"
#include "main_window.h"
#include "table.h"
#include "menu.h"
#include "party_summary.h"
#include "party_settings.h"
#include "players_db.h"
#include "game_debug.h"
#include "chatter.h"
#include "help.h"
#include "license.h"
#include "changelog.h"
#include "support.h"
#include "about.h"
#include "preferences.h"
#ifdef USE_NETWORK
#include "network.h"
#include "network_log.h"
#ifdef USE_NETWORK_DOKOLOUNGE
#include "dokolounge/lounge.h"
#endif
#endif
#include "bug_report.h"
#include "bug_report_replay.h"

#include "../../misc/setting.h"


#include <gtkmm/drawingarea.h>
namespace UI_GTKMM_NS {

  bool UI_GTKMM::sleeping = false;

  /**
   ** constructor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.4
   **/
  UI_GTKMM::UI_GTKMM() :
    UI(UI_TYPE::GTKMM_DOKO),
    Base(this, this),
    thrower(),
    base_objects_number(0),
    kit(NULL),
    colormap(NULL),
    logo(NULL),
    icon(NULL),
    translations(NULL),
    cards(NULL),
    icons(NULL),
    first_run_window(NULL),
    program_updated_window(NULL),
    splash_screen(NULL),
    main_window(NULL),
    table(NULL),
    party_summary(NULL),
    party_settings(NULL),
    players_db(NULL),
    game_debug(NULL),
    help(NULL),
    license(NULL),
    changelog(NULL),
    chatter(NULL),
    about(NULL),
    preferences(NULL),
#ifdef USE_NETWORK
    network(NULL),
    network_log(NULL),
#ifdef USE_NETWORK_DOKOLOUNGE
    lounge(NULL),
#endif
#endif
    bug_report(NULL),
    bug_report_replay(NULL),
    parts(),
    windows()
  {
    { // the only option is the name of the program
      int argc_gtkmm = 1;
      char const* argv_tmp[] = { PROGRAM_NAME, NULL };
      char** argv_gtkmm = const_cast<char**>(argv_tmp);

      this->kit = new Gtk::Main(argc_gtkmm, argv_gtkmm);
    }

    // count the number of objects
    this->base_objects_number = 29;
#ifdef RELEASE
    this->base_objects_number += 2; // ?
#endif

#ifndef RELEASE
    this->base_objects_number += 1; // gameinfo
#endif
#ifdef USE_NETWORK
    this->base_objects_number += 1; // network, log
#ifdef USE_NETWORK_DOKOLOUNGE
    this->base_objects_number += 1; // lounge
#endif
#endif

    this->translations = new Translations(this);

    this->colormap = Gdk::Colormap::get_system();

#ifdef GLIBMM_EXCEPTIONS_ENABLED
    { // the logo
      try {
        this->logo = Gdk::Pixbuf::create_from_file(::setting.path(Setting::LOGO_FILE));
      } catch (Glib::FileError file_error) {
        ::ui->error(::translation("Error::loading logo (%sfile%)",
                                  (  ::setting.path(Setting::LOGO_FILE).empty()
                                   ? ::setting.value(Setting::LOGO_FILE)
                                   : ::setting.path(Setting::LOGO_FILE))
                                 ).translation()
                    + "\n"
                    + ::translation("searched paths:").translation() + "\n"
                    + ::setting.data_directories_string("  "));
      } catch (Gdk::PixbufError pixbuf_error) {
        ::ui->error(::translation("Error::loading logo (%sfile%)",
                                  (  ::setting.path(Setting::LOGO_FILE).empty()
                                   ? ::setting.value(Setting::LOGO_FILE)
                                   : ::setting.path(Setting::LOGO_FILE))
                                 ).translation()
                    + "\n"
                    + ::translation("searched paths:").translation() + "\n"
                    + ::setting.data_directories_string("  "));
      } // try
    } // the logo
    { // the icon
      try {
        this->icon = Gdk::Pixbuf::create_from_file(::setting.path(Setting::ICON_FILE));
      } catch (Glib::FileError file_error) {
        ::ui->error(::translation("Error::loading icon (%sfile%)",
                                  (  ::setting.path(Setting::ICON_FILE).empty()
                                   ? ::setting.value(Setting::ICON_FILE)
                                   : ::setting.path(Setting::ICON_FILE))
                                 ).translation()
                    + "\n"
                    + ::translation("searched paths:").translation() + "\n"
                    + ::setting.data_directories_string("  "));
      } catch (Gdk::PixbufError pixbuf_error) {
        ::ui->error(::translation("Error::loading icon (%sfile%)",
                                  (  ::setting.path(Setting::ICON_FILE).empty()
                                   ? ::setting.value(Setting::ICON_FILE)
                                   : ::setting.path(Setting::ICON_FILE))
                                 ).translation()
                    + "\n"
                    + ::translation("searched paths:").translation() + "\n"
                    + ::setting.data_directories_string("  "));
      } // try
    } // the icon
#else
    std::auto_ptr<Glib::Error> error;
    this->logo = Gdk::Pixbuf::create_from_file(::setting.path(Setting::LOGO_FILE), error);
    if (error.get())
      ::ui->error(::translation("Error::loading logo (%sfile%)",
                                ::setting.path(Setting::LOGO_FILE)).translation()
                  + "\n"
                  + ::translation("searched paths:").translation() + "\n"
                  + ::setting.data_directories_string("  "));
    this->icon = Gdk::Pixbuf::create_from_file(::setting.path(Setting::ICON_FILE), error);
    if (error.get())
      ::ui->error(::translation("Error::loading icon (%sfile%)",
                                ::setting.path(Setting::ICON_FILE)).translation()
                  + "\n"
                  + ::translation("searched paths:").translation() + "\n"
                  + ::setting.data_directories_string("  "));
#endif

    return ;
  } // UI_GTKMM::UI_GTKMM()

  /**
   ** destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  UI_GTKMM::~UI_GTKMM()
  {
    delete this->bug_report_replay;
    delete this->bug_report;
#ifdef USE_NETWORK
    delete this->network_log;
    delete this->network;
#ifdef USE_NETWORK_DOKOLOUNGE
    delete this->lounge;
#endif
#endif
    delete this->about;
    delete this->chatter;
    delete this->changelog;
    delete this->help;
    delete this->license;
    delete this->game_debug;
    delete this->players_db;
    delete this->party_settings;
    delete this->party_summary;
    delete this->table;
    delete this->main_window;
    delete this->splash_screen;
    delete this->program_updated_window;
    delete this->first_run_window;
    delete this->icons;
    delete this->cards;
    delete this->translations;

    delete this->preferences;

    if (this->kit && this->kit->level())
      Gtk::Main::quit();
    delete this->kit;

    return ;
  } // UI_GTKMM::~UI_GTKMM()


  /**
   ** Initialises the UI with the command line arguments.
   ** GTKMM removes the arguments it understands from the list.
   **
   ** @param     argc   number of arguments
   ** @param     argv   arguments
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  void
    UI_GTKMM::init(int& argc, char**& argv)
    {
      this->thrower.inc_depth();

      this->set_max_progress(3);

      // I have always to construct the window for 'Icons'
      // (I need one window and now I take the root window from 'splash_screen')
      Gtk::Window::set_auto_startup_notification(false);
      this->help = new Help(this);
      this->splash_screen = new SplashScreen(this);
      if (::setting(Setting::SHOW_SPLASH_SCREEN)) {
        this->splash_screen->show();

        if (this->ui->first_run_window)
          this->ui->first_run_window->raise();
        if (this->ui->program_updated_window)
          this->ui->program_updated_window->raise();
      } // if (::setting(Setting::SHOW_SPLASH_SCREEN))


      this->add_status_message(::translation("~loading the cards"));
      this->cards = new Cards(this);
      this->add_status_message(::translation("~loading the icons"));
      this->remove_status_message(::translation("~loading the cards"));
      this->icons = new Icons(this);
      this->add_status_message(::translation("~initiating the GUI"));
      this->remove_status_message(::translation("~loading the icons"));

      // there are some orders on initializing:
      //   bug_report < party_settings
      //   bug_report < bug_report_replay
      //   bug_report < main_window
      //   main_window < party_summary
      //   main_window < party_settings
      //   main_window < table
      //   main_window < network
      //   main_window < network_log
      //   network < party_settings
      this->license = new License(this);
      this->changelog = new ChangeLog(this);
      this->support = new Support(this);
      this->about = new About(this);

      this->preferences = new Preferences(this);

      this->bug_report = new BugReport(this);
      this->bug_report_replay = new BugReportReplay(this);

      this->main_window = new MainWindow(this);
      this->table = new Table(this);

#ifdef USE_NETWORK
      this->network = new Network(this);
      this->network_log = new NetworkLog(this);
#ifdef USE_NETWORK_DOKOLOUNGE
      this->lounge = new DokoLounge::Lounge(this);
#endif
#endif
      this->party_summary = new PartySummary(this);
      this->party_settings = new PartySettings(this);

      this->players_db = new PlayersDB(this);

      this->game_debug = new GameDebug(this);

      this->chatter = new Chatter(this);


      this->main_window->menu->set_signals();

      this->language_update();

      { // set the size of the main window
        int const width_hint = static_cast<int>(7 * this->cards->height());
        int const height_hint = static_cast<int>(6 * this->cards->height());
        int const width = std::min(width_hint,
                                   Gdk::Screen::get_default()->get_width() - 2 em);
        int const height = std::min(height_hint,
                                    Gdk::Screen::get_default()->get_height() - 2 ex);
        this->main_window->set_default_size(width, height);
        //this->main_window->set_default_size(width_hint - 1 em, height_hint - 1 ex);
      } // set the size of the main window

      Gtk::Window::set_auto_startup_notification(true);

      this->main_window->show();
      this->splash_screen->hide();

      this->finish_progress();
      this->remove_status_message(::translation("~initiating the GUI"));

      delete this->splash_screen;
      this->splash_screen = NULL;

      this->thrower.dec_depth();

#ifdef WORKAROUND
#if defined(_WIN32)
      // Under MS-Windows there are problems with special characters (like 'ü')
      // When the language is changed, the characters are shown as they should,
      // I wonder, why.
      {
        string const language = ::setting(Setting::LANGUAGE);
        ::setting.set(Setting::LANGUAGE, "en");
        ::setting.set(Setting::LANGUAGE, language);
      }
#endif
#endif

      return ;
    } // UI_GTKMM::init(argc, argv)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
