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

#ifndef GTKMM_NETWORK_HEADER
#define GTKMM_NETWORK_HEADER

#include "base.h"

#include "../../network/connection.h"

class Rule;

#include "widgets/sticky_dialog.h"
#include <gtkmm/liststore.h>
#include <gtkmm/treemodel.h>
namespace Gtk {
  class TreeView;
  class CellRenderer;
  class CheckButton;
  class StockButton;
  class LabelSpinButton;
  class LabelEntry;
  class Entry;
  class Container;
} // namespace Gtk

namespace Network {
  class Listener;
  class Connection;
} // namespace Network

namespace UI_GTKMM_NS {
  /**
   ** @brief	the network dialog
   **
   ** @author	Diether Knof
   **
   ** @todo	type of connection
   ** @todo	accept connections
   ** @todo	kick connections
   ** @todo	save connection
   ** @todo	direct sending of messages
   **/
  class Network : public Base, public Gtk::StickyDialog {
    public:
      // how long an urgency hint is shown (in milisec)
      static unsigned const URGENCY_HINT_TIMEOUT;

      // all information of a connection
      class ConnectionModel : public Gtk::TreeModel::ColumnRecord {
	public:
	  ConnectionModel();

	  Gtk::TreeModelColumn< ::Network::Connection*> connection;
	  Gtk::TreeModelColumn<Glib::ustring> name;
	  Gtk::TreeModelColumn<Glib::ustring> address;
	  Gtk::TreeModelColumn<unsigned> port;
	  Gtk::TreeModelColumn<Glib::ustring> relation;
	  Gtk::TreeModelColumn<Glib::ustring> type;
	  Gtk::TreeModelColumn<Glib::ustring> found;
	  Gtk::TreeModelColumn<Glib::ustring> color;
	  Gtk::TreeModelColumn<bool> accept;
	  Gtk::TreeModelColumn<Glib::ustring> incoming_bytes;
	  Gtk::TreeModelColumn<Glib::ustring> outgoing_bytes;
      }; // class ConnectionModel : public Gtk::TreeModel::ColumnRecord

      // the listeners
      class ListenersModel : public Gtk::TreeModel::ColumnRecord {
	public:
	  ListenersModel();

	  Gtk::TreeModelColumn<int> port;
      }; // class ListenersModel : public Gtk::TreeModel::ColumnRecord

      // the local address
      class LocalAddressesModel : public Gtk::TreeModel::ColumnRecord {
	public:
	  LocalAddressesModel();

	  Gtk::TreeModelColumn<Glib::ustring> name;
      }; // class LocalAddressesModel : public Gtk::TreeModel::ColumnRecord

      // the difference of the rules
      class RuleDiffsModel : public Gtk::TreeModel::ColumnRecord {
	public:
	  RuleDiffsModel();

	  Gtk::TreeModelColumn<Glib::ustring> name;
	  Gtk::TreeModelColumn<Glib::ustring> self_value;
	  Gtk::TreeModelColumn<Glib::ustring> other_value;
      }; // class RuleDiffsModel : public Gtk::TreeModel::ColumnRecord

    public:
      Network(Base* const parent);

      ~Network();

      // the party has opened
      void party_open();

      // a new listener was created
      void network_listener_new(::Network::Listener const&
				listener);
      // a listener was deleted
      void network_listener_deleted(::Network::Listener const&
				    listener);
      // a new connection was established
      void network_connection_new(::Network::Connection const&
				  connection);
      // a connection has changed
      void network_connection_changed(::Network::Connection const&
				      connection);
      // a connection is deleted
      void network_connection_deleted(::Network::Connection const&
				      connection);
      // there was traffic over the connection
      void traffic(::Network::Connection const& connection,
		   ::Network::TrafficType const traffic_type,
		   string const& text);

      // update the rule
      void rule_update(int const type);
    private:
      void init();
      void update_sensitivity();
      void recreate_connections_list();
      void recreate_listeners_list();
      void recreate_local_addresses_list();
      void recreate_rule_differences_list();

      void reset_port_event();

      void new_connection_address_change_event();
      void connect_event();
      void send_event();

      void create_listener_event();

      void clear_event();
      void synchronize_event();

      void update_traffic(::Network::Connection const& connection);

      // stop the urgency hint
      bool stop_urgency_hint();

    public:
      void language_update();

    private:
      Gtk::Container* network_container;

      ConnectionModel* connection_model;
      Glib::RefPtr<Gtk::ListStore> connection_list;
      list<Gtk::TreeModel::Row> connection_rows;
      Gtk::TreeView* connection_treeview;

      Gtk::Container* new_connection_container;
      Gtk::LabelEntry* new_connection_address;
      Gtk::LabelSpinButton* new_connection_port;
      Gtk::StockButton* connect;

      ListenersModel* listeners_model;
      Glib::RefPtr<Gtk::ListStore> listeners_list;
      Gtk::TreeView* listeners_treeview;

      Gtk::Container* new_listener_container;
      Gtk::Button* create_listener;
      Gtk::LabelSpinButton* listener_port;
      Gtk::StockButton* standard_listener_port;

      LocalAddressesModel* local_addresses_model;
      Glib::RefPtr<Gtk::ListStore> local_addresses_list;
      Gtk::TreeView* local_addresses_treeview;

      Gtk::Label* rule_diffs_label;
      RuleDiffsModel* rule_diffs_model;
      Glib::RefPtr<Gtk::ListStore> rule_diffs_list;
      Gtk::TreeView* rule_diffs_treeview;

      Gtk::Entry* sender;

      Gtk::StockButton* clear_button;
      Gtk::StockButton* synchronize_button;

      Rule* rule_backup;

    private: // unused
      Network();
      Network(Network const&);
      Network& operator=(Network const&);
  }; // class Network : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_NETWORK_HEADER

#endif // #ifdef USE_NETWORK
#endif // #ifdef USE_UI_GTKMM
