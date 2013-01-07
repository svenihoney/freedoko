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

#include "ui.h"

#include "main_window.h"
#include "menu.h"
#include "table.h"
#include "reservation.h"
#include "players_db.h"

#include "../../player/player.h"
#include "../../basistypes.h"

namespace UI_GTKMM_NS {

  /**
   **
   ** the game is opened
   **
   ** @param     game   the game that is opened
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.7
   **/
  void
    UI_GTKMM::game_open(Game& game)
    {
      this->thrower.inc_depth();

      this->UI::game_open(game);

      this->table->game_open();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::game_open(Game& game)

  /**
   ** the cards are distributed
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.8
   **/
  void
    UI_GTKMM::game_cards_distributed()
    {
      this->thrower.inc_depth();

      this->table->game_cards_distributed();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::game_cards_distributed()

  /**
   ** ask 'player' for a reservation
   **
   ** @param     player   player who is asked
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    UI_GTKMM::reservation_ask(Player const& player)
    {
      this->table->reservation_ask(player);

      return ;
    } // void UI_GTKMM::reservation_ask(Player player)

  /**
   ** Gets the reservation of the given player:
   ** pops up a window
   ** This is the first time, the hands can (should) be shown.
   **
   ** @param     player   player, whose reservation schould be get
   **
   ** @return    reservation of the player
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.0
   **/
  ::Reservation
    UI_GTKMM::reservation_get(Player const& player)
    {
      this->thrower.inc_depth();

      ::Reservation const reservation = this->table->reservation_get(player);

      this->thrower.dec_depth();

      return reservation;
    } // ::Reservation UI_GTKMM::reservation_get(player)

  /**
   ** the game is redistributed
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.7b
   **/
  void
    UI_GTKMM::game_redistribute()
    {
      this->thrower.inc_depth();

      this->table->game_redistribute();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::game_redistribute()

  /**
   ** the game is started:
   ** the gametype and announcements of the other players are shown
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6,1
   **/
  void
    UI_GTKMM::game_start()
    {
      this->thrower.inc_depth();

      this->main_window->menu->game_start();
      this->table->game_start();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::game_start()

  /**
   ** the game is finished:
   ** the winner and the points of the game are shown.
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6,1
   **/
  void
    UI_GTKMM::game_finish()
    { 
      this->thrower.inc_depth();

      this->players_db->update_db();
      this->main_window->menu->game_finish();
      this->table->game_finished();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::game_finish()

  /**
   ** the game is closed
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.7
   **/
  void
    UI_GTKMM::game_close()
    {
      this->thrower.inc_depth();

      this->UI::game_close();

      this->main_window->menu->game_close();
      this->table->game_close();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::game_close()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
