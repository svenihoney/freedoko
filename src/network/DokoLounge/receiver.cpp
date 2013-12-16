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
 ********************************************************************/

#ifdef USE_NETWORK

#include "constants.h"

#include "receiver.h"
#include "sender.h"

#include "parser/parsers.h"

#include "../server.h"

#include "../../party/party.h"
#include "../../game/gameplay_action.h"
#include "../../misc/lounge.h"
#include "../../utils/string.h"

#include <cstring>

#define EXPECT_KEYWORD(KW) \
        this->strip_next_part(text, keyword, entry); \
        if (keyword != KW) { \
          CLOG << "'interpret alert': unknown keyword '" << keyword << "' expected '" << KW << "'\n" \
            << "entry: " << entry << "\n" \
            << "remaining text: " << text << "\n"; \
          return; \
        } else

namespace Network {
  namespace DokoLounge {
    /*
<<neuesKonto>>Der Name "FreeDoko" ist leider schon vergeben.
Bitte ein Passwort mit mindestens 4 Buchsteben/Zahlen wählen.<</neuesKonto>>

Mit falscher Version angemeldet
<<deinName>><</deinName>><<fuxbauchat>>Bitte lade dir von unserer Homepage (www.doko-lounge.de) eine aktuelle Version herunter. Mit dieser Version kannst du nicht mehr spielen.<</fuxbauchat>><<offline>><</offline>>


Anzahl der Spieler hat sich geändert
<<LobbySpieler>><<anz>>3<</anz>><<User1>>MacHolstein<</User1>><<UserIcon1>>face4.jpg<</UserIcon1>><<User2>>Diether<</User2>><<UserIcon2>>tux.jpg<</UserIcon2>><<User3>>FreeDoko<</User3>><<UserIcon3>>fux1.jpg<</UserIcon3>><</LobbySpieler>>

Spieler ist einem Tisch beigetreten
<<Tischart>><<Tischanz>>5<</Tischanz>><<1>>Sonderregeln<</1>><<2>>Lerntisch<</2>><<3>>Sonderregeln<</3>><<4>>DDV<</4>><<5>>Sonderregeln<</5>><</Tischart>>
< <<alleTische>>(TischSpieler01)Diether,frei,frei,frei||(TischSpieler02)frei,frei,frei,frei||(TischSpieler03)frei,frei,frei,frei||(TischSpieler04)frei,frei,frei,frei||(TischSpieler05)frei,frei,frei,frei||<</alleTische>><<Tischart>><<Tischanz>>5<</Tischanz>><<1>>Sonderregeln<</1>><<2>>Lerntisch<</2>><<3>>Sonderregeln<</3>><<4>>DDV<</4>><<5>>Sonderregeln<</5>><</Tischart>><<alleTische>>(TischSpieler01)Diether,frei,frei,frei||(TischSpieler02)frei,frei,frei,frei||(TischSpieler03)frei,frei,frei,frei||(TischSpieler04)frei,frei,frei,frei||(TischSpieler05)frei,frei,frei,frei||<</alleTische>>
*/

    /**
     ** constructor
     **
     ** @param	interpreter	corresponding interpreter
     **
     ** @return	-
     **
     ** @author	Diether Knof
     **
     ** @version	0.7.12
     **/
    Interpreter::Receiver::Receiver(Interpreter& interpreter) :
      Connection::Interpreter::Receiver(interpreter),
      pending_line(),
      parser(NULL)
    { }

    /**
     ** destructor
     **
     ** @param	-
     **
     ** @return	-
     **
     ** @author	Diether Knof
     **
     ** @version	0.7.1
     **/
    Interpreter::Receiver::~Receiver()
    { }

    /**
     ** -> result
     **
     ** @param		-
     **
     ** @return		the DokoLounge interpreter
     **
     ** @author		Diether Knof
     **
     ** @version	0.7.1
     **/
    Interpreter::Sender&
      Interpreter::Receiver::sender()
      {
        return dynamic_cast<Interpreter::Sender&>(this->interpreter().sender());
      } // Sender& Interpreter::Receiver::sender()

