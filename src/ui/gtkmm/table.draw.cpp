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
 *********************************************************************/

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "table.h"
#include "poverty.h"
#include "party_points.h"
#include "party_finished.h"
#include "last_trick.h"
#include "trick.h"
#include "hand.h"
#include "trickpile.h"
#include "icongroup.h"
#include "name.h"
#include "reservation.h"

#include "ui.h"
#include "cards.h"
#include "utils.h"
#include "gameinfo_dialog.h"
#include "full_trick.h"
#include "game_finished.h"

#include "../../party/party.h"
#include "../../party/rule.h"
#include "../../game/game.h"
#include "../../player/player.h"
#include "../../card/trick.h"
#include "../../misc/setting.h"
#include "../../utils/string.h"

#include <gtkmm/drawingarea.h>
#include <cmath>
using std::pow;

namespace UI_GTKMM_NS {
  /**
   **
   ** clear the full pixmap
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Table::clear_all()
    {
      this->clear(Gdk::Rectangle(0, 0, this->width(), this->height()));

      return ;
    } // void Table::clear_all()

  /**
   **
   ** clears the rectangle
   **
   ** @param     area   area to clear
   **
   ** @return    -
   **
   ** @version   0.6.2
   **
   ** @author    Diether Knof
   **
   **/
  void
    Table::clear(Gdk::Rectangle const& area)
    {
      this->pixmap->draw_rectangle(this->gc, true,
				   area.get_x(), area.get_y(),
				   area.get_width(), area.get_height());

      return ;
    } // void Table::clear(Gdk::Rectangle const& area)

