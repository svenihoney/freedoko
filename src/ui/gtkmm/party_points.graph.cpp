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

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "party_points.graph.h"

#include "ui.h"
#include "translations.h"

#include "../../party/party.h"
#include "../../party/rule.h"
#include "../../game/game.h"

#include "../../utils/string.h"

#include <gdkmm/pixmap.h>
#include <gdkmm/gc.h>
#include <pangomm/layout.h>

namespace UI_GTKMM_NS {

  /**
   ** constructor
   **
   ** @param     party_points   the parent object
   **
   ** @return    -
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.3
   **/
  PartyPoints::Graph::Graph(PartyPoints* const party_points) :
    Base(party_points),
    Gtk::DrawingArea(),
    pixmap(NULL),
    gc(NULL)
  {
#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
    this->signal_expose_event().connect(sigc::mem_fun(*this, &Graph::on_expose_event));
    this->signal_configure_event().connect(sigc::mem_fun(*this, &Graph::on_configure_event));
#endif

    return ;
  } // PartyPoints::Graph::Graph(PartyPoints* party_points)

  /**
   ** destructor
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.3
   **/
  PartyPoints::Graph::~Graph()
  { }

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the width of the graph
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  unsigned
    PartyPoints::Graph::width() const
    {
      return this->get_width();
    } // unsigned PartyPoints::Graph::width() const

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the height of the graph
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  unsigned
    PartyPoints::Graph::height() const
    {
      return this->get_height();
    } // unsigned PartyPoints::Graph::height() const

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the depth of the graph
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  unsigned
    PartyPoints::Graph::depth() const
    {
      return this->get_window()->get_depth();
    } // unsigned PartyPoints::Graph::depth() const

