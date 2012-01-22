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

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "ui.h"

#include "table.h"
#include "poverty.h"

#include "../../card/hand_card.h"

namespace UI_GTKMM_NS {

  /**
   **
   ** 'player' has a poverty and shifts cards
   **
   ** @param     player   the player who has the poverty
   **
   ** @return    -
   **
   ** @version   0.6.6
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::poverty_shifting(Player const& player)
    {
      this->thrower.inc_depth();

      this->table->draw_all();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::poverty_shifting(Player const& player)

  /**
   **
   ** 'player' shifts 'cardno' cards
   **
   ** @param     player   the player who shifts the cards
   ** @param     cardno   the number of cards that are shifted
   **
   ** @return    -
   **
   ** @version   0.6.2
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::poverty_shift(Player const& player, unsigned const cardno)
    { 
      this->thrower.inc_depth();

      this->table->poverty().shift(player, cardno);

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::poverty_shift(Player const& player, unsigned const cardno)

  /**
   **
   ** ask 'player' whether to accept the poverty
   **
   ** @param     player   the player who is asked
   ** @param     cardno   the number of cards that are shifted
   **
   ** @return    -
   **
   ** @version   0.6.2
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::poverty_ask(Player const& player, unsigned const cardno)
    { 
      this->thrower.inc_depth();

      this->table->poverty().ask(player, cardno);

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::poverty_ask(Player const& player, unsigned const cardno)

  /**
   **
   ** 'player' denied to take the shifted cards
   **
   ** @param     player   the player who has denied to take the cards
   **
   ** @return    -
   **
   ** @version   0.6.2
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::poverty_take_denied(Player const& player)
    { 
      this->thrower.inc_depth();

      this->table->poverty().take_denied(player);

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::poverty_take_denied(Player const& player)

  /**
   **
   ** all players have denied to take the cards
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.2
   **
   ** @author    Diether Knof
   **
   ** @todo	show a window
   **
   **/
  void
    UI_GTKMM::poverty_take_denied_by_all()
    { 
      this->thrower.inc_depth();

      this->table->poverty().take_denied_by_all();

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::poverty_take_denied_by_all()

  /**
   **
   ** 'player' accepts to take the shifted cards
   ** and returns 'cardno' cards with 'trumpno' trumps
   **
   ** @param     player   the player who has denied to take the cards
   ** @param     cardno   number of cards that are given back
   ** @param     trumpno   number of trumps of the cards
   **
   ** @return    -
   **
   ** @version   0.6.2
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::poverty_take_accepted(Player const& player,
				    unsigned const cardno,
				    unsigned const trumpno)
    { 
      this->thrower.inc_depth();

      this->table->poverty().take_accepted(player, cardno, trumpno);

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::poverty_take_accepted(Player player, unsigned cardno, unsigned trumpno)

  /**
   **
   ** returns which cards the player shifts
   **
   ** @param     player   the player who shifts the cards
   **
   ** @return    the cards that are to be shifted
   **
   ** @version   0.6.8
   **
   ** @author    Diether Knof
   **
   **/
  HandCards
    UI_GTKMM::poverty_shift(Player& player)
    { 
      this->thrower.inc_depth();

      HandCards const& cards = this->table->poverty().shift(player);

      this->thrower.dec_depth();

      return cards;
    } // vector<HAndCard> UI_GTKMM::poverty_shift(Player& player)

  /**
   **
   ** returns whether 'player' accepts the shifted cards
   **
   ** @param     player   the player who shifts the cards
   ** @param     cardno   the number of shifted cards
   **
   ** @return    whether to accept the cards
   **
   ** @version   0.6.8
   **
   ** @author    Diether Knof
   **
   **/
  bool
    UI_GTKMM::poverty_take_accept(Player& player, unsigned cardno)
    { 
      this->thrower.inc_depth();

      bool const accept = this->table->poverty().take_accept(player, cardno);

      this->thrower.dec_depth();

      return accept;
    } // void UI_GTKMM::poverty_take_accept(Player& player, unsigned cardno)

  /**
   **
   ** changes the cards from the poverty-player
   **
   ** @param     player   the player who has accepted the cards
   ** @param     cards   the cards that are given to the player
   **
   ** @return    the cards that are returned to the poverty-player
   **
   ** @version   0.6.8
   **
   ** @author    Diether Knof
   **
   **/
  HandCards
    UI_GTKMM::poverty_cards_change(Player& player,
				   HandCards const& cards)
    { 
      this->thrower.inc_depth();

      HandCards const& returned_cards
	= this->table->poverty().cards_change(player, cards);

      this->thrower.dec_depth();

      return returned_cards;
    } // void UI_GTKMM::poverty_cards_change(Player& player, HandCards cards)

  /**
   **
   ** the poverty player 'player' gets 'cards'
   **
   ** @param     player   the player gets the cards
   ** @param     cards   the cards that are given to the player
   **
   ** @return    -
   **
   ** @version   0.6.8
   **
   ** @author    Diether Knof
   **
   **/
  void
    UI_GTKMM::poverty_cards_get_back(Player& player,
				     HandCards const& cards)
    { 
      this->thrower.inc_depth();

      this->table->poverty().cards_get_back(player, cards);

      this->thrower.dec_depth();

      return ;
    } // void UI_GTKMM::poverty_cards_get_back(Player& player, HandCards cards)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