  /**
   **
   ** draw all
   **
   ** @param     update   whether to update the area
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Table::draw_all(bool const update)
    {
      this->clear_all();

      if (::game_status & GAMESTATUS::GAME) {
	for (vector<Hand*>::iterator h = this->hand_.begin();
	     h != this->hand_.end();
	     h++)
	  (*h)->draw(false);
	for (vector<TrickPile*>::iterator t = this->trickpile_.begin();
	     t != this->trickpile_.end();
	     t++)
	  (*t)->draw(false);
	for (vector<Icongroup*>::iterator t = this->icongroup_.begin();
	     t != this->icongroup_.end();
	     t++)
	  (*t)->draw(false);
	for (vector<Name*>::iterator n = this->name_.begin();
	     n != this->name_.end();
	     n++)
	  (*n)->draw(false);
	if (this->trick_)
	  this->trick_->draw(false);

	if ( (this->ui->game().type() == GAMETYPE::POVERTY)
	     && (((::game_status == GAMESTATUS::GAME_POVERTY_SHIFT)
		  || (::game_status == GAMESTATUS::GAME_REDISTRIBUTE) ) )
	     && this->poverty_)
	  this->poverty().draw(false);

      } else if (::game_status == GAMESTATUS::PARTY_FINISHED) {
#ifdef OUTDATED
	// -> party_summary window
	this->draw_party_summary();
#endif
      } else { // if (::game_status)
	this->draw_logo();
      } // if (::game_status)

      if (update)
	this->update_full_area();

      this->mouse_cursor_update();

      return ;
    } // void Table::draw_all(bool const update = true)

  /**
   **
   ** draw the logo
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.8
   **
   ** @author    Diether Knof
   **
   ** @bug	clip mask does not work (GTK-bug?)
   **
   **/
  void
    Table::draw_logo()
    {
      this->pixmap->draw_pixbuf(this->gc,
				this->ui->logo,
				0, 0,
				(this->width()
				 - this->ui->logo->get_width()) / 2,
				(this->height()
				 - this->ui->logo->get_height()) / 2,
				-1, -1,
				Gdk::RGB_DITHER_NONE,
				0, 0);
      { // draw a border with the cards
	vector<Card> border_card; // the border cards
	unsigned const card_width = this->ui->cards->width();
	unsigned const card_height = this->ui->cards->height();

	// the bitmap of the border cards on the top
	vector<Glib::RefPtr<Gdk::Bitmap> > border_card_top_bitmap;

	// the border of the top card row
	unsigned border_top_x;

	{ // top
	  border_card.clear();
	  border_card.push_back(Card(Card::HEART,	Card::TEN));
	  border_card.push_back(Card(Card::CLUB,	Card::QUEEN));
	  border_card.push_back(Card(Card::SPADE,	Card::QUEEN));
	  border_card.push_back(Card(Card::HEART,	Card::QUEEN));
	  border_card.push_back(Card(Card::DIAMOND,	Card::QUEEN));
	  border_card.push_back(Card(Card::CLUB,	Card::JACK));
	  border_card.push_back(Card(Card::SPADE,	Card::JACK));

	  border_top_x
	    = (card_width
	       + ( (border_card.size() * card_height
		    + card_width >= this->width())
		   ? 0
		   : ((this->width() - (border_card.size() * card_height
					+ card_width)
		      ) / border_card.size())
		 )
	      );

	  for (unsigned n = 0; n < border_card.size(); n++) {
	    this->pixmap->draw_pixbuf(this->gc,
				      this->ui->cards->card(border_card[n],
							    LEFT),
				      0, 0,
				      border_top_x +
				      n * (this->width() - 2 * border_top_x
					   - card_height)
				      / (border_card.size() - 1),
				      card_width / 2,
				      -1, -1,
				      Gdk::RGB_DITHER_NONE,
				      0, 0);
	    // set the bitmap
	    border_card_top_bitmap.push_back(bitmap(this->ui->cards->card(border_card[n], LEFT)));
	  } // for (n < border_card.size())
	} // top
	{ // right
	  border_card.clear();
	  border_card.push_back(Card(Card::HEART,	Card::JACK));
	  border_card.push_back(Card(Card::DIAMOND,	Card::JACK));
	  border_card.push_back(Card(Card::DIAMOND,	Card::ACE));
	  border_card.push_back(Card(Card::DIAMOND,	Card::TEN));
	  border_card.push_back(Card(Card::DIAMOND,	Card::KING));

	  unsigned const border_y
	    = (card_width
	       + ( (border_card.size() * card_height
		    + card_width >= this->height())
		   ? 0
		   : ((this->height() - (border_card.size() * card_height
					 + card_width)
		      ) / border_card.size())
		 )
	      );

	  for (unsigned n = 0; n < border_card.size(); n++)
	    this->pixmap->draw_pixbuf(this->gc,
				      this->ui->cards->card(border_card[n],
							    UP),
				      0, 0,
				      this->width() - 3 * card_width / 2,
				      border_y +
				      n * (this->height() - 2 * border_y
					   - card_height)
				      / (border_card.size() - 1),
				      -1, -1,
				      Gdk::RGB_DITHER_NONE,
				      0, 0);
	} // right
	{ // bottom
	  border_card.clear();
	  border_card.push_back(Card(Card::DIAMOND,	Card::NINE));
	  border_card.push_back(Card(Card::CLUB,	Card::ACE));
	  border_card.push_back(Card(Card::CLUB,	Card::TEN));
	  border_card.push_back(Card(Card::CLUB,	Card::KING));
	  border_card.push_back(Card(Card::CLUB,	Card::NINE));
	  border_card.push_back(Card(Card::HEART,	Card::ACE));
	  border_card.push_back(Card(Card::HEART,	Card::KING));

	  unsigned const border_x
	    = (card_width
	       + ( (border_card.size() * card_height
		    + card_width >= this->width())
		   ? 0
		   : ((this->width() - (border_card.size() * card_height
					+ card_width)
		      ) / border_card.size())
		 )
	      );

	  for (unsigned n = 0; n < border_card.size(); n++)
	    this->pixmap->draw_pixbuf(this->gc,
				      this->ui->cards->card(border_card[n],
							    RIGHT),
				      0, 0,
				      border_x +
				      (border_card.size() - 1 - n)
				      * (this->width() - 2 * border_x
					 - card_height)
				      / (border_card.size() - 1),
				      this->height()
				      - 3 * card_width / 2,
				      -1, -1,
				      Gdk::RGB_DITHER_NONE,
				      0, 0);
	} // bottom
	{ // left
	  border_card.clear();
	  border_card.push_back(Card(Card::HEART,	Card::NINE));
	  border_card.push_back(Card(Card::SPADE,	Card::ACE));
	  border_card.push_back(Card(Card::SPADE,	Card::TEN));
	  border_card.push_back(Card(Card::SPADE,	Card::KING));
	  border_card.push_back(Card(Card::SPADE,	Card::NINE));


	  unsigned const border_y
	    = (card_width
	       + ( (border_card.size() * card_height
		    + card_width >= this->height())
		   ? 0
		   : ((this->height() - (border_card.size() * card_height
					 + card_width)
		      ) / border_card.size())
		 )
	      );

	  for (unsigned n = 0; n < border_card.size(); n++) {
	    int const pos_x = card_width / 2;
	    int const pos_y = (border_y +
			       (border_card.size() - 1 - n)
			       * (this->height() - 2 * border_y
				  - card_height)
			       / (border_card.size() - 1));

	    // for the overlapping of the left with the upper row
	    // use a separate gc
	    Glib::RefPtr<Gdk::Bitmap> bitmap_overlapping
	      = bitmap(this->ui->cards->card(border_card[n],
					     DOWN));
	    // for adding the bitmaps
	    Glib::RefPtr<Gdk::GC> gc_add
	      = (!bitmap_overlapping
		 ? Gdk::GC::create(this->pixmap)
		 : Gdk::GC::create(bitmap_overlapping));
	    gc_add->set_function(Gdk::AND_INVERT);
	    if (bitmap_overlapping)
	      for (unsigned i = 0; i < border_card_top_bitmap.size(); i++) {
		bitmap_overlapping->draw_drawable(gc_add,
						  border_card_top_bitmap[i],
						  0, 0,
						  border_top_x +
						  i * (this->width() - 2 * border_top_x
						       - card_height)
						  / (border_card.size() - 1)
						  - pos_x,
						  card_width / 2
						  - pos_y,
						  card_height, card_width);
	      } // for (i < border_card_top_bitmap.size())

	    Glib::RefPtr<Gdk::GC> gc_overlapping
	      = Gdk::GC::create(this->pixmap);
	    if (bitmap_overlapping)
	      gc_overlapping->set_clip_mask(bitmap_overlapping);
	    gc_overlapping->set_clip_origin(pos_x, pos_y);

	    this->pixmap->draw_pixbuf(gc_overlapping,
				      this->ui->cards->card(border_card[n],
							    DOWN),
				      0, 0,
				      pos_x, pos_y,
				      -1, -1,
				      Gdk::RGB_DITHER_NONE,
				      0, 0);
	  }
	} // left
      } // draw a border with the cards

      return ;
    } // void Table::draw_logo()

