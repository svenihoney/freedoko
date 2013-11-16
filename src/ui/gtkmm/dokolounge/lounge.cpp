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
#ifdef USE_NETWORK
#ifdef USE_NETWORK_DOKOLOUNGE

#include "lounge.h"

#include "help.h"
#include "blog.h"
#include "pin_board.h"
#include "messages.h"
#include "alert.h"

#include "../ui.h"
#include "../translations.h"
#include "../../../misc/setting.h"

#include "../../../misc/lounge.h"
#include "../../../network/server.h"
#include "../../../network/connection.h"
#include "../../../network/DokoLounge/sender.h"
#include "../../../utils/string.h"

#include "../widgets/stock_button.h"
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/alignment.h>
#include <gtkmm/table.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>

// todo: use dummy connection (text output) for debugging
//
// Umlaute konvertieren
// Eigenes Flüstern farbig
// Fett

namespace UI_GTKMM_NS {
  namespace DokoLounge {

    /**
     ** constructor
     **
     ** @param     parent   the parent object
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    Lounge::Lounge(Base* const parent) :
      Base(parent),
      Gtk::StickyDialog("DokoLounge: Lounge"),
      connection(NULL),
      sender(NULL),
      help(NULL), // subwindows/subelements
      blog(NULL),
      pin_board(NULL),
      messages(NULL),
      alert(NULL),
      name(NULL), // widgets
      create_account_button(NULL),
      login_button(NULL),
      logout_button(NULL),
      refresh_button(NULL),
      klingel_button(NULL),
      hilfe_button(NULL),
      blog_button(NULL),
      pinnwand_button(NULL),
      messages_button(NULL),
      players_list(NULL),
      tables_list(NULL),
      chat_messages(NULL),
      chat_line(NULL),
      close_button(NULL),
      player_colors()
    {
      this->ui->add_window(*this);
      this->signal_realize().connect(sigc::mem_fun(*this, &Lounge::init));

      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(1, 0, 0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(0, 0.8, 0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(0.5, 0.5, 0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(0.5, 0, 0.5);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(0, 0.5, 0.5);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(138/256.0,43/256.0,226/256.0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(65/256.0,105/256.0,225/256.0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(200/256.0,0/256.0,0/256.0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(205/256.0,102/256.0,0/256.0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(165/256.0,42/256.0,42/256.0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(16/256.0,78/256.0,139/256.0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(0/256.0,100/256.0,0/256.0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(108/256.0,19/256.0,53/256.0);
      this->player_colors.push(Gdk::Color());
      this->player_colors.back().set_rgb_p(47/256.0,79/256.0,79/256.0);

      return ;
    } // Lounge::Lounge(Base* const parent)

    /**
     ** destruktor
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    Lounge::~Lounge()
    { 
      if (this->connection != NULL) {
        this->sender->logout();
        ::server->delete_connection(this->connection);
        delete this->connection;
        this->connection = NULL;
      }
        delete this->blog;
    } // Lounge::~Lounge()

    /**
     ** the lounge is shown
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::on_show()
      {
#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
        this->Gtk::Window::on_show();
#endif

        if (::setting(Setting::DOKOLOUNGE_AUTO_LOGIN))
          this->login_signal();

        return;
      } // void Lounge::on_show()

    /**
     ** create all subelements
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::init()
      {
        this->help = new Help(this);
        this->blog = new Blog(this);
        this->pin_board = new PinBoard(this);
        this->messages = new Messages(this);
        this->alert = new Alert(this);

        this->ui->translations->add(*this, ::translation("FreeDoko %ttitle%",
                                                         ::translation("DokoLounge: Lounge")));
        this->set_icon(this->ui->icon);

        this->set_skip_taskbar_hint();

        this->set_default_size((this->ui->logo->get_width() * 3),
                               (this->ui->logo->get_height() * 4));

        { // action area
          this->close_button
            = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
          this->ui->translations->add(*close_button,
                                      ::translation("close"));
          this->add_action_widget(*close_button, Gtk::RESPONSE_CLOSE);
          close_button->show_all();
        } // action area

        { // main area
          { // name
            this->name
              = Gtk::manage(new Gtk::Label("name: Name"));
            this->name->set_use_markup();
            this->ui->translations->add(*this->name,
                                        "<b>"
                                        + ::translation("Name: %sname%",
                                                        ::setting(Setting::DOKOLOUNGE_NAME))
                                        + "</b>");
            //this->name->set_markup("<b><big>Name</big></b>");
            this->get_vbox()->pack_start(*this->name, Gtk::PACK_SHRINK, 2 ex);
          } // name
          { // buttons
            Gtk::HBox* box = Gtk::manage(new Gtk::HBox(true, 0));

            this->create_account_button
              = Gtk::manage(new Gtk::StockButton("create account"));
            this->ui->translations->add(*this->create_account_button,
                                        ::translation("create account"));
            box->add(*this->create_account_button);

            this->login_button
              = Gtk::manage(new Gtk::StockButton("login"));
            this->ui->translations->add(*this->login_button,
                                        ::translation("login"));
            box->add(*this->login_button);

            this->logout_button
              = Gtk::manage(new Gtk::StockButton("logout"));
            this->ui->translations->add(*this->logout_button,
                                        ::translation("logout"));
// add at the end

#if 0
            this->refresh_button
              = Gtk::manage(new Gtk::StockButton("refresh"));
            this->ui->translations->add(*this->refresh_button,
                                        ::translation("refresh"));
            box->add(*this->refresh_button);
#endif

            this->klingel_button
              = Gtk::manage(new Gtk::StockButton("klingel"));
            this->ui->translations->add(*this->klingel_button,
                                        ::translation("klingel"));
            box->add(*this->klingel_button);

            this->hilfe_button
              = Gtk::manage(new Gtk::StockButton("hilfe"));
            this->ui->translations->add(*this->hilfe_button,
                                        ::translation("hilfe"));
            box->add(*this->hilfe_button);

            this->blog_button
              = Gtk::manage(new Gtk::StockButton("blog"));
            this->ui->translations->add(*this->blog_button,
                                        ::translation("blog"));
            box->add(*this->blog_button);

            this->pinnwand_button
              = Gtk::manage(new Gtk::StockButton("pinnwand"));
            this->ui->translations->add(*this->pinnwand_button,
                                        ::translation("pinnwand"));
            box->add(*this->pinnwand_button);

            this->messages_button
              = Gtk::manage(new Gtk::StockButton("messages"));
            this->ui->translations->add(*this->messages_button,
                                        ::translation("messages"));
            box->add(*this->messages_button);

            box->add(*this->logout_button);

            this->get_vbox()->pack_start(*box, Gtk::PACK_SHRINK);
          } // buttons
          Gtk::HBox* box = Gtk::manage(new Gtk::HBox(true, 0));
          { // chat
            Gtk::Frame* frame = Gtk::manage(new Gtk::Frame("Chat"));
            this->ui->translations->add(*dynamic_cast<Gtk::Label*>(frame->get_label_widget()),
                                        ::translation("Chat"));
            Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 0));

            this->chat_messages = Gtk::manage(new Gtk::TextView());
            this->chat_messages->set_wrap_mode(Gtk::WRAP_WORD);
            this->chat_messages->set_editable(false);
            this->chat_messages->set_cursor_visible(false);
            Gtk::ScrolledWindow* scroll_window = Gtk::manage(new Gtk::ScrolledWindow());
            scroll_window->add(*this->chat_messages);
            scroll_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
            vbox->add(*scroll_window);

            this->chat_line = Gtk::manage(new Gtk::Entry());
            vbox->pack_end(*this->chat_line, Gtk::PACK_SHRINK);
            this->chat_line->grab_focus();

            frame->add(*vbox);
            box->add(*frame);
          } // chat
          { // table list
            Gtk::Frame* frame = Gtk::manage(new Gtk::Frame("Table list"));
            this->ui->translations->add(*dynamic_cast<Gtk::Label*>(frame->get_label_widget()),
                                        ::translation("Table list"));
            Gtk::Alignment* alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_TOP, 1, 0));
            Gtk::ScrolledWindow* scroll_window = Gtk::manage(new Gtk::ScrolledWindow());
            scroll_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
            this->tables_list = Gtk::manage(new Gtk::Table(1, 1));
            alignment->add(*this->tables_list);
            scroll_window->add(*alignment);
            frame->add(*scroll_window);
            box->add(*frame);
          } // table list
          { // player list
            Gtk::Frame* frame = Gtk::manage(new Gtk::Frame("Player list"));
            this->ui->translations->add(*dynamic_cast<Gtk::Label*>(frame->get_label_widget()),
                                        ::translation("player list"));
            Gtk::Alignment* alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_TOP, 1, 0));
            Gtk::ScrolledWindow* scroll_window = Gtk::manage(new Gtk::ScrolledWindow());
            scroll_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
            this->players_list = Gtk::manage(new Gtk::Table(1, 1));
            alignment->add(*this->players_list);
            scroll_window->add(*alignment);
            frame->add(*scroll_window);
            box->add(*frame);
          } // player list
          this->get_vbox()->pack_end(*box, Gtk::PACK_EXPAND_WIDGET);
        } // main area

        { // signals
          // this->signal_show().connect(sigc::mem_fun(*this, &Network::update_sensitivity));
          this->close_button->signal_clicked().connect(sigc::mem_fun(*this, &Gtk::Widget::hide));
          this->create_account_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::create_account_signal));
          this->login_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::login_signal));
          this->chat_line->signal_activate().connect(sigc::mem_fun(*this, &Lounge::chat_signal));
        } // signals

        { // the text
          this->language_update();
        } // the text

        this->sensitivity_update();
        this->show_all_children();

        return ;
      } // void Lounge::init()

    /**
     ** update the sensitivity
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::sensitivity_update()
      {
        if (!this->is_realized())
          return;

        bool const logged_in = ::lounge->is_logged_in();
        this->create_account_button->set_sensitive(!logged_in);
        this->login_button->set_sensitive(         !logged_in);

        this->logout_button->set_sensitive(         logged_in);
        this->klingel_button->set_sensitive(        logged_in);
        this->hilfe_button->set_sensitive(          logged_in);
        this->blog_button->set_sensitive(           logged_in);
        this->pinnwand_button->set_sensitive(       logged_in);
        this->messages_button->set_sensitive(           logged_in);

        return;
      } // void Lounge::sensitivity_update()

    /**
     ** the game has logged in
     **
     ** @param     name   name of the player
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::logged_in(string const& name)
      {
        this->ui->translations->change(*this->name,
                                       ::translation("Name: %sname%",
                                                     ::lounge->name()));
        this->sensitivity_update();
        return;
      } // void Lounge::logged_in(string name)

    /**
     ** the game is logged out
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::logged_out()
      {
        this->ui->translations->change(*this->name,
                                       ::translation("Name: %sname%",
                                                     ::setting(Setting::DOKOLOUNGE_NAME)));
        this->sensitivity_update();
        return;
      } // void Lounge::logged_out()

    /**
     ** a chat entry has been added
     **
     ** @param     entry    new chat entry
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **
     ** @todo      type: bold/italic
     **/
    void
      Lounge::chat_entry_added(::LoungeChatEntry const& entry)
      {
        cout << "chat entry: " << entry.text << endl;
        string text = entry.text;
        if (text.empty())
          return;
        if (text[text.size() - 1] != '\n')
          text.append("\n");

        Glib::RefPtr<Gtk::TextBuffer::Tag> tag;
        if (   entry.with_color
            && (entry.type != LoungeChatEntry::PLAYER_FOR_ALL)) {
          Gdk::Color color;
          color.set_rgb_p(entry.color.r, entry.color.g, entry.color.b);
          tag = this->chat_messages->get_buffer()->create_tag();
          tag->property_foreground_gdk() = color;
        } else if (!entry.player.empty()) {
          tag = this->chat_messages->get_buffer()->get_tag_table()->lookup(entry.player + "::" + ::name(entry.type));
        }
        Gtk::TextBuffer::iterator iter = this->chat_messages->get_buffer()->end();

        struct tm const* const time_tm = localtime(&entry.time);
        string time_str;
        if (time_tm->tm_hour < 10)
          time_str += "0";
        time_str += DK::Utils::String::to_string(time_tm->tm_hour);
        time_str += ":";
        if (time_tm->tm_min < 10)
          time_str += "0";
        time_str += DK::Utils::String::to_string(time_tm->tm_min);
        time_str += ":";
        if (time_tm->tm_sec < 10)
          time_str += "0";
        time_str += DK::Utils::String::to_string(time_tm->tm_sec);

        text = time_str + " " + text;
        if (tag == 0) {
          iter = this->chat_messages->get_buffer()->insert(iter, text);
        } else {
        if (entry.player.empty()) {
          iter = this->chat_messages->get_buffer()->insert_with_tag(iter, text, tag);
        }
        else {
          iter = this->chat_messages->get_buffer()->insert_with_tag(iter,
                                                                    text,
                                                                    tag);
        }
        }
        // insert_with_tag(..., tag = entry.player)
        this->chat_messages->scroll_to(iter);
        return;
      } // void Lounge::chat_entry_added(::LoungeChatEntry entry)

