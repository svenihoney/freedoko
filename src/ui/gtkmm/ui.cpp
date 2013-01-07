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
#include "translations.h"
#include "preferences.h"
#include "table.h"
#include "players.h"
#include "gameinfo_dialog.h"
#include "game_finished.h"
#include "game_review.h"
#include "party_points.h"
#include "main_window.h"
#include "menu.h"
#include "chatter.h"
#include "network_log.h"
#include "bug_report.h"
#include "error.h"
#include "splash_screen.h"
#include "first_run.h"
#include "program_updated.h"

#include "../help.h"
#include "../../basistypes.h"
#include "../../party/party.h"
#include "../../game/game.h"
#include "../../card/trick.h"
#include "../../player/human/human.h"
#include "../../misc/setting.h"
#ifndef RELEASE
#include "../../game/gameplay.h"
#include "../../player/ai/cards_information.h"
#include "../../player/ai/team_information.h"
#endif

#include "widgets/stock_button.h"

#include <gtkmm/main.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gdk/gdkkeysyms.h>

namespace UI_GTKMM_NS {

  /**
   ** -> result
   **
   ** @param     text   text to analys
   **
   ** @return    whether the text is an utf8 encoded text
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.6
   **/
  bool
    UI_GTKMM::is_utf8(string const& text)
    {
      // see http://en.wikipedia.org/wiki/UTF-8
      for (unsigned i = 0; i < text.size(); ++i) {
        char const& c = text[i];
        if ((c & 0x80) == 0) {
		// OK, ASCII character
        } else if ((c & 0xE0) == 0xC0 ) {
          if (i + 1 >= text.size())
            return false;
          if ((text[i + 1] & 0xC0) != 0x80)
            return false;
	  i += 1;
        } else if ((c & 0xF0) == 0xE0 ) {
          if (i + 2 >= text.size())
            return false;
          if (   ((text[i + 1] & 0xC0) != 0x80)
              || ((text[i + 2] & 0xC0) != 0x80) )
            return false;
	  i += 2;
        } else if ((c & 0xF8) == 0xF0 ) {
          if (i + 3 >= text.size())
            return false;
          if (   ((text[i + 1] & 0xC0) != 0x80)
              || ((text[i + 2] & 0xC0) != 0x80)
              || ((text[i + 3] & 0xC0) != 0x80) )
            return false;
	  i += 3;
        } else {
          return false;
        } // if (c)
      } // for (i)

      return true;
    } // static bool UI_GTKMM::is_utf8(string text)

