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

#include "htin.h"
#include "table.h"

#include "../../player/player.h"

namespace UI_GTKMM_NS {

  /**
   **
   ** Constructor
   **
   ** @param     table   the table
   ** @param     player   the player
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.0
   **
   **/
  HTIN::HTIN(Table* const table, Player const* const player) :
    Base(table),
  player(player)
  {
    return ;
  } // HTIN::HTIN(Table* const table, Player const* const player)

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
   ** @version   0.6.0
   **
   **/
  HTIN::~HTIN()
  {
    return ;
  } // HTIN::~HTIN()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the table
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  Table&
    HTIN::table()
    {
      return *dynamic_cast<Table*>(this->parent);
    } // Table& HTIN::table()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the table
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  Table const&
    HTIN::table() const
    {
      return *dynamic_cast<Table*>(this->parent);
    } // Table const& HTIN::table() const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the position of the object
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  Position
    HTIN::position() const
    {
      return this->table().position(*this->player);
    } // Position HTIN::position() const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the rotation of the object
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  Rotation
    HTIN::rotation() const
    {
      return this->table().rotation(*this->player);
    } // Rotation HTIN::rotation() const

  /**
   **
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the rotation of the object at 'position'
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  Rotation
    HTIN::rotation(Position const position) const
    {
      return this->table().rotation(position);
    } // Rotation HTIN::rotation(Position const position) const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the corresponding hand
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  Hand&
    HTIN::hand()
    {
      return this->table().hand(*(this->player));
    } // Hand& HTIN::hand()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the corresponding hand
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  Hand const&
    HTIN::hand() const
    {
      return this->table().hand(*(this->player));
    } // Hand const& HTIN::hand() const

  /**
   **
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the hand at 'position'
   **
   ** @version   0.6.3
   **
   ** @author    Diether Knof
   **
   **/
  Hand&
    HTIN::hand(Position const position)
    {
      return this->table().hand(position);
    } // Hand& HTIN::hand(Position const position)

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the corresponding trickpile
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  TrickPile&
    HTIN::trickpile()
    {
      return this->table().trickpile(*(this->player));
    } // TrickPile& HTIN::trickpile()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the corresponding trickpile
   **
   ** @version   0.6.0
   **
   ** @author    Diether Knof
   **
   **/
  TrickPile const&
    HTIN::trickpile() const
    {
      return this->table().trickpile(*(this->player));
    } // TrickPile const& HTIN::trickpile() const

  /**
   **
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the trickpile at 'position'
   **
   ** @version   0.6.3
   **
   ** @author    Diether Knof
   **
   **/
  TrickPile&
    HTIN::trickpile(Position const position)
    {
      return this->table().trickpile(position);
    } // TrickPile& HTIN::trickpile(Position const position)

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the corresponding icongroup
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Icongroup&
    HTIN::icongroup()
    {
      return this->table().icongroup(*(this->player));
    } // Icongroup& HTIN::icongroup()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the corresponding icongroup
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Icongroup const&
    HTIN::icongroup() const
    {
      return this->table().icongroup(*(this->player));
    } // Icongroup const& HTIN::icongroup() const

  /**
   **
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the icongroup at 'position'
   **
   ** @version   0.6.3
   **
   ** @author    Diether Knof
   **
   **/
  Icongroup&
    HTIN::icongroup(Position const position)
    {
      return this->table().icongroup(position);
    } // Icongroup& HTIN::icongroup(Position const position)

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the corresponding name
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Name&
    HTIN::name()
    {
      return this->table().name(*(this->player));
    } // Name& HTIN::name()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the corresponding name
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Name const&
    HTIN::name() const
    {
      return this->table().name(*(this->player));
    } // Name const& HTIN::name() const

  /**
   **
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the name at 'position'
   **
   ** @version   0.6.3
   **
   ** @author    Diether Knof
   **
   **/
  Name&
    HTIN::name(Position const position)
    {
      return this->table().name(position);
    } // Name& HTIN::name(Position const position)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
