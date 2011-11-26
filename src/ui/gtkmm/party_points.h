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

#ifndef GTKMM_PARTY_POINTS_HEADER
#define GTKMM_PARTY_POINTS_HEADER

#include "base.h"
#include "widgets/sticky_dialog.h"

#include "../../basistypes.h"

class Party;
class GameSummary;

#include <gtkmm/treestore.h>
namespace Gtk {
  class Notebook;
  class TreeView;
  class CellRenderer;
  class CheckButton;
  class RadioButton;
  class StockButton;
} // namespace Gtk
class Player;

namespace UI_GTKMM_NS {
  class GameSummary;

  /**
   **
   ** @brief	the party points window
   **
   ** Note: the first row in each round
   ** either shows the current game (expanded round)
   ** or shows the point sum of the round (collapsed)
   **
   ** @author	Diether Knof
   **
   ** @todo	bold number for the startplayer
   ** @todo	lines between the columns
   ** @todo	graphical representation (graph)
   **
   **/
  class PartyPoints : public Base, public Gtk::StickyDialog {
    class GameOverview;
    friend class GameOverview;
    class Graph;

    class PartyPointsModel : public Gtk::TreeModel::ColumnRecord {
      public:
	PartyPointsModel(unsigned const playerno);

	Gtk::TreeModelColumn<Glib::ustring> empty;
	Gtk::TreeModelColumn<unsigned> round;
	Gtk::TreeModelColumn<Glib::ustring> round_str;
	Gtk::TreeModelColumn<Glib::ustring> round_color;
	Gtk::TreeModelColumn<unsigned> gameno;
	Gtk::TreeModelColumn<Glib::ustring> gameno_str;
	vector<Gtk::TreeModelColumn<Glib::ustring> > playerpoints;
	Gtk::TreeModelColumn<int> gamepoints;
	Gtk::TreeModelColumn<Glib::ustring> gamepoints_str;
	Gtk::TreeModelColumn<int> bock_multiplier;
	Gtk::TreeModelColumn<Glib::ustring> bock_multiplier_str;
#ifdef POSTPONED
	Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > gametype;
#else
	Gtk::TreeModelColumn<Glib::ustring> gametype;
#endif
    }; // class PartyPointsModel : public Gtk::TreeModel::ColumnRecord

    public:
    PartyPoints(Base* const parent);
    ~PartyPoints();

    void party_open();
    void party_start_round(unsigned const round);
    void party_finish();
    void party_close();
    void game_start();
    void game_finished();

    void name_changed(Player const& player);
    void language_update();

    protected:
    void clear_selection();

    private:
    void init();
    public:
    void recalc_all();
    void recreate_all();
    private:
    void add_game(unsigned const gameno, bool const update = true);
    void set_points(unsigned const gameno);
    void update_round(unsigned const round);
    void update_sum();
    void update_duty_soli();
    void update_player_columns_size();

    void add_future_bock_multipliers();
    void add_row(unsigned const gameno);
    void row_selection_changed();
    void sum_columns_size_update();
    void show_game_overview_toggled();
    void collaps_setting_changed();

    void row_collapsed_or_expanded(Gtk::TreeModel::iterator const& iter,
				   Gtk::TreeModel::Path const& path);
    void set_cell_color(Gtk::CellRenderer* cell_renderer,
			Gtk::TreeModel::iterator const& iter,
			unsigned const column);
    Gdk::Color color(::GameSummary const& game_summary);


    bool on_key_press_event(GdkEventKey* key);

    private: // elements
    public:
    Gtk::Notebook* notebook;
    private:
    //TrickpointsModel party_points_model;
    PartyPointsModel* party_points_model;
    Glib::RefPtr<Gtk::TreeStore> party_points_list;
    Gtk::TreeView* party_points_treeview;
    vector<Gtk::TreeModel::Row> round_rows;
    vector<Gtk::TreeModel::Row> game_rows;

    Gtk::TreeView* party_points_sum_treeview;

    Glib::RefPtr<Gtk::TreeStore> duty_soli_list;
    Gtk::TreeModel::Row duty_free_soli_row;
    Gtk::TreeModel::Row duty_color_soli_row;
    Gtk::TreeModel::Row duty_picture_soli_row;
    Gtk::TreeView* duty_soli_treeview;

    protected:
    Gtk::CheckButton* show_game_overview_button;
    private:
    Gtk::CheckButton* add_up_points;

    Gtk::RadioButton* collapse_rounds;
    Gtk::RadioButton* expand_rounds;

    Gtk::StockButton* close_button;

    GameOverview* game_overview;
    Graph* graph;

    Gtk::Label* remaining_rounds_label;
    Gtk::Label* remaining_points_label;

    private: // unused
    PartyPoints();
    PartyPoints(PartyPoints const& party_points);
    PartyPoints& operator=(PartyPoints const& party_points);
  }; // class PartyPoints : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_PARTY_POINTS_HEADER

#endif // #ifdef USE_UI_GTKMM
