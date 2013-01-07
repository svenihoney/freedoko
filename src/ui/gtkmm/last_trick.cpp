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
 *********************************************************************/

#include "constants.h"

#ifdef USE_UI_GTKMM

#include "last_trick.h"
#include "table.h"
#include "trick_summary.h"

#include "ui.h"
#include "translations.h"
#include "main_window.h"

#include "../../game/game.h"
#include "../../party/rule.h"
#include "../../card/trick.h"
#include "../../player/player.h"

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/alignment.h>
#include <gtkmm/label.h>
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gdk/gdkkeysyms.h>
#include "widgets/stock_button.h"

namespace UI_GTKMM_NS {

  /**
   **
   ** Constructor
   **
   ** @param     table   the table
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **
   **/
  LastTrick::LastTrick(Table* const table) :
    Base(table),
    Gtk::StickyDialog("last trick"),
    last_trick(NULL),
    navigation_container(NULL),
    previous_trick_button(NULL),
    following_trick_button(NULL),
    trick_summary(NULL)
  {
    this->signal_realize().connect(sigc::mem_fun(*this, &LastTrick::init));

    return ;
  } // LastTrick::LastTrick(Table* const table)

  /**
   **
   ** Destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  LastTrick::~LastTrick()
  {
    delete this->trick_summary;

    return ;
  } // LastTrick::~LastTrick()

  /**
   **
   ** create all subelements
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.3
   **
   **/
  void
    LastTrick::init()
    {
      this->ui->translations->add(*this,
                                  ::translation("~last trick"));

      { // action area
        Gtk::StockButton* close_button
          = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
        this->ui->translations->add(*close_button,
                                    ::translation("close"));
        this->add_action_widget(*close_button, Gtk::RESPONSE_CLOSE);

        close_button->grab_default();

        close_button->signal_clicked().connect(sigc::mem_fun(*this, &LastTrick::hide));
      } // action area

      this->get_vbox()->set_spacing(1 ex);

      this->navigation_container = Gtk::manage(new Gtk::HBox());
      this->previous_trick_button = Gtk::manage(new Gtk::Button());
      this->following_trick_button = Gtk::manage(new Gtk::Button());
      this->previous_trick_button->add(*(Gtk::manage(new Gtk::Image(Gtk::Stock::GO_BACK, Gtk::ICON_SIZE_BUTTON))));
      this->following_trick_button->add(*(Gtk::manage(new Gtk::Image(Gtk::Stock::GO_FORWARD, Gtk::ICON_SIZE_BUTTON))));
      this->navigation_container->pack_start(*(this->previous_trick_button),
                                             false, true);
      this->navigation_container->pack_end(*(this->following_trick_button),
                                           false, true);
      this->navigation_container->pack_start(*(Gtk::manage(new Gtk::Alignment())),
                                             Gtk::PACK_EXPAND_WIDGET);


      this->previous_trick_button->signal_clicked().connect(sigc::mem_fun(*this, &LastTrick::previous_trick));
      this->following_trick_button->signal_clicked().connect(sigc::mem_fun(*this, &LastTrick::following_trick));

      this->get_vbox()->add(*this->navigation_container);

      this->trick_summary = new TrickSummary(this);
      this->trick_summary->set_trick(*this->last_trick);

      this->get_vbox()->add(*this->trick_summary);

      this->show_all_children();
      this->set_trickno(this->trickno());
      this->trick_summary->update();

      this->signal_key_press_event().connect(sigc::mem_fun(*this,
                                                           &LastTrick::key_press));

      return ;
    } // void LastTrick::init()

  /**
   **
   ** show the last trick
   **
   ** @param     last_trick   the last trick
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **
   **/
  void
    LastTrick::show_last_trick(::Trick const& last_trick)
    {
      this->last_trick = &last_trick;
      if (this->last_trick->game().tricks_in_trickpiles() == 0)
        return ;

      this->realize();

      this->set_trickno(this->trickno());
      this->present();

      return ;
    } // void LastTrick::show_last_trick(::Trick const& last_trick)

  /**
   ** hide the window
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.9
   **/
  void
    LastTrick::close()
    {
      this->hide();

      return ;
    } // void LastTrick::close()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    number of the current trick
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.7
   **
   **/
  unsigned
    LastTrick::trickno() const
    {
      if (this->last_trick == NULL)
        return UINT_MAX;
      else
        return this->last_trick->no();
    } // unsigned LastTrick::trickno() const

