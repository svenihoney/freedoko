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

#ifndef GTKMM_GAME_SUMMARY_HEADER
#define GTKMM_GAME_SUMMARY_HEADER

#include "base.h"
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>

class Player;
class GameSummary;

namespace Gtk {
  class Label;
  class TreeView;
} // namespace Gtk

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the summary of a game
   **
   ** @author	Diether Knof
   **
   **/
  class GameSummary : public Base, public Gtk::HBox {

    /**
     ** @brief	the columns of the trickpoints table
     **
     ** @author	Diether Knof
     **/
    class TrickpointsModel : public Gtk::TreeModel::ColumnRecord {
      public:
	TrickpointsModel()
	{
	  this->add(this->name);
	  this->add(this->points);
	  this->add(this->team);
	  this->add(this->game_points);
	  this->add(this->party_points);
	}

	Gtk::TreeModelColumn<Glib::ustring> name;
	Gtk::TreeModelColumn<unsigned> points;
	Gtk::TreeModelColumn<Glib::ustring> team;
	Gtk::TreeModelColumn<Glib::ustring> game_points;
	Gtk::TreeModelColumn<int> party_points;
    }; // class TrickpointsModel : public Gtk::TreeModel::ColumnRecord

    /**
     ** @brief	the columns of the special points table
     **
     ** @author	Diether Knof
     **/
    class PointsDescriptionModel : public Gtk::TreeModel::ColumnRecord {
      public:
	PointsDescriptionModel()
	{
	  this->add(this->team);
	  this->add(this->player);
#ifdef OUTDATED
	  this->add(this->sign);
#endif
	  this->add(this->points);
	  this->add(this->description);
	}

	Gtk::TreeModelColumn<Glib::ustring> team;
	Gtk::TreeModelColumn<Glib::ustring> player;
#ifdef OUTDATED
	Gtk::TreeModelColumn<Glib::ustring> sign;
	Gtk::TreeModelColumn<int> points;
#endif
	Gtk::TreeModelColumn<Glib::ustring> points;
	Gtk::TreeModelColumn<Glib::ustring> description;
    }; // class PointsDescriptionModel : public Gtk::TreeModel::ColumnRecord

    /**
     ** @brief	the columns of the bock triggers table
     **
     ** @author	Diether Knof
     **/
    class BockTriggersModel : public Gtk::TreeModel::ColumnRecord {
      public:
	BockTriggersModel()
	{ this->add(this->name); }

	Gtk::TreeModelColumn<Glib::ustring> name;
    }; // class BockTriggersModel : public Gtk::TreeModel::ColumnRecord

    public:
    GameSummary(Base* const parent);
    ~GameSummary();

    void set_gameno(unsigned const gameno_a);
    void set_game_summary(::GameSummary const& game_summary);
    void update();

    void name_changed(Player const& player);
    void language_update();

    private:
    void init();

    private:
    unsigned gameno;
    ::GameSummary const* game_summary;

    Gtk::Label* gamenumber;
    Gtk::Label* seed;
    Gtk::Label* startplayer;
    Gtk::Label* type;
    Gtk::Label* winner;
    TrickpointsModel trickpoints_model;
    Glib::RefPtr<Gtk::ListStore> trickpoints_list;
    Gtk::TreeView* trickpoints_treeview;
    PointsDescriptionModel points_description_model;
    Glib::RefPtr<Gtk::ListStore> points_description_list;
    Gtk::TreeView* points_description_treeview;
    BockTriggersModel bock_triggers_model;
    Glib::RefPtr<Gtk::ListStore> bock_triggers_list;
    Gtk::TreeView* bock_triggers_treeview;

    private: // unused
    GameSummary();
    GameSummary(GameSummary const& game_summary);
    GameSummary& operator=(GameSummary const& game_summary);
  }; // class GameSummary : public Base, public Gtk::VBox

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_GAME_SUMMARY_HEADER

#endif // #ifdef USE_UI_GTKMM
