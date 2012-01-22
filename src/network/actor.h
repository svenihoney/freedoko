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

#ifndef CLASS_NETWORK_CONNECTION_ACTOR_HEADER
#define CLASS_NETWORK_CONNECTION_ACTOR_HEADER

#include "connection.h"

class GameplayAction;

namespace Network {

  /**
   ** actor for the network messages
   **
   ** @author	Diether Knof
   **/
  class Connection::Actor {
    public:
      // constructor
      Actor(Connection& connection);
      // destructor
      ~Actor();

      // chat
      void chat(string const& text);

      // set the rule
      bool set_rule(string const& text);
      // set the rule
      bool set_rule(int const type, int const value);

      // sets the player type
      bool set_player_type(Player const& player, bool const network);
      // sets the player name
      bool set_player_name(Player const& player, string const& name);
      // set the party startplayer
      void players_switched(unsigned const player_a, unsigned const player_b);


      // a gameplay action
      void gameplay_action(GameplayAction const& action);

      // set the party startplayer
      void party_set_startplayer(unsigned const startplayer);
      // set the party seed
      void party_set_seed(unsigned const seed);
      // start the party
      void party_start();
      // start the round 'roundno'
      void party_start_round(unsigned const roundno);
      // finish the party
      void party_finish();

      // open the game
      void game_open();
      // distribute the cards
      void game_distribute_cards(vector<Hand*> hands);
      // start the game
      void game_start();
      // finish the game
      void game_finish();

      // the corresponding party
      Party& party();
      // the corresponding game
      Game& game();

    private:
      // the corresponding connection
      PRIV_VAR_P_RW(Connection, connection);

    private: // unused
      Actor();
      Actor(Actor const&);
      Actor& operator=(Actor const&);
  }; // class Connection::Actor

} // namespace Network

#endif // #ifndef CLASS_NETWORK_CONNECTION_ACTOR_HEADER

#endif // #ifdef USE_NETWORK
