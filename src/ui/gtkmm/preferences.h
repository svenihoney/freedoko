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

#ifndef GTKMM_PREFERENCES_HEADER
#define GTKMM_PREFERENCES_HEADER

#include "base.h"
#include "widgets/sticky_dialog.h"
#include <gtkmm/notebook.h>
namespace Gtk {
  class Widget;
  class Notebook;
  class Label;
  class Alignment;
  class Button;
  class StockButton;
  class CheckButton;
  class RadioButton;
  class LabelSpinButton;
  class Entry;
  class Image;
  class FontSelectionDialog;
  class ColorSelectionDialog;
  class Menu;

  class FileMenu;
} // namespace Gtk

#include "../../misc/setting.cardsorder.h"
class Setting;

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the settings dialog
   **
   ** @author	Diether Knof
   **
   ** @todo	update of the cardset in 'cards order'
   ** @todo	change the style of the fonts and colors buttons
   ** @todo	alpha-channel for the colors
   ** @todo	on-the-fly changing of the fonts
   **
   **/
  class Preferences : public Base, public Gtk::StickyDialog {
    public:
      Preferences(Base* const parent);
      ~Preferences();

      void create_backup();
      Setting const& backup() const;

      void reset();
      void save();

      void sensitivity_update();
      void update_all();
      void update_cards_order();
      void setting_update(int const type);
      void update(int const type, bool const update_sensitivity = true);
      void change(int const type);
      bool focus_out_event(GdkEventFocus*, int const type);

      void show_cards_order();
      void cards_order_direction_set(unsigned const pos);
      void cards_order_pos_to_left(unsigned const pos);
      void cards_order_pos_to_right(unsigned const pos);
      void cards_order_set_sorted();
      void cards_order_mix();

      void language_menu_create(bool const popup);
      void language_selected(string const language);
      void background_menu_popup();
      void background_selected(string const background);
      void cardset_selected(string const cardset);
      void cards_back_selected(string const cards_back);
      
      void font_accepted(Gtk::FontSelectionDialog* const font_selector);
      void font_declined(Gtk::FontSelectionDialog* const font_selector);
      void color_accepted(Gtk::ColorSelectionDialog* const color_selector);
      void color_declined(Gtk::ColorSelectionDialog* const color_selector);

    private:
      void init();
      void init_cardsorder();

      Gtk::VBox* add_group_vbox(string const& group_name);
      Gtk::Notebook* add_group_notebook(string const& group_name);
      void add_group(string const& group_name, Gtk::Widget& widget);
      Gtk::VBox* add_subgroup_vbox(Gtk::Notebook& subgroup_notebook,
				   string const& subgroup_name);

#ifndef RELEASE
      void update_translation();
#endif

      bool on_key_press_event(GdkEventKey* key);
      void group_notebook_page_changed(GtkNotebookPage* page, guint pageno);

    public:
      Gtk::StockButton* reset_button;
      Gtk::StockButton* close_button;

      Gtk::Notebook* group_notebook;

      vector<Gtk::CheckButton*> type_bool;
      vector<Gtk::LabelSpinButton*> type_unsigned;
      vector<Gtk::Widget*> type_string;
      vector<Gtk::Label*> type_string_label;

      vector<Gtk::Image*> sorting_tcolor_image;
      vector<Gtk::RadioButton*> sorting_up_button;
      vector<Gtk::RadioButton*> sorting_down_button;
      Gtk::RadioButton* sorted_button;
      Gtk::RadioButton* unsorted_button;
      Gtk::Button* mix_cards_button;

      Gtk::Menu* language_menu;
      Gtk::FileMenu* background_menu;
      Gtk::FileMenu* cardset_menu;
      Gtk::FileMenu* cards_back_menu;
      vector<Gtk::FontSelectionDialog*> font_selector;
      vector<Gtk::ColorSelectionDialog*> color_selector;

#ifndef RELEASE
      Gtk::Button* update_translation_button;
#endif

    private:
      Setting* backup_;

    private: // unused
      Preferences();
      Preferences(Preferences const&);
      Preferences& operator=(Preferences const&);
  }; // class Preferences : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_PREFERENCES_HEADER

#endif // #ifdef USE_UI_GTKMM