  /**
   ** draw the party summary
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.8
   **/
  void
    Table::draw_party_summary()
    {
      /*
       *                           _________
       *   _________               |       |
       *   |       |________       |       |
       *   |       |       |       |       |
       *   |  75   |  50   |  -50  |  100  |
       *    Diether  Erwin | Theo  |  Rudi |
       *                   |_______|
       */

      Party const& party = this->ui->party();

      int min_points = 0; // minimal points made by the players
      int max_points = 0; // maximal points made by the players

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

      Gdk::Color color_pillar("brown");
      Gdk::Color color_name(::setting(Setting::NAME_FONT_COLOR));
      Gdk::Color color_winner_pillar("gold");
      Gdk::Color color_winner_name(::setting(Setting::NAME_ACTIVE_FONT_COLOR));

      int const border_x = this->width() / 10;
      int const border_y = this->height() / 10;
      int const total_width = this->width() - 2 * border_x;
      int const total_height = this->height() - 2 * border_y;

      int const baseline = (border_y
			    + (max_points
			    * total_height
			    / (max_points - min_points)) );
      int const width = total_width / party.playerno();
      for (unsigned p = 0; p < party.playerno(); ++p) {
	Player const& player = party.player(p);
	bool winner = (party.rang(player) == 0);
	int const points = party.pointsum(player);

	int const pos_x = border_x + p * width;
	int height = (points
		      * total_height
		      / (max_points - min_points));

	{ // text
	  Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(this->pixmap);
	  gc->set_rgb_fg_color( winner
				? color_winner_name
				: color_name);

	  { // draw name
	    this->name_layout->set_text(player.name());
	    int layout_width, layout_height;
	    this->name_layout->get_pixel_size(layout_width,
					      layout_height);
	    // bug: there seems to be a problem with italic characters
	    layout_width += 2 * layout_height / 5;
	    this->pixmap->draw_layout(gc,
				      pos_x + (width - layout_width) / 2,
				      ( (height >= 0)
					? (baseline - layout_height * 6 / 5)
					: baseline ),
				      this->name_layout);
	  } // draw name
	  { // draw points
	    this->trickpile_points_layout->set_text(DK::Utils::String::to_string(points));
	    int layout_width, layout_height;
	    this->trickpile_points_layout->get_pixel_size(layout_width,
							  layout_height);
	    // bug: there seems to be a problem with italic characters
	    layout_width += 2 * layout_height / 5;

	    // fix y-position so that the name and the points do not overlap
	    if ( (height >= 0)
		 && (height < layout_height) )
	      height = layout_height;
	    else if ( (height < 0)
		      && (height > -layout_height) )
	      height = -layout_height;

	    this->pixmap->draw_layout(gc,
				      pos_x + (width - layout_width) / 2,
				      baseline - height
				      + ( (height >= 0)
					  ? ( - layout_height * 6 / 5)
					  : ( layout_height * 1 / 5)),
				      this->trickpile_points_layout);
	  } // draw points
	} // text
      } // for ( p < party.playerno())
      for (unsigned p = 0; p < party.playerno(); ++p) {
	Player const& player = party.player(p);
	int const points = party.pointsum(player);
	bool winner = (party.rang(player) == 0);

	int const pos_x = border_x + p * width;

	{ // draw pillar
	  int const height = (points
			      * total_height
			      / (max_points - min_points));

	  int const x = pos_x;
	  int const y = ( (height >= 0) ? baseline - height : baseline );
	  int const w = width;
	  int const h = ( (height >= 0) ? height : -height );

	  // ToDo: make the darkening faster

	  Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(this->pixmap);
	  gc->set_function(Gdk::COPY);

	  Glib::RefPtr<Gdk::Image> image
	    = this->pixmap->get_image(x, y, w, h);
	  Gdk::Color c;
	  int const round_border = w / 10;
	  for (int i = 0; i < w; ++i)
	    for (int j = 0; j < h; ++j) {
	      // round border
	      if (height >= 0) {
		if ((j < round_border)
		    && ( ( (i < round_border)
			   && ( ( QUAD(round_border - i)
				  + QUAD(round_border - j) )
				> QUAD(round_border) ) )
			 || ((i > w - round_border)
			     && ( ( QUAD(round_border - (w - i))
				    + QUAD(round_border - j) )
				  > QUAD(round_border) ) ) ) )
		  continue;
	      } else {
		if ((j > h - round_border)
		    && ( ( (i < round_border)
			   && ( ( QUAD(round_border - i)
				  + QUAD(round_border - (h - j)) )
				> QUAD(round_border) ) )
			 || ((i > w - round_border)
			     && ( ( QUAD(round_border - (w - i))
				    + QUAD(round_border - (h - j)) )
				  > QUAD(round_border) ) ) ) )
		  continue;
	      }

	      image->get_colormap()->query_color(image->get_pixel(i, j), c);
	      if (winner)
		c.set_rgb_p(pow(c.get_red_p(), 3),
			    pow(c.get_green_p(), 3),
			    pow(c.get_blue_p(), 3));
	      else
		c.set_rgb_p(pow(c.get_red_p(), 2),
			    pow(c.get_green_p(), 2),
			    pow(c.get_blue_p(), 2));
	      gc->set_rgb_fg_color(c);
	      this->pixmap->draw_point(gc, x + i, y + j);
	    }
	} // draw pillar
      } // for ( p < party.playerno())

      return ;
    } // void Table::draw_party_summary()

