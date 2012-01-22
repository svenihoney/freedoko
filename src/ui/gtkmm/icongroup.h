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

#ifndef GTKMM_ICONGROUP_HEADER
#define GTKMM_ICONGROUP_HEADER

#include "htin.h"

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the graphic representation of a hand
   **
   ** @author	Diether Knof
   **
   ** @todo	test overlapping of icons
   ** @todo	use of 'Gdk::Point' and 'Gdk::Rectangle'
   **
   **/
  class Icongroup : public HTIN {
    public:
      Icongroup(Table* const table, Player const* const player);

      ~Icongroup();

      void draw(bool const update = true);
      void draw_team(bool const update = true);
      void draw_announcement(bool const update = true);
      void draw_swines(bool const update = true);

      int width(Position const position) const;
      int height(Position const position) const;
      int margin_x(Position const position) const;
      int margin_y(Position const position) const;

      int width() const;
      int height() const;
      int margin_x() const;
      int margin_y() const;

      int team_start_x(int const width) const;
      int team_start_y(int const height) const;
      int announcement_start_x(int const width) const;
      int announcement_start_y(int const height) const;
      int swines_start_x(int const width) const;
      int swines_start_y(int const height) const;

      bool mouse_over_next_announcement() const;

    private: // unused
      Icongroup();
      Icongroup(Icongroup const& hand);
      Icongroup& operator=(Icongroup const& hand);
  }; // class Icongroup : public HTIN

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_ICONGROUP_HEADER

#endif // #ifdef USE_UI_GTKMM
