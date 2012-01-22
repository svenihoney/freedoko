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
 *********************************************************************/

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "support.h"

#include "ui.h"
#include "translations.h"

#include "widgets/stock_button.h"
#include <gtkmm/stock.h>
#include <gtkmm/image.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/alignment.h>

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
   ** @version   0.7.4
   **/
  Support::Support(Base* const parent) :
    Base(parent),
  StickyDialog("Support", false),
  text(NULL)
  {
    this->ui->add_window(*this);
    this->signal_realize().connect(sigc::mem_fun(*this,
					      &Support::init));

    return ;
  } // Support::Support(Base* const parent)

  /**
   ** destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.4
   **/
  Support::~Support()
  { }

  /**
   ** create all subelements
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.4
   **/
  void
    Support::init()
    {
      this->ui->translations->add(*this, ::translation("FreeDoko %ttitle%",
						      ::translation("~support")));
      this->set_icon(this->ui->icon);

      this->set_skip_taskbar_hint();

      this->text = Gtk::manage(new Gtk::TextView());
      this->ui->translations->add(*this->text,
				    ::translation("Version::%sversion% %tsupport%",
						 static_cast<string>(*::version),
						 ::translation("Support::support")));

      Gtk::StockButton* close_button
	= Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
      this->ui->translations->add(*close_button,
				  ::translation("close"));
      this->add_action_widget(*close_button, Gtk::RESPONSE_CLOSE);
      close_button->show_all();

      close_button->grab_default();
      // signals
      close_button->signal_clicked().connect(sigc::mem_fun(*this, &Gtk::Widget::hide));


      this->set_default_size((this->ui->logo->get_width() * 3) / 2,
			     (this->ui->logo->get_height() * 5) / 2);
#ifdef POSTPONED
      this->get_window()->set_decorations(Gdk::DECOR_BORDER
					  | Gdk::DECOR_RESIZEH
					  | Gdk::DECOR_TITLE
					  | Gdk::DECOR_MENU);
#endif

      { // the image
	Gtk::Alignment* alignment
	  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
					   0, 0));
	alignment->set_border_width(parent->ui->logo->get_width() / 20);
	Gtk::Image* image = Gtk::manage(new Gtk::Image(parent->ui->logo));
	alignment->add(*image);

	this->get_vbox()->pack_start(*alignment, Gtk::PACK_SHRINK);
      } // the image
      { // the text
	Gtk::ScrolledWindow* text_window = Gtk::manage(new Gtk::ScrolledWindow());
	text_window->add(*this->text);
	text_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	this->text->set_editable(false);
	this->text->set_wrap_mode(Gtk::WRAP_WORD);
	this->text->set_cursor_visible(false);

	this->get_vbox()->add(*text_window);
      } // the text

      this->show_all_children();

      return ;
    } // void Support::init()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
