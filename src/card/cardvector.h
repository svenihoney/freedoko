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
 **********************************************************************/

#ifndef CLASS_CARD_VECTOR_HEADER
#define CLASS_CARD_VECTOR_HEADER

#include "../basistypes.h"

#include "card.h"

#include <map>

struct Card_map_less
{
  bool operator()(Card const& a, Card const& b) const
  {
    return ((a.color() < b.color())
	    || ((a.color() == b.color())
		&& (a.value() < b.value())));
  }
}; // struct map_less

#ifdef POSTPONED

template<typename T>
typedef std::map<Card, T, Card_map_less> CardVector;

#else

template<typename T>
class CardVector : public std::map<Card, T, Card_map_less> {
  public:
    CardVector() :
      std::map<Card, T, Card_map_less>()
      { }
    CardVector(CardVector const& cv) :
      std::map<Card, T, Card_map_less>(cv)
    { }
    ~CardVector()
    { }

    CardVector const&
    operator=(CardVector const& cv)
    {
      static_cast<std::map<Card, T, Card_map_less>& >(*this) = cv;
      return *this;
    }

  private:
}; // class CardVector : public std::map<Card, T, map_less>

#endif

#endif // HEADERGUARD
