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

#ifndef GTKMM_PARTY_FINISHED_HEADER
#define GTKMM_PARTY_FINISHED_HEADER

#include "base.h"
#include "ui.h"
#include "widgets/sticky_dialog.h"

#include <gdk/gdkevents.h>
namespace Gtk {
  class DrawingArea;
} // namespace Gtk

class Player;

namespace UI_GTKMM_NS {
  /**
   **
   ** @brief	the party finished window
   **
   ** @author	Diether Knof
   **
   **/
  class PartyFinished : public Base, public Gtk::StickyDialog {

    public:
    PartyFinished(Base* const parent);
    ~PartyFinished();

    void party_finished();

    void redraw_points_graph();

    void name_changed(::Player const& player);

    private:
    void init();
    void on_hide();

    bool event_expose_points_graph(GdkEventExpose* event);

    private:
    Gtk::DrawingArea* points_graph;

    Gtk::StockButton* show_party_points;
    Gtk::StockButton* close_party_button;

    private: // unused
    PartyFinished();
    PartyFinished(PartyFinished const& party_finished);
    PartyFinished& operator=(PartyFinished const& party_finished);
  }; // class PartyFinished : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_PARTY_FINISHED_HEADER

#endif // #ifdef USE_UI_GTKMM
