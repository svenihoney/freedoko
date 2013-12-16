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

#include "sender.h"

#include "../../misc/setting.h"
#include "../../misc/lounge.h"

namespace Network {
  namespace DokoLounge {

#if 0
  \item "<<hilfe>>" + MeinName + "<</hilfe>>"
  \item "<<hilfechange>>" + s + "<</hilfechange>>"
  \item "<<bloglesen>>" + MeinName + "<</bloglesen>>"
  \item "<<blogsenden>>" + t + "<</blogsenden>>"
  \item "<<ganzepinnwand>>" + sendstr + "<</ganzepinnwand>>"
  \item "<<PinnwandRefresh>>" + MeinName + "<</PinnwandRefresh>>"
  \item "<<PinnwandSenden>>" + sendstr +  "<</PinnwandSenden>>"
  \item "<<GibRangliste>><<Name>>" + MeinName + "<</Name>><<Spiele>>" + Format(LetzteRangspiele) + "<</Spiele>><</GibRangliste>>"
  \item "<<GibRangliste2>><<Name>>" + MeinName + "<</Name>><<Spiele>>" + s + "<</Spiele>><</GibRangliste2>>"
  \item "<<angstgegner>><<name>>" + MeinName + "<</name>><<gegner>>" + g + "<</gegner>><</angstgegner>>"
  \item "<<neuerTisch>><<Name>>" + MeinName + "<</Name>><<TischArt>>" + helper + "<</TischArt>><</neuerTisch>>"
  <<maillesen>><<name>>Name<</name>><</maillesen>>

  <<offline>>
#endif

   /**
    ** create an account
    ** example: <<Anmelden>>Name<</Anmelden>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::create_account()
     {
       if (::setting(Setting::DOKOLOUNGE_NAME) == "")
         ::setting.set(Setting::DOKOLOUNGE_NAME,
                             ::setting(Setting::NAME));
       if (::setting(Setting::DOKOLOUNGE_PASSWORD) == "")
         ::setting.set(Setting::DOKOLOUNGE_PASSWORD,
                             "Standardpasswort");
       this->send_command("neuesKonto",
                          this->command("Name", ::setting(Setting::DOKOLOUNGE_NAME))
                          + this->command("passwd", ::setting(Setting::DOKOLOUNGE_PASSWORD)));

       return ;
     } // void Interpreter::Sender::create_account()

   /**
    ** login
    ** example: <<Anmelden>>Name<</Anmelden>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::login()
     {
       this->send_command("anmelden",
                          this->command("Name",
                                        ::setting(Setting::DOKOLOUNGE_NAME)
                          + "|"
                          + ::setting(Setting::DOKOLOUNGE_PASSWORD))
                          + this->command("MacAdresse", "")
                          //+ this->command("AppName", "FreeDoko " + static_cast<string>(*::version))
                          + this->command("version", "DokoClient50")
                         );
#if 0
          this->send(this->command("fuxbauchat",
                             "Diether: /newpassword Test1234"));
#endif

       return ;
     } // void Interpreter::Sender::login()

   /**
    ** logout
    ** example: <<Abmelden>>Name<</Abmelden>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::logout()
     {
       this->send_name_command("Abmelden");
       ::lounge->logged_out();

       return ;
     } // void Interpreter::Sender::logout()

   /**
    ** refresh
    ** example: <<Refresh>>Name<</Refresh>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::refresh()
     {
       this->send_name_command("Refresh");

       return ;
     } // void Interpreter::Sender::refresh()


   /**
    ** Klingel betätigen
    ** example: <<klingel>><</klingel>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::klingel()
     {
       this->send_simple_command("klingel");

       return ;
     } // void Interpreter::Sender::klingel()

   /**
    ** Hilfe aufrufen
    ** example: <<hilfe>>Name<</hilfe>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::hilfe()
     {
       this->send_name_command("hilfe");

       return ;
     } // void Interpreter::Sender::hilfe()

   /**
    ** Blog aufrufen
    ** example: <<bloglesen>>Name<</bloglesen>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::blog()
     {
       this->send_name_command("bloglesen");

       return ;
     } // void Interpreter::Sender::blog()

   /**
    ** Pinnwand aufrufen
    ** example: <<bloglesen>>Name<</bloglesen>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::pinnwand()
     {
       this->send_name_command("PinnwandRefresh");

       return ;
     } // void Interpreter::Sender::pinnwand()

   /**
    ** eigene Pinnwand aufrufen
    ** example: <<maillesen>><<name>>Name<</name>><</maillesen>>
    **
    ** @param     -
    **
    ** @return    -
    **
    ** @author    Diether Knof
    **
    ** @version   0.7.12
    **/
   void
     Interpreter::Sender::messages()
     {
       this->send_command("maillesen",
                          this->name_command("name"));

       return ;
     } // void Interpreter::Sender::messages()

  } // namespace DokoLounge
} // namespace Network

#endif // #ifdef USE_NETWORK
