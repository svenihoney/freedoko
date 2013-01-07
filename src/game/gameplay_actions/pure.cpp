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

#include "pure.h"

#include "../../misc/translations.h"

/**
 ** constructor
 **
 ** @param	type   action type
 **
 ** @return	-
 **
 ** @author	Diether Knof
 ** 
 ** @version	0.7.3
 **/
GameplayAction::Pure::Pure(Type const& type) :
 GameplayAction(type, UINT_MAX)
{ }

/**
 ** constructor
 **
 ** @param	type   action type
 ** @param	line   line with the data
 **
 ** @return	action corresponding to the line
 **		(to be freed)
 **
 ** @author	Diether Knof
 ** 
 ** @version	0.7.3
 **/
GameplayAction::Pure::Pure(Type const& type, string const& line) :
  GameplayAction(type, UINT_MAX)
{ }

/**
 ** copy constructor
 **
 ** @param	action   action to copy
 **
 ** @return	-
 **
 ** @author	Diether Knof
 ** 
 ** @version	0.7.3
 **/
GameplayAction::Pure::Pure(Pure const& action) :
 GameplayAction(action)
{ }

/**
 ** copy constructor
 **
 ** @param	action   action to copy
 **
 ** @return	this action
 **
 ** @author	Diether Knof
 ** 
 ** @version	0.7.3
 **/
GameplayAction::Pure&
GameplayAction::Pure::operator=(Pure const& action)
{
  this->GameplayAction::operator=(action);
  return *this;
} // Pure& Pure::operator=(Pure action)

/**
 ** destructor
 **
 ** @param	-
 **
 ** @return	-
 **
 ** @author	Diether Knof
 ** 
 ** @version	0.7.3
 **/
GameplayAction::Pure::~Pure()
{ }

/**
 **
 ** writes the action in the output stream
 **
 ** @param	ostr	output stream
 **
 ** @return	-
 **
 ** @author	Diether Knof
 ** 
 ** @version	0.7.3
 **/
void
GameplayAction::Pure::write(ostream& ostr) const
{
  ostr << this->type;

  return ;
} // void write(ostream& ostr) const

/**
 ** -> result
 **
 ** @param	-
 **
 ** @return	a translation representing the data
 ** 
 ** @author	Diether Knof
 **
 ** @version	0.7.3
 **/
Translator::Translation
GameplayAction::Pure::data_translation() const
{
  return ::translation("");
} // virtual Translator::Translation GameplayAction::Pure::data_translation() const
