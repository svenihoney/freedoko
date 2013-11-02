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
#ifdef USE_NETWORK_DOKOLOUNGE

#ifndef CLASS_LOUNGE_HEADER
#define CLASS_LOUNGE_HEADER

class Lounge;
extern Lounge* lounge;

/**
 ** A lounge
 ** Till now: for DokoLounge (http://www.doko-lounge.de/)
 **
 ** @author   Diether Knof
 **/
    struct LoungeChatEntry {
      public:
        LoungeChatEntry(string const& player, string const& text) :
          player(player), text(text)
      {}

      public:
        string player;
        string text;
    }; // class LoungeChatEntry
class Lounge {
  public:
    // constructor
    Lounge();
    // destructor
    ~Lounge();

    // whether the player is logged in
    bool is_logged_in() const;

    // logged in
    void logged_in(string const& name);
    // logged out
    void logged_out();

    // add a chat entry
    void add_chat_entry(string const& player, string const& text);
    // set the help
    void set_help(string const& text);
    // set the help
    void set_blog(string const& text);
    // set the pinnwand
    void set_pin_board(string const& text);
    // set the messages
    void set_messages(string const& text);

  private:
    PRIV_VAR_R(string, name); // the name of the player, if logged in
    // Tische
    // Spieler
    PRIV_VAR_R(list<LoungeChatEntry>, chat); // the chat entries
    PRIV_VAR_R(string, help); // the help text
    PRIV_VAR_R(string, blog); // the blog text
    PRIV_VAR_R(string, pin_board); // the pin board text
    PRIV_VAR_R(string, messages); // the messages (individuel pin board)

  private: // unused
    Lounge(Lounge const&);
    Lounge& operator=(Lounge const&);
}; // class Lounge

#endif // #ifndef CLASS_LOUNGE_HEADER

#endif // #ifdef USE_NETWORK_DOKOLOUNGE
#endif // #ifdef USE_NETWORK
