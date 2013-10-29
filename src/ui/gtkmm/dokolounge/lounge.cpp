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

#include "../ui.h"
#include "../translations.h"
#include "../../../misc/setting.h"

#include "../../../network/server.h"
#include "../../../network/connection.h"
#include "../../../network/DokoLounge/sender.h"

#include "../widgets/stock_button.h"
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/alignment.h>

// todo: use dummy connection (text output) for debugging

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
      name(NULL),
      create_account_button(NULL),
      login_button(NULL),
      logout_button(NULL),
      klingel_button(NULL),
      hilfe_button(NULL),
      blog_button(NULL),
      pinnwand_button(NULL),
      mail_button(NULL),
      close_button(NULL)
    {
      this->ui->add_window(*this);
      this->signal_realize().connect(sigc::mem_fun(*this, &Lounge::init));

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
     ** @version   0.7.3
     **/
    Lounge::~Lounge()
    { 
      if (this->connection != NULL) {
        this->sender->logout();
        ::server->delete_connection(this->connection);
        delete this->connection;
        this->connection = NULL;
      }
    } // Lounge::~Lounge()

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
        this->ui->translations->add(*this, ::translation("FreeDoko %ttitle%",
                                                         ::translation("DokoLounge: Lounge")));
        this->set_icon(this->ui->icon);

        this->set_skip_taskbar_hint();

        { // action area
          this->close_button
            = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
          this->ui->translations->add(*close_button,
                                      ::translation("close"));
          this->add_action_widget(*close_button, Gtk::RESPONSE_CLOSE);
          close_button->show_all();
        } // action area

        { // main area
          Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 0));

          this->name
            = Gtk::manage(new Gtk::Label("name: Name"));
          this->ui->translations->add(*this->name,
                                      ::translation("Name: %sname%",
                                                    ::setting(Setting::DOKOLOUNGE_NAME)));
          vbox->add(*this->name);

          this->create_account_button
            = Gtk::manage(new Gtk::StockButton("create account"));
          this->ui->translations->add(*this->create_account_button,
                                      ::translation("create account"));
          vbox->add(*this->create_account_button);

          this->login_button
            = Gtk::manage(new Gtk::StockButton("login"));
          this->ui->translations->add(*this->login_button,
                                      ::translation("login"));
          vbox->add(*this->login_button);

          this->logout_button
            = Gtk::manage(new Gtk::StockButton("logout"));
          this->ui->translations->add(*this->logout_button,
                                      ::translation("logout"));
          vbox->add(*this->logout_button);

          this->klingel_button
            = Gtk::manage(new Gtk::StockButton("klingel"));
          this->ui->translations->add(*this->klingel_button,
                                      ::translation("klingel"));
          vbox->add(*this->klingel_button);

          this->hilfe_button
            = Gtk::manage(new Gtk::StockButton("hilfe"));
          this->ui->translations->add(*this->hilfe_button,
                                      ::translation("hilfe"));
          vbox->add(*this->hilfe_button);

          this->blog_button
            = Gtk::manage(new Gtk::StockButton("blog"));
          this->ui->translations->add(*this->blog_button,
                                      ::translation("blog"));
          vbox->add(*this->blog_button);

          this->pinnwand_button
            = Gtk::manage(new Gtk::StockButton("pinnwand"));
          this->ui->translations->add(*this->pinnwand_button,
                                      ::translation("pinnwand"));
          vbox->add(*this->pinnwand_button);

          this->mail_button
            = Gtk::manage(new Gtk::StockButton("mail"));
          this->ui->translations->add(*this->mail_button,
                                      ::translation("mail"));
          vbox->add(*this->mail_button);

          this->get_vbox()->add(*vbox);
        } // main area

        { // signals
          // this->signal_show().connect(sigc::mem_fun(*this, &Network::update_sensitivity));
          this->close_button->signal_clicked().connect(sigc::mem_fun(*this, &Gtk::Widget::hide));
          this->create_account_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::create_account_signal));
          this->login_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::login_signal));
        } // signals

        { // the text
          this->language_update();
        } // the text

        this->show_all_children();

        return ;
      } // void Lounge::init()

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
        this->connection
          = ::server->create_connection("h2079177.stratoserver.net", 51401,
                                        ::Network::DOKOLOUNGE);
        if (this->connection == NULL) {
#ifndef RELEASE
          CLOG << "Failed to create the connection to the DokoLounge." << endl;
#endif
          return;
        }
        this->sender = dynamic_cast< ::Network::DokoLounge::Interpreter::Sender*>(&this->connection->interpreter().sender());
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
          this->klingel_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::klingel_signal));
          this->hilfe_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::hilfe_signal));
          this->blog_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::blog_signal));
          this->pinnwand_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::pinnwand_signal));
          this->mail_button->signal_clicked().connect(sigc::mem_fun(*this, &Lounge::mail_signal));
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
     ** mail from DokoLounge
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
      Lounge::mail_signal()
      {
        if (!this->sender)
          return;

        this->sender->mail();
        return;
      } // void Lounge::mail_signal()

  } // namespace DokoLounge
} // namespace UI_GTKMM_NS

#endif // #ifdef USE_NETWORK_DOKOLOUNGE
#endif // #ifdef USE_NETWORK
#endif // #ifdef USE_UI_GTKMM
