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

#ifndef CLASS_NETWORK_CONNECTION_INTERPRETER_DOKOLOUNGE_RECEIVER_HEADER
#define CLASS_NETWORK_CONNECTION_INTERPRETER_DOKOLOUNGE_RECEIVER_HEADER

#include "interpreter.h"
#include "../receiver.h"

namespace Network {
  namespace DokoLounge {

    /**
     ** receiver interpreter for DokoLounge
     **
     ** @author	Diether Knof
     **/
    class Interpreter::Receiver : public Connection::Interpreter::Receiver {
      public:
	// constructor
	Receiver(Interpreter& interpreter);
	// destructor
	~Receiver();

	// the corresponding sender
	Sender& sender();

	// the text was received
	bool received(string const& text);

      private:
        // interpret the command
        bool interpret_command(string const& keyword, string const& text);

        // interpret a chat message
        void interpret_chat(string text);
        // interpret an alert
        void interpret_alert(string text);
        // interpret player list
        void interpret_player_list(string text);
        // interpret table list (Besetzung)
        void interpret_table_list(string text);
        // interpret table types
        void interpret_table_types(string text);

        // returns the text part of <<part>>text<</part>>
        static string text_of_part(string const& text, string const& part);
        // gets the next part of 'text' and remove it from text
        static bool strip_next_part(string& text, string& keyword, string& entry);
        // gets the name of 'text' and remove it from text
        static bool strip_next_name(string& text, string& name, string& entry);

      private:
	// a line was not send in total
	string pending_line;
	// a parser
	Network::DokoLounge::Parser::Base* parser;

      private: // unused
	Receiver();
	Receiver(Receiver const&);
	Receiver& operator=(Receiver const&);
    }; // class Interpreter::Receiver : public Connection::Interpreter::Receiver 

  } // namespace DokoLounge
} // namespace Network

#endif // #ifndef CLASS_NETWORK_CONNECTION_INTERPRETER_DOKOLOUNGE_RECEIVER_HEADER

#endif // #ifdef USE_NETWORK