    /**
     ** a text has been received
     **
     ** @param     text   received text
     **
     ** @return    whether the text was accepted (or saved for later)
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    bool
        Interpreter::Receiver::received(string const& text)
      {
        // add the text to the one already gotten
        this->pending_line += DK::Utils::String::latin1_to_utf8(text);

        CLOG << text << endl;
        string keyword;
        string entry;
        while (strip_next_part(this->pending_line, keyword, entry)) {
          { // hack
            // <<alert>><</alert>>...<<alerttext>><</alerttext>>
            if (keyword == "alert") {
              this->pending_line = "<<alerttext>>" + this->pending_line;
            }
            if (keyword == "alerttext") {
              entry += "<</alerttext>>";
            }
          } // hack

          this->interpret_command(keyword, entry);

        } // while (strip_next_part(this->pending_line, keyword, entry))
        return !keyword.empty();
      } // bool Interpreter::Receiver::received(string text)

    /**
     ** interpret the command
     **
     ** @param     keyword     keyword (command)
     ** @param     text        command text
     **
     ** @return    whether the keyword was known
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    bool
      Interpreter::Receiver::interpret_command(string const& keyword,
                                               string const& text)
      {
        CLOG << "< " << keyword << endl;
        if (keyword == "neuesKonto") {
          // Fehler beim neuen Konto
          CLOG << "neuesKonto: " << DK::Utils::String::latin1_to_utf8(text) << endl;
          return true;
        } else if (keyword == "deinName") {
          // Anmeldung
          ::lounge->logged_in(text);
          return true;
        } else if (keyword == "bell") {
          // Die Klingel wurde geläutet
          CLOG << "bell: " << DK::Utils::String::latin1_to_utf8(text) << endl;
          return true;
        } else if (keyword == "alert") {
          // ignore it, see hack
          return true;
        } else if (keyword == "alerttext") {
          // contains alert, alerthead, bu1, bu2, alerttext
          this->interpret_alert(text);
          return true;
        } else if (keyword == "hilfe") {
          // Hilfetext
          ::lounge->set_help(text);
          return true;
        } else if (keyword == "blogtxt") {
          // Blogtext
          ::lounge->set_blog(text);
          return true;
        } else if (keyword == "PinnwandData") {
          // Pinnwand
          ::lounge->set_pin_board(text);
          return true;
        } else if (keyword == "mail") {
          // Eigene Pinnwand / Nachrichten
          ::lounge->set_messages(text);
          return true;
        } else if (keyword == "fuxbauchat") {
          // Chat, mit Farbe/Spielername
          this->interpret_chat(text);
          return true;
        } else if (keyword == "TischChat") {
          // Chat, mit Farbe/Spielername
          CLOG << "TischChat: " << DK::Utils::String::latin1_to_utf8(text) << endl;
          return true;
        } else if (keyword == "LobbySpieler") {
          this->interpret_player_list(text);
          return true;
        } else if (keyword == "Tischart") {
          this->interpret_table_types(text);
          return true;
        } else if (keyword == "alleTische") {
          this->interpret_table_list(text);
          return true;
        } else {
          cout << "unknown keyword: " << keyword << " -- " << DK::Utils::String::latin1_to_utf8(text) << endl;
          return false;
        } // if (keyword == ...)

        return false;
      } // bool Interpreter::Receiver::interpret_command(string keyword, string text)

    /**
     ** -> result
     **
     ** @param     text    text
     ** @param     part    part to search
     **
     ** @return    the text of <<part>>text<</part>>
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    string
      Interpreter::Receiver::text_of_part(string const& text, string const& part)
      {
        if (   (text.find("<<" + part + ">>") == string::npos)
            || (text.find("<</" + part + ">>") == string::npos)
            || (text.find("<<" + part + ">>")
                > text.find("<</" + part + ">>")) ) {
          return "";
        }
        return string(text,
                      text.find("<<" + part + ">>") + string("<<" + part + ">>").size(),
                      text.find("<</" + part + ">>") - text.find("<<" + part + ">>") - string("<<" + part + ">>").size());
      } // static string Interpreter::Receiver::text_of_part(string text, string part)

    /**
     ** gets the next part of 'text' and remove it from text
     ** <<part>>entry<</part>>moretext
     **
     ** @param     text      text (will be changed)
     ** @param     keyword   keyword of the part (will be overwritten)
     ** @param     entry     entry of the part (will be overwritten)
     **
     ** @return    whether a part was found
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    bool
      Interpreter::Receiver::strip_next_part(string& text,
                                             string& keyword, string& entry)
      {
        if (text.size() < 2 + 1 + 2 + 3 + 1 + 2) // minimum: <<a>><</a>>
          return false;

        if (!(   (text[0] == '<')
              && (text[1] == '<') ) )
          return false;

        if (text.find(">>") == string::npos) {
          return false;
        }
        string const keyword_t = string(text, 2, text.find(">>") - 2);

        string::size_type const n = text.find("<</" + keyword_t + ">>");
        if (n == string::npos)
          return false;

        keyword = keyword_t;
        entry = string(text,
                       ("<<" + keyword + ">>").size(),
                       n - ("<<" + keyword + ">>").size());
        text.erase(0, n + ("<</" + keyword + ">>").size());

        return true;
      } // bool Interpreter::Receiver::strip_next_part(string& text, string& keyword, string& entry)

    /**
     ** gets the next part of 'text' and remove it from text
     ** Name: moretext
     **
     ** @param     text      text (will be changed)
     ** @param     name      name of the part (will be overwritten)
     ** @param     entry     entry of the part (will be overwritten)
     **
     ** @return    whether a name was found
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    bool
      Interpreter::Receiver::strip_next_name(string& text,
                                             string& name, string& entry)
      {
        if (text.size() < 1 + 2 + 1) // minimum: N: x
          return false;

        if (text.find(": ") == string::npos) {
          return false;
        }

        name = string(text, 2, text.find(": ") - 2);
        entry = string(text, (name + ": ").size());
        text.erase(0, (name + ": ").size());

        return true;
      } // bool Interpreter::Receiver::strip_next_name(string& text, string& name, string& entry)


  } // namespace DokoLounge
} // namespace Network

#endif // #ifdef USE_NETWORK