  /**
   **
   ** draw all hands
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
    Table::draw_all_hands()
    {
      if (::game_status & GAMESTATUS::GAME) {
	for (vector<Hand*>::iterator h = this->hand_.begin();
	     h != this->hand_.end();
	     h++)
	  (*h)->draw(true);
      } // if (::game_status & GAMESTATUS::GAME)

      return ;
    } // void Table::draw_all_hands()

  /**
   **
   ** update all cards
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
    Table::cards_update()
    {
      this->draw_all();

      return ;
    } // void Table::cards_update()

  /**
   **
   ** update all cards back
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
    Table::cards_back_update()
    {
      this->draw_all_hands();

      return ;
    } // void Table::cards_back_update()

  /**
   **
   ** update all icons
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
    Table::icons_update()
    {
      // update all, because the size of the icons can have changed
      this->draw_all();

      return ;
    } // void Table::icons_update()

  /**
   **
   ** update the full area
   ** (just copy the pixmap to the drawing_area)
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Table::update_full_area()
    {
      this->update(Gdk::Rectangle(0, 0, -1, -1));

      return ;
    } // void Table::update_full_area()

  /**
   **
   ** update the area of the drawing area
   ** (just copy the area from the pixmap)
   **
   ** @param     area   area to update
   **
   ** @return    -
   **
   ** @version   0.6.2
   **
   ** @author    Diether Knof
   **
   **/
  void
    Table::update(Gdk::Rectangle const& area)
    {
      this->get_window()->draw_drawable(this->gc,
					this->pixmap,
					area.get_x(), area.get_y(),
					area.get_x(), area.get_y(),
					area.get_width(), area.get_height());

      return ;
    } // void Table::update(Gdk::Rectangle const& area);

