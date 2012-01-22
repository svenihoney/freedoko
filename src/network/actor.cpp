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
 *********************************************************************/

#ifdef USE_NETWORK

#include "constants.h"

#include "actor.h"
#include "server.h"

#include "../party/party.h"
#include "../party/rule.h"
#include "../game/game.h"

#include "../ui/ui.h"

namespace Network {

  /**
   ** constructor
   **
   ** @param     connection   corresponding connection
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  Connection::Actor::Actor(Connection& connection) :
    connection_(&connection)
  { }


  /**
   ** destructor
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  Connection::Actor::~Actor()
  { }


  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the party (writable)
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **
   ** @bug       const cast
   **/
  Party&
    Connection::Actor::party()
    {
      return const_cast<Party&>(this->connection().server().party());
    } // Party& Connection::Actor::party()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the game (writable)
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **
   ** @bug       const cast
   **/
  Game&
    Connection::Actor::game()
    {
      return const_cast<Game&>(this->connection().server().game());
    } // Game& Connection::Actor::game()

  /**
   ** chat the text
   **
   ** @param     text   text to chat
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  void
    Connection::Actor::chat(string const& text)
    {
      ::ui->chat(this->connection().name(), text, false);
      return ;
    } // void Connection::Actor::chat(string text)

  /**
   ** set the player type
   **
   ** @param     player    the player to set the type of
   ** @param     network   whether the player is a network player (or a local)
   **
   ** @return    whether the type was accepted
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **
   ** @todo      all
   **/
  bool
    Connection::Actor::set_player_type(Player const& player,
                                       bool const network)
    {
      if (!(this->connection().relation() == Connection::PARENT))
        return false;

      if (network)
        this->connection().set_to_network_player(player);
      else
        this->connection().set_to_local_player(player);

      return true;
    } // bool Connection::Actor::set_player_type(Player player, bool network)

  /**
   ** set the player name
   **
   ** @param     player   the player to set the name of
   ** @param     name     name of the player
   **
   ** @return    whether the name was accepted
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **/
  bool
    Connection::Actor::set_player_name(Player const& player,
                                       string const& name)
    {
      if (!this->connection().manages_player(player))
        return false;

      this->party().player(player.no()).set_name(name);

      return true;
    } // bool Connection::Actor::set_player_name(Player player, string name)

  /**
   ** the players are switched
   **
   ** @param	player_a   first player
   ** @param	player_b   second player
   **
   ** @return	-
   **
   ** @author	Diether Knof
   **
   ** @version	0.7.3
   **/
  void
    Connection::Actor::players_switched(unsigned const player_a,
                                        unsigned const player_b)
    {
      this->party().switch_players(player_a, player_b);

      return ;
    } // void Connection::Actor::players_switched(unsigned player_a, unsigned player_b)

} // namespace Network

#endif // #ifdef USE_NETWORK
