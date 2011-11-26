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
#ifdef USE_NETWORK

#ifndef GTKMM_NETWORK_LOG_HEADER
#define GTKMM_NETWORK_LOG_HEADER

#include "base.h"
#include "widgets/sticky_dialog.h"
#include <gtkmm/liststore.h>
namespace Gtk {
  class TreeView;
  class CellRenderer;
} // namespace Gtk

namespace Network {
  class Connection;
}
#include "../../network/types.h"

namespace UI_GTKMM_NS {
  /**
   ** @brief	the network log
   **
   ** @author	Diether Knof
   **
   ** @todo	colorize the different connections
   ** @todo	restrictions: in/out, per connection
   **/
  class NetworkLog : public Base, public Gtk::StickyDialog {
    class LogModel : public Gtk::TreeModel::ColumnRecord {
      public:
	LogModel();

	Gtk::TreeModelColumn<Glib::ustring> time;
	Gtk::TreeModelColumn<Glib::ustring> address;
	Gtk::TreeModelColumn<Glib::ustring> direction;
	Gtk::TreeModelColumn<Glib::ustring> text;
    }; // class LogModel : public Gtk::TreeModel::ColumnRecord

    public:
      NetworkLog(Base* const parent);

      ~NetworkLog();

    private:
      void init();

    public:
      void reset_port_event();

      void add_traffic(::Network::Connection const& connection,
		       ::Network::TrafficType const traffic_type,
		       string const& text);
      void language_update();

    private:
      LogModel* log_model;
      Glib::RefPtr<Gtk::ListStore> log_list;
      Gtk::TreeView* log_treeview;

    private: // unused
      NetworkLog();
      NetworkLog(NetworkLog const&);
      NetworkLog& operator=(NetworkLog const&);
  }; // class NetworkLog : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_NETWORK_LOG_HEADER

#endif // #ifdef USE_NETWORK
#endif // #ifdef USE_UI_GTKMM
