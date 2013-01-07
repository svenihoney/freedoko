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

#ifndef GTKMM_GENSCHER_SELECTION_HEADER
#define GTKMM_GENSCHER_SELECTION_HEADER

#include "base.h"

class Player;

#include "widgets/sticky_dialog.h"
namespace Gtk {
  class Label;
  class Button;
}; // namespace Gtk

namespace UI_GTKMM_NS {
  class Table;

  /**
   **
   ** @brief	genscher selection dialog
   **
   ** @author	Diether Knof
   **
   **/
  class GenscherSelection : public Base, public Gtk::StickyDialog {
    public:
      GenscherSelection(Table* const table);
      ~GenscherSelection();

      Player const* select_partner(Player const& genscher);

      void name_changed(Player const& player);

    private:
      void init();

      void select_partner_event(Player const* const partner);

    private:
      Gtk::Label* info_label;
      vector<Gtk::Button*> partner_button;
      Gtk::Button* no_partner_change_button;

      Player const* genscher;
      Player const* partner;

    private: // unused
      GenscherSelection();
      GenscherSelection(GenscherSelection const&);
      GenscherSelection& operator=(GenscherSelection const&);
  }; // class GenscherSelection : public GameinfoDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_GENSCHER_SELECTION_HEADER

#endif // #ifdef USE_UI_GTKMM
