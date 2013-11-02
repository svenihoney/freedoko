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

#include "constants.h"

#ifdef USE_NETWORK
#ifdef USE_NETWORK_DOKOLOUNGE

#include "lounge.h"
#include "../ui/ui.h"

Lounge* lounge;

/**
 ** constructor
 **
 ** @param     -
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
Lounge::Lounge() :
  name_(),
  chat_(),
  help_(),
  blog_(),
  pin_board_(),
  messages_()
{
  return;
} // Lounge::Lounge()

/**
 ** destructor
 **
 ** @param     -
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
Lounge::~Lounge()
{}

/**
 ** -> result
 **
 ** @param     -
 **
 ** @return    whether the game is logged in at DokoLounge
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
bool
Lounge::is_logged_in() const
{
  return !this->name().empty();
} // bool Lounge::is_logged_in() const


/**
 ** the game has logged in at DokoLounge
 **
 ** @param     name    name of the player
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::logged_in(string const& name)
{
  DEBUG_ASSERTION(!this->is_logged_in(),
                  "Lounge::logged_in(" + name + ")\n"
                  "Already logged in as " + this->name());
  this->name_ = name;
  ::ui->lounge_logged_in(name);
  return;
} // void Lounge::logged_in(string name)

/**
 ** the game has logged out at DokoLounge
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
Lounge::logged_out()
{
  DEBUG_ASSERTION(this->is_logged_in(),
                  "Lounge::logged_out()\n"
                  "Not logged in");
  this->name_ = "";
  ::ui->lounge_logged_out();
  return;
} // void Lounge::logged_in(string name)

/**
 ** add a chat entry
 **
 ** @param     player   player who has chatted
 ** @param     text     chat text
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::add_chat_entry(string const& player, string const& text)
{
  this->chat_.push_back(LoungeChatEntry(player, text));
  ::ui->lounge_chat_entry_added(this->chat().back());
  return;
} // void Lounge::add_chat_entry(string const& player, string const& text)

/**
 ** sets the help text
 **
 ** @param     text     the help text
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::set_help(string const& text)
{
  if (this->help() == text)
    return;

  this->help_ = text;
  while (   (this->help()[this->help().size() - 1] == '\n')
         || (this->help()[this->help().size() - 1] == '\r') )
    this->help_.resize(this->help().size() - 1);
  ::ui->lounge_help_changed(this->help());
  return;
} // void Lounge::set_help(string const& text)

/**
 ** sets the blog text
 **
 ** @param     text     the blog text
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::set_blog(string const& text)
{
  if (this->blog() == text)
    return;

  this->blog_ = text;
  while (   (this->blog()[this->blog().size() - 1] == '\n')
         || (this->blog()[this->blog().size() - 1] == '\r') )
    this->blog_.resize(this->blog().size() - 1);
  ::ui->lounge_blog_changed(this->blog());
  return;
} // void Lounge::set_blog(string const& text)


/**
 ** sets the pin board text
 **
 ** @param     text     the pin board text
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::set_pin_board(string const& text)
{
  if (this->pin_board() == text)
    return;

  this->pin_board_ = text;
  while (   (this->pin_board()[this->pin_board().size() - 1] == '\n')
         || (this->pin_board()[this->pin_board().size() - 1] == '\r') )
    this->pin_board_.resize(this->pin_board().size() - 1);
  ::ui->lounge_pin_board_changed(this->pin_board());
  return;
} // void Lounge::set_pin_board(string const& text)


/**
 ** sets the messages text
 **
 ** @param     text     the messages text
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::set_messages(string const& text)
{
  if (this->messages() == text)
    return;

  this->messages_ = text;
  while (   (this->messages()[this->messages().size() - 1] == '\n')
         || (this->messages()[this->messages().size() - 1] == '\r') )
    this->messages_.resize(this->messages().size() - 1);
  ::ui->lounge_messages_changed(this->messages());
  return;
} // void Lounge::set_messages(string const& text)

#endif // #ifdef USE_NETWORK_DOKOLOUNGE
#endif // #ifdef USE_NETWORK
