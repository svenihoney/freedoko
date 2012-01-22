/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2012  by Diether Knof and Borg Enders
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

#ifndef GTKMM_WIDGET_STOCK_BUTTON_HEADER
#define GTKMM_WIDGET_STOCK_BUTTON_HEADER

#include <gtkmm/button.h>
namespace Gtk {
  class StockID;
  class Image;
  class Label;
}

namespace Gtk {
  /**
   ** @brief	a button with a stock image in it
   **
   ** @author	Diether Knof
   **/
  class StockButton : public Button {
    public:
      StockButton(Glib::ustring const& label);
      StockButton(Gtk::StockID const& stock_id, Glib::ustring const& label,
                  bool const align_center = true);

      virtual ~StockButton();

      void set_label(Glib::ustring const& label);

      Image* image;
      Label* label;

    private: // unused
      StockButton();
      StockButton(StockButton const&);
      StockButton& operator=(StockButton const&);
  }; // class StockButton : public HBox

} // namespace Gtk

#endif // #ifdef GTKMM_WIDGET_STOCK_BUTTON_HEADER

#endif // #ifdef USE_UI_GTKMM
