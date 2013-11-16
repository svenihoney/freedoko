/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2013  by Diether Knof and Borg Enders
 *
 *   This program is free software; you can redistribute it and/or 
 *   modify it under the terms of the GNU General Public ScaledPixbuf as 
 *   published by the Free Software Foundation; either version 2 of 
 *   the ScaledPixbuf, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public ScaledPixbuf for more details. 
 *   You can find this license in the file 'gpl.txt'.
 *
 *   You should have received a copy of the GNU General Public ScaledPixbuf
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

#ifndef GTKMM_WIDGET_SCALED_PIXBUF_HEADER
#define GTKMM_WIDGET_SCALED_PIXBUF_HEADER

#include "../../../basistypes.h"
#include <gdkmm/pixbuf.h>

namespace Gdk {
  /**
   ** @brief	a scaled pixmap
   **		scaled is only when needed
   **
   ** @author	Diether Knof
   **/
  class ScaledPixbuf {
    public:
      ScaledPixbuf(string const filename);
      ScaledPixbuf(Glib::RefPtr<Pixbuf> const pixbuf);
      ScaledPixbuf();
      ScaledPixbuf(ScaledPixbuf const& pixbuf_rotations);
      ScaledPixbuf& operator=(ScaledPixbuf const&);

      virtual ~ScaledPixbuf();

      virtual operator Glib::RefPtr<Pixbuf> const&();
      virtual Glib::RefPtr<Pixbuf> const& pixbuf();
      virtual Glib::RefPtr<Pixbuf> const& orig_pixbuf() const;

      virtual operator bool() const;

      virtual double get_scaling() const;
      virtual void set_scaling(double const scaling);
      virtual void set_to_max_size(int const width, int const height);

      virtual int get_width() const;
      virtual int get_height() const;
      virtual int get_orig_width() const;
      virtual int get_orig_height() const;

    private:
      virtual void scale();

    private:
      double scaling_;
      Glib::RefPtr<Pixbuf> orig_pixbuf_;
      Glib::RefPtr<Pixbuf> scaled_pixbuf_;

  }; // class ScaledPixbuf

} // namespace Gdk

#endif // #ifdef GTKMM_WIDGET_SCALED_PIXBUF_HEADER

#endif // #ifdef USE_UI_GTKMM
