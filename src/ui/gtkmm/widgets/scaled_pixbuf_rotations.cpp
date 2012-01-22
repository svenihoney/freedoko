/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2012  by Diether Knof and Borg Enders
 *
 *   This program is free software; you can redistribute it and/or 
 *   modify it under the terms of the GNU General Public Settings as 
 *   published by the Free Software Foundation; either version 2 of 
 *   the Settings, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public Settings for more details. 
 *   You can find this license in the file 'gpl.txt'.
 *
 *   You should have received a copy of the GNU General Public Settings
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

#include "scaled_pixbuf_rotations.h"
#include "rotations.h"

namespace Gdk {
  
  /**
   **
   ** Constructor
   **
   ** @param	filename	the file to load the pixbuf from
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  ScaledPixbufRotations::ScaledPixbufRotations(string const filename) :
    ScaledPixbuf(filename),
  scaled_pixbuf(4)
  {
    this->scaled_pixbuf[0] = this;

    return ;
  } // ScaledPixbufRotations::ScaledPixbufRotations(string const filename)

  /**
   **
   ** Constructor
   **
   ** @param	pixbuf	the pixbuf for the 'up' rotation
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  ScaledPixbufRotations::ScaledPixbufRotations(Glib::RefPtr<Pixbuf> const pixbuf) :
    ScaledPixbuf(pixbuf),
  scaled_pixbuf(4)
  {
    this->scaled_pixbuf[0] = this;

    return ;
  } // ScaledPixbufRotations::ScaledPixbufRotations(Glib::RefPtr<Pixbuf> const pixbuf)

  /**
   **
   ** Constructor
   **
   ** @param	-
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  ScaledPixbufRotations::ScaledPixbufRotations() :
    ScaledPixbuf(),
  scaled_pixbuf(4)
  {
    this->scaled_pixbuf[0] = this;

    return ;
  } // ScaledPixbufRotations::ScaledPixbufRotations()

  /**
   **
   ** Constructor
   **
   ** @param	scaled_pixbuf_rotations	scaled_pixbuf_rotations to copy
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  ScaledPixbufRotations::ScaledPixbufRotations(ScaledPixbufRotations const& scaled_pixbuf_rotations) :
    ScaledPixbuf(scaled_pixbuf_rotations),
  scaled_pixbuf(4)
  {
    this->scaled_pixbuf[0] = this;
    for (unsigned i = 1; i < 4; ++i)
      if (scaled_pixbuf_rotations.scaled_pixbuf[i] != NULL)
	this->scaled_pixbuf[i]
	  = new ScaledPixbuf(*(scaled_pixbuf_rotations.scaled_pixbuf[i]));

    return ;
  } // ScaledPixbufRotations::ScaledPixbufRotations(ScaledPixbufRotations const& scaled_pixbuf_rotations)

  /**
   **
   ** copy operator
   **
   ** @param	scaled_pixbuf_rotations	scaled_pixbuf_rotations to copy
   **
   ** @return	copied element
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  ScaledPixbufRotations&
    ScaledPixbufRotations::operator=(ScaledPixbufRotations const& scaled_pixbuf_rotations)
    {
      static_cast<ScaledPixbuf&>(*this)
	= static_cast<ScaledPixbuf const&>(scaled_pixbuf_rotations);

      this->scaled_pixbuf[0] = this;
      for (unsigned i = 1; i < 4; ++i) {
	delete this->scaled_pixbuf[i];
	if (scaled_pixbuf_rotations.scaled_pixbuf[i] == NULL)
	  this->scaled_pixbuf[i] = NULL;
	else
	  this->scaled_pixbuf[i]
	    = new ScaledPixbuf(*(scaled_pixbuf_rotations.scaled_pixbuf[i]));
      } // for (unsigned i = 1; i < 4; ++i)

      return *this;
    } // ScaledPixbufRotations& ScaledPixbufRotations::operator=(ScaledPixbufRotations const& scaled_pixbuf_rotations) :

  /**
   **
   ** Destruktor
   **
   ** @param	-
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.5.4
   **
   **/
  ScaledPixbufRotations::~ScaledPixbufRotations()
  {
    for (unsigned i = 1; i < 4; ++i)
      delete this->scaled_pixbuf[i];

    return ;
  } // ScaledPixbufRotations::~ScaledPixbufRotations()

