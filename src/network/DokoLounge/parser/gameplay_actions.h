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

#ifdef USE_NETWORK

#ifndef CLASS_NETWORK_CONNECTION_INTERPRETER_DOKOLOUNGE_PARSER_GAMEPLAY_ACTIONS_HEADER
#define CLASS_NETWORK_CONNECTION_INTERPRETER_DOKOLOUNGE_PARSER_GAMEPLAY_ACTIONS_HEADER

#include "lines.h"

namespace Network {
  namespace DokoLounge {
    namespace Parser {

    /**
     ** parsers for the gameplay actions
     **
     ** @author   Diether Knof
     **/
    class GameplayActions : public Lines {
      public:
	// constructor
	GameplayActions(Connection::Interpreter& interpreter);
	// destructor
	~GameplayActions();

	// interpret the given line
	bool interpret_line(string const& line);

      private:
        // finish the parsing
        void finish();
        // synchronize the game according to the actions
        void synchronize_game();

      private:
        // list with the actions
        list<GameplayAction*> actions;

      private: // unused
	GameplayActions();
	GameplayActions(GameplayActions const&);
	GameplayActions& operator=(GameplayActions const&);
    }; // class Network::Interpreter::DokoLounge::Parser::GameplayActions : public Lines

    } // namespace Parser
  } // namespace DokoLounge
} // namespace Network

#endif // #ifndef CLASS_NETWORK_CONNECTION_INTERPRETER_DOKOLOUNGE_PARSER_GAMEPLAY_ACTIONS_HEADER

#endif // #ifdef USE_NETWORK
