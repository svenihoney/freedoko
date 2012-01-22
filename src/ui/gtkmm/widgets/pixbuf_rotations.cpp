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

#include "pixbuf_rotations.h"
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
   ** @version	0.6.1
   **
   **/
  PixbufRotations::PixbufRotations(string const filename) :
    Glib::RefPtr<Pixbuf>(),
  pixbuf(4)
  {
#ifdef GLIBMM_EXCEPTIONS_ENABLED
    try {
      static_cast<Glib::RefPtr<Pixbuf>&>(*this)
	= Pixbuf::create_from_file(filename);
    } // try
    catch (Glib::FileError file_error) { }
    catch (Gdk::PixbufError pixbuf_error) { }
#else
    std::auto_ptr<Glib::Error> error;
    static_cast<Glib::RefPtr<Pixbuf>&>(*this) = Pixbuf::create_from_file(filename, error);
#endif

    this->pixbuf[0] = *this;

    return ;
  } // PixbufRotations::PixbufRotations(string const filename)

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
   ** @version	0.6.1
   **
   **/
  PixbufRotations::PixbufRotations(Glib::RefPtr<Pixbuf> const pixbuf) :
    Glib::RefPtr<Pixbuf>(pixbuf),
  pixbuf(4)
  {
    this->pixbuf[0] = *this;

    return ;
  } // PixbufRotations::PixbufRotations(Glib::RefPtr<Pixbuf> const pixbuf)

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
   ** @version	0.6.1
   **
   **/
  PixbufRotations::PixbufRotations() :
    Glib::RefPtr<Pixbuf>(),
  pixbuf(4)
  {
    this->pixbuf[0] = *this;

    return ;
  } // PixbufRotations::PixbufRotations()

  /**
   **
   ** Constructor
   **
   ** @param	pixbuf_rotations	pixbuf_rotations to copy
   **
   ** @return	-
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.1
   **
   **/
  PixbufRotations::PixbufRotations(PixbufRotations const& pixbuf_rotations) :
    Glib::RefPtr<Pixbuf>(),
  pixbuf(4)
  {
    if (pixbuf_rotations)
      static_cast<Glib::RefPtr<Gdk::Pixbuf>& >(*this)
	= pixbuf_rotations->copy();

    this->pixbuf[0] = *this;
    for (unsigned i = 0; i < 4; i++)
      if (pixbuf_rotations.pixbuf[i])
	this->pixbuf[i] = pixbuf_rotations.pixbuf[i]->copy();

    return ;
  } // PixbufRotations::PixbufRotations(PixbufRotations const& pixbuf_rotations)

  /**
   **
   ** copy operator
   **
   ** @param	pixbuf_rotations	pixbuf_rotations to copy
   **
   ** @return	copied element
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.1
   **
   **/
  PixbufRotations&
    PixbufRotations::operator=(PixbufRotations const& pixbuf_rotations)
    {
      if (pixbuf_rotations)
	static_cast<Glib::RefPtr<Gdk::Pixbuf>& >(*this)
	  = pixbuf_rotations->copy();
      else
	static_cast<Glib::RefPtr<Gdk::Pixbuf>& >(*this).clear();

      this->pixbuf[0] = *this;
      for (unsigned i = 1; i < 4; i++)
	if (pixbuf_rotations.pixbuf[i])
	  this->pixbuf[i] = pixbuf_rotations.pixbuf[i]->copy();
	else
	  this->pixbuf[i].clear();

      return *this;
    } // PixbufRotations& PixbufRotations::operator=(PixbufRotations const& pixbuf_rotations) :

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
  PixbufRotations::~PixbufRotations()
  {
    return ;
  } // PixbufRotations::~PixbufRotations()

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
   ** @version	0.6.1
   **
   **/
  Glib::RefPtr<Pixbuf>
    PixbufRotations::operator[](Rotation const rotation)
    {
      if (!*this)
	return *this;
      return (!this->pixbuf[rotation]
	      ? this->create_rotation(rotation)
	      : this->pixbuf[rotation]);
    } // Glib::RefPtr<Pixbuf> PixbufRotations::operator[](Rotation const rotation)

  /**
   **
   ** -> result
   **
   ** @param	rotation	rotation (default: 'UP')
   **
   ** @return	the width of the pixbuf with 'rotation'
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.1
   **
   **/
  int
    PixbufRotations::width(Rotation const rotation) const
    {
      switch(rotation) {
      case UP:
      case DOWN:
	return (*this)->get_width();
      case LEFT:
      case RIGHT:
	return (*this)->get_height();
      } // switch(rotation)

      return 0;
    } // int PixbufRotations::width(Rotation const rotation) const

  /**
   **
   ** -> result
   **
   ** @param	rotation	rotation (default: 'UP')
   **
   ** @return	the height of the pixbuf with 'rotation'
   ** 
   ** @author	Diether Knof
   **
   ** @version	0.6.1
   **
   **/
  int
    PixbufRotations::height(Rotation const rotation) const
    {
      switch(rotation) {
      case UP:
      case DOWN:
	return (*this)->get_height();
      case LEFT:
      case RIGHT:
	return (*this)->get_width();
      } // switch(rotation)

      return 0;
    } // int PixbufRotations::height(Rotation const rotation) const

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
   ** @version	0.6.1
   **
   **/
  Glib::RefPtr<Pixbuf>
    PixbufRotations::create_rotation(Rotation const rotation)
    {
      switch(rotation) {
      case UP:
	// cannot happen
	return *this;
      case DOWN:
	return (this->pixbuf[rotation] = rotate_180(*this));
      case LEFT:
	if (!this->pixbuf[RIGHT])
	  return (this->pixbuf[rotation] = rotate_left(*this));
	else
	  return (this->pixbuf[rotation] = rotate_180(this->pixbuf[RIGHT]));
      case RIGHT:
	if (!this->pixbuf[LEFT])
	  return (this->pixbuf[rotation] = rotate_right(*this));
	else
	  return (this->pixbuf[rotation] = rotate_180(this->pixbuf[LEFT]));
      } // switch(rotation)

      return *this;
    } // Glib::RefPtr<Pixbuf> PixbufRotations::create_rotation(Rotation const rotation)

} // namespace Gdk

#endif // #ifdef USE_UI_GTKMM
