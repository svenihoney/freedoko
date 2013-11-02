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

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "ui.h"

#ifdef USE_NETWORK
#ifdef USE_NETWORK_DOKOLOUNGE
#include "../../misc/lounge.h"
#include "dokolounge/lounge.h"

namespace UI_GTKMM_NS {

  /**
   ** the game has logged in
   **
   ** @param     name   name of the player
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.12
   **/
  void
    UI_GTKMM::lounge_logged_in(string const& name)
    {
      if (this->lounge)
        this->lounge->logged_in(name);
      return ;
    } // void UI_GTKMM::lounge_logged_in(string name)

  /**
   ** the game has logged out
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
    UI_GTKMM::lounge_logged_out()
    {
      if (this->lounge)
        this->lounge->logged_out();
      return ;
    } // void UI_GTKMM::lounge_logged_out()

  /**
   ** a chat entry has been added
   **
   ** @param     entry   new chat entry
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.12
   **/
  void
    UI_GTKMM::lounge_chat_entry_added(::LoungeChatEntry const& entry)
    {
      if (this->lounge)
        this->lounge->chat_entry_added(entry);
      return ;
    } // void UI_GTKMM::lounge_chat_entry_added(::LoungeChatEntry entry)

  /**
   ** the help of the lounge has changed
   **
   ** @param     text   text of the help
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.12
   **/
  void
    UI_GTKMM::lounge_help_changed(string const& text)
    {
      if (this->lounge)
        this->lounge->help_changed(text);
      return ;
    } // void UI_GTKMM::lounge_help_changed(string text)

  /**
   ** the blog of the lounge has changed
   **
   ** @param     text   text of the blog
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.12
   **/
  void
    UI_GTKMM::lounge_blog_changed(string const& text)
    {
      if (this->lounge)
        this->lounge->blog_changed(text);
      return ;
    } // void UI_GTKMM::lounge_blog_changed(string text)

  /**
   ** the pin_board of the lounge has changed
   **
   ** @param     text   text of the pin_board
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.12
   **/
  void
    UI_GTKMM::lounge_pin_board_changed(string const& text)
    {
      if (this->lounge)
        this->lounge->pin_board_changed(text);
      return ;
    } // void UI_GTKMM::lounge_pin_board_changed(string text)

  /**
   ** the messages of the lounge has changed
   **
   ** @param     text   text of the messages
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.12
   **/
  void
    UI_GTKMM::lounge_messages_changed(string const& text)
    {
      if (this->lounge)
        this->lounge->messages_changed(text);
      return ;
    } // void UI_GTKMM::lounge_messages_changed(string text)


} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
#endif
#endif