  /**
   ** loads the background
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.4
   **/
  void
    Table::load_background()
    {
      try {
        Glib::RefPtr<Gdk::Pixmap> back_new
          = Gdk::Pixmap::create_from_xpm(this->pixmap,
                                         Gdk::Color(),
                                         ::setting.path(Setting::BACKGROUND));
        if (!back_new)
          throw Glib::FileError(Glib::FileError::FAILED,
                                "error loading pixmap from '"
                                + ::setting.path(Setting::BACKGROUND) + "'");

        this->gc->set_tile(back_new);
        this->draw_all();
      } catch (Glib::FileError const& file_error) {
        this->ui->error(::translation("Error::loading the background %sbackground%",
                                      ::setting.value(Setting::BACKGROUND)
                                     ).translation());
      } // try

      return ;
    } // void Table::load_background()

  /**
   **
   ** the name of 'player' has changed
   **
   ** @param     player   the player with the changed name
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    Table::name_changed(Player const& player)
    {
      this->draw_all();
      if (this->party_points_)
        this->party_points_->name_changed(player);

      this->gameinfo_->name_changed(player);

      if (this->last_trick_)
        this->last_trick_->name_changed(player);

      if (this->game_finished_)
        this->game_finished_->name_changed(player);

      if (this->party_finished_)
        this->party_finished_->name_changed(player);

      return ;
    } // void Table::name_changed(Player const& player)

  /**
   **
   ** updates the font
   **
   ** @param     fontname   the name of the new font
   ** @param     type   	the type of the font
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    Table::new_font(string const& fontname, Setting::TypeString const type)
    {
      switch (type) {
      case Setting::NAME_FONT:
        if (!this->name_layout)
          break;

        this->name_layout->set_font_description(Pango::FontDescription(fontname));

        this->draw_all();
        break;

      case Setting::TRICKPILE_POINTS_FONT:
        if (!this->trickpile_points_layout)
          break;

        this->trickpile_points_layout->set_font_description(Pango::FontDescription(fontname));

        for (vector<TrickPile*>::iterator t = this->trickpile_.begin();
             t != this->trickpile_.end();
             t++)
          (*t)->draw(true);
        break;

      default:
        break;
      } // switch (type)

      return ;
    } // void Table::new_font(string const& fontname, Setting::TypeString const type)

  /**
   **
   ** updates the 
   **
   ** @param     colorname   the name of the new color
   ** @param     type   	the type of the font
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   ** @todo	using 'set_foreground'
   **
   **/
  void
    Table::new_color(string const& colorname, Setting::TypeString const type)
    {
      switch (type) {
      case Setting::NAME_FONT_COLOR:
      case Setting::NAME_ACTIVE_FONT_COLOR:
      case Setting::NAME_RESERVATION_FONT_COLOR: {
        Glib::RefPtr<Gdk::GC> gc;
        switch (type) {
        case Setting::NAME_FONT_COLOR:
          gc = this->name_gc;
          break;
        case Setting::NAME_ACTIVE_FONT_COLOR:
          gc = this->name_active_gc;
          break;
        case Setting::NAME_RESERVATION_FONT_COLOR:
          gc = this->name_reservation_gc;
          break;
        default:
          break;
        } // switch (type)
        if (!gc)
          break;

        gc->set_rgb_fg_color(Gdk::Color(colorname));

        for (vector<Name*>::iterator n = this->name_.begin();
             n != this->name_.end();
             n++)
          (*n)->draw();

      } break;

      case Setting::TRICKPILE_POINTS_FONT_COLOR:
        if (!this->trickpile_points_gc)
          break;

        this->trickpile_points_gc->set_rgb_fg_color(Gdk::Color(colorname));
#ifdef POSTPONED
        this->trickpile_points_gc->set_foreground(Gdk::Color(colorname));
#endif

        for (vector<TrickPile*>::iterator t = this->trickpile_.begin();
             t != this->trickpile_.end();
             t++)
          (*t)->draw_points(true);
        break;

      case Setting::POVERTY_SHIFT_ARROW_COLOR:
        if (!this->poverty_shift_arrow_gc)
          break;

        this->poverty_shift_arrow_gc->set_rgb_fg_color(Gdk::Color(colorname));
        if ((::game_status == GAMESTATUS::GAME_POVERTY_SHIFT)
            && this->poverty_)
          this->poverty().draw();

        break;

      default:
        break;
      } // switch(type)

      return ;
    } // void Table::new_color(string const& colorname, Setting::TypeString const type)

