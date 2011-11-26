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

#ifndef GTKMM_TRICKPILE_HEADER
#define GTKMM_TRICKPILE_HEADER

#include "htin.h"
class Trick;

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the graphic representation of a trickpile with the points
   **
   ** @author	Diether Knof
   **
   ** @todo	use of 'Gdk::Point' and 'Gdk::Rectangle'
   **
   **/
  class TrickPile : public HTIN {
    public:
      TrickPile(Table* const table, Player const* const player);

      ~TrickPile();

      void draw(bool const update = true);
      void draw_tricks();
      void draw_points(bool const update = false);

      Rotation pile_rotation() const;
      int pos_x() const;
      int pos_y() const;
      int width() const;
      int height() const;
      int start_x() const;
      int start_y() const;
      int tricks_step_x() const;
      int tricks_step_y() const;
      int cards_step_x() const;
      int cards_step_y() const;

      ::Trick const* over_trick(int const x, int const y) const;

    private: // unused
      TrickPile();
      TrickPile(TrickPile const& hand);
      TrickPile& operator=(TrickPile const& hand);
  }; // class TrickPile : public HTIN

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_TRICKPILE_HEADER

#endif // #ifdef USE_UI_GTKMM
