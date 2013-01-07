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

#ifndef CLASS_TEAM_INFORMATION_HEADER
#define CLASS_TEAM_INFORMATION_HEADER

#include "../../basistypes.h"
#include "../../card/card.h"

class HandCard;
class Player;
class Ai;
class Game;
class Trick;

#ifndef RELEASE
// card of interest, for debugging
#define COI Card(Card::CLUB, Card::ACE)
#endif

/**
 ** Contains and analyses the information, which team a player has
 ** Remark: cannot be an 'OS' because then the functions are not called in
 **         virtual games
 **
 ** @author    Diether Knof
 **/
class TeamInformation {
  friend bool operator==(TeamInformation const& team_information_a,
			  TeamInformation const& team_information_b);

  public:
  // constructor
  TeamInformation(Ai const& player);
  // copy constructor
  TeamInformation(TeamInformation const& team_information);
  // copy operator
  TeamInformation& operator=(TeamInformation const& team_information);
  // destructor
  ~TeamInformation();


  // writes the information in 'ostr'
  void write(ostream& ostr) const;


  // set the player
  void set_player(Ai const& player);

  // the corresponding game
  Game const& game() const;


  // the team value of the player
  int team_value(Player const& player) const;
  // the team value of the player
  int team_value(unsigned const playerno) const;
  // the team of the player
  Team const& team(Player const& player) const;
  // the team of the player
  Team const& team(unsigned const playerno) const;
  // the team of the player
  Team& team(Player const& player);
  // the team of the player
  Team& team(unsigned const playerno);
  // the guessed team of the player
  Team guessed_team(Player const& player) const;
  // the guessed team of the player
  Team guessed_team(unsigned const playerno) const;
  // the guessed partner
  Player const* guessed_partner() const;

  // all teams
  vector<Team> guessed_teams() const;
  // set the teams
  void set_teams(vector<Team> const& teams);

  // whether all teams are known
  bool all_known() const;
  // the certainty of the team decision
  int certainty() const;

  // the information from the game

  // the game starts
  void game_start();
  // the card is played
  void card_played(HandCard const& card);
  // an announcement is made
  void announcement_made(Announcement const& announcement,
			 Player const& player);
  // the trick is full
  void trick_full(Trick const& trick);
  // the marriage is determined
  void marriage(Player const& bridegroom, Player const& bride);
  // a player has announced a genscher
  void genscher(Player const& genscher, Player const& partner);

  // reset all values
  void reset();
  public:
  // updates the teams of the players according to the team values
  void update_teams();

  private:
  // the corresponding player
  PRIV_VAR_CP_R(Ai, player);

  // values for the team: positive -> re, negative -> contra
  PRIV_VAR_R(vector<int>, team_values);
  // the team (including maybe)
  PRIV_VAR_R(vector<Team>, teams);
  // the number of known teams (including the own)
  PRIV_VAR_R(unsigned, known_teams_no);

  private: // unused
  TeamInformation();
}; // class TeamInformation

ostream& operator<<(ostream& ostr, TeamInformation const& team_information);

bool operator==(TeamInformation const& team_information_a,
		TeamInformation const& team_information_b);
bool operator!=(TeamInformation const& team_information_a,
		TeamInformation const& team_information_b);

#endif // #ifndef CLASS_TEAM_INFORMATION_HEADER
