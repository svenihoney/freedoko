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

#ifndef CLASS_CARDS_INFORMATION_HEURISTICS_HEADER
#define CLASS_CARDS_INFORMATION_HEURISTICS_HEADER

#include "../../basistypes.h"
#include "../../card/card.h"

class HandCard;
class Trick;
class Player;
class Ai;

/**
 ** heuristics for the cards information
 **
 ** @author	Diether Knof
 **
 ** @todo       handle the case, when more then one heuristics matches
 **/
namespace CardsInformationHeuristics {
  // the player has accepted a poverty and returned cards
  void poverty_returned(Player const& player,
			Ai const& ai,
			map<Card, int>& weightings,
			unsigned const trumpno);

  void card_played(HandCard const& played_card,
	    Trick const& trick,
	    Ai const& ai,
	    map<Card, int>& weightings);
} // namespace CardsInformationHeuristics

#endif // #ifndef CLASS_CARDS_INFORMATION_HEURISTICS_HEADER