    /**
     ** the help has changed
     **
     ** @param     text   new help text
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::help_changed(string const& text)
      {
        this->help->show(text);
        return;
      } // void Lounge::help_changed(string text)

    /**
     ** the blog has changed
     **
     ** @param     text   new blog text
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::blog_changed(string const& text)
      {
        this->blog->show(text);
        return;
      } // void Lounge::blog_changed(string text)

    /**
     ** the pin_board has changed
     **
     ** @param     text   new pin_board text
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::pin_board_changed(string const& text)
      {
        this->pin_board->show(text);
        return;
      } // void Lounge::pin_board_changed(string text)

    /**
     ** the messages has changed
     **
     ** @param     text   new messages text
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::messages_changed(string const& text)
      {
        this->messages->show(text);
        return;
      } // void Lounge::messages_changed(string text)

    /**
     ** an alert
     **
     ** @param     title   alert title
     ** @param     text    alert text
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::show_alert(string const& title, string const& text)
      {
#ifdef DKNOF
        if (title != "Willkommen FreeDoko")
#endif
          this->alert->show(title, text);
        return;
      } // void Lounge::show_alert(string title, string text)

    /**
     ** connect to the server
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::connect_server()
      {
        // connect to the DokoLounge server
        // server 1: flockendoko.dyndns.org
        // server 2: h2079177.stratoserver.net
        CLOG << endl;
        this->connection
          = ::server->create_connection("h2079177.stratoserver.net", 51401,
                                        ::Network::DOKOLOUNGE);
        CLOG << endl;
        if (this->connection == NULL) {
#ifndef RELEASE
          CLOG << "Failed to create the connection to the DokoLounge." << endl;
#endif
          return;
        }
        CLOG << endl;
        this->sender = dynamic_cast< ::Network::DokoLounge::Interpreter::Sender*>(&this->connection->interpreter().sender());
        CLOG << endl;
        if (this->sender == NULL) {
#ifndef RELEASE
          CLOG << "Failed to get the DokoLounge-sender." << endl;
#endif
          return;
        }

        { // create the signals
          //this->create_account->signal_clicked().connect(sigc::mem_fun(*this->sender, &::Network::DokoLounge::Interpreter::Sender::create_account));
          //this->login->signal_clicked().connect(sigc::mem_fun(*this->sender, &::Network::DokoLounge::Interpreter::Sender::login));
          this->logout_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::logout_signal));
#if 0
          this->refresh_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::refresh_signal));
#endif
          this->klingel_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::klingel_signal));
          this->hilfe_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::hilfe_signal));
          this->blog_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::blog_signal));
          this->pinnwand_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::pinnwand_signal));
          this->messages_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::messages_signal));
        } // create the signals
        return;
      } // void Lounge::connect_server()

    /**
     ** create an account at DokoLounge
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::create_account_signal()
      {
        // already connected?
        // todo: also check sender for already 
        if (this->sender)
          return;
        this->connect_server();

        if (this->sender == NULL)
          return ;
        this->sender->create_account();
        //this->sender->login();

        return ;
      } // void Lounge::create_account_signal()

    /**
     ** loging into DokoLounge
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::login_signal()
      {
        // already connected?
        if (this->sender)
          return;

        this->connect_server();
        if (this->sender == NULL)
          return ;
        this->sender->login();
        return ;
      } // void Lounge::login_signal()

    /**
     ** logout from DokoLounge
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::logout_signal()
      {
        if (!this->sender)
          return;

        this->sender->logout();
        ::server->delete_connection(this->connection);
        this->sender = NULL;
        this->connection = NULL;
        return;
      } // void Lounge::logout_signal()

    /**
     ** refresh
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::refresh_signal()
      {
        if (!this->sender)
          return;

        this->sender->refresh();
        return;
      } // void Lounge::refresh_signal()

    /**
     ** klingel from DokoLounge
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::klingel_signal()
      {
        if (!this->sender)
          return;

        this->sender->klingel();
        return;
      } // void Lounge::klingel_signal()

    /**
     ** hilfe from DokoLounge
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::hilfe_signal()
      {
        if (!this->sender)
          return;

        this->sender->hilfe();
        return;
      } // void Lounge::hilfe_signal()

    /**
     ** blog from DokoLounge
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::blog_signal()
      {
        if (!this->sender)
          return;

        this->sender->blog();
        return;
      } // void Lounge::blog_signal()

    /**
     ** pinnwand from DokoLounge
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::pinnwand_signal()
      {
        if (!this->sender)
          return;

        this->sender->pinnwand();
        return;
      } // void Lounge::pinnwand_signal()

    /**
     ** messages from DokoLounge
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::messages_signal()
      {
        if (!this->sender)
          return;

        this->sender->messages();
        return;
      } // void Lounge::messages_signal()

    /**
     ** the player chats the text in 'entry'
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::chat_signal()
      {
        string text = this->chat_line->get_text();
        this->sender->chat(Glib::convert(text, "latin1", "utf8"));
        this->chat_line->set_text("");
        this->chat_line->grab_focus();
        return ;
      } // void Lounge::chat_signal()

    /**
     ** the players have changed
     **
     ** @param     players    players list
     **
     ** @return    -
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **
     ** @todo      Icon
     ** @todo      beobachten
     ** @todo      Nachricht schreiben
     ** @todo      einspringen
     **/
    void
      Lounge::players_changed(vector< ::Lounge::Player> const& players)
      {
        this->players_list->resize(players.size(), 1);
        this->players_list->children().clear();

        cout << "Players: " << players.size() << "\n";
        for (unsigned i = 0; i < players.size(); ++i) {
          cout << "  " << i << ": " << players[i].name << "\n";
          this->players_list->attach(*Gtk::manage(new Gtk::Label(players[i].name)),
                                     0, 1, i, i+1
                                    );
          // set color for chat
          if (this->chat_messages->get_buffer()->get_tag_table()->lookup(players[i].name + "::" + ::name(LoungeChatEntry::PLAYER)) == 0) {
            Glib::RefPtr<Gtk::TextBuffer::Tag> tag;
            tag = this->chat_messages->get_buffer()->create_tag(players[i].name + "::" + ::name(LoungeChatEntry::PLAYER));
            tag->property_foreground_gdk() = player_colors.front();
            tag = this->chat_messages->get_buffer()->create_tag(players[i].name + "::" + ::name(LoungeChatEntry::PLAYER_WHISPER));
            tag->property_foreground_gdk() = player_colors.front();
            tag->property_style() = Pango::STYLE_ITALIC;
            tag = this->chat_messages->get_buffer()->create_tag(players[i].name + "::" + ::name(LoungeChatEntry::PLAYER_FOR_ALL));
            tag->property_foreground_gdk() = player_colors.front();
            tag->property_weight() = Pango::WEIGHT_BOLD;
            player_colors.push(player_colors.front());
            player_colors.pop();
          }
        }
        this->players_list->show_all_children();
        return ;
      } // void Lounge::players_changed(vector< ::Lounge::Player> players)

    /**
     ** the tables have changed
     **
     ** @param     tables    tables list
     **
     ** @return    -
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    void
      Lounge::tables_changed(vector< ::Lounge::Table> const& tables)
      {
        this->tables_list->resize(tables.size(), 1);
        this->tables_list->children().clear();

        cout << "Tables: " << tables.size() << "\n";
        for (unsigned i = 0; i < tables.size(); ++i) {
          cout << "  " << i << ": " << tables[i].type << "\n";
          this->tables_list->attach(*Gtk::manage(new Gtk::Label(tables[i].type)),
                                    0, 1, i, i+1
                                   );
        }
        this->tables_list->show_all_children();
        return ;
      } // void Lounge::tables_changed(vector< ::Lounge::Table> tables)

  } // namespace DokoLounge
} // namespace UI_GTKMM_NS

#endif // #ifdef USE_NETWORK_DOKOLOUNGE
#endif // #ifdef USE_NETWORK
#endif // #ifdef USE_UI_GTKMM