  /**
   **
   ** -> result
   ** the rotation is automatically created if it does not yet exists
   **
   ** @param	rotation	rotation
   **
   ** @return	the pixbuf with 'rotation'
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  ScaledPixbuf&
    ScaledPixbufRotations::operator[](Rotation const rotation)
    {
      if (!*this)
	return *this;
      return ((this->scaled_pixbuf[rotation] == NULL)
	      ? this->create_rotation(rotation)
	      : *this->scaled_pixbuf[rotation]);
    } // ScaledPixbuf& ScaledPixbufRotations::operator[](Rotation const rotation)

  /**
   **
   ** set the scaling
   **
   ** @param	scaling		the new scaling of the pixbuf
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  void
    ScaledPixbufRotations::set_scaling(double const scaling) 
    {
      this->ScaledPixbuf::set_scaling(scaling);
      this->scaled_pixbuf[0] = this;
      for (int i = 1; i < 4; ++i) {
	delete this->scaled_pixbuf[i];
	this->scaled_pixbuf[i] = NULL;
      }

      return ;
    } // void ScaledPixbufRotations::set_scaling(double const scaling)

  /**
   ** -> result
   **
   ** @param     rotation   rotation (default: 'UP')
   **
   ** @return    the width of the pixbuf with 'rotation'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.2
   **/
  int
    ScaledPixbufRotations::get_width(Rotation const rotation) const
    {
      if (!*this)
        return 0;

      switch(rotation) {
      case UP:
      case DOWN:
	return this->ScaledPixbuf::get_width();
      case LEFT:
      case RIGHT:
	return this->ScaledPixbuf::get_height();
      } // switch(rotation)

      return 0;
    } // int ScaledPixbufRotations::get_width(Rotation rotation) const

  /**
   ** -> result
   **
   ** @param     rotation   rotation (default: 'UP')
   **
   ** @return    the height of the pixbuf with 'rotation'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.2
   **/
  int
    ScaledPixbufRotations::get_height(Rotation const rotation) const
    {
      if (!*this)
        return 0;

      switch(rotation) {
      case UP:
      case DOWN:
	return this->ScaledPixbuf::get_height();
      case LEFT:
      case RIGHT:
	return this->ScaledPixbuf::get_width();
      } // switch(rotation)

      return 0;
    } // int ScaledPixbufRotations::get_height(Rotation rotation) const

  /**
   **
   ** -> result
   **
   ** @param	rotation	rotation (default: 'UP')
   **
   ** @return	the original width of the pixbuf with 'rotation'
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  int
    ScaledPixbufRotations::get_orig_width(Rotation const rotation) const
    {
      switch(rotation) {
      case UP:
      case DOWN:
	return this->ScaledPixbuf::get_orig_width();
      case LEFT:
      case RIGHT:
	return this->ScaledPixbuf::get_orig_height();
      } // switch(rotation)

      return 0;
    } // int ScaledPixbufRotations::get_orig_width(Rotation const rotation) const

  /**
   **
   ** -> result
   **
   ** @param	rotation	rotation (default: 'UP')
   **
   ** @return	the original height of the pixbuf with 'rotation'
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  int
    ScaledPixbufRotations::get_orig_height(Rotation const rotation) const
    {
      switch(rotation) {
      case UP:
      case DOWN:
	return this->ScaledPixbuf::get_orig_height();
      case LEFT:
      case RIGHT:
	return this->ScaledPixbuf::get_orig_width();
      } // switch(rotation)

      return 0;
    } // int ScaledPixbufRotations::get_orig_height(Rotation const rotation) const

  /**
   **
   ** creates the rotation 'rotation' of '*this'
   **
   ** @param	rotation	the rotation
   **
   ** @return	rotated pixbuf
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.2
   **
   **/
  ScaledPixbuf&
    ScaledPixbufRotations::create_rotation(Rotation const rotation)
    {

      switch(rotation) {
      case UP:
	// cannot happen
	return *this;
      case DOWN:
	this->scaled_pixbuf[rotation]
	  = new ScaledPixbuf(rotate_180(*this));
	break;
      case LEFT:
	if (this->scaled_pixbuf[RIGHT] == NULL)
	  this->scaled_pixbuf[rotation]
	    = new ScaledPixbuf(rotate_left(*this));
	else
	  this->scaled_pixbuf[rotation]
	    = new ScaledPixbuf(rotate_180(*(this->scaled_pixbuf[RIGHT])));
	break;
      case RIGHT:
	if (this->scaled_pixbuf[LEFT] == NULL)
	  this->scaled_pixbuf[rotation]
	    = new ScaledPixbuf(rotate_right(*this));
	else
	  this->scaled_pixbuf[rotation]
	    = new ScaledPixbuf(rotate_180(*(this->scaled_pixbuf[LEFT])));
	break;
      } // switch(rotation)

      return *(this->scaled_pixbuf[rotation]);
    } // ScaledPixbuf& ScaledPixbufRotations::create_rotation(Rotation const rotation)

} // namespace Gdk

#endif // #ifdef USE_UI_GTKMM
