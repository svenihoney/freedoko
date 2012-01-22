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

#ifndef GTKMM_NAME_HEADER
#define GTKMM_NAME_HEADER

#include "htin.h"

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the graphic representation of a name
   **
   ** @author	Diether Knof
   **
   ** @todo	use of 'Gdk::Point' and 'Gdk::Rectangle'
   **
   **/
  class Name : public HTIN {
    public:
      Name(Table* const table, Player const* const player);

      ~Name();

      void draw(bool const update = true);

      int pos_x() const;
      int pos_y() const;
      int width() const;
      int height() const;

    private: // unused
      Name();
      Name(Name const& hand);
      Name& operator=(Name const& hand);
  }; // class Name : public HTIN

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_NAME_HEADER

#endif // #ifdef USE_UI_GTKMM
