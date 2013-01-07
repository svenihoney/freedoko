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
#include "trick.h"
#include "hand.h"
#include "trickpile.h"
#include "icongroup.h"
#include "name.h"

#include "ui.h"
#include "cards.h"

#include "../../player/player.h"
#include "../../player/human/human.h"
#include "../../party/party.h"
#include "../../game/game.h"
#include "../../misc/setting.h"

#include <gtkmm/drawingarea.h>

namespace UI_GTKMM_NS {

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the poverty
   **
   ** @version   0.6.2
   **
   ** @author    Diether Knof
   **
   **/
  Poverty&
    Table::poverty()
    {
      return *this->poverty_;
    } //Poverty& Table::poverty()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the trick
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Trick&
    Table::trick()
    {
      return *this->trick_;
    } // Trick& Table::trick()

  /**
   **
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the hand object of 'player'
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Hand&
    Table::hand(Player const& player)
    {
      DEBUG_ASSERTION((this->hand_.size() > player.no()),
		      "Table::hand(player):\n"
		      "  player.no() = " << player.no() 
		      << " > " << this->hand_.size()
		      << " = this->hand_.size()");

      return *this->hand_[player.no()];
    } // Hand& Table::hand(Player const& player)

  /**
   **
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the hand object of 'player'
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Hand const&
    Table::hand(Player const& player) const
    {
      DEBUG_ASSERTION((this->hand_.size() > player.no()),
		      "Table::hand(player):\n"
		      "  player.no() = " << player.no() 
		      << " > " << this->hand_.size()
		      << " = this->hand_.size()");

      return *this->hand_[player.no()];
    } // Hand const& Table::hand(Player const& player) const

  /**
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the hand object at 'position'
   **
   ** @version   0.6.3
   **
   ** @author    Diether Knof
   **/
  Hand&
    Table::hand(Position const position)
    {
      return this->hand(this->player(position));
    } // Hand& Table::hand(Position const position)

  /**
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the hand object at 'position'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  Hand const&
    Table::hand(Position const position) const
    {
      return this->hand(this->player(position));
    } // Hand const& Table::hand(Position const position) const

  /**
   **
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the trickpile object of 'player'
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  TrickPile&
    Table::trickpile(Player const& player)
    {
      DEBUG_ASSERTION((this->trickpile_.size() > player.no()),
		      "Table::trickpile(player):\n"
		      "  player.no() = " << player.no() 
		      << " > " << this->trickpile_.size()
		      << " = this->trickpile_.size()");

      return *this->trickpile_[player.no()];
    } // TrickPile& Table::trickpile(Player const& player)

  /**
   **
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the trickpile object of 'player'
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  TrickPile const&
    Table::trickpile(Player const& player) const
    {
      DEBUG_ASSERTION((this->trickpile_.size() > player.no()),
		      "Table::trickpile(player):\n"
		      "  player.no() = " << player.no() 
		      << " > " << this->trickpile_.size()
		      << " = this->trickpile_.size()");

      return *this->trickpile_[player.no()];
    } // TrickPile const& Table::trickpile(Player const& player) const

  /**
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the hand object at 'position'
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.3
   **/
  TrickPile&
    Table::trickpile(Position const position)
    {
      return this->trickpile(this->player(position));
    } // TrickPile& Table::trickpile(Position const position)

  /**
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the hand object at 'position'
   **
   ** @author    Diether Knof
   **
   ** @version   0.8.5
   **/
  TrickPile const&
    Table::trickpile(Position const position) const
    {
      return this->trickpile(this->player(position));
    } // TrickPile const& Table::trickpile(Position const position) const

