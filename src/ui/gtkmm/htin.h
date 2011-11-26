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

#ifndef GTKMM_HTIN_HEADER
#define GTKMM_HTIN_HEADER

#include "base.h"
#include "../../basistypes.h"

class Player;

namespace UI_GTKMM_NS {
  class Table;

  class Hand;
  class TrickPile;
  class Icongroup;
  class Name;

  /**
   **
   ** @brief	base class for 'Hand', 'TrickPile', 'Icongroup' and 'Name'
   **
   ** @author	Diether Knof
   **
   **/
  class HTIN : public Base {
    public:
      HTIN(Table* const table, Player const* const player);

      ~HTIN();

      Table& table();
      Table const& table() const;
      Position position() const;
      Rotation rotation() const;
      Rotation rotation(Position const position) const;

      Hand& hand();
      Hand const& hand() const;
      Hand& hand(Position const position);
      TrickPile& trickpile();
      TrickPile const& trickpile() const;
      TrickPile& trickpile(Position const position);
      Icongroup& icongroup();
      Icongroup const& icongroup() const;
      Icongroup& icongroup(Position const position);
      Name& name();
      Name const& name() const;
      Name& name(Position const position);

      virtual void draw(bool const update = true) = 0;

    public:
      Player const* const player;

    private: // unused
      HTIN();
      HTIN(HTIN const& htin);
      HTIN& operator=(HTIN const& htin);
  }; // class HTIN : public Base

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_HTIN_HEADER

#endif // #ifdef USE_UI_GTKMM
