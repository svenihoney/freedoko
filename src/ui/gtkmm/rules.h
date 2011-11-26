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

#ifndef GTKMM_RULES_HEADER
#define GTKMM_RULES_HEADER

#include "base.h"

#include "widgets/sticky_dialog.h"
#include <gtkmm/notebook.h>
namespace Gtk {
  class Label;
  class StockButton;
  class RadioButton;
  class CheckButton;
  class LabelSpinButton;
  class VBox;
  class Notebook;
  class FileChooserDialog;
  class Image;
} // namespace Gtk

class Rule;

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the rules dialog
   **
   ** @author	Diether Knof
   **
   ** @todo	use a treeview
   **
   **/
  class Rules : public Base, public Gtk::StickyDialog {
    public:
      Rules(Base* const parent);
      ~Rules();

      void create_backup();
      Rule const& backup() const;

      void reset();
      void load();
      void load_file(string const filename);
      void save();

      void sensitivity_update();
      void update(int const type, bool const update_sensitivity = true);
      void change(int const type);

      void update_all();

    private:
      void init();

      // adjust the announcement times to the game with nines
      void adjust_to_with_nines();
      // adjust the announcement times to the game without nines
      void adjust_to_without_nines();

      Gtk::VBox* add_group(string const& name, Gtk::Image* image = NULL);
      void group_notebook_page_changed(GtkNotebookPage* page, guint pageno);

      // a key was pressed
      bool on_key_press_event(GdkEventKey* key);

      // update the announcement text (because of knocking)
      void update_announcement_text();

    public:
      vector<Gtk::CheckButton*> type_bool;
      vector<Gtk::LabelSpinButton*> type_unsigned;
      vector<Gtk::RadioButton*> counting;

    private:
      Gtk::StockButton* reset_button;

      Gtk::StockButton* load_button;
      Gtk::StockButton* save_button;
      Gtk::FileChooserDialog* save_file_chooser;

      Gtk::Notebook* group_notebook;

    private:
      Rule* backup_;
      // whether the info dialog changes for the rule 'with nines' <-> 'without nines' was shown
      bool without_nines_info_shown;

    private: // unused
      Rules();
      Rules(Rules const& main_window);
      Rules& operator=(Rules const& main_window);
  }; // class Rules : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_RULES_HEADER

#endif // #ifdef USE_UI_GTKMM