  /**
   **
   ** the setting has changed
   **
   ** @param     type   the type that has changed
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    Table::setting_update(int const type)
    {
      switch(type) {
      case Setting::SHOW_ALL_HANDS:
        this->draw_all_hands();
        if ((::game_status == GAMESTATUS::GAME_POVERTY_SHIFT)
            && this->poverty_)
          this->poverty().draw();
        break;
      case Setting::EMPHASIZE_VALID_CARDS:
        if ( (::game_status == GAMESTATUS::GAME_PLAY)
            && (this->ui->game().player_current().type() == Player::HUMAN) ) {
          this->draw_all();
          this->mouse_cursor_update();
        }
        break ;
      case Setting::AUTOMATIC_CARD_SUGGESTION:
        if (::setting(Setting::AUTOMATIC_CARD_SUGGESTION))
          if (   (::game_status == GAMESTATUS::GAME_PLAY)
              && this->ui->party().rule()(Rule::CARD_HINT)
              && !this->ui->game().trick_current().isfull()
              && (this->ui->game().player_current().type() == Player::HUMAN))
            this->show_card_suggestion(false);
        break;
      case Setting::SHOW_TRICKPILES_POINTS:
        for (vector<TrickPile*>::iterator t = this->trickpile_.begin();
             t != this->trickpile_.end();
             t++)
          (*t)->draw();
        break;
      case Setting::ANNOUNCE_SWINES_AUTOMATICALLY:
        if (::game_status == GAMESTATUS::GAME_RESERVATION) {
          for (vector<Reservation*>::iterator r = this->reservation_.begin();
               r != this->reservation_.end();
               ++r)
            (*r)->sensitivity_update();
        } // if (::game_status == GAMESTATUS::GAME_RESERVATION)
      case Setting::BACKGROUND:
        this->load_background();
        break;
      case Setting::ROTATE_TRICK_CARDS:
#ifdef POSTPONED
        // The old trick is not cleared fully
        this->trick_->draw();
#else
        this->draw_all();
#endif
        break;
      case Setting::NAME_FONT:
      case Setting::TRICKPILE_POINTS_FONT:
        this->new_font(::setting(static_cast<Setting::TypeString const>(type)),
                       static_cast<Setting::TypeString const>(type));
        break;
      case Setting::NAME_FONT_COLOR:
      case Setting::NAME_ACTIVE_FONT_COLOR:
      case Setting::TRICKPILE_POINTS_FONT_COLOR:
      case Setting::POVERTY_SHIFT_ARROW_COLOR:
        this->new_color(::setting(static_cast<Setting::TypeString const>(type)),
                        static_cast<Setting::TypeString const>(type));
        break;
      default:
        break;
      } // switch(type)

      return ;
    } // void Table::setting_update(int const type)

  /**
   ** update the mouse cursor
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.5
   **/
  void
    Table::mouse_cursor_update()
    {
      int x, y;
      this->get_pointer(x, y);

      CursorType new_cursor_type = DEFAULT;
      if (this->ui->busy())
        new_cursor_type = BUSY;

      switch (::game_status) {
      case GAMESTATUS::GAME_POVERTY_SHIFT:
        DEBUG_ASSERTION(this->poverty_,
                        "Table::mouse_cursor_update():\n"
                        "  'this->poverty_' == NULL");
        switch (this->poverty().possible_action(x, y)) {
        case Poverty::NONE:
          break;
        case Poverty::SHIFT_CARDS:
          new_cursor_type = POVERTY_SHIFT;
          break;
        case Poverty::ACCEPT_CARDS:
          new_cursor_type = POVERTY_ACCEPT;
          break;
        case Poverty::TAKE_CARD:
          new_cursor_type = POVERTY_GET_CARD;
          break;
        case Poverty::PUT_CARD:
          new_cursor_type = POVERTY_PUT_CARD;
          break;
        case Poverty::FILL_UP:
          new_cursor_type = POVERTY_FILL_UP;
          break;
        case Poverty::RETURN_CARDS:
          new_cursor_type = POVERTY_SHIFT_BACK;
          break;
        case Poverty::GET_CARDS_BACK:
          new_cursor_type = POVERTY_GETTING_BACK;
          break;

        } // switch (this->poverty().possible_action(x, y))
        break;

      case GAMESTATUS::GAME_PLAY: {
        // the human player has to play a card
        Player& player = this->ui->game().player_current();
        if (this->get_card()) {
          DEBUG_ASSERTION((player.type() == Player::HUMAN),
                          "UI_GTKMM::Table::mouse_cursor_update():\n"
                          "  a card should be get, but the player '"
                          << player.no() << "' is not human but '"
                          << player.type() << "'");
          Card const card = this->hand(player).card_at_position(x, y);
          if (!card.is_empty()) {
            if (::setting(Setting::SHOW_IF_VALID)
                && player.game().rule()(Rule::SHOW_IF_VALID)) {
              if (player.game().trick_current().isvalid(card,
                                                        player.hand()))
                new_cursor_type = CARD_VALID;
              else
                new_cursor_type = CARD_INVALID;
            } else { // if !(SHOW_IF_VALID)
              new_cursor_type = PLAY_CARD;
            } // if !(SHOW_IF_VALID)

          } // if (card)
        } // if (player.type() == Player::HUMAN)
        break;
      } // case GAMESTATUS::GAME_PLAY:
      default:
        if (   ::in_running_game()
            && this->ui->game().trick_current().isfull()
            && this->full_trick_
            && this->trick().mouse_over_cards_sloppy()) {
          new_cursor_type = CLOSE_FULL_TRICK;
        } // if (::game_status == ...)
        break;
      } // switch(::game_type)

      if (::in_running_game()) {
        Game const& game = this->ui->game();
        for (vector<Player*>::const_iterator p = game.players_begin();
             p != game.players_end();
             ++p) {
          if (this->trickpile(**p).over_trick(x, y))
            new_cursor_type = SHOW_LAST_TRICK;
          if ((*p)->type() == Player::HUMAN) {
            if (this->icongroup(**p).mouse_over_next_announcement())
              new_cursor_type = NEXT_ANNOUNCEMENT;
          } // if ((*p)->type() == Player::HUMAN)
        } // for (p \in game.player)
      }	// if (::game_status in game)


      if (this->in_game_review())
        new_cursor_type = GAME_REVIEW;

      if (this->cursor_type != new_cursor_type) {
        if (   (this->cursor_type == NEXT_ANNOUNCEMENT)
            || (new_cursor_type == NEXT_ANNOUNCEMENT) ) {
          this->cursor_type = new_cursor_type;
          this->draw_all();
        }
        this->cursor_type = new_cursor_type;
        this->get_window()->set_cursor(Table::cursor(this->cursor_type));
      } // if (this->cursor_type != new_cursor_type)

      return ;
    } // void Table::mouse_cursor_update()

