/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2012  by Diether Knof and Borg Enders
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

#ifndef GTKMM_PARTY_SUMMARY_HEADER
#define GTKMM_PARTY_SUMMARY_HEADER

#include "base.h"
#include "widgets/sticky_dialog.h"
#include <gtkmm/liststore.h>

class Player;

namespace Gtk {
  class TreeView;
  class StockButton;
  class Label;
} // namespace Gtk

namespace UI_GTKMM_NS {

  /**
   ** @brief	the summary of a party
   ** The summary is shown, when a party has been loaded.
   ** The summary contains:
   **   players
   **     points
   **     duty soli
   **   remaining points
   **   remaining rounds
   **   gamenumber
   **   next startplayaer
   **
   ** @author	Diether Knof
   **/
  class PartySummary : public Base, public Gtk::StickyDialog {
  /**
   ** @brief	the columns of the players table
   **
   ** @author	Diether Knof
   **/
    class PlayersSummaryModel : public Gtk::TreeModel::ColumnRecord {
      public:
	PlayersSummaryModel(unsigned const playerno) :
	  name(), player(playerno)
	{
	  this->add(this->name);
	  for (unsigned p = 0; p < playerno; ++p)
	    this->add(this->player[p]);
	}

	Gtk::TreeModelColumn<Glib::ustring> name;
	vector<Gtk::TreeModelColumn<int> > player;
    }; // class PlayersSummartyTable : public Gtk::TreeModel::ColumnRecord

    public:
    PartySummary(Base* const parent);
    ~PartySummary();

    void party_loaded();
    void on_show();

    void name_update(Player const& player);
    void language_update();

    private:
    void init();
    void update();

    void continue_party_event();
    void new_party_event();

    private:
    PlayersSummaryModel* player_model;
    Glib::RefPtr<Gtk::ListStore> player_list;
    Gtk::TreeView* player_treeview;

    Gtk::Label* roundnumber;
    Gtk::Label* gamenumber;
    Gtk::Label* remaining_rounds;
    Gtk::Label* remaining_points;
    Gtk::Label* next_startplayer;

    Gtk::StockButton* continue_party_button;
    Gtk::StockButton* new_party_button;

    private: // unused
    PartySummary();
    PartySummary(PartySummary const& game_summary);
    PartySummary& operator=(PartySummary const& game_summary);
  }; // class PartySummary : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_PARTY_SUMMARY_HEADER

#endif // #ifdef USE_UI_GTKMM
