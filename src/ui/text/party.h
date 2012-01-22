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

#ifdef USE_UI_TEXT

#ifndef UI_TEXT_PARTY_HEADER
#define UI_TEXT_PARTY_HEADER

#include "ui.h"

namespace UI_TEXT_NS {

  /**
   ** @brief	interface for viewing and changing party settings
   **
   ** @author	Diether Knof
   **/
  class UI_Text::Party {
    public:
      Party(UI_Text* const ui);
      ~Party();

      // interprets the read linj
      bool interpret_line();

      // show the party settings
      void show() const;
      // get the party settings
      void get_settings() const;

    private:
      UI_Text* const ui;

    private: // unused
      Party();
      Party(Party const&);
      Party& operator=(Party const&);
  }; // class UI_Text::Party

} // namespace UI_TEXT_NS
#endif // #ifdef UI_TEXT_PARTY_HEADER

#endif // #ifdef USE_UI_TEXT

