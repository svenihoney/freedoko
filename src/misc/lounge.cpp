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
#include "../utils/string.h"

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
  help_("empty"),
  blog_("empty"),
  pin_board_("empty"),
  messages_("empty"),
  alert_title_("empty"),
  alert_("empty")
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
 ** -> result
 **
 ** @param     name   the name of the player
 **
 ** @return    the player 'name'
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
Lounge::Player const&
Lounge::player(string const& name) const
{
  for (vector<Player>::const_iterator p = this->players().begin();
       p != this->players().end();
       ++p)
    if (p->name == name)
      return *p;
  return this->players()[0];
} // Lounge::Player const& Lounge::player(string name) const

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
 ** @param     type     the chat type
 ** @param     text     chat text
 ** @param     player   player who has chatted
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::add_chat_entry(LoungeChatEntry::Type const& type,
                       string const& text, string const& player)
{
  this->chat_.push_back(LoungeChatEntry(type, text, player));
  ::ui->lounge_chat_entry_added(this->chat().back());
  return;
} // void Lounge::add_chat_entry(Type type, string text, string player)

/**
 ** add a chat entry
 **
 ** @param     type     the chat type
 ** @param     text     chat text
 ** @param     player   player who has chatted
 ** @param     color    color
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::add_chat_entry(LoungeChatEntry::Type const& type,
                       string const& text, string const& player,
                       LoungeChatEntry::Color const& color)
{
  this->chat_.push_back(LoungeChatEntry(type, text, player, color));
  ::ui->lounge_chat_entry_added(this->chat().back());
  return;
} // void Lounge::add_chat_entry(Type type, string text, string player, LoungeChatEntry::Color color)

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
  DK::Utils::String::remove_trailing_newlines(this->help_);
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
  DK::Utils::String::remove_trailing_newlines(this->blog_);
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
 **
 ** @todo      seperate individual entries
 **/
void
Lounge::set_pin_board(string const& text)
{
  if (this->pin_board() == text)
    return;

  this->pin_board_ = text;
  DK::Utils::String::remove_trailing_newlines(this->pin_board_);
  DK::Utils::String::remove_double_newlines(this->pin_board_);
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
 **
 ** @todo      seperate individual entries
 **/
void
Lounge::set_messages(string const& text)
{
  if (this->messages() == text)
    return;

  this->messages_ = text;
  DK::Utils::String::remove_trailing_newlines(this->messages_);
  ::ui->lounge_messages_changed(this->messages());
  return;
} // void Lounge::set_messages(string text)

/**
 ** sets the alert
 **
 ** @param     title    alert title
 ** @param     text     the alert text
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **
 ** @todo      buttons
 **/
void
Lounge::set_alert(string const& title, string const& text)
{
  if (   (this->alert_title() == title)
      && (this->alert() == text) )
    return;

  this->alert_title_ = title;
  this->alert_ = text;
  ::ui->lounge_alert(this->alert_title(), this->alert());
  return;
} // void Lounge::set_alert(string title, string text)

/**
 ** sets the players
 **
 ** @param     players   players
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::set_player_list(vector<Lounge::Player> const& players)
{
  this->players_ = players;
  ::ui->lounge_players_changed(this->players());
  return;
} // void Lounge::set_player_list(vector<Lounge::Player> players)

/**
 ** sets the tables
 **
 ** @param     tables   tables
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
void
Lounge::set_table_list(vector<Lounge::Table> const& tables)
{
  this->tables_ = tables;
  ::ui->lounge_tables_changed(this->tables());
  return;
} // void Lounge::set_table_list(vector<Lounge::Table> tables)

/**
 ** the name of the type
 **
 ** @param     type  chat entry type
 **
 ** @return    -
 ** 
 ** @author    Diether Knof
 **
 ** @version   0.7.12
 **/
string
name(LoungeChatEntry::Type const& type)
{
  switch(type) {
  case LoungeChatEntry::SYSTEM:
    return "system";
  case LoungeChatEntry::PLAYER:
    return "player";
  case LoungeChatEntry::PLAYER_WHISPER:
    return "whisper";
  case LoungeChatEntry::PLAYER_FOR_ALL:
    return "for all";
  }; // switch(type)
  return "";
} // string name(LoungeChatEntry::Type const& type)

#endif // #ifdef USE_NETWORK_DOKOLOUNGE
#endif // #ifdef USE_NETWORK
