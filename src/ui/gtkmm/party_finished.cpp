/**********************************************************************
single_ *
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
 *********************************************************************/

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "party_finished.h"

#include "ui.h"
#include "translations.h"
#include "table.h"
#include "main_window.h"
#include "party_points.h"
#include "cards.h"

#include "widgets/stock_button.h"

#include "../../party/party.h"
#include "../../party/rule.h"
#include "../../misc/setting.h"

#include "../../utils/string.h"

#include <gtkmm/main.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/alignment.h>
#include <gtkmm/stock.h>

namespace UI_GTKMM_NS {

  /**
   **
   ** Constructor
   **
   ** @param     parent   the parent widget
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.8
   **
   **/
  PartyFinished::PartyFinished(Base* const parent) :
    Base(parent),
    StickyDialog("party finished", *parent->ui->main_window, false),
    points_graph(NULL),
    show_party_points(NULL),
    close_party_button(NULL)
  {
    this->ui->add_window(*this);

    { // set size
      Gdk::Geometry geometry;
      geometry.min_width = 6 * this->ui->cards->width();
      geometry.min_height = 3 * this->ui->cards->height();

      this->set_geometry_hints(*this, geometry, Gdk::HINT_MIN_SIZE);
    }

    this->signal_realize().connect(sigc::mem_fun(*this, &PartyFinished::init));
#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
    this->signal_show().connect(sigc::mem_fun(*this, &PartyFinished::on_show));
    this->signal_hide().connect(sigc::mem_fun(*this, &PartyFinished::on_hide));
#endif

    return ;
  } // PartyFinished::PartyFinished(Base* const parent)

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
   ** @version   0.6.8
   **
   **/
  PartyFinished::~PartyFinished()
  {
    return ;
  } // PartyFinished::~PartyFinished()

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
   ** @version   0.6.8
   **
   **/
  void
    PartyFinished::init()
    {
      this->ui->translations->add(*this, ::translation("~party finished"));

      this->set_icon(this->ui->icon);

      this->set_position(Gtk::WIN_POS_CENTER_ON_PARENT);

      this->get_vbox()->set_spacing(10);

      this->points_graph = Gtk::manage(new Gtk::DrawingArea());
      this->points_graph->signal_expose_event().connect(sigc::mem_fun(*this, &PartyFinished::event_expose_points_graph));
      this->get_vbox()->add(*this->points_graph);
      {
	Gtk::Alignment* alignment
	  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
					   Gtk::ALIGN_CENTER,
					   0, 0));
	this->show_party_points
	  = Gtk::manage(new Gtk::StockButton("show party points"));
	this->ui->translations->add(*this->show_party_points->label,
				    ::translation("show party points"));
	alignment->add(*this->show_party_points);
	this->get_vbox()->pack_start(*alignment, false, true);
	this->show_party_points->signal_clicked().connect(sigc::mem_fun0(*this->ui->table->party_points_, &Gtk::Window::present));
      }

      { // action area
	this->close_party_button
	  = Gtk::manage(new Gtk::StockButton(Gtk::Stock::OK, "finish party"));
	this->ui->translations->add(*this->close_party_button->label,
				    ::translation("finish party"));
	this->add_action_widget(*this->close_party_button, Gtk::RESPONSE_CLOSE);

	this->close_party_button->grab_default();
	this->close_party_button->grab_focus();
      } // action area

      this->show_all_children();

      // signals
      this->close_party_button->signal_clicked().connect(sigc::mem_fun(*this, &PartyFinished::hide));

      return ;
    } // void PartyFinished::init()

  /**
   **
   ** the party has finished:
   ** show the summary and the winner
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
    PartyFinished::party_finished()
    {
      this->present();

      while (!this->ui->thrower
	     && this->is_visible())
        ::ui->wait();
      if (this->ui->thrower)
	return ;

      this->hide();

      return ;
    } // void PartyFinished::party_finished()

  /**
   ** redraw the whole points graph
   **
   ** @param    -
   **
   ** @return    -
   **
   ** @version   0.7.4
   **
   ** @author    Diether Knof
   **
   ** @todo      mark the winner(s)
   **/
  void
    PartyFinished::redraw_points_graph()
    {
      /*
       *    Diether  Erwin   Theo     Rudi
       *                           _________
       *   _________               |       |
       *   |       |________       |       |
       *   |       |       |       |       |
       *   |  75   |  50   |  -50  |  100  |
       *                   |       |       |
       *                   |_______|
       */

      if (!this->is_realized())
	return ;

      int const width = this->points_graph->get_width();
      int const height = this->points_graph->get_height();
      Glib::RefPtr<Gdk::Pixmap> pixmap
	= Gdk::Pixmap::create(this->points_graph->get_window(),
			      width, height,
			      this->points_graph->get_window()->get_depth());
      Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(pixmap);
      gc->set_foreground(Gdk::Color("white"));
      gc->set_background(Gdk::Color("white"));
      gc->set_rgb_fg_color(Gdk::Color("white"));
      gc->set_rgb_bg_color(Gdk::Color("white"));

      // clear all
      pixmap->draw_rectangle(gc, true,
			     0, 0, width, height);

      int min_points = INT_MAX; // minimal points made by the players
      int max_points = INT_MIN; // maximal points made by the players

      Party const& party = this->ui->party();

      for (vector<Player*>::const_iterator p = party.players().begin();
	   p != party.players().end();
	   ++p) {
	int const points = party.pointsum(**p);
	if (points < min_points)
	  min_points = points;
	if (points > max_points)
	  max_points = points;
      } // for (p \in party.players())
      if (max_points == min_points)
	max_points += 1;

      int max_layout_height = 0;
      for (unsigned p = 0; p < party.playerno(); ++p) {
	this->ui->table->name_layout->set_text(party.player(p).name());
	int layout_width, layout_height;
	this->ui->table->name_layout->get_pixel_size(layout_width,
						     layout_height);
	if (layout_height > max_layout_height)
	  max_layout_height = layout_height;
      } // for (p < party.playerno())

      int const points_difference = (std::max(max_points, 0)
				     - std::min(min_points, 0));
      int const border_left = this->ui->cards->width() / 5;
      int const border_right = border_left;
      int const border_top = (2 * max_layout_height
			      + ((max_points >= 0)
				 ? max_layout_height
				 : 0));
      int const border_bottom = (max_layout_height / 2
				 + ((min_points < 0)
				    ? max_layout_height
				    : 0));
      int const total_width = width - border_left - border_right;
      int const total_height = height - border_top - border_bottom;

      int const baseline = (border_top
			    + (std::max(max_points, 0)
			       * total_height
			       / points_difference));
      int const single_width = total_width / party.playerno();
      int const distance = 4; // for the distance between the columns
      { // draw all
	Gdk::Color color_name(::setting(Setting::NAME_FONT_COLOR));
	Gdk::Color color_winner_name(::setting(Setting::NAME_ACTIVE_FONT_COLOR));
	for (unsigned p = 0; p < party.playerno(); ++p) {
	  Player const& player = party.player(p);
	  bool winner = (party.rang(player) == 0);
	  int const points = party.pointsum(player);
	  int const pos_x = border_left + p * single_width;
	  int height = (points
			* total_height
			/ points_difference);

	  gc->set_rgb_fg_color( winner
				? color_winner_name
				: color_name);
	  { // draw name
	    this->ui->table->name_layout->set_text(player.name());
	    int layout_width, layout_height;
	    this->ui->table->name_layout->get_pixel_size(layout_width,
							 layout_height);
	    // bug: there seems to be a problem with italic characters
	    layout_width += 2 * layout_height / 5;
	    pixmap->draw_layout(gc,
				pos_x + (single_width - layout_width) / 2,
				max_layout_height / 2,
				this->ui->table->name_layout);
	  } // draw name
	  { // draw points
	    this->ui->table->trickpile_points_layout->set_text(DK::Utils::String::to_string(points));
	    int layout_width, layout_height;
	    this->ui->table->trickpile_points_layout->get_pixel_size(layout_width,
								     layout_height);
	    // bug: there seems to be a problem with italic characters
	    layout_width += 2 * layout_height / 5;

	    pixmap->draw_layout(gc,
				pos_x + (single_width - layout_width) / 2,
				baseline - height
				- ((height >= 0)
				   ? layout_height
				   : 0),
				this->ui->table->trickpile_points_layout);
	  } // draw points
	  { // draw point column
	    pixmap->draw_rectangle(gc, true,
				   pos_x + single_width / distance,
				   baseline,
				   single_width - 2 * single_width / distance,
				   1);
	    if (height >= 0)
	      pixmap->draw_rectangle(gc, true,
				     pos_x + single_width / distance,
				     baseline - height,
				     single_width - 2 * single_width / distance,
				     height);
	    else
	      pixmap->draw_rectangle(gc, true,
				     pos_x + single_width / distance,
				     baseline,
				     single_width - 2 * single_width / distance,
				     -height);
	  } // draw point column
	} // for (p)

	if (party.rule()(Rule::COUNTING) == COUNTING_NS::PLUSMINUS) {
	// draw baseline
	  gc->set_rgb_fg_color(color_name);
	    pixmap->draw_rectangle(gc, true,
				   border_left + single_width / distance,
				   baseline,
				   total_width - 2 * (single_width / distance),
				   1);
	} // if (draw baseline)
      } // draw all

      // copy the pixmap to the drawing area
      this->points_graph->get_window()->draw_drawable(gc,
						      pixmap,
						      0, 0, -1, -1);

      return ;
    } // void PartyFinished::redraw_points_graph()

  /**
   **
   ** the name of 'player' has changed
   **
   ** @param     player   the player with the changed name
   **
   ** @return    -
   **
   ** @version   0.6.8
   **
   ** @author    Diether Knof
   **
   ** @todo	all
   **
   **/
  void
    PartyFinished::name_changed(::Player const& player)
    {
      if (!this->is_realized())
	return ;

      this->redraw_points_graph();

      return ;
    } // void PartyFinished::name_changed(Player const& player)

  /**
   **
   ** event: hide the window
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
    PartyFinished::on_hide()
    {
      this->ui->main_quit();
#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
      this->Gtk::Dialog::on_hide();
#endif

      return ;
    } // void PartyFinished::on_hide()

  /**
   **
   ** event: the points graph has to be redrawn
   **
   ** @param     event   data of the event
   **
   ** @return    true
   **
   ** @version   0.6.8
   **
   ** @author    Diether Knof
   **
   **/
  bool
    PartyFinished::event_expose_points_graph(GdkEventExpose* event)
    {
      this->redraw_points_graph();

      return true;
    } // bool PartyFinished::event_expose_points_graph(GdkEventExpose*)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
