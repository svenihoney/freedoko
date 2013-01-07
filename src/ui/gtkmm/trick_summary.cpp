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
 *********************************************************************/

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "trick_summary.h"

#include "trick_drawing.h"
#include "ui.h"
#include "translations.h"
#include "table.h"

#include "../../game/game.h"
#include "../../player/player.h"
#include "../../card/trick.h"

#include <gtkmm/alignment.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>
#include <gtkmm/treeview.h>

namespace UI_GTKMM_NS {

  /**
   **
   ** Constructor
   **
   ** @param     parent   		parent widget
   ** @param     show_trick_picture   whether to show the trick picture
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **
   **/
  TrickSummary::TrickSummary(Base* const parent, bool const show_trick_picture) :
    Base(parent),
  HBox(),
  trick(NULL),
  show_trick_picture(show_trick_picture),
  trick_drawing(NULL),
  trick_picture(NULL),
  winner(NULL),
  points(NULL),
  specialpoints_model(),
  specialpoints_list(NULL),
  specialpoints_treeview(NULL)
  {
    this->signal_realize().connect(sigc::mem_fun(*this,
					      &TrickSummary::init));

    return ;
  } // TrickSummary::TrickSummary(Base* const parent, bool const show_trick_picture = true)

  /**
   **
   ** Destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **
   **/
  TrickSummary::~TrickSummary()
  {
    delete this->trick_drawing;

    return ;
  } // TrickSummary::~TrickSummary()

  /**
   **
   ** create all subelements
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    TrickSummary::init()
    {
      this->set_spacing(10);

      if (show_trick_picture) {
	this->trick_drawing = new TrickDrawing(this);
	if (this->trick != NULL)
	  this->trick_drawing->set_trick(*(this->trick));

	this->trick_picture = Gtk::manage(new Gtk::DrawingArea());
	this->trick_picture->set_size_request(this->trick_drawing->width()
					      * 6 / 5,
					      this->trick_drawing->height()
					      * 6 / 5);
	this->trick_picture->signal_expose_event().connect(sigc::mem_fun(*this, &TrickSummary::trick_picture_expose));
	this->trick_picture->signal_configure_event().connect(sigc::mem_fun(*this, &TrickSummary::trick_picture_configure));

	this->pack_start(*(this->trick_picture), false, true);
      } // if (show_trick_picture)

      { // info box
        Gtk::VBox* info_box = Gtk::manage(new Gtk::VBox(true));
        info_box->set_border_width(2 ex);
        this->pack_end(*info_box);

        { // the labels
          Gtk::Alignment* alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_BOTTOM, 0, 0));
          Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex));
          vbox->set_border_width(2 ex);
          alignment->add(*vbox);

          this->winner = Gtk::manage(new Gtk::Label("Winner:"));
          this->ui->translations->add(*(this->winner),
                                      ::translation("The trick goes to %swinner%.",
                                                    this->trick->winnerplayer().name()));
          vbox->pack_start(*(this->winner), false, true);

          this->points = Gtk::manage(new Gtk::Label("Points:"));
          this->ui->translations->add(*(this->points),
                                      ::translation("Points in the trick: %upoints%",
                                                    this->trick->points()));

          vbox->pack_start(*(this->points), false, true);

          info_box->pack_start(*alignment, false, true);
        } // the labels

        { // specialpoints
          Gtk::Alignment* alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_TOP, 0, 0));
          Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex));
          vbox->set_border_width(2 ex);
          alignment->add(*vbox);

          this->specialpoints_list
            = Gtk::ListStore::create(this->specialpoints_model);
          this->specialpoints_treeview
            = Gtk::manage(new Gtk::TreeView(this->specialpoints_list));
          this->specialpoints_treeview->get_selection()->set_mode(Gtk::SELECTION_NONE);

          this->specialpoints_treeview->append_column("~specialpoint",
                                                      this->specialpoints_model.special_point);
          this->specialpoints_treeview->append_column("Player",
                                                      this->specialpoints_model.player);

          vbox->pack_start(*(this->specialpoints_treeview), false, true);

          info_box->pack_end(*alignment, false, true);
        } // specialpoints
      } // info box

      this->show_all();
      this->update();
      this->language_update();

      return ;
    } // void TrickSummary::init()

  /**
   **
   ** set the trick
   **
   ** @param     trick   the trick to show the info of
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    TrickSummary::set_trick(::Trick const& trick)
    {
      this->trick = &trick;
      if (this->trick_drawing != NULL)
        this->trick_drawing->set_trick(trick);
      this->update();

      return ;
    } // void TrickSummary::set_trick(::Trick const& trick)

  /**
   **
   ** removes the trick
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.8
   **
   ** @author    Diether Knof
   **
   **/
  void
    TrickSummary::remove_trick()
    {
      this->trick = NULL;
      this->trick_drawing->remove_trick();

      return ;
    } // void TrickSummary::remove_trick()

