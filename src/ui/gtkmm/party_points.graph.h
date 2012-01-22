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

#ifndef GTKMM_PARTY_POINTS_GRAPH_HEADER
#define GTKMM_PARTY_POINTS_GRAPH_HEADER

#include "base.h"

#include "party_points.h"

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/pixmap.h>
#include <gdkmm/gc.h>

namespace UI_GTKMM_NS {
  /**
   ** @brief	a curve showing the points of the players
   **
   ** @author	Diether Knof
   **/
  class PartyPoints::Graph : public Base, public Gtk::DrawingArea {
    public:
      // constructor
      Graph(PartyPoints* const party_points);
      // desctructor
      ~Graph();

      // the width of the drawing area
      unsigned width() const;
      // the height of the drawing area
      unsigned height() const;
      // the pixel depth
      unsigned depth() const;

      // draw the whole graph
      void draw_all();

    private:
      // expose event
      bool on_expose_event(GdkEventExpose* event);
      // the size has changed
      bool on_configure_event(GdkEventConfigure* event);
      // update the whole area (from the pixmap)
      void update_full_area();
      // update the area (from the pixmap)
      void update(Gdk::Rectangle const& area);

    private:
      // the pixmap with the graph
      Glib::RefPtr<Gdk::Pixmap> pixmap;
      // the gc for the graph
      Glib::RefPtr<Gdk::GC> gc;

    private: // unused
      Graph();
      Graph(Graph const& graph);
      Graph& operator=(Graph const& graph);
  }; // class PartyPoints::Graph : public Base, public Gtk::DrawingArea

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_PARTY_POINTS_GRAPH_HEADER

#endif // #ifdef USE_UI_GTKMM
