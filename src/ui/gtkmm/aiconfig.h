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

#ifndef GTKMM_AICONFIG_HEADER
#define GTKMM_AICONFIG_HEADER

#include "base.h"
#include "player.h"
#include "../../player/aiconfig.h"

#include "widgets/label_type_selector.h"
#include <sigc++/object.h>
#include <gtkmm/liststore.h>
namespace Gtk {
  class VBox;
  class Notebook;
  class StockButton;
  class Label;
  class CheckButton;
  class LabelSpinButton;
  class SpinButton;
  class OptionMenu;
  class MenuItem;
  class FileChooserDialog;
  class TreeView;
  class TextView;
  class Widget;

  class FileMenu;
  class LabelCardSelector;
} // namespace Gtk

class Player;
class Aiconfig;
namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the ai configuration of one player
   **
   ** @author	Diether Knof
   **
   **/
  class Players::Player::AiConfig : public Base, public sigc::trackable {
    public:
      friend class Players;
      friend class Player;

    class HeuristicsModel : public Gtk::TreeModel::ColumnRecord {
      public:
	HeuristicsModel()
	{
	  this->add(this->active);
	  this->add(this->heuristic);
	  this->add(this->heuristic_name);
	  this->add(this->gametype_group);
	  this->add(this->playertype_group);
	}

	Gtk::TreeModelColumn<bool> active;
	Gtk::TreeModelColumn<int> heuristic;
	Gtk::TreeModelColumn<Glib::ustring> heuristic_name;
	Gtk::TreeModelColumn<int> gametype_group;
	Gtk::TreeModelColumn<int> playertype_group;
    }; // class HeuristicsModel : public Gtk::TreeModel::ColumnRecord

    public:
    AiConfig(Player* const player);
    ~AiConfig();

    ::Aiconfig& aiconfig();
    ::Aiconfig const& aiconfig_const() const;

    void aitype_change(int const type);
    void change(int const type);

    void hardcoded();
    void adopt();
    void adopt_aiconfig(unsigned const playerno);
    void allot();
    void load();
    void load_file(string const filename);
    void save();

    void sensitivity_update();
    void update(::Aiconfig const& aiconfig);
    void update();
    void name_update();
    void difficulty_description_update();
    void language_update();

    public:
    void init();

    private:
    void create_container();
    void
      add_heuristic_treeview(Gtk::Notebook* notebook,
			     HeuristicsMap::GametypeGroup const
			     gametype_group,
			     HeuristicsMap::PlayerTypeGroup const
			     playertype_group,
			     Translator::Translation const& label_text);
    void
      add_heuristic_treeviews_re_contra(Gtk::Notebook* notebook,
					HeuristicsMap::GametypeGroup const
					gametype_group);
    void
      add_heuristic_treeviews_special_re_contra(Gtk::Notebook* notebook,
						HeuristicsMap::GametypeGroup const
						gametype_group);
    Gtk::Widget*
      create_heuristic_treeview(HeuristicsMap::GametypeGroup const
				gametype_group,
				HeuristicsMap::PlayerTypeGroup const
				playertype_group);
    void
      init_heuristic_treeview(Gtk::Box* const box,
                              HeuristicsMap::GametypeGroup const
                              gametype_group,
                              HeuristicsMap::PlayerTypeGroup const
                              playertype_group);


    void difficulty_changed();
    void aitype_changed(unsigned const trickno);
    void rating_changed(unsigned const trickno);
    void future_limit_changed(unsigned const trickno);
    void aitype_as_previous_changed(unsigned const trickno);

    void heuristics_row_changed_event(Gtk::TreeModel::Path const& path,
                                      Gtk::TreeModel::iterator const& row);
    void heuristics_row_deleted_event(Gtk::TreeModel::Path const& path,
                                      Glib::RefPtr<Gtk::ListStore> store);

    void update_heuristics_default_button_state(HeuristicsMap::Key const& key);
    void heuristics_default_button_toggled_event(HeuristicsMap::Key const key);
    void update_heuristic_up_down_buttons(HeuristicsMap::Key const& key);
    void update_heuristic_description(HeuristicsMap::Key const& key);

    void heuristic_move_up_event(HeuristicsMap::Key const key);
    void heuristic_move_down_event(HeuristicsMap::Key const key);

    private:
    Player* const player;

    protected:
    Gtk::VBox* container;
    Gtk::Notebook* notebook;
    vector<Gtk::Notebook*> notebooks;

    Gtk::Container* difficulty_container;
    Gtk::Label* difficulty_label;
    Gtk::StockButton* difficulty_button;
    Gtk::LabelTypeSelector< ::Aiconfig::Difficulty>* difficulty_selector;
    Gtk::StockButton* adopt_button;
    Gtk::StockButton* allot_button;
    Gtk::StockButton* load_button;
    Gtk::StockButton* save_button;
    Gtk::FileChooserDialog* save_file_chooser;

    vector<Gtk::OptionMenu*> aitype;
    vector<Gtk::OptionMenu*> rating;
    vector<Gtk::SpinButton*> future_limit;
    vector<Gtk::CheckButton*> aitype_as_previous;
    vector<Gtk::CheckButton*> type_bool;
    vector<Gtk::LabelSpinButton*> type_int;
    vector<Gtk::LabelCardSelector*> type_card;


    std::map<HeuristicsMap::Key, Gtk::ToggleButton*> heuristics_default_button;
    HeuristicsModel* heuristics_model;
    std::map<HeuristicsMap::Key, Glib::RefPtr<Gtk::ListStore> > heuristics_list;
    std::map<HeuristicsMap::Key, Gtk::TreeView*> heuristics_treeview;
    std::map<HeuristicsMap::Key, Gtk::StockButton*> heuristic_up_button;
    std::map<HeuristicsMap::Key, Gtk::StockButton*> heuristic_down_button;
    std::map<HeuristicsMap::Key, Gtk::TextView*> heuristic_description;
    bool heuristics_signals_active;

    private: // unused
    AiConfig();
    AiConfig(AiConfig const& aiconfig);
    AiConfig& operator=(AiConfig const& aiconfig);
  }; // class Players::Player::AiConfig : public Base, public Gtk::Object

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_AICONFIG_HEADER

#endif // #ifdef USE_UI_GTKMM
