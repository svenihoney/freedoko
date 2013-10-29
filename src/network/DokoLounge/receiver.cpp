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

#include <cstring>

namespace Network {
  namespace DokoLounge {
    /*
<<neuesKonto>>Der Name "FreeDoko" ist leider schon vergeben.
Bitte ein Passwort mit mindestens 4 Buchsteben/Zahlen wählen.<</neuesKonto>>

Mit falscher Version angemeldet
<<fuxbauchat>><<col>>000,000,192<</col>>Willkommen in der Doko-Lounge,  :-)<</fuxbauchat>>
<<deinName>><</deinName>><<fuxbauchat>>Bitte lade dir von unserer Homepage (www.doko-lounge.de) eine aktuelle Version herunter. Mit dieser Version kannst du nicht mehr spielen.<</fuxbauchat>><<offline>><</offline>>

Angemeldet:

<<deinName>><</deinName>><<Tischart>><<Tischanz>>5<</Tischanz>><<1>>Sonderregeln<</1>><<2>>Lerntisch<</2>><<3>>Sonderregeln<</3>><<4>>DDV<</4>><<5>>Sonderregeln<</5>><</Tischart>><<alleTische>>(TischSpieler01)frei,frei,frei,frei||(TischSpieler02)frei,frei,frei,frei||(TischSpieler03)frei,frei,frei,frei||(TischSpieler04)frei,frei,frei,frei||(TischSpieler05)frei,frei,frei,frei||<</alleTische>><<fuxbauchat>>Das Logo Doko-Lounge, was so ca. in der Mitte vom Bildschild steht, ist gleichzeitig eine Klingel.Wenn du darauf klickst, können dich die anderen hören. Bestimmt wird dann einer so nett sein, dir zu erklären wie es hier so läuft :-)<</fuxbauchat>><<bell>>kjh<</bell>><<alert>><</alert>><<bu2>><</bu2>><<bu1>>Fein<</bu1>><<alerthead>>Willkommen <</alerthead>><<alerttext>>Hallo liebe Lounger,


meistens trudeln die ersten so gegen 19 oder 20 Uhr hier ein. 
Ist halt eine "Feierabend-Plattform" :-)
Ihr könnt auch gerne mal länger als 5 Sekunden in der Lounge verweilen.
Damit erhöht sich d
< ie Wahrscheinlichkeit andere Mitspieler zu erwischen enorm ;-)

Wenn du neu bist, dann lese dir am besten erst einmal die Hilfe, die Pinnwand und den Blog durch.
<</alerttext>>

Chat:
<<fuxbauchat>>MacHolstein: ja, ok - ich bin auch nicht soo oft hier
<</fuxbauchat>>

Die Klingel wurde geläutet
<<bell>><</bell>>

<<hilfe>>
Mehrere Zeilen

<<blogtxt>>

Stevie am 01.09.2013---19:33
..warm anzieh...

<</PinnwandData>>

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
     ** @version	0.7.1
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
        cout << "< " << text << endl;
        return false;
      } // bool Interpreter::Receiver::received(string text)

    /**
     ** a line has been received
     **
     ** @param     line	received line
     **
     ** @return    whether the line was accepted (or saved for later)
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.12
     **/
    bool
      Interpreter::Receiver::received_line(string const& line)
      {
        // ignore empty lines
        if (line.empty())
          return true;

        cout << "< " << line << endl;

        return false;
      } // bool Interpreter::Receiver::received_line(string line)

    /**
     ** act according to a simple message
     **
     ** @param     message   simple message
     **
     ** @return    whether the message could be interpreted
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.2
     **
     ** @todo      bye
     **/
    bool
      Interpreter::Receiver::act_simple(string const& message)
      {
        return false;
      } // bool Interpreter::Receiver::act_simple(string message)

    /**
     ** act according to a special message
     **
     ** @para      message   special message
     **
     ** @return    whether the message could be interpreted
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.1
     **
     ** @todo      bye
     **/
    bool
      Interpreter::Receiver::act_special(string const& message)
      {
        cerr << "DokoLounge::Interpreter::Receiver::act_special(message)\n"
          << "  I do not know the special message '" << message << "' :-("
          << endl;
        return false;
      } // bool Interpreter::Receiver::act_special(string message)

    /**
     ** act according to a normal message
     **
     ** @param     message   normal message
     **
     ** @return    whether the message could be interpreted
     **
     ** @author    Diether Knof
     **
     ** @version   0.7.2
     **/
    bool Interpreter::Receiver::act_normal(string const& message)
    {
      return false;
    } // bool Interpreter::Receiver::act_normal(string message)

  } // namespace DokoLounge
} // namespace Network

#endif // #ifdef USE_NETWORK
