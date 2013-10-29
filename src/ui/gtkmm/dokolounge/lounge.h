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
#ifdef USE_NETWORK
#ifdef USE_NETWORK_DOKOLOUNGE

#ifndef GTKMM_DOKOLOUNGE_HEADER
#define GTKMM_DOKOLOUNGE_HEADER

#include "../base.h"
#include "../widgets/sticky_dialog.h"
namespace Gtk {
  class Label;
  class StockButton;
}; // namespace Gtk
#include "../../../network/DokoLounge/sender.h"

namespace UI_GTKMM_NS {
  namespace DokoLounge {
  class Lounge : public Base, public Gtk::StickyDialog {
    public:
      // constructor
      Lounge(Base* const parent);
      // destructor
      ~Lounge();

    private:
      // initialize the Lounge
      void init();

      // close the window
      void close();

      // connect to the server
      void connect_server();
      // connect to the server
      void create_account_signal();
      // connect to the server
      void login_signal();

      // logout
      void logout_signal();
      // klingel
      void klingel_signal();
      // hilfe
      void hilfe_signal();
      // blog
      void blog_signal();
      // pinnwand
      void pinnwand_signal();
      // mail
      void mail_signal();

    private:
      ::Network::Connection* connection;
      ::Network::DokoLounge::Interpreter::Sender* sender;

      Gtk::Label* name;
      Gtk::StockButton* create_account_button;
      Gtk::StockButton* login_button;
      Gtk::StockButton* logout_button;
      Gtk::StockButton* klingel_button;
      Gtk::StockButton* hilfe_button;
      Gtk::StockButton* blog_button;
      Gtk::StockButton* pinnwand_button;
      Gtk::StockButton* mail_button;

      /*
         Hilfe
         Blog
         Globale Pinnwand
         Persönliche Pinnwand
         Spielerliste
         Tischliste
         Rangliste
         Chat
         */

      // close button
      Gtk::StockButton* close_button;

    private: // unused
      Lounge();
      Lounge(Lounge const&);
      Lounge& operator=(Lounge const&);
  }; // class Lounge
  } // namespace DokoLounge
} // namespace UI_GTKMM_NS

#endif // #ifndef GTKMM_DOKOLOUNGE_HEADER
#endif // #ifdef USE_NETWORK_DOKOLOUNGE
#endif // #ifdef USE_NETWORK
#endif // #ifdef USE_UI_GTKMM
