/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Trick
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

#ifndef GTKMM_TRICK_SUMMARY_HEADER
#define GTKMM_TRICK_SUMMARY_HEADER

#include "base.h"
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>

class Trick;
class Player;

namespace Gtk {
  class DrawingArea;
  class Label;
  class TreeView;
} // namespace Gtk

namespace UI_GTKMM_NS {
  class TrickDrawing;

  /**
   **
   ** @brief	the summary of a trick
   **
   ** @author	Diether Knof
   **
   ** @todo	make the background as in 'table'
   **
   ** @todo	name update
   **
   **/
  class TrickSummary : public Base, public Gtk::HBox {

  /**
   **
   ** @brief	the columns of the trickpoints table
   **
   ** @author	Diether Knof
   **
   **/
    class SpecialpointsModel : public Gtk::TreeModel::ColumnRecord {
      public:
	SpecialpointsModel()
	{
	  this->add(this->special_point);
	  this->add(this->player);
	}

	Gtk::TreeModelColumn<Glib::ustring> special_point;
	Gtk::TreeModelColumn<Glib::ustring> player;
    }; // class SpecialpointsModel : public Gtk::TreeModel::ColumnRecord

    public:
    TrickSummary(Base* const parent, bool const show_trick_picture = true);
    ~TrickSummary();

    void set_trick(::Trick const& trick);
    void remove_trick();
    void update();
    void update_trick_picture();

    private:
    bool trick_picture_expose(GdkEventExpose* event);
    bool trick_picture_configure(GdkEventConfigure* event);

    void cards_update();
    void setting_update(int const type);
    void language_update();

    private:
    void init();

    private:
    ::Trick const* trick;

    bool const show_trick_picture;
    TrickDrawing* trick_drawing;
    Gtk::DrawingArea* trick_picture;
    Gtk::Label* winner;
    Gtk::Label* points;
    SpecialpointsModel specialpoints_model;
    Glib::RefPtr<Gtk::ListStore> specialpoints_list;
    Gtk::TreeView* specialpoints_treeview;

    private: // unused
    TrickSummary();
    TrickSummary(TrickSummary const& game_summary);
    TrickSummary& operator=(TrickSummary const& game_summary);
  }; // class TrickSummary : public Base, public Gtk::VBox

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_TRICK_SUMMARY_HEADER

#endif // #ifdef USE_UI_GTKMM
