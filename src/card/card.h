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
 **********************************************************************/

#ifndef CLASS_CARD_HEADER
#define CLASS_CARD_HEADER

#include "../basistypes.h"
#include "../utils/compare.h"

class HandCard;
class Game;

/// Class for a card
class Card {
  public:
    static Card const EMPTY;

    static Card const FOX;
    static Card const DOLLE;
    static Card const CHARLIE;

    static Card const CLUB_ACE;
    static Card const SPADE_ACE;
    static Card const HEART_ACE;
    static Card const DIAMOND_ACE;
    static Card const CLUB_TEN;
    static Card const SPADE_TEN;
    static Card const HEART_TEN;
    static Card const DIAMOND_TEN;
    static Card const CLUB_KING;
    static Card const SPADE_KING;
    static Card const HEART_KING;
    static Card const DIAMOND_KING;
    static Card const CLUB_QUEEN;
    static Card const SPADE_QUEEN;
    static Card const HEART_QUEEN;
    static Card const DIAMOND_QUEEN;
    static Card const CLUB_JACK;
    static Card const SPADE_JACK;
    static Card const HEART_JACK;
    static Card const DIAMOND_JACK;
    static Card const CLUB_NINE;
    static Card const SPADE_NINE;
    static Card const HEART_NINE;
    static Card const DIAMOND_NINE;


    static unsigned const NUMBER_OF_COLORS;
    static unsigned const NUMBER_OF_TCOLORS;
    static unsigned const NUMBER_OF_VALUES;

    enum Value {
      NINE = 0,
      JACK = 2,
      QUEEN = 3,
      KING = 4,
      TEN = 10,
      ACE = 11,
      NOCARDVALUE = 100 // for simpler error-tracking
    }; // enum Value

    enum TColor {
      DIAMOND = 0,
      HEART,
      SPADE,
      CLUB,
      TRUMP,
      NOCARDCOLOR = 100 // for simpler error-tracking
    }; // enum TColor
    typedef TColor Color;

    // converts the string in a value
    static Value value(string const& value);
    // converts the string in a color
    static Color color(string const& color);

    // Converts an Integer into a Value
    static Value InttoValue(int const i);

    // Covert an Integer into a Color
    static Color InttoColor(int const i);

    // maximal integer for 'to_int()'
    static int const MAX_INT;

  public:

    Card();
    Card(Card const& c);
    Card(Color const c, Value const v);
    Card(string const& name);

    ~Card();

    Card& operator=(Card const& card);
    istream& read(istream& istr);

    bool is_empty() const;
    operator bool() const;

    // convert to an integer (between 1 and 
    int to_int() const;

    unsigned points() const;

    Card::TColor tcolor(Game const& game) const;
    bool istrump(Game const& game) const;
    bool istrump(GameType const gametype, bool const dollen) const;
    bool isdolle(Game const& game) const;
    bool isdolle(GameType const gametype, bool const dollen) const;

    bool less(HandCard const& card) const;

  private:
    PRIV_VAR_R(Color, color); // Color of the card
    PRIV_VAR_R(Value, value); // Value of the card

  private: // not to be used
    Card(unsigned const color_u, unsigned const value_u);
    Card(int const color, int const value);
}; // class Card

// comparison between two cards
// for lexicographical order (not order in a trick)

bool operator==(Card const& a, Card const& b);
bool operator<(Card const& a, Card const& b);
CREATE_COMPARE_FUNCTIONS(Card)

  // whether the marriage is determined by a 'tcolor'-trick
  bool is_selector(Card::TColor const tcolor,
                   MarriageSelector const marriage_selector);

// the name of the value
string name(Card::Value const& value);
// the name of the tcolor
string name(Card::TColor const& tcolor);
// the name of the card
string name(Card const& card);


// output of the value in the stream
ostream& operator<<(ostream& ostr, Card::Value const& value);
// read the value out of the stream
istream& operator>>(istream& istr, Card::Value& value);
// output of the tcolor in the stream
ostream& operator<<(ostream& ostr, Card::TColor const& tcolor);
// read the color out of the stream
istream& operator>>(istream& istr, Card::Color& color);
// output of the card in the stream
ostream& operator<<(ostream& ostr, Card const& card);
// read the card out of the stream
istream& operator>>(istream& istr, Card& card);

#endif // #ifndef CLASS_CARD_HEADER