  /**
   **
   ** event: redraw part of the window
   **
   ** @param     event   the event
   **
   ** @return    true
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  bool
    Table::on_expose_event(GdkEventExpose* event)
    {
      this->update(Gdk::Rectangle(event->area.x, event->area.y,
                                  event->area.width, event->area.height));

      return true;
    } // bool Table::on_expose_event(GdkEventExpose* event)

  /**
   **
   ** event: the size has changed
   **
   ** @param     event   the event
   **
   ** @return    true
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  bool
    Table::on_configure_event(GdkEventConfigure* event)
    {
      this->pixmap = Gdk::Pixmap::create(this->get_window(),
                                         this->width(), this->height(),
                                         this->depth());

      if (!this->gc) {
        this->gc = Gdk::GC::create(this->pixmap);
        this->gc->set_fill(Gdk::TILED);
        this->load_background();

        this->poverty_shift_arrow_gc = Gdk::GC::create(this->pixmap);
        this->new_color(::setting(Setting::POVERTY_SHIFT_ARROW_COLOR),
                        Setting::POVERTY_SHIFT_ARROW_COLOR);

        this->name_layout = this->create_pango_layout("");
        this->new_font(::setting(Setting::NAME_FONT), Setting::NAME_FONT);

        this->name_gc = Gdk::GC::create(this->pixmap);
        this->new_color(::setting(Setting::NAME_FONT_COLOR),
                        Setting::NAME_FONT_COLOR);

        this->name_active_gc = Gdk::GC::create(this->pixmap);
        this->new_color(::setting(Setting::NAME_ACTIVE_FONT_COLOR),
                        Setting::NAME_ACTIVE_FONT_COLOR);

        this->name_reservation_gc = Gdk::GC::create(this->pixmap);
        this->new_color(::setting(Setting::NAME_RESERVATION_FONT_COLOR),
                        Setting::NAME_RESERVATION_FONT_COLOR);

        this->trickpile_points_layout = this->create_pango_layout("");
        this->new_font(::setting(Setting::TRICKPILE_POINTS_FONT),
                       Setting::TRICKPILE_POINTS_FONT);

        this->trickpile_points_gc = Gdk::GC::create(this->pixmap);
        this->new_color(::setting(Setting::TRICKPILE_POINTS_FONT_COLOR),
                        Setting::TRICKPILE_POINTS_FONT_COLOR);
      } // if (!this->gc)

      this->draw_all();

      return true;
    } // bool Table::on_configure_event(GdkEventConfigure* event)

  /**
   ** event: mouse motion
   **
   ** @param     event   the event
   **
   ** @return    whether there was a reaction
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.5
   **/
  bool
    Table::on_motion_notify_event(GdkEventMotion* event)
    {
      this->mouse_cursor_update();

      return false;
    } // bool Table::on_motion_notify_event(GdkEventMotion* event)