  /**
   **
   ** update (rewrite) all information
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   ** @todo	if there is no 'player_of' don't show the column
   **
   **/
  void
    TrickSummary::update()
    {
      if (!this->is_realized())
        return ;

      if (this->trick == NULL)
        return ;

      this->ui->translations->change(*this->winner,
                                     ::translation("The trick goes to %swinner%.",
                                                   this->trick->winnerplayer().name()));
      this->ui->translations->change(*this->points,
                                     ::translation("Points in the trick: %upoints%",
                                                   this->trick->points()));

      this->update_trick_picture();

      { // special points

        this->specialpoints_list->clear();

        bool valid_point = false;
        Specialpointsvector const specialpoints = this->trick->specialpoints();
        for (Specialpointsvector::const_iterator
             sp = specialpoints.begin();
             sp != specialpoints.end();
             sp++) {
          // test whether to show the point
          if (sp->player_get_no == UINT_MAX)
#ifndef RELEASE
            // no 'player of' (can this happen?)
            DEBUG_ASSERTION(false,
                            "Gtkmm::TrickSummary(trick):\n"
                            "  no 'player_get_no' in specialpoint");
#else
          continue;
#endif
          else if (sp->player_of_no != UINT_MAX) {
            Team const team_get = this->trick->game().teaminfo_for_humans(this->trick->game().player(sp->player_get_no));
            Team const team_of = this->trick->game().teaminfo_for_humans(this->trick->game().player(sp->player_of_no));

            if ( ( (team_get == TEAM::RE)
                  || (team_get == TEAM::CONTRA) )
                && (team_get == team_of) )
              continue;
          } // if !(player_get_no == UINT_MAX) && (player_of_no != UINT_MAX)

          valid_point |= true;

          Gtk::TreeModel::Row row = *(this->specialpoints_list->append());
          row[this->specialpoints_model.special_point]
            = ::translation(sp->type).translation();
          if (sp->player_of_no != UINT_MAX)
            row[this->specialpoints_model.player]
              = this->trick->game().player(sp->player_of_no).name();
          else
            row[this->specialpoints_model.player] = "";
        } // for (sp \in specialpoints)

        this->specialpoints_treeview->show_all();

        if (valid_point)
          this->specialpoints_treeview->show();
        else
          this->specialpoints_treeview->hide();

      } // special points

      return ;
    } // void TrickSummary::update()

  /**
   **
   ** update the trick in the drawing area
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    TrickSummary::update_trick_picture()
    {
      if ((this->trick == NULL)
          || (this->trick_picture == NULL))
        return ;

#ifdef POSTPONED
      this->trick_picture->get_window()->draw_rectangle(this->ui->table->gc,
                                                        true,
                                                        0, 0, -1, -1);
#endif
      this->trick_drawing->draw(this->trick_picture->get_window());


      return ;
    } // void TrickSummary::update_trick_picture()

  /**
   **
   ** event: redraw part of the window
   **
   ** @param     event   the event
   **
   ** @return    true
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  bool
    TrickSummary::trick_picture_expose(GdkEventExpose* event)
    {
      if (this->trick == NULL)
        return true;

      this->trick_drawing->draw(this->trick_picture->get_window());

      return true;
    } // bool TrickSummary::trick_picture_expose(GdkEventExpose* event)

  /**
   **
   ** event: the size has changed
   **
   ** @param     event   the event
   **
   ** @return    true
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  bool
    TrickSummary::trick_picture_configure(GdkEventConfigure* event)
    {
      this->trick_drawing->set_center(Gdk::Point(this->trick_picture->get_width() / 2,
                                                 this->trick_picture->get_height() / 2));
      this->update_trick_picture();

      return true;
    } // bool TrickSummary::trick_picture_configure(GdkEventConfigure* event)

  /**
   **
   ** updates the cards
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    TrickSummary::cards_update()
    {
      if (!this->is_realized())
        return ;

      if (this->trick_picture != NULL)
        this->trick_picture->set_size_request(this->trick_drawing->width()
                                              * 6 / 5,
                                              this->trick_drawing->height()
                                              * 6 / 5);

      return ;
    } // void TrickSummary::cards_update()

  /**
   **
   ** the setting has changed
   **
   ** @param     type   the type that has changed
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    TrickSummary::setting_update(int const type)
    {
      switch(type) {
      case Setting::ROTATE_TRICK_CARDS:
        if (this->trick_picture != NULL) {
          this->trick_picture->set_size_request(this->trick_drawing->width()
                                                * 6 / 5,
                                                this->trick_drawing->height()
                                                * 6 / 5);
          if (this->trick_picture->is_realized())
            this->trick_picture->get_window()->clear();
        } // if (this->trick_picture != NULL)
        this->update_trick_picture();
        break;
      case Setting::LANGUAGE:
        this->update();
        break;
      default:
        break;
      } // switch(type)

      return ;
    } // void TrickSummary::setting_update(int const type)

  /**
   **
   ** updates all language elements
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    TrickSummary::language_update()
    {
      if (!this->is_realized())
        return ;

      this->specialpoints_treeview->get_column(0)->set_title(::translation("specialpoint").translation());
      this->specialpoints_treeview->get_column(1)->set_title(::translation("of player").translation());

      this->update();

      return ;
    } // void TrickSummary::language_update()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
