/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2013  by Diether Knof and Borg Enders
 *
 *   This program is free software; you can redistribute it and/or 
 *   modify it under the terms of the GNU General Public Rules as 
 *   published by the Free Software Foundation; either version 2 of 
 *   the Rules, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public Rules for more details. 
 *   You can find this license in the file 'gpl.txt'.
 *
 *   You should have received a copy of the GNU General Public Rules
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
 *   MA  02111-1307  USA
 *
 *  Contact:
 *    Diether Knof dknof@gmx.de
 *    Borg Enders  borg@borgsoft.de
 *
 **********************************************************************/

#ifdef USE_UI_GTKMM
#ifndef GTKMM_WIDGET_TYPE_SELECTOR_HEADER
#error type_selector.hpp: must be included by 'type_selector.h'
#endif

#include "constants.h"

#include "type_selector.h"

#include "../../../misc/translations.h"

namespace Gtk {

  /**
   ** Constructor
   **
   ** @param	-
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.7.2
   **/
  template<typename Type>
    TypeSelector<Type>::TypeSelector() :
      ComboBoxText()
    {  }

  /**
   ** Destructor
   **
   ** @param	-
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.7.2
   **/
  template<typename Type>
    TypeSelector<Type>::~TypeSelector()
    { }

  /**
   ** adds the type to the selector
   **
   ** @param	type   type to be added
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.7.2
   **/
  template<typename Type>
    void
    TypeSelector<Type>::add(Type const& type)
    {
      this->append_text(::translation(type).translation());
      this->types.push_back(type);

      return ;
    } // void TypeSelector::add(Type type)

  /**
   ** -> result
   **
   ** @param	-
   **
   ** @return	the selected type
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.7.2
   **/
  template<typename Type>
    Type
    TypeSelector<Type>::type()
    {
      if (this->get_active_row_number() == -1)
	return Type();
      return this->types[this->get_active_row_number()];
    }

  /**
   ** sets the selected type
   **
   ** @param	type	new selected type
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.7.2
   **/
  template<typename Type>
    void
    TypeSelector<Type>::set_type(Type const& type)
    {
      for (unsigned i = 0; i < this->types.size(); ++i)
	if (this->types[i] == type) {
	  this->set_active(i);
	  return ;
	}

      DEBUG_ASSERTION(false,
		      "TypeSelector::set_type(" << type << ")\n"
		      << "  type not found");
      this->set_active(-1);
      return ;
    } // void TypeSelector<Type>::set_type(Type const& type)

  /**
   ** updates the language
   **
   ** @param	-
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.7.8
   **/
  template<typename Type>
    void
    TypeSelector<Type>::language_update()
    {
#ifdef OUTDATED
      // 0.7.8 2009-09-29
      int index = this->get_active_row_number();
      this->clear();
#ifdef WORKAROUND

      for (unsigned i = 0; i < this->types.size(); ++i) {
	this->append_text(::translation(this->types[i]).translation());
      }
#else
      // problem with the netsted template
      for (vector<Type>::const_iterator c = this->types.begin();
	   c != this->types.end();
	   ++c)
	this->append_text(::translation(*c).translation());
#endif

      this->set_active(index);

      return ;
#endif


      for (unsigned i = 0; i < this->types.size(); ++i) {
	this->get_model()->children()[i][this->m_text_columns.m_column]
	  = ::translation(this->types[i]).translation();
      }
      return ;
    } // void TypeSelector<Type>::language_update()

} // namespace Gtk

#endif // #ifdef USE_UI_GTKMM