  /**
   **
   ** set the trickno
   **
   ** @param     trickno   new trickno
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.7
   **
   **/
  void
    LastTrick::set_trickno(unsigned const trickno)
    {
      this->last_trick = &this->last_trick->game().trick(trickno);
      this->trick_summary->set_trick(*this->last_trick);
      if (this->previous_trick_button != NULL)
        this->previous_trick_button->set_sensitive((this->trickno() > 0)
                                                   && (this->trickno()
                                                       + this->last_trick->game().rule()(Rule::SHOW_TRICKS_NUMBER)
                                                       > this->last_trick->game().tricks_in_trickpiles())
                                                  );
      if (this->following_trick_button != NULL)
        this->following_trick_button->set_sensitive(this->trickno()
                                                    < this->last_trick->game().tricks_in_trickpiles() - 1);

      if (this->trickno()
          == this->last_trick->game().tricks_in_trickpiles() - 1)
        this->ui->translations->change(*this,
                                       ::translation("~last trick"));
      else
        this->ui->translations->change(*this,
                                       ::translation("%unumber%. trick",
                                                     this->trickno() + 1));
      if ((this->last_trick->game().rule()(Rule::SHOW_TRICKS_NUMBER) > 1)
          && (this->last_trick->game().tricks_in_trickpiles() >= 2))
        this->navigation_container->show();
      else
        this->navigation_container->hide();

      return ;
    } // void LastTrick::set_trickno(unsigned const trickno)

  /**
   ** the current trick is closed
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.9
   **/
  void
    LastTrick::current_trick_closed()
    {
      // Check, whether it is still allowed to show the current "last trick".
      if (this->last_trick == NULL)
        return ;

      if (this->trickno()
          + this->last_trick->game().rule()(Rule::SHOW_TRICKS_NUMBER)
          >= this->last_trick->game().tricks_in_trickpiles())
        this->set_trickno(this->trickno());
      else
        this->hide();

      return ;
    } // void LastTrick::current_trick_closed()

  /**
   **
   ** show the previous trickno
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    LastTrick::previous_trick()
    {
      if (this->trickno() > 0)
        if (this->trickno()
            + this->last_trick->game().rule()(Rule::SHOW_TRICKS_NUMBER)
            > this->last_trick->game().tricks_in_trickpiles())
          this->set_trickno(this->trickno() - 1);

      return ;
    } // void LastTrick::previous_trick()

  /**
   **
   ** show the following trickno
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    LastTrick::following_trick()
    {
      if (this->trickno()
          < this->last_trick->game().tricks_in_trickpiles() - 1)
        this->set_trickno(this->trickno() + 1);

      return ;
    } // void LastTrick::following_trick()

  /**
   ** the window is hidden
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.9
   **
   **/
  void
    LastTrick::on_hide()
    {
      this->Gtk::StickyDialog::on_hide();
      this->last_trick = NULL;
      this->trick_summary->remove_trick();

      return ;
    } // void LastTrick::on_hide()

  /**
   **
   ** the name of 'player' has changed
   **
   ** @param     player   the player whose name has changed
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    LastTrick::name_changed(::Player const& player)
    {
      if (this->last_trick)
        this->trick_summary->update();

      return ;
    } // void LastTrick::name_changed(Player const& player)

  /**
   ** a key has been pressed
   **
   ** @param     key   the key
   **
   ** @return    from 'ui->key_press(key)'
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   ** @bug	signal does not get here for '->', '<-' and '\<Esc\>'
   **/
  bool
    LastTrick::key_press(GdkEventKey* key)
    {
      // whether the key was accepted
      bool accepted = false;

      if ((key->state & GDK_SHIFT_MASK) == 0) {
        switch (key->keyval) {
        case GDK_Left: // show previous trick
          this->previous_trick();
          accepted = true;
          break;
        case GDK_Right: // show following trick
          this->following_trick();
          accepted = true;
          break;
        case GDK_Escape: // close the window
          this->hide();
          accepted = true;
          break;
        } // switch (key->keyval)
      } // if (key->state == 0)

      if (accepted)
        return false;

      return this->ui->key_press(key);
    } // bool LastTrick::key_press(GdkEventKey* key)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
