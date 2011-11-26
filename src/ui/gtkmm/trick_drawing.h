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

#ifndef GTKMM_TRICK_DRAWING_HEADER
#define GTKMM_TRICK_DRAWING_HEADER

#include "base.h"

class Trick;

#include <gdkmm/drawable.h>

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	drawing a trick on a Gdk::Drawable
   **
   ** @author	Diether Knof
   **
   ** @todo	use 'Gdk::Point' and 'Gdk::Rectangle'
   **
   **/
  class TrickDrawing : public Base {
    public:
      TrickDrawing(Base* const base);

      ~TrickDrawing();

      void set_center(Gdk::Point const& center);
      void set_trick(::Trick const& trick);
      void remove_trick();

      // draw the cards
      void draw(Glib::RefPtr<Gdk::Drawable> drawable);
      // whether the mouse is over the trick
      bool mouse_over_cards(int const mouse_x, int const mouse_y) const;

      Gdk::Point start() const;
      Gdk::Rectangle outline() const;
      int width() const;
      int height() const;
      int overlap_x() const;
      int overlap_y() const;

    private:
      Gdk::Point center;
    protected:
      Trick const* trick;

    private: // unused
      TrickDrawing();
      TrickDrawing(TrickDrawing const& hand);
      TrickDrawing& operator=(TrickDrawing const& hand);
  }; // class TrickDrawing : public Base 

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_TRICK_DRAWING_HEADER

#endif // #ifdef USE_UI_GTKMM
