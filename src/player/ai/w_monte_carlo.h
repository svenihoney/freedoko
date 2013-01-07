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

#ifndef CLASS_W_MONTE_CARLO_HEADER
#define CLASS_W_MONTE_CARLO_HEADER

#include "../../card/hand.h"
#include "../rating/rating.h"
#include "../aiconfig.h"

class VirtualGamesInterface;
class Player;
class Trick;

/// create for each card a virtual game and look, how many points the ai makes
class WMonteCarlo {
  public:
    // constructor
    WMonteCarlo(VirtualGamesInterface const& vgi,
                AiType const ai_type,
                unsigned const& future_limit,
                Rating::Type const& rating_type);
    ~WMonteCarlo();

    // the ai
    VirtualGamesInterface const& vgi() const;


    // choose the best card for the ai
    Card best_card();


  private:

    // creates the game and the players
    void init();

    // calculate the weightings for the cards of the hand
    void weightings_calc();


  private:


    void calctricks(VirtualGamesInterface const& vgi,
                    Trick const& T, unsigned const pl,
                    bool const future);

    void calc_one(VirtualGamesInterface const& vgi,
                  Trick const& T, vector<Hand> const& all,
                  unsigned const depth, unsigned const pl, int const start);

    void calc_tricks(VirtualGamesInterface const& vgi,
                     Trick const& T ,unsigned const depth,
                     unsigned const pl,
                     int const start = -1);

  private:
    // the interface for virtual games
    VirtualGamesInterface const* const vgi_p;

    // the aitype
    PRIV_VAR_R(AiType, ai_type);

    // the future limit
    PRIV_VAR_R(unsigned, future_limit);

    // the rating type
    PRIV_VAR_R(Rating::Type, rating_type);

    // the weighting (points) for playing the card
    vector<Player*> player_virt_p;

    // the hand
    PRIV_VAR_R(HandCards, hand);

    // the weighting (points) for playing the cards of the hand
    vector<long long int> weighting_p;


  private: // not implemeted
    WMonteCarlo();
    WMonteCarlo(WMonteCarlo const&);
    WMonteCarlo& operator=(WMonteCarlo const&);
}; // class WMonteCarlo 

#endif // Header Guard