  /**
   ** -> result
   **
   ** @param     text   text to convert
   **
   ** @return    utf8 format of 'text'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  Glib::ustring
    UI_GTKMM::to_utf8(string const& text)
    {
      // I do not know any function to identify the encoding of a string.
      // So we decide according to the german umlauts latin1-characters.

      // searches for german umlauts
      if (UI_GTKMM::is_utf8(text))
        return text;

#ifdef GLIBMM_EXCEPTIONS_ENABLED
      try {
        return Glib::locale_to_utf8(text);
      } catch (Glib::ConvertError const error) {
        try {
          return Glib::convert(text, "utf8", "latin1");
        } catch (Glib::ConvertError const error) {
        } // try
      } // try
#else
      std::auto_ptr<Glib::Error> error;
      Glib::ustring ret;
      ret = Glib::locale_to_utf8(text, error);
      if (!error.get())
        return ret;
      ret = Glib::convert(text, "utf8", "latin1", error);
      if (!error.get())
        return ret;
#endif

      return text;
    } // static Glib::ustring UI_GTKMM::to_utf8(string text)


  /**
   ** add a part to the list
   **
   ** @param     base   part to add
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    UI_GTKMM::add_part(Base& base)
    {
      this->parts.push_back(&base);
      return ;
    } // void UI_GTKMM::add_part(Base& base)

  /**
   ** add a window to the list
   **
   ** @param     window   window to add
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    UI_GTKMM::add_window(Gtk::Window& window)
    {
      this->windows.push_back(&window);
      return ;
    } // void UI_GTKMM::add_window(Gtk::Window& window)

  /**
   ** updates the UI
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    UI_GTKMM::update()
    {
      this->thrower.inc_depth();

      while(this->kit->events_pending())
        this->kit->iteration();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::update()

  /**
   ** sleeps the given time
   ** (in miliseconds, UINT_MAX for infinity)
   **
   ** @param     sleep_msec   time to sleep
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   ** @todo	remove variable 'sleeping'
   **/
  void
    UI_GTKMM::sleep(unsigned const sleep_msec)
    {
      sigc::connection timeout_connection
        = Glib::signal_timeout().connect(&UI_GTKMM::stop_sleeping,
                                         //sigc::mem_fun(*(this->kit),
                                         //&Gtk::Main::quit),
                                         sleep_msec);

      UI_GTKMM::sleeping = true;

      this->thrower.inc_depth();

      while (!this->thrower
             && UI_GTKMM::sleeping)
        Gtk::Main::iteration();

      timeout_connection.disconnect();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::sleep(unsigned const sleep_msec)

  /**
   ** quits the innermost main loop of gtk
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    UI_GTKMM::main_quit()
    {
      if (this->kit->level() > 0)
        this->kit->quit();

      return ;
    } // void UI_GTKMM::main_quit()

  /**
   ** quits the innermost main loop of gtk
   **
   ** @param     -
   **
   ** @return    true
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  bool
    UI_GTKMM::stop_sleeping()
    {
      if (Gtk::Main::level() > 0)
        Gtk::Main::quit();

      UI_GTKMM::sleeping = false;

      return true;
    } // static bool UI_GTKMM::stop_sleeping()

  /**
   ** quits the program
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    UI_GTKMM::quit_program()
    {
      this->thrower(GAMESTATUS::QUIT, __FILE__, __LINE__);
      this->main_quit();

      return ;
    } // void UI_GTKMM::quit_program()

  /**
   ** a key has been pressed
   **
   ** @param     key   the key
   **
   ** @return    true (stop other handlers from being invoked for the event)
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   ** @todo	some shortcuts (search 'ToDo')
   **/
  bool
    UI_GTKMM::key_press(GdkEventKey const* const key)
    {
#if 0
      struct GdkEventKey {
        GdkEventType type;
        GdkWindow *window;
        gint8 send_event;
        guint32 time;
        guint state;
        guint keyval;
        gint length;
        gchar *string;
        guint16 hardware_keycode;
        guint8 group;
      };
#endif

      if (this->table
          && this->table->in_game_review())
        if (!this->table->game_finished_->game_review->key_press(key))
          return true;

      if (key->state == GDK_SHIFT_MASK) {
        switch (key->keyval) {
        case GDK_Escape: // quit the program
          this->quit_program();
          break;
        } // switch (key->keyval)
      } // if (key->state == GDK_SHIFT_MASK)

      if (::game_status == GAMESTATUS::GAME_PLAY) {
        // play a card?
        Player& player = const_cast<Player&>(this->game().player_current());
        if (player.type() == Player::HUMAN) {
          SortedHand& hand = player.sorted_hand();
          if (!(key->state & ~(GDK_SHIFT_MASK | GDK_MOD1_MASK | GDK_MOD2_MASK))) {
            { // request a card to play
              unsigned requested_cardno = UINT_MAX;
              switch (key->keyval) {
              case GDK_1:
                requested_cardno = 0;
                break;
              case GDK_2:
                requested_cardno = 1;
                break;
              case GDK_3:
                requested_cardno = 2;
                break;
              case GDK_4:
                requested_cardno = 3;
                break;
              case GDK_5:
                requested_cardno = 4;
                break;
              case GDK_6:
                requested_cardno = 5;
                break;
              case GDK_7:
                requested_cardno = 6;
                break;
              case GDK_8:
                requested_cardno = 7;
                break;
              case GDK_9:
                requested_cardno = 8;
                break;
              case GDK_0:
              case GDK_a:
                requested_cardno = 9;
                break;
              case GDK_b:
                if (player.game().rule()(Rule::WITH_NINES))
                  requested_cardno = 10;
                break;
              case GDK_c:
                if (player.game().rule()(Rule::WITH_NINES))
                  requested_cardno = 11;
                break;
              } // switch (key->keyval)
              if (this->table->get_card()) {
                if (   (requested_cardno != UINT_MAX)
                    && !hand.played(requested_cardno)
                    && player.game().trick_current().isvalid(hand.card_all(requested_cardno)) ) {
                  hand.request_position_all(requested_cardno);
                  this->thrower(hand.requested_card(), __FILE__, __LINE__);
                }
              } // if (this->table->card_get())
            } // request a card to play
          } // if ((key->state & ~GDK_SHIFT_MASK) == 0)

        } // if (player.type() == Player::HUMAN)
      } // if (::game_status == GAMESTATUS::GAME_PLAY)

      if (!(key->state & ~(GDK_SHIFT_MASK | GDK_MOD1_MASK | GDK_MOD2_MASK))) {
        switch (key->keyval) {
        case GDK_question: // "?": help
          cout << "key codes:\n"
            << " ?: help on key codes\n"
            << " 1-0abc: play card\n"
            << " n: next announcement\n"
            << " m: announcement reply\n"
            << " h: card suggestion\n"
            << " l: show last trick\n"
            << " r: open rules window\n"
            << " p: open party points window (list page)\n"
            << " g: open party points window (graph page)\n"
            << " P: open players window\n"
#ifdef USE_NETWORK
            << " c: activate chatter\n"
            << " L: show network log\n"
#endif
            << " F1: open manual\n"
            << " F2: show preferences\n"
            << " Pause/Break: create bug report\n"
            << " C-c: exit pogram\n"
            << " S-Esc: exit program\n"
            << " C-f: toggle fullscreen\n"
            << " C-n: start new party\n"
            << " C-l: load party\n"
            << " C-s: save party\n"
            << " C-q: quit the program\n"
            << " C-r: redraw all\n"
            << " C-0: default card size\n"
            << " C-+: increase card size\n"
            << " C--: decrease card size\n"
#ifndef RELEASE
            << " i: show game information\n"
            << " C-g: print gameplay\n"
            << " e: print human cards information\n"
            << " v: print cards information of the player at the bottom (short form)\n"
            << " V: print cards information of the player at the bottom (long form)\n"
            << " t: print team information of the player at the bottom\n"
            << " T: print all team information of the player at the bottom\n"
#endif
            ;
          break;
        case GDK_Clear: // close windows
          // @todo    all
          break;
#ifdef USE_NETWORK
          // when network is activated, the menu takes care of the key
        case GDK_c: // show chat window
          // -> menu.cpp
          this->main_window->menu->chatter->activate();
          break;

        case GDK_L: // show network log
          this->network_log->present();
          break;
#endif

        case GDK_l: // show last trick
          // -> menu.cpp
          if (this->main_window->menu->last_trick->is_sensitive())
            this->main_window->menu->last_trick->activate();
          break;

#ifndef RELEASE
        case GDK_i: // show game information
          // -> menu.cpp
          if (this->main_window->menu->game_debug->is_sensitive())
            this->main_window->menu->game_debug->activate();
          break;
#endif

        case GDK_n: // next announcement
          if ((::game_status >= GAMESTATUS::GAME_PLAY)
              && (::game_status < GAMESTATUS::GAME_FINISHED))
            if (this->game().humanno() == 1) {
              this->game().make_announcement(this->game().human_player()->next_announcement(),
                                             *this->game().human_player()
                                            );
            } // if (only one human
          break;
        case GDK_m: // announcement reply
          if ((::game_status >= GAMESTATUS::GAME_PLAY)
              && (::game_status < GAMESTATUS::GAME_FINISHED))
            if (this->game().humanno() == 1)
              this->game().make_announcement(ANNOUNCEMENT::REPLY,
                                             *(this->game().human_player())
                                            );
          break;
        case GDK_h: // card suggestion
          // -> menu.cpp
          if (this->main_window->menu->card_suggestion->is_sensitive())
            this->main_window->menu->card_suggestion->activate();
          break;
        case GDK_r: // show the rules
          // -> menu.cpp
          this->main_window->menu->rules->activate();
          break;
        case GDK_p: // show the party points
          // -> menu.cpp
          if (this->main_window->menu->party_points->is_sensitive()) {
            this->ui->table->party_points_->present();
            this->ui->table->party_points_->notebook->set_current_page(0);
          }
          break;
        case GDK_g: // show the party points graph
          if (this->main_window->menu->party_points->is_sensitive()) {
            this->ui->table->party_points_->present();
            this->ui->table->party_points_->notebook->set_current_page(1);
          }
          break;
        case GDK_P: // show the players
          // -> menu.cpp
          this->main_window->menu->players->activate();
          break;
#ifndef RELEASE
        case GDK_e: // print human cards information
          if (   (::game_status >= GAMESTATUS::GAME_POVERTY_SHIFT)
              && (::game_status < GAMESTATUS::GAME_FINISHED)
              && this->game().human_player()) {
            cout << '\n'
              << this->game().human_player()->name() << ": "
              << this->game().human_player()->cards_information() << '\n';
          }
          break;
        case GDK_v: // print the cards information of the player at the bottom
          if (   (::game_status >= GAMESTATUS::GAME_POVERTY_SHIFT)
              && (::game_status < GAMESTATUS::GAME_FINISHED) ) {
            Ai const* ai = dynamic_cast<Ai const*>(&this->table->player(SOUTH));
            if (ai)
              cout << '\n'
                << ai->name() << ": "
                << ai->cards_information() << '\n';
          }
          break;
        case GDK_V: // print the cards information of the player at the bottom
          if (   (::game_status >= GAMESTATUS::GAME_POVERTY_SHIFT)
              && (::game_status < GAMESTATUS::GAME_FINISHED) ) {
            Ai const* ai = dynamic_cast<Ai const*>(&this->table->player(SOUTH));
            if (ai)
              cout << '\n'
                << ai->name() << ": ";
            ai->cards_information().write(cout);
            cout << '\n';
          }
          break;
        case GDK_t: // print team information of the player at the bottom
          if (   (::game_status >= GAMESTATUS::GAME_POVERTY_SHIFT)
              && (::game_status < GAMESTATUS::GAME_FINISHED) ) {
            Ai const* ai = dynamic_cast<Ai const*>(&this->table->player(SOUTH));
            if (ai)
              cout << '\n'
                << ai->name() << ": "
                << ai->team_information() << '\n';
          }
          break;
        case GDK_T: // print all team information
          if (   (::game_status >= GAMESTATUS::GAME_POVERTY_SHIFT)
              && (::game_status < GAMESTATUS::GAME_FINISHED) ) {
            for (vector<Player*>::const_iterator
                 p = this->game().players_begin();
                 p != this->game().players_end();
                 ++p) {
              if (!dynamic_cast<Ai*>(*p)) {
                cout << '\n'
                  << (*p)->name() << ": no ai\n";
                continue;
              }
              cout << '\n'
                << (*p)->name() << ": "
                << dynamic_cast<Ai*>(*p)->team_information() << '\n';
            } // for (p)
          }
          break;
#endif // #ifndef RELEASE

        case GDK_F1: // show manual
          // -> menu.cpp
          this->main_window->menu->help->activate();
          break;
        case GDK_F2: // show preferences
          // -> menu.cpp
          this->main_window->menu->preferences->activate();
          break;
        case GDK_Pause: // create a bug report
        case GDK_Break: // create a bug report
          this->bug_report->create_report();
          break;
        case GDK_period: // toggle show valid cards
          // -> menu.cpp
          this->main_window->menu->show_all_hands->activate();
          break;
        case GDK_Escape:
          break;
        } // switch (key->keyval)
      } // if (key->state == 0)

      if (key->state & GDK_CONTROL_MASK) {
        switch (key->keyval) {
        case GDK_c: // exit the program
          exit(EXIT_SUCCESS);
          break;
        case GDK_f: // toggle fullscreen
          if (this->main_window->get_window()->get_state()
              & Gdk::WINDOW_STATE_FULLSCREEN)
            this->main_window->unfullscreen();
          else
            this->main_window->fullscreen();
          break;

#ifndef RELEASE
        case GDK_g: // print the gameplay
          cout << this->game().gameplay();
          break;
#endif

        case GDK_n: // start new party
          // -> menu.cpp
          this->main_window->menu->new_party->activate();
          break;
        case GDK_l: // load party
          // -> menu.cpp
          this->main_window->menu->load_party->activate();
          break;
        case GDK_s: // save party
          // -> menu.cpp
          this->main_window->menu->save_party->activate();
          break;
        case GDK_q: // quit the program
          // -> menu.cpp
          this->main_window->menu->quit->activate();
          break;
        case GDK_r: // redraw all
          this->redraw_all();
          break;
        case GDK_0: // default card size
          ::setting.set(Setting::ORIGINAL_CARDS_SIZE, true);
          break;
        case GDK_plus: // increase card size
          ::setting.set(Setting::CARDS_HEIGHT,
                        ::setting(Setting::CARDS_HEIGHT) + 1);
          ::setting.set(Setting::ORIGINAL_CARDS_SIZE, false);
          break;
        case GDK_minus: // decrease card size
          ::setting.set(Setting::CARDS_HEIGHT,
                        ::setting(Setting::CARDS_HEIGHT) - 1);
          ::setting.set(Setting::ORIGINAL_CARDS_SIZE, false);
          break;
        } // switch (key->keyval)
      } // if (key->state == GDK_CONTROL_MASK)

      return true;
    } // bool UI_GTKMM::key_press(GdkEventKey const* const key)

  /**
   **
   ** @param     page   the page to be loaded
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.2
   **
   ** @todo	all
   **/
  void
    UI_GTKMM::help_load(string const& page)
    {
      return ;
    } // void UI_GTKMM::help_load(string const& page)

  /**
   **
   ** @param     player   name of the player who has chatted
   ** @param     message   message
   ** @param     self   whether this player has chattet
   **			(else the chatter comes from the network)
   **			default: true
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.8
   **/
  void
    UI_GTKMM::chat(string const& player, string const& message,
                   bool const self)
    {
      this->chatter->chat(player, message);

      return ;
    } // void UI_GTKMM::chat(string player, string message, bool self = true)

  /**
   ** this is the first run of the program
   **
   ** @param     message   	message for the user
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  void
    UI_GTKMM::first_run(Translator::Translation const& message)
    {
      DEBUG_ASSERTION((this->first_run_window == NULL),
                      "UI_GTKMM::first_run(message)\n"
                      "  the window is already created.");

      this->first_run_window = new FirstRun(this, message);

      this->first_run_window->show();

      return ;
    } // void UI_GTKMM::first_run(Translator::Translation message)

  /**
   ** the program is updated
   **
   ** @param     old_version   the old version
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  void
    UI_GTKMM::program_updated(DK::Utils::Version const& old_version)
    {
      DEBUG_ASSERTION((this->program_updated_window == NULL),
                      "UI_GTKMM::program_updated(old_version)\n"
                      "  the window is already created.");

      this->program_updated_window = new ProgramUpdated(this, old_version);

      this->program_updated_window->show();

      return ;
    } // void UI_GTKMM::program_updated(DK::Utils::Version old_version)

  /**
   ** show the message in an information window
   ** Does not shows the message, if it has already been shown and is no error message and is not forced to be shown.
   **
   ** @param     message   	message to display
   ** @param     type   	the type of the message
   ** @param     force_show     whether to show the message, although it was already shown (default: false)
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.8
   **
   ** @todo	remove memory leak
   ** @todo	add a parameter for a title
   ** @todo	put the windows on top of all windows
   **
   ** @bug	memory leak (the dialog is never deleted)
   **/
  void
    UI_GTKMM::information(Translator::Translation const& message,
                          INFORMATION::Type const type,
                          bool const force_show)
    {
      // check, whether the message was already shown
      if (   !this->add_to_messages_shown(message)
          && (type != INFORMATION::PROBLEM)
          && !force_show)
        return ;

      Gtk::MessageType message_type = Gtk::MESSAGE_INFO;
      switch (type) {
      case INFORMATION::NORMAL:
        message_type = Gtk::MESSAGE_INFO;
        break;
      case INFORMATION::WARNING:
        message_type = Gtk::MESSAGE_WARNING;
        break;
      case INFORMATION::PROBLEM:
        message_type = Gtk::MESSAGE_ERROR;
        break;
      } // switch(type)

      Gtk::MessageDialog* information
        = new Gtk::MessageDialog(message.translation(),
                                 false,
                                 message_type,
                                 Gtk::BUTTONS_NONE,
                                 false);
#ifdef GLIBMM_EXCEPTIONS_ENABLED
      try {
        this->icon = Gdk::Pixbuf::create_from_file(::setting.path(Setting::ICON_FILE));
      } catch (Glib::FileError file_error) {
        ::ui->error(::translation("Error::loading icon (%sfile%)",
                                  ::setting.path(Setting::ICON_FILE)).translation());
      } catch (Gdk::PixbufError pixbuf_error) {
        ::ui->error(::translation("Error::loading icon (%sfile%)",
                                  ::setting.path(Setting::ICON_FILE)).translation());
      } // try
#else
      std::auto_ptr<Glib::Error> error;
      this->icon = Gdk::Pixbuf::create_from_file(::setting.path(Setting::ICON_FILE), error);
      if (error.get())
        ::ui->error(::translation("Error::loading icon (%sfile%)",
                                  ::setting.path(Setting::ICON_FILE)).translation());
#endif

      // memory leak: no call of 'ui->remove_from_translation(*close_button)'
      Gtk::Button* close_button
        = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
      this->translations->add(*close_button, ::translation("close"));
      information->add_action_widget(*close_button, Gtk::RESPONSE_CLOSE);
      close_button->show_all();

      close_button->signal_clicked().connect(sigc::mem_fun(*information,
                                                           &Gtk::Widget::hide)
                                            );

#ifdef OUTDATED
      if (this->main_window)
        information->set_transient_for(*this->main_window);
#endif

      if (type == INFORMATION::PROBLEM) {
        information->set_modal();
        Gtk::Main::run(*information);
        delete information;
      } else {
        information->present();
      }
      return ;
    } // void UI_GTKMM::information(Translator::Translation message, INFORMATION::Type type, bool force_show = false)

  /**
   ** shown an error window
   ** and quit the program
   **
   ** @param     message   	message to display
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.3
   **/
  void
    UI_GTKMM::error(string const& message)
    {
      static bool already_running = false;

      if (already_running) {
        cerr << '\n'
          << ::translation("Press <Return> to exit.").translation() << endl;
        cin.get();
      } else { // if !(already_running)

        already_running = true;

        if (this->splash_screen)
          this->splash_screen->hide();

        Error error(message, this);
#ifdef RELEASE
        error.run();
#else
        error.present();
        while (error.is_visible()) {
          this->sleep(1000);
          this->thrower.clear();
        }
#endif
      } // if !(already_running)

#ifdef ASSERTION_GENERATES_SEGFAULT
      cerr << ::translation("Creating segmentation fault.").translation()
        << endl;
      SEGFAULT;
#endif // #ifdef ASSERTION_GENERATES_SEGFAULT

      ::exit(EXIT_FAILURE);

      return ;
    } // void UI_GTKMM::error(string const& message)

  /**
   ** generate an error (for testings)
   **
   ** @param     error_message   error message
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.3
   **/
  void
    UI_GTKMM::generate_error(string const error_message)
    {
      ::generate_error(error_message);

      return ;
    } // void UI_GTKMM::generate_error(string const error_message)

  /**
   ** iconifies all windows
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void 
    UI_GTKMM::iconify_all_windows()
    {
      for (std::list<Gtk::Window*>::iterator w = this->windows.begin();
           w != this->windows.end();
           ++w)
        (*w)->iconify();

      return ;
    } // void UI_GTKMM::conify_all_windows()

  /**
   ** deiconifies all windows
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    UI_GTKMM::deiconify_all_windows()
    {
      for (std::list<Gtk::Window*>::iterator w = this->windows.begin();
           w != this->windows.end();
           ++w)
        (*w)->deiconify();

      return ;
    } // void UI_GTKMM::deiconify_all_windows()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