  /**
   ** -> result
   **
   ** @param     type   the cursor type
   **
   ** @return    the cursor for 'type'
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.5
   **/
  Gdk::Cursor const&
    Table::cursor(CursorType const type)
    {
      switch (type) {
      case NONE:
        DEBUG_ASSERTION(false,
                        "Table::cursor(type):\n"
                        "  'type' == NONE");
        break;
      case DEFAULT: {
        static Gdk::Cursor const cursor(Gdk::X_CURSOR);
        return cursor;
      }
        break;
      case BUSY: {
#ifdef POSTPONED
        Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_file("ui/gtkmm/cursor/busy.png");
        {
          GdkCursor* cursor = gdk_cursor_new_from_pixbuf(Gdk::Cursor(Gdk::X_CURSOR).get_display()->gobj(), pixbuf->gobj(), 0, 0);
          //Gdk::Cursor(Gdk::Pixbuf::create_from_file("ui/gtkmm/cursor/busy.png"),);
          return cursor;
        }
#endif
      }
        {
          static Gdk::Cursor const cursor(Gdk::WATCH);
          return cursor;
        }
        //return Gdk::Cursor(Gdk::EXCHANGE);
        //return Gdk::Cursor(Gdk::CLOCK);
      case POVERTY_SHIFT: {
        static Gdk::Cursor const cursor(Gdk::SB_LEFT_ARROW);
        return cursor;
      }
      case POVERTY_SHIFT_BACK: {
        static Gdk::Cursor const cursor(Gdk::SB_RIGHT_ARROW);
        return cursor;
      }
      case POVERTY_GET_CARD:
      case POVERTY_PUT_CARD:
      case POVERTY_FILL_UP: {
        static Gdk::Cursor const cursor(Gdk::HAND1);
        return cursor;
      }
      case POVERTY_ACCEPT:
      case POVERTY_GETTING_BACK: {
        static Gdk::Cursor const cursor(Gdk::HAND2);
        return cursor;
      }
      case NEXT_ANNOUNCEMENT: {
        static Gdk::Cursor const cursor(Gdk::HAND2);
        return cursor;
      }
      case PLAY_CARD: {
        static Gdk::Cursor const cursor(Gdk::HAND1);
        return cursor;
      }
      case CARD_VALID: {
        static Gdk::Cursor const cursor(Gdk::HAND1);
        return cursor;
      }
      case CARD_INVALID: {
        static Gdk::Cursor const cursor(Gdk::PIRATE);
        return cursor;
      }
      case CLOSE_FULL_TRICK: {
        static Gdk::Cursor const cursor(Gdk::HAND1);
        return cursor;
      }
      case SHOW_LAST_TRICK: {
        static Gdk::Cursor const cursor(Gdk::HAND2);
        return cursor;
      }
      case GAME_REVIEW: {
        static Gdk::Cursor const cursor(Gdk::SB_H_DOUBLE_ARROW);
        return cursor;
      }
      } // switch (type)

      {
        static Gdk::Cursor const cursor;
        return cursor;
      }
    } // static Gdk::Cursor const& Table::cursor(CursorType const type)

  /**
   ** the progress has changed -- redraw the name of the active player
   **
   ** @param     progress   new progress
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7,6
   **/
  void
    Table::progress_changed(double const progress)
    {
      if (::game_status & GAMESTATUS::GAME)
        this->name(this->ui->game().player_current()).draw();
      return ;
    } // void Table::progress_changed(double progress)

  /**
   ** the progress has finished -- redraw the name of the active player
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7,6
   **/
  void
    Table::progress_finished()
    {
      if (::game_status & GAMESTATUS::GAME)
        this->name(this->ui->game().player_current()).draw();
      return ;
    } // void Table::progress_finished()


} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
