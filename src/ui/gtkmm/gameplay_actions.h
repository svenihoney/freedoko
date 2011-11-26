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

#ifndef GTKMM_GAMEPLAY_ACTIONS_HEADER
#define GTKMM_GAMEPLAY_ACTIONS_HEADER

#include "base.h"

#include "../../game/gameplay_action.h"
#include "../../player/player.h"

#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>

namespace UI_GTKMM_NS {

  /**
   ** A list store containing the gameplay actions from a vector
   **/
  class GameplayActions : public Base, public Gtk::TreeView {
    public:
      // the colors for the action type
      static pair<string, string>
	colors(::GameplayAction::Type const action_type,
	       ::GameplayAction::Discrepancy const discrepancy);

    public:
      /**
       ** @brief	the columns of game actions
       **
       ** @author	Diether Knof
       **/
      class GameplayActionModel : public Gtk::TreeModel::ColumnRecord {
	public:
	  GameplayActionModel()
	  {
	    this->add(this->type);
	    this->add(this->player);
	    this->add(this->data);
	    this->add(this->action);
	    this->add(this->no);
	  }

	  Gtk::TreeModelColumn<Glib::ustring> type;
	  Gtk::TreeModelColumn<Glib::ustring> player;
	  Gtk::TreeModelColumn<Glib::ustring> data;
	  Gtk::TreeModelColumn<GameplayAction const*> action;
	  Gtk::TreeModelColumn<unsigned> no;
      }; // class GameplayActionModel : public Gtk::TreeModel::ColumnRecord

    public:
      // constructor
      GameplayActions(Base* const parent);
      // destructor
      ~GameplayActions();

      void update();

      void set_actions(vector< ::GameplayAction const*> const& actions);
      void set_discrepancies(vector< ::GameplayAction::Discrepancy> const&
			     discrepancies);
      void remove_actions();
      void set_current_action_no(unsigned const current_action_no);

      void name_changed(Player const& player);
      void language_update();

      void set_cell_color(Gtk::CellRenderer* cell_renderer,
			  Gtk::TreeModel::iterator const& row);
    private:
      void init();

    private:
      // the actions
      vector< ::GameplayAction const*> const* actions;
      // the discrepancy
      vector< ::GameplayAction::Discrepancy> const* discrepancies;
      // the number of the current action
      unsigned current_action_no;

    public:
      // the model
      GameplayActionModel model;
    private:
      // the list
      Glib::RefPtr<Gtk::ListStore> list;
  }; // class GameplayActions

} // namespace UI_GTKMM_NS

#endif // #ifndef GTKMM_GAMEPLAY_ACTIONS_HEADER

#endif // #ifdef USE_UI_GTKMM
