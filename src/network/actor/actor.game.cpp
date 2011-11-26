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
 *********************************************************************/

#ifdef USE_NETWORK

#include "constants.h"

#include "../actor.h"

#include "../../party/party.h"
#include "../../party/rule.h"
#include "../../player/network/network_player.h"
#include "../../game/gameplay_actions.h"

namespace Network {

  /**
   ** a gameplay action
   **
   ** @param     action   gameplay action
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **
   ** @todo      mostly all
   **/
  void
    Connection::Actor::gameplay_action(GameplayAction const& action)
    {
      //COUT << "actor: gameplay action: " << action << endl;

      NetworkPlayer* player = NULL;
      if (   (dynamic_cast<GameplayAction::Player1 const*>(&action))
	  && (action.type != GameplayAction::ANNOUNCEMENT) ) {
	player = this->connection().managed_player(dynamic_cast<GameplayAction::Player1 const&>(action).player);
	if (player == NULL)
	  return ;
      } // if (dynamic_cast<GameplayAction::Player1*>(&action))


      switch (action.type) {
      case GameplayAction::RESERVATION:
	player->set_reservation(dynamic_cast<GameplayAction::Reservation const&>(action).reservation);
	break;

      case GameplayAction::MARRIAGE:
	// nothing to do
	break;

      case GameplayAction::GENSCHER:
	player->set_genscher_partner(dynamic_cast<GameplayAction::Genscher const&>(action).partner);
	break;

      case GameplayAction::POVERTY_SHIFT:
	player->set_poverty_cards_to_shift(dynamic_cast<GameplayAction::PovertyShift const&>(action).cards);
	break;

      case GameplayAction::POVERTY_ACCEPTED:
	player->set_poverty_accept(true);
	break;

      case GameplayAction::POVERTY_RETURNED:
	player->set_poverty_cards_to_shift(dynamic_cast<GameplayAction::PovertyReturned const&>(action).cards);
	break;

      case GameplayAction::POVERTY_DENIED:
	player->set_poverty_accept(false);
	break;

      case GameplayAction::POVERTY_DENIED_BY_ALL:
	// nothing to do
	break;

      case GameplayAction::ANNOUNCEMENT: {
	// different handling, see 'Game::make_announcement_from_network()'
	GameplayAction::Announcement const& announcement_action
	  = dynamic_cast<GameplayAction::Announcement const&>(action);
	this->game().make_announcement_from_network(announcement_action.announcement,
						    this->game().player(announcement_action.player));
      } break;

      case GameplayAction::SWINES:
	player->set_swines_announcement();
	break;

      case GameplayAction::HYPERSWINES:
	player->set_hyperswines_announcement();
	break;

      case GameplayAction::CARD_PLAYED:
	player->set_card_to_play(dynamic_cast<GameplayAction::CardPlayed const&>(action).card);
	break;

      case GameplayAction::TRICK_OPEN:
	break;

      case GameplayAction::TRICK_FULL:
	// ToDo: check the trick
	break;

      case GameplayAction::TRICK_TAKEN:
	for (vector<NetworkPlayer*>::const_iterator
	     p = this->connection().players.begin();
	     p != this->connection().players.end();
	     ++p)
	  (*p)->set_trick_taken();
	break;

      case GameplayAction::CHECK:
	// shall not happen
        DEBUG_ASSERTION(false,
                        "Gameplay action 'Check' should not be processed here");
	break;
      } // switch (action.type)

      return ;
    } // void Connection::Actor::gameplay_action(GameplayAction action)

  /**
   ** open the game
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  void
    Connection::Actor::game_open()
    {
      return ;
    } // void Connection::Actor::game_open()

  /**
   ** start the round 'roundno'
   **
   ** @param     hands   hands of the players
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **
   ** @todo      all
   **/
  void
    Connection::Actor::game_distribute_cards(vector<Hand*> hands)
    {
      return ;
    } // void Connection::Actor::game_distribute_cards(vector<Hand*> hands)

  /**
   ** start the game
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **
   ** @todo      all
   **/
  void
    Connection::Actor::game_start()
    {
      return ;
    } // void Connection::Actor::game_start()

  /**
   ** finish the game
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **
   ** @todo      all
   **/
  void
    Connection::Actor::game_finish()
    {
      return ;
    } // void Connection::Actor::game_finish()

} // namespace Network

#endif // #ifdef USE_NETWORK