  /**
   **
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the icongroup object of 'player'
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Icongroup&
    Table::icongroup(Player const& player)
    {
      DEBUG_ASSERTION((this->icongroup_.size() > player.no()),
		      "Table::icongroup(player):\n"
		      "  player.no() = " << player.no() 
		      << " > " << this->icongroup_.size()
		      << " = this->icongroup_.size()");

      return *this->icongroup_[player.no()];
    } // Icongroup& Table::icongroup(Player const& player)

  /**
   **
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the icongroup object of 'player'
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Icongroup const&
    Table::icongroup(Player const& player) const
    {
      DEBUG_ASSERTION((this->icongroup_.size() > player.no()),
		      "Table::icongroup(player):\n"
		      "  player.no() = " << player.no() 
		      << " > " << this->icongroup_.size()
		      << " = this->icongroup_.size()");

      return *this->icongroup_[player.no()];
    } // Icongroup const& Table::icongroup(Player const& player) const

  /**
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the hand object at 'position'
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.3
   **/
  Icongroup&
    Table::icongroup(Position const position)
    {
      return this->icongroup(this->player(position));
    } // Icongroup& Table::icongroup(Position const position)

  /**
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the hand object at 'position'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  Icongroup const&
    Table::icongroup(Position const position) const
    {
      return this->icongroup(this->player(position));
    } // Icongroup const& Table::icongroup(Position const position) const

  /**
   **
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the name object of 'player'
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Name&
    Table::name(Player const& player)
    {
      DEBUG_ASSERTION((this->name_.size() > player.no()),
		      "Table::name(player):\n"
		      "  player.no() = " << player.no() 
		      << " > " << this->name_.size()
		      << " = this->name_.size()");

      return *this->name_[player.no()];
    } // Name& Table::name(Player const& player)

  /**
   **
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the name object of 'player'
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  Name const&
    Table::name(Player const& player) const
    {
      DEBUG_ASSERTION((this->name_.size() > player.no()),
		      "Table::name(player):\n"
		      "  player.no() = " << player.no() 
		      << " > " << this->name_.size()
		      << " = this->name_.size()");

      return *this->name_[player.no()];
    } // Name const& Table::name(Player const& player) const

  /**
   **
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the name object at 'position'
   **
   ** @version   0.6.3
   **
   ** @author    Diether Knof
   **
   **/
  Name&
    Table::name(Position const position)
    {
      return this->name(this->player(position));
    } // Name& Table::name(Position const position)
  
  /**
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the position of 'player'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **/
  Position
    Table::position(Player const& player) const
    {
      return ::setting.position(player);
    } // Position Table::position(Player const& player) const

  /**
   ** -> result
   **
   ** @param     position   position of the player
   **
   ** @return    the player at 'position'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **
   ** @todo      remove the for-loop
   **/
  Player const&
    Table::player(Position const position) const
    {
      for (unsigned p = 0; p < this->ui->game().playerno(); ++p)
        if (::setting.position(this->ui->game().player(p)) == position)
          return this->ui->game().player(p);

      DEBUG_ASSERTION(false,
                      "Table::player(position)\n"
                      "  could not find player at position "
                      << "'" << position << "'");

      return this->ui->game().player(0);
    } // Player const& Table::player(Position const position) const

  /**
   ** -> result
   **
   ** @param     player   the player
   **
   ** @return    the rotation of 'player'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **/
  Rotation
    Table::rotation(Player const& player) const
    {
      return this->rotation(this->position(player));
    } // Rotation Table::rotation(Player const& player) const

  /**
   ** -> result
   **
   ** @param     position   the position
   **
   ** @return    the rotation of the player at 'position'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **/
  Rotation
    Table::rotation(Position const position) const
    {
      switch (position) {
      case NORTH:
        return DOWN;
      case SOUTH:
        return UP;
      case WEST:
        return RIGHT;
      case EAST:
        return LEFT;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Table::rotation(position):\n"
                        "  wrong position CENTER (" << position << ")");
      } // switch(this->position())

      return UP;
    } // Rotation Table::rotation(Position const position) const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the width of the table
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  unsigned
    Table::width() const
    {
      return this->get_width();
    } // unsigned Table::width() const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the height of the table
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  unsigned
    Table::height() const
    {
      return this->get_height();
    } // unsigned Table::height() const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the depth of the table
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  unsigned
    Table::depth() const
    {
      return this->get_window()->get_depth();
    } // unsigned Table::depth() const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    border on the left and right
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  unsigned
    Table::border_x() const
    {
      return (this->ui->cards->height() / 10);
    } // unsigned Table::border_x() const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    border on the top and bottom
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  unsigned
    Table::border_y() const
    {
      return (this->ui->cards->height() / 10);
    } // unsigned Table::border_y() const

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
