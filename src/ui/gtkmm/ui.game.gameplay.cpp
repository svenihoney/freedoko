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

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "ui.h"

#include "table.h"
#include "trick.h"
#include "hand.h"
#include "name.h"
#include "main_window.h"
#include "menu.h"
#include "players_db.h"

#include "../../party/party.h"
#include "../../game/game.h"
#include "../../player/player.h"
#include "../../player/ai/ai.h"
#include "../../card/trick.h"
#include "../../os/bug_report_replay.h"

#include "../../game/gameplay_actions.h"

#include <gtkmm/main.h>

namespace UI_GTKMM_NS {

  /**
   ** a gameplay action
   **
   ** @param     action   the action
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    UI_GTKMM::gameplay_action(GameplayAction const& action)
    {
      this->thrower.inc_depth();

      this->table->gameplay_action(action);

	switch (action.type) {
	case GameplayAction::ANNOUNCEMENT: {
	  Announcement const& announcement
	    = static_cast<GameplayAction::Announcement const&>(action).announcement;
	  Player const& player
	    = this->game().player(static_cast<GameplayAction::Announcement const&>(action).player);
	  this->main_window->menu->announcements_update();
	  this->table->announcement_made(announcement, player);
	  break;
	}

	case GameplayAction::SWINES: {
	  Player const& player
	    = this->game().player(static_cast<GameplayAction::Swines const&>(action).player);
	  this->main_window->menu->announcements_update();
	  this->table->swines_announced(player);
	}
	  break;

	case GameplayAction::HYPERSWINES: {
	  Player const& player
	    = this->game().player(static_cast<GameplayAction::Hyperswines const&>(action).player);
	  this->main_window->menu->announcements_update();
	  this->table->hyperswines_announced(player);
	  break;
	}

	case GameplayAction::MARRIAGE: {
	  Player const& bridegroom
	    = this->game().player(static_cast<GameplayAction::Marriage const&>(action).player);
	  Player const& bride
	    = this->game().player(static_cast<GameplayAction::Marriage const&>(action).bride);
      this->main_window->menu->announcements_update();
      this->main_window->menu->language_update();
      this->table->marriage(bridegroom, bride);
      break;
	}

	case GameplayAction::GENSCHER: {
	  Player const& genscher
	    = this->game().player(static_cast<GameplayAction::Genscher const&>(action).player);
	  Player const& partner
	    = this->game().player(static_cast<GameplayAction::Genscher const&>(action).partner);
	  this->main_window->menu->announcements_update();
	  this->table->genscher(genscher, partner);
	  break;
	}

	case GameplayAction::RESERVATION:
	  // nothing to do
	  break;
	case GameplayAction::CARD_PLAYED:
	  // much to do, see this->card_played(HandCard const& card)
	  break;

	case GameplayAction::POVERTY_SHIFT:
	case GameplayAction::POVERTY_ACCEPTED:
	case GameplayAction::POVERTY_RETURNED:
	case GameplayAction::POVERTY_DENIED:
	case GameplayAction::POVERTY_DENIED_BY_ALL:
	case GameplayAction::TRICK_OPEN:
	case GameplayAction::TRICK_FULL:
	case GameplayAction::TRICK_TAKEN:
	case GameplayAction::CHECK:
	  // ToDo
	  break;
	} // switch (action.type)

      this->thrower.dec_depth();

	return ;
    } // void UI_GTKMM::gameplay_action(GameplayAction const& action)

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    selected partner of the player
   **		(NULL if the genscher is not announced)
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  Player const*
    UI_GTKMM::genscher_partner()
    {
      this->thrower.inc_depth();

      Player const* const partner
	= this->table->select_genscher_partner(::party.game().player_current());

      this->thrower.dec_depth();

      return partner;
    } // Player const* UI_GTKMM::genscher_partner()

  /**
   **
   ** let the user select a card for the given player
   **
   ** @param     player   the player who is to play a card
   **
   ** @return    the selected card
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  HandCard
    UI_GTKMM::card_get(Player const& player)
    {
      this->thrower.inc_depth();

      this->table->card_get(player);

      this->main_window->menu->card_get();
      const_cast<Player&>(player).hand().forget_request();

      try {
	if (   (   ::setting(Setting::AUTOMATIC_CARD_SUGGESTION)
		&& this->game().rule()(Rule::CARD_HINT))
	    || (   ::bug_report_replay
		&& (player.type() == Player::HUMAN) )
	   )
	  this->table->show_card_suggestion(false);

	do {
          while (   !this->ui->thrower
                 && (::game_status == GAMESTATUS::GAME_PLAY))
            ::ui->wait();
          if (this->ui->thrower) {
            try {
              this->thrower.throw_it();
            } catch (Card const& card) {
              DEBUG_ASSERTION((card == player.hand().requested_card()),
                              "UI_GTKMM::card_get(player):\n"
                              "  did not catch the requested card:\n"
                              "  " << card << " != " << player.hand().requested_card()
                             );
            } // try
          } // if (this->ui->thrower)

          if (player.hand().requested_card().is_empty()) {
            const_cast<Player&>(player).hand().forget_request();
            continue;
          }
#ifndef OUTDATED
          // the player cannot play an invalid card
          if (!player.game().trick_current().isvalid(player.hand().requested_card(),
                                                     player.hand())) {
            const_cast<Player&>(player).hand().forget_request();
            continue;
          }
#endif

        } while (player.hand().requested_position() == UINT_MAX);
      } catch (...) {
        this->table->card_got();
        this->main_window->menu->card_got();
        throw;
      } // catch(...)

      this->table->card_got();
      this->main_window->menu->card_got();

      this->thrower.dec_depth();

      return player.hand().requested_card();
    } // HandCard UI_GTKMM::card_get(Player const& player)

  /**
   **
   ** the given player has played 'card.
   ** remove the card from the hand
   **
   ** @param     card   the played card
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::card_played(HandCard const& card)
    { 
      this->thrower.inc_depth();

      Player const& player = card.player();

      // for the sensitivity of the swines button
      this->main_window->menu->announcements_update();

      // for updating the heuristics
      this->players_db->update_db();

      this->table->hand(player).draw(true);
      this->table->name(player).draw(true);
      this->table->trick().draw(true);
      this->table->name(player.game().player_current()).draw(true);

      if (::setting(Setting::SHOW_ALL_HANDS)
          || (::game_status == GAMESTATUS::GAME_FINISHED)
          || (::game_status == GAMESTATUS::GAME_REDISTRIBUTE)
          || !::setting(Setting::SHOW_AI_INFORMATION_HANDS)
          || (player.game().humanno() != 1)
         )
        this->table->card_played(player);
      else
        this->table->draw_all();

      this->update();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::card_player(HandCard const& card)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