  /**
   ** event: redraw part of the window
   **
   ** @param     event   the event
   **
   ** @return    true
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  bool
    PartyPoints::Graph::on_expose_event(GdkEventExpose* event)
    {
      this->update(Gdk::Rectangle(event->area.x, event->area.y,
				  event->area.width, event->area.height));

      return true;
    } // bool PartyPoints::Graph::on_expose_event(GdkEventExpose* event)

  /**
   ** event: the size has changed
   **
   ** @param     event   the event
   **
   ** @return    true
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  bool
    PartyPoints::Graph::on_configure_event(GdkEventConfigure* event)
    {
      this->pixmap = Gdk::Pixmap::create(this->get_window(),
					 this->width(), this->height(),
					 this->depth());

      if (!this->gc) {
	this->gc = Gdk::GC::create(this->pixmap);
	this->gc->set_background(Gdk::Color("white"));
	this->gc->set_fill(Gdk::SOLID);
      } // if (!this->gc)

      this->draw_all();

      return true;
    } // bool PartyPoints::Graph::on_configure_event(GdkEventConfigure* event)

  /**
   ** update the full area
   ** (just copy the pixmap to the drawing_area)
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    PartyPoints::Graph::update_full_area()
    {
      this->update(Gdk::Rectangle(0, 0, -1, -1));

      return ;
    } // void PartyPoints::Graph::update_full_area()

  /**
   ** update the area of the drawing area
   ** (just copy the area from the pixmap)
   **
   ** @param     area   area to update
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    PartyPoints::Graph::update(Gdk::Rectangle const& area)
    {
      this->get_window()->draw_drawable(this->gc,
					this->pixmap,
					area.get_x(), area.get_y(),
					area.get_x(), area.get_y(),
					area.get_width(), area.get_height());

      return ;
    } // void PartyPoints::Graph::update(Gdk::Rectangle const& area);

  /**
   ** draw the whole graph
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **
   ** @todo     add game numbers
   **/
  void
    PartyPoints::Graph::draw_all()
    {
      if (!this->pixmap)
	return ;

      // clear the whole area
      this->gc->set_rgb_fg_color(Gdk::Color("white"));
      this->pixmap->draw_rectangle(this->gc, true,
				   0, 0, -1, -1);


      if (::game_status <= GAMESTATUS::PARTY_NEW) {
	this->update_full_area();
	return ;
      }

      Party const& party = this->ui->party();

      Counting const counting = party.rule()(Rule::COUNTING);

      // last game to draw + 1
      unsigned const played_gamesno
	= (party.gameno()
	   + ( (::game_status < GAMESTATUS::GAME_FINISHED)
	      ? 0
	      : 1) );

      // calculate the minimum and maximum points made by the players
      int min_points = 0; // minimal points made by the players
      int max_points = 0; // maximal points made by the players

      if (played_gamesno == 0) {
	if (party.rule()(Rule::POINTS_LIMITED))
	  max_points = party.rule()(Rule::POINTS) / 2;
	if (max_points == 0)
	  max_points = 10;

	switch (party.rule()(Rule::COUNTING)) {
	case COUNTING_NS::PLUS:
	  break;
	case COUNTING_NS::MINUS:
	  min_points = -max_points;
	  max_points = 0;
	  break;
	case COUNTING_NS::PLUSMINUS:
	  min_points = -max_points;
	  break;
	} // switch (party.rule()(Rule::COUNTING))
      } // if (played_gamesno == 0)

      for (unsigned g = 0; g < played_gamesno; ++g) {
	for (unsigned p = 0; p < party.playerno(); ++p) {
	  int const points = party.pointsum_till_game(g, p);
	  if (points < min_points)
	    min_points = points;
          if (points > max_points)
            max_points = points;
        } // for (p < party.playerno())
      } // for (g < played_gamesno)
      if (max_points == min_points)
        max_points += 1;

      // how many games to display
      unsigned gamesno = UINT_MAX;
      if (party.is_last_game()) {
        gamesno = played_gamesno;
        if (::game_status < GAMESTATUS::GAME_FINISHED)
          gamesno += 1;
      } else {
        if (party.rule()(Rule::NUMBER_OF_ROUNDS_LIMITED))
          gamesno = std::min(gamesno,
                             party.round_startgame(party.rule()(Rule::NUMBER_OF_ROUNDS)));
        if (   party.rule()(Rule::POINTS_LIMITED)
            && (party.points() > 0) )
          gamesno = std::min(gamesno,
                             std::max( (  (party.rule()(Rule::POINTS)
                                           * (played_gamesno + 1))
                                        / static_cast<int>(party.points())),
                                      played_gamesno + 1));
        if (gamesno < UINT_MAX) {
          gamesno += party.remaining_duty_soli();
          if (   (::game_status == GAMESTATUS::GAME_FINISHED)
              && party.game().is_duty_solo())
            gamesno += 1;
        }
      } // if (party.is_last_game())

      if (gamesno == UINT_MAX)
        gamesno = played_gamesno;
      if (gamesno < party.round_startgame(1))
        gamesno = party.round_startgame(1);
      // how many rounds to display
      unsigned const roundsno = party.round_of_game(gamesno);


      // layout:
      // +-----------------------------+ border_y
      // | 10+       /                 |
      // |   |     _/                  |
      // |   |   _/  _                 |
      // |  5+  /   /                  | height_graph
      // |   | / __/                   |
      // |   |/ /                      |
      // |  0+----+----+----+----+----+| border_roundno
      // |   0    5   10   15   20   25| height_roundno, border_names
      // |    player1 player2 p3  p4   | height_names
      // +-----------------------------+ border_x
      // border_x width_points border_points width_graph border_x

      // the layout values
      unsigned border_y = 0;
      unsigned height_graph = 0;
      unsigned border_roundno = 0;
      unsigned height_roundno = 0;
      unsigned border_names = 0;
      unsigned height_names = 0;

      unsigned border_x = 0;
      unsigned width_points = 0;
      unsigned border_points = 0;
      unsigned width_graph = 0;

      int zero_x = 0;           // the x position of the zero point of the graph
      int zero_y = 0;           // the y position of the zero point of the graph
      double scale_games = 0;   // the distance beween two games
      double scale_points = 0;  // the distance between two points
      // note: the 'scale_points' distance is negative, so that the points go up
      unsigned points_step = 0; // the steps beween two points markers

      // whether the player names are shown on the bottom (else on the top)
      bool const names_on_bottom = (counting != COUNTING_NS::MINUS);

      // the layout for the name and the description
      Glib::RefPtr<Pango::Layout> text_layout
        = this->create_pango_layout("Serif Bold 14");

      // size of the markers (in each direction)
      int marker_pixel_size = 10;

      { // marker size
        int layout_width, layout_height;
        text_layout->set_text("0");
        text_layout->get_pixel_size(layout_width, layout_height);
        marker_pixel_size = layout_height / 2;
      } // marker size


      { // set the layout values
        { // y
          border_y = std::max((this->height() / 50), 2u);
          border_names = border_y / 2;
          { // height name
            height_names = 0;
            int layout_width, layout_height;
            for (unsigned p = 0; p < party.playerno(); ++p) {
              text_layout->set_text(party.player(p).name());
              text_layout->get_pixel_size(layout_width, layout_height);
              if (static_cast<unsigned>(layout_height) > height_names)
                height_names = layout_height;
            }
          } // height name
          border_roundno = border_y / 2;
          if (counting == COUNTING_NS::PLUSMINUS) {
            height_roundno = 0;
#ifdef WORKAROUND
            scale_points = 0;
#endif
            // ensure that there is enough place for the roundno below the graph
            if (static_cast<unsigned>(min_points * scale_points)
                < height_names + border_names + marker_pixel_size)
              height_roundno = ((height_names + border_names
                                 + marker_pixel_size)
                                - static_cast<int>(min_points * scale_points));
          } else { // if !(counting = PLUSMINUS)
            height_roundno = height_names;
          } // if !(counting = PLUSMINUS)
          if (this->height() < (2 * border_y
                                + height_names + border_names
                                + height_roundno + border_roundno)) {
            this->update_full_area();
            return ;
          }
          height_graph = (this->height() - (2 * border_y
                                            + height_names + border_names
                                            + height_roundno + border_roundno));
          scale_points = - (height_graph
                            / static_cast<double>(max_points - min_points));
          zero_y = (border_y - static_cast<int>(max_points * scale_points));
          if (!names_on_bottom)
            zero_y += (height_names + border_names
                       + height_roundno + border_roundno);
        } // y

        { // x
          border_x = std::max((this->width() / 50), 2u);
          border_points = border_x / 2;
          { // width points
            list<unsigned> steps;
            steps.push_back(1);
            steps.push_back(2);
            steps.push_back(5);

            int layout_width, layout_height;
            text_layout->set_text("0");
            text_layout->get_pixel_size(layout_width, layout_height);

            unsigned const min_pixel_distance
              = (static_cast<int>(layout_height) * 3) / 2;
            points_step = 0;
            for (list<unsigned>::const_iterator s = steps.begin(); 
                 s != steps.end();
                 ++s) {
              if (height_graph / ((max_points - min_points) / *s)
                  >= min_pixel_distance) {
                points_step = *s;
                break;
              }
              steps.push_back(10 * *s);
            } // for (s \in steps)
            if (points_step == 0) {
              this->update_full_area();
              return ;
            }

            width_points = 0;
            for (int p = -((-min_points / points_step) * points_step);
                 p <= max_points;
                 p += points_step)  {
              text_layout->set_text(DK::Utils::String::to_string(p));
              text_layout->get_pixel_size(layout_width, layout_height);
              if (width_points < static_cast<unsigned>(layout_width))
                width_points = static_cast<unsigned>(layout_width);
            }
          } // width points
          zero_x = border_x + width_points + border_points;
          if (this->width() < zero_x + border_x) {
            this->update_full_area();
            return ;
          }
          width_graph = this->width() - zero_x - border_x;

          scale_games = (width_graph / static_cast<double>(gamesno));
        } // x
      } // set the layout values

      { // draw the curves of the points of the players
        // reorder the players so that the human players are drawn last
        vector<unsigned> players;
        for (unsigned p = 0; p < party.playerno(); ++p)
          if (   (party.player(p).type() != Player::HUMAN)
#ifdef USE_NETWORK
              && (party.player(p).type() != Player::NETWORK)
#endif
             )
            players.push_back(p);
#ifdef USE_NETWORK
        for (unsigned p = 0; p < party.playerno(); ++p)
          if (party.player(p).type() == Player::NETWORK)
            players.push_back(p);
#endif
        for (unsigned p = 0; p < party.playerno(); ++p)
          if (party.player(p).type() == Player::HUMAN)
            players.push_back(p);

        for (vector<unsigned>::const_iterator p_it = players.begin();
             p_it != players.end();
             ++p_it) {
          // the player number
          unsigned const p = *p_it;
          // the y offset for the line of the player
          int const player_offset_y = ( ((static_cast<int>(p) - 1) * 2 - 1)
                                       / (static_cast<int>(party.roundno())
                                          / 5 + 1) );
          this->gc->set_rgb_fg_color(Gdk::Color(party.player(p).color()));
          this->gc->set_line_attributes(3,
                                        Gdk::LINE_SOLID,
                                        Gdk::CAP_NOT_LAST,
                                        Gdk::JOIN_MITER);

          // the name
          text_layout->set_text(party.player(p).name());
          int layout_width, layout_height;
          text_layout->get_pixel_size(layout_width, layout_height);
          this->pixmap->draw_layout(this->gc,
                                    zero_x
                                    + (width_graph * p / party.playerno())
                                    + (width_graph / party.playerno()
                                       - layout_width) / 2,
                                    ( names_on_bottom
                                     ? (this->height()
                                        - border_y
                                        - height_names)
                                     : border_y
                                    ),
                                    text_layout);

          std::list<Gdk::Point> points;
          points.push_back(Gdk::Point(zero_x, zero_y + player_offset_y));
          for (unsigned g = 0; g < played_gamesno; ++g) {
            points.push_back(Gdk::Point(zero_x
                                        + static_cast<int>(scale_games
                                                           * (g + 1)),
                                        zero_y + player_offset_y
                                        + static_cast<int>(scale_points
                                                           * party.pointsum_till_game(g, p))
                                       ));
          } // for (g <= party.gameno())

          this->pixmap->draw_lines(this->gc, points);

        } // for (p < party.playerno())
      } // draw the curves of the points of the players

      { // zero lines
        this->gc->set_rgb_fg_color(Gdk::Color("black"));
        this->gc->set_line_attributes(1, Gdk::LINE_SOLID,
                                      Gdk::CAP_ROUND, Gdk::JOIN_MITER);
        this->pixmap->draw_line(this->gc,
                                zero_x, zero_y,
                                zero_x + width_graph, zero_y);
        this->pixmap->draw_line(this->gc,
                                zero_x,
                                zero_y
                                + static_cast<int>(scale_points * min_points),
                                zero_x,
                                zero_y
                                + static_cast<int>(scale_points * max_points));
      } // zero lines
      { // round markers
        unsigned round_step = 0;

        list<unsigned> steps;
        steps.push_back(1);
        steps.push_back(2);
        steps.push_back(5);

        int layout_width, layout_height;
        text_layout->set_text(DK::Utils::String::to_string(roundsno));
        text_layout->get_pixel_size(layout_width, layout_height);

        // minimal pixel distance between two markers
        unsigned const min_pixel_distance = 2 * layout_width;

        if (roundsno == 0) {
          round_step = 2 * width_graph;
        } else { // if !(roundsno == 0)
          for (list<unsigned>::const_iterator s = steps.begin(); 
               s != steps.end();
               ++s) {
            if (width_graph / (roundsno / *s) >= min_pixel_distance) {
              round_step = *s;
              break;
            }
            steps.push_back(10 * *s);
          } // for (s \in steps)
        } // if !(roundsno == 0)
        if (round_step == 0) {
          this->update_full_area();
          return ;
        }

        // how many pixel the marker goes up/down
        int const marker_y_up = ( (counting == COUNTING_NS::MINUS)
                                 ? 0
                                 : marker_pixel_size);

        int const marker_y_down = ( (counting == COUNTING_NS::PLUS)
                                   ? 0
                                   : marker_pixel_size);

        for (unsigned r = 0; r <= roundsno; r += round_step) {
          int const x = (zero_x
                         + static_cast<int>(scale_games
                                            * party.round_startgame(r)));
          this->pixmap->draw_line(this->gc,
                                  x, zero_y + marker_y_down,
                                  x, zero_y - marker_y_up);
          int layout_width, layout_height;
          text_layout->set_text(DK::Utils::String::to_string(r));
          text_layout->get_pixel_size(layout_width, layout_height);
          if (counting == COUNTING_NS::MINUS) {
            this->pixmap->draw_layout(this->gc,
                                      zero_x
                                      + static_cast<int>(scale_games
                                                         * party.round_startgame(r))
                                      - (layout_width / 2),
                                      zero_y - marker_y_up - border_roundno
                                      - layout_height,
                                      text_layout);
          } else { // if (counting == COUNTING_NS::MINUS)
            this->pixmap->draw_layout(this->gc,
                                      zero_x
                                      + static_cast<int>(scale_games
                                                         * party.round_startgame(r))
                                      - (layout_width / 2),
                                      zero_y + marker_y_down + border_roundno,
                                      text_layout);
          } // if (counting == COUNTING_NS::MINUS)
        }
      } // round markers
      { // point markers
        int layout_width, layout_height;
        for (int p = -((-min_points / points_step) * points_step);
             p <= max_points;
             p += points_step)  {
          text_layout->set_text(DK::Utils::String::to_string(p));
          text_layout->get_pixel_size(layout_width, layout_height);
          this->pixmap->draw_layout(this->gc,
                                    border_x + width_points
                                    - layout_width,
                                    zero_y + static_cast<int>(scale_points * p)
                                    - layout_height / 2,
                                    text_layout);
          this->pixmap->draw_line(this->gc,
                                  zero_x,
                                  zero_y + static_cast<int>(scale_points * p),
                                  zero_x + marker_pixel_size,
                                  zero_y + static_cast<int>(scale_points * p)
                                 );
        }
      } // point markers


      this->update_full_area();

      return ;
    } // void PartyPoints::Graph::draw_all()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
