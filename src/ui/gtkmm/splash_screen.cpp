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

#include "splash_screen.h"

#include "ui.h"
#include "translations.h"

#include "../../misc/setting.h"

#include <gtkmm/progressbar.h>
#include <gtkmm/box.h>
#include <gtkmm/image.h>
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
   ** @version   0.5.4
   **/
  SplashScreen::SplashScreen(Base* const parent) :
    Base(parent),
    Window(Gtk::WINDOW_POPUP),
    image(),
    progress_bar(),
    logo_pixmap(),
    logo_bitmap(),
    gc()
  {
    this->set_type_hint(Gdk::WINDOW_TYPE_HINT_SPLASHSCREEN);
    this->signal_realize().connect(sigc::mem_fun(*this,
					      &SplashScreen::init));
    this->signal_show().connect(sigc::mem_fun(*this,
					   &SplashScreen::language_update));

    return ;
  } // SplashScreen::SplashScreen(Base* const parent)

  /**
   ** Destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  SplashScreen::~SplashScreen()
  {
    return ;
  } // SplashScreen::~SplashScreen()

  /**
   ** create all subelements
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    SplashScreen::init()
    {
      this->Window::realize();

      this->ui->translations->add(*this, ::translation("~splash screen"));

      this->image = Gtk::manage(new Gtk::Image(this->ui->logo));
      this->progress_bar = Gtk::manage(new Gtk::ProgressBar());

      if (::setting(Setting::SPLASH_SCREEN_TRANSPARENT)) {
	this->add(*(this->image));
	image->show();

	Glib::RefPtr<Gdk::Pixmap> logo_pixmap; // = Gdk::Pixmap::create();
	Glib::RefPtr<Gdk::Bitmap> logo_bitmap;
	this->ui->logo->render_pixmap_and_mask(logo_pixmap, logo_bitmap,
					       ALPHA_THRESHOLD);

	this->get_window()->shape_combine_mask(logo_bitmap, 0, 0);

	this->ui->logo->render_pixmap_and_mask(this->logo_pixmap,
					       this->logo_bitmap,
					       ALPHA_THRESHOLD);
	this->gc = Gdk::GC::create(this->logo_pixmap);

	// make the foreground color 'red'
	Gdk::Color color;
	color.set("red");
	this->ui->colormap->alloc_color(color);
	this->gc->set_foreground(color);
	this->gc->set_background(color);
	this->gc->set_fill(Gdk::SOLID);
      } else { // if !(::setting(Setting::SPLASH_SCREEN_TRANSPARENT))
	Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 0));
	this->add(*vbox);

	vbox->pack_start(*(this->image), Gtk::PACK_SHRINK);
	vbox->pack_end(*(this->progress_bar), Gtk::PACK_SHRINK);
	vbox->show_all();
      } // if !(::setting(Setting::SPLASH_SCREEN_TRANSPARENT))

      this->set_position(Gtk::WIN_POS_CENTER);
      return ;
    } // void SplashScreen::init()

  /**
   ** the status message has changed
   **
   ** @param     status_message   the new status_message
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    SplashScreen::status_message_changed(Translator::Translation const& status_message)
    {
      if (!this->is_visible())
	return ;

      if (!::setting(Setting::SPLASH_SCREEN_TRANSPARENT))
	this->progress_bar->set_text(status_message.translation());

      this->ui->update();

      return ;
    } // void SplashScreen::status_message_changed(Translator::Translation const& status_message)

  /**
   ** the progress has changed
   **
   ** @param     progress   the actual progress
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    SplashScreen::progress_changed(double const progress)
    {
      if (!this->is_visible())
	return ;

      if (::setting(Setting::SPLASH_SCREEN_TRANSPARENT)) {
	this->logo_pixmap->draw_rectangle(this->gc,
					  true,
					  55, 191,
					  static_cast<int>((259 - 55)
							   * this->ui->progress()),
					  226 - 191);

	this->image->set(this->logo_pixmap, this->logo_bitmap);
      } else { //  (::setting(Setting::SPLASH_SCREEN_TRANSPARENT))
	this->progress_bar->set_fraction(progress);
      } // if (!::setting(Setting::SPLASH_SCREEN_TRANSPARENT))

      this->ui->update();

      return ;
    } // void SplashScreen::progress_changed(double const progress)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
