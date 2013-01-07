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
 **********************************************************************/

#ifdef USE_UI_GTKMM

#ifndef GTKMM_TRANSLATIONS_HEADER
#define GTKMM_TRANSLATIONS_HEADER

#include "base.h"

#include "../../misc/translations.h"

namespace Gtk {
  class Label;
  class StockButton;
  class Button;
  class Window;
  class TextView;
  class Widget;
} // namespace Gtk
#include <glibmm/refptr.h>
#include <gdkmm/pixbuf.h>

namespace UI_GTKMM_NS {
  class UI_GTKMM;

  /**
   **
   ** @brief	automatic translation for the widgets
   **
   ** @author	Diether Knof
   **
   **/
  class Translations : public Base, public sigc::trackable {
    public:
    Translations(UI_GTKMM* const ui);

    ~Translations();


    // updates the language
    void language_update();


    // automatic translate the text of the widget
    void add(Gtk::Label& label,
	     Translator::Translation const& translation);
    void add(Gtk::StockButton& stock_button,
	     Translator::Translation const& translation);
    void add(Gtk::Button& button,
	     Translator::Translation const& translation);
    void add(Gtk::Window& window,
	     Translator::Translation const& title);
    void add(Gtk::TextView& text_view,
	     Translator::Translation const& translation);
    void change(Gtk::Widget& widget,
		Translator::Translation const& translation);
    void change(Gtk::StockButton& stock_button,
		Translator::Translation const& translation);
    void change(Gtk::TextView& text_view,
		Translator::Translation const& translation);
    void remove(Gtk::Widget const& widget);


    void add_label_signal(Gtk::Label* const label,
			  Translator::Translation const translation)
    {
      this->add(*label, translation);
      return ;
    }
    void add_button_signal(Gtk::Button* const button,
			   Translator::Translation const translation)
    {
      this->add(*button, translation);
      return ;
    }
    void add_window_signal(Gtk::Window* const window,
			   Translator::Translation const title)
    {
      this->add(*window, title);
      return ;
    }
    void add_text_view_signal(Gtk::TextView* const text_view,
			      Translator::Translation const translation)
    {
      this->add(*text_view, translation);
      return ;
    }

    void change_signal(Gtk::Widget* const widget,
		       Translator::Translation const
		       translation)
    {
      this->change(*widget, translation);
      return ;
    }

    void remove_signal(Gtk::Widget const* const widget)
    {
      this->remove(*widget);
      return ;
    }

    private:
    // for the automatic translation
    // the data "translation" is a pointer to the translation
    vector<Gtk::Label*> label;
    vector<Gtk::Button*> button;
    vector<Gtk::Window*> window; // translates the title
    vector<Gtk::TextView*> text_view;

    private: // unused
    Translations(Translations const&);
    Translations& operator=(Translations const&);
  }; // class Translation : public Base

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_TRANSLATIONS_HEADER

#endif // #ifdef USE_UI_GTKMM
