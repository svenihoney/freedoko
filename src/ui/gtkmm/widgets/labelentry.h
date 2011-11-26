/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2011  by Diether Knof and Borg Enders
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

#ifndef GTKMM_WIDGET_LABEL_ENTRY_HEADER
#define GTKMM_WIDGET_LABEL_ENTRY_HEADER

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/alignment.h>

namespace Gtk {
  /**
   ** @brief	a spin button with a label
   **
   ** @author	Diether Knof
   **/
  class LabelEntry : public HBox {
    public:
      LabelEntry() :
        HBox(false),
        label_(),
        entry_()
    { 
      this->set_spacing(1 ex);

      Gtk::Alignment* alignment
        = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
                                         0, 0));
      alignment->add(this->get_entry());
      this->pack_start(*alignment, true, true);
    }
      LabelEntry(Glib::ustring const& label) :
        HBox(false),
        label_(label),
        entry_()
    { 
      this->set_spacing(10);

      Gtk::Alignment* alignment
        = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT, Gtk::ALIGN_CENTER,
                                         0, 0));
      alignment->add(this->get_label());
      this->pack_start(*alignment, true, true);
      this->pack_end(this->get_entry(), false, true);
    }

      virtual ~LabelEntry()
      { }


      // Label functions
      Label& get_label()
      { return this->label_; }

      void set_label(const Glib::ustring& str)
      { return this->get_label().set_text(str); }


      // Entry functions
      Entry& get_entry()
      { return this->entry_; }

      Glib::ustring get_text()
      { return this->get_entry().get_text(); }

      virtual void set_text(Glib::ustring const& text)
      { return this->get_entry().set_text(text); }

    private:
      Label label_;
      Entry entry_;

    private: // unused
      LabelEntry(LabelEntry const&);
      LabelEntry& operator=(LabelEntry const&);
  }; // class LabelEntry : public HBox

} // namespace Gtk

#endif // #ifdef GTKMM_WIDGET_LABEL_ENTRY_HEADER

#endif // #ifdef USE_UI_GTKMM
