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

#ifndef GTKMM_WIDGET_LABEL_CARD_SELECTOR_HEADER
#define GTKMM_WIDGET_LABEL_CARD_SELECTOR_HEADER

#include "label_type_selector.h"

#include "../../../card/card.h"

namespace Gtk {
  /**
   ** @brief    a selector for cards with a label
   **
   ** @author   Diether Knof
   **/
  class LabelCardSelector : public LabelTypeSelector<Card> {
    public:
      // constructor
      LabelCardSelector() :
        LabelTypeSelector<Card>()
    { }
      // constructor
      LabelCardSelector(Glib::ustring const& label) :
        LabelTypeSelector<Card>(label)
    { }
      // destructor
      ~LabelCardSelector()
      { }


      // returns the selected type
      Card card()
      { return this->type(); }
      // set the selected type
      void set_card(Card const& card)
      { this->set_type(card); }

    private: // unused
      LabelCardSelector(LabelCardSelector const&);
      LabelCardSelector& operator=(LabelCardSelector const&);
  }; // class LabelCardSelector : public HBox

#ifdef OUTDATED
  class LabelCardSelector : public Gtk::HBox {
    public:
      // constructor
      LabelCardSelector() :
        label_(),
        selector_()
    { this->init(); }
      // constructor
      LabelCardSelector(Glib::ustring const& label) :
        label_(label),
        selector_()
    { this->init(); }
      // destructor
      ~LabelCardSelector()
      { }

      // the label
      Label& get_label()
      { return this->label_; }
      // the type selector
      TypeSelector<Card>& get_selector()
      { return this->selector_; }

      // adds the type to the selector
      void add(Card const& card)
      { this->get_selector().add(card); }

      // returns the selected type
      Card card()
      { return this->get_selector().type(); }
      // set the selected type
      void set_card(Card const& card)
      { this->get_selector().set_type(card); }

      // updates the text
      void language_update()
      { this->get_selector().language_update(); }

      Glib::SignalProxy0<void> signal_value_changed()
      { return this->get_selector().signal_changed(); }

    private:
      // inits the label type selector
      void init()
      {
        this->set_spacing(1 ex);

        Gtk::Alignment* alignment
          = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT, Gtk::ALIGN_CENTER,
                                           0, 0));
        alignment->add(this->get_label());
        this->pack_start(*alignment, true, true);
        this->pack_end(this->get_selector(), false, true);
      }
    private:
      // the label with the description
      Label label_;
      // the menu with the types
      TypeSelector<Card> selector_;

    private: // unused
      LabelCardSelector(LabelCardSelector const&);
      LabelCardSelector& operator=(LabelCardSelector const&);
  }; // class LabelCardSelector : public HBox
#endif

} // namespace Gtk

#endif // #ifdef GTKMM_WIDGET_LABEL_CARD_SELECTOR_HEADER

#endif // #ifdef USE_UI_GTKMM
