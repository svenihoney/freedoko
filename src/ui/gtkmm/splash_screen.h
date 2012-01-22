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

#ifdef USE_UI_GTKMM

#ifndef GTKMM_SPLASH_SCREEN_HEADER
#define GTKMM_SPLASH_SCREEN_HEADER

#include "base.h"

#include "../../misc/translation.h"

#include <gtkmm/window.h>
namespace Gtk {
  class Image;
  class ProgressBar;
} // namespace Gtk

namespace UI_GTKMM_NS {

  /**
   **
   ** @brief	the splash screen
   **
   ** @author	Diether Knof
   **
   **/
  class SplashScreen : public Base, public Gtk::Window {
    friend class UI_GTKMM;
    public:
    SplashScreen(Base* const parent);
    ~SplashScreen();

    void status_message_changed(Translator::Translation const& status_message);
    void progress_changed(double const progress_max);

    private:
    void init();
    private:
    Gtk::Image* image;
    Gtk::ProgressBar* progress_bar;

    Glib::RefPtr<Gdk::Pixmap> logo_pixmap;
    Glib::RefPtr<Gdk::Bitmap> logo_bitmap;
    Glib::RefPtr<Gdk::GC> gc;

    private: // unused
    SplashScreen();
    SplashScreen(SplashScreen const& splash);
    SplashScreen& operator=(SplashScreen const& splash);
  }; // class SplashScreen : public Base, public Gtk::Window

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_SPLASH_SCREEN_HEADER

#endif // #ifdef USE_UI_GTKMM
