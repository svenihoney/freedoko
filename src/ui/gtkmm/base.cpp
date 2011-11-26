/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2011  by Diether Knof and Borg Enders
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

#include "base.h"

#include "ui.h"

#ifdef USE_NETWORK
#ifdef RELEASE
#define PROGRESS_PER_BASE (1.0 / 33)
#else
#define PROGRESS_PER_BASE (1.0 / 32)
#endif
#else
#ifdef RELEASE
#define PROGRESS_PER_BASE (1.0 / 31)
#else
#define PROGRESS_PER_BASE (1.0 / 30)
#endif
#endif
// + 1 (Translations)

namespace UI_GTKMM_NS {

  /**
   ** constructor
   **
   ** @param     parent   the parent object
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  Base::Base(Base* const parent) :
    ui(parent->ui),
    parent(parent)
  {
    // note: the parent object must not be fully initialized

    if (   (::game_status == GAMESTATUS::PROGRAMSTART)
	|| (::game_status == GAMESTATUS::PARTY_INITIAL_LOADED) )
      this->ui->add_progress(PROGRESS_PER_BASE);

    if (this != this->ui)
      this->ui->add_part(*this);

    return ;
  } // Base::Base(Base* const parent)

  /**
   **
   ** Constructor
   **
   ** @param     parent   the parent object
   ** @param     ui   the ui
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  Base::Base(Base* const parent, UI_GTKMM* const ui) :
    ui(ui),
    parent(parent)
  {
    if (   (::game_status == GAMESTATUS::PROGRAMSTART)
	|| (::game_status == GAMESTATUS::PARTY_INITIAL_LOADED) )
      this->ui->add_progress(PROGRESS_PER_BASE);

    if (this != this->ui)
      this->ui->parts.push_back(this);

    return ;
  } // Base::Base(Base* const parent, UI_GTKMM* const ui)

  /**
   **
   ** Destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  Base::~Base()
  {
    if (this != this->ui)
      this->ui->parts.erase(std::find(this->ui->parts.begin(),
				      this->ui->parts.end(),
				      this));

    return ;
  } // virtual Base::~Base()

  /**
   **
   ** update the cards
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    Base::cards_update()
    {
      return ;
    } // virtual void Base::cards_update()

  /**
   **
   ** update the cards back
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    Base::cards_back_update()
    {
      return ;
    } // virtual void Base::cards_back_update()

  /**
   **
   ** the setting has changed
   **
   ** @param     type   the type that has changed
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    Base::setting_update(int const type)
    {
      return ;
    } // virtual void Base::setting_update(int const type)

  /**
   **
   ** update the language elements
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    Base::language_update()
    {
      return ;
    } // virtual void Base::language_update()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
