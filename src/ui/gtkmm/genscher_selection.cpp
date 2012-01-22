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

#include "genscher_selection.h"
#include "table.h"
#include "ui.h"
#include "translations.h"
#include "main_window.h"

#include "../../game/game.h"
#include "../../player/player.h"

#include "widgets/stock_button.h"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/alignment.h>
#include <gtkmm/table.h>
#include <gtkmm/main.h>

namespace UI_GTKMM_NS {

  /**
   ** constructor
   **
   ** @param     table   	the table
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  GenscherSelection::GenscherSelection(Table* const table) :
    Base(table),
    Gtk::StickyDialog("genscher selection", *table->ui->main_window, true),
    info_label(NULL),
    partner_button(),
    no_partner_change_button(NULL),
    genscher(NULL),
    partner(NULL)
  {
    this->ui->add_window(*this);

    this->signal_realize().connect(sigc::mem_fun(*this,
                                                 &GenscherSelection::init));

    return ;
  } // GenscherSelection::GenscherSelection(Table* const table)

  /**
   ** destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  GenscherSelection::~GenscherSelection()
  { }

  /**
   **
   ** init the dialog
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
    GenscherSelection::init()
    {
      this->set_icon(this->ui->icon);
      this->set_skip_taskbar_hint();

      this->ui->translations->add(*this,
                                  ::translation("~genscher"));

      Gtk::Alignment* alignment = Gtk::manage(new Gtk::Alignment());
      alignment->set_border_width(2 ex);
      this->get_action_area()->add(*alignment);

      Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex));
      vbox->set_border_width(1 ex);
      alignment->add(*vbox);

      this->info_label = Gtk::manage(new Gtk::Label("Genscher: Please choose your partner."));
      this->ui->translations->add(*(this->info_label),
                                  ::translation("Genscher: Please choose your partner."));
      vbox->add(*(this->info_label));

      Gtk::Table* table = Gtk::manage(new Gtk::Table(3, 3, true));
      table->set_spacings(10);
      table->set_border_width(10);

      for (unsigned p = 0; p < this->genscher->game().playerno(); p++) {
        Player const& player = this->genscher->game().player(p);
        this->partner_button.push_back(Gtk::manage(new Gtk::Button(player.name())));
        this->partner_button.back()->signal_clicked().connect(sigc::bind<Player const* const>(sigc::mem_fun(*this, &GenscherSelection::select_partner_event), &player));
        switch(static_cast<Table*>(this->parent)->position(player)) {
        case NORTH:
          table->attach(*(this->partner_button.back()),
                        1, 2, 0, 1);
          break;
        case SOUTH:
          table->attach(*(this->partner_button.back()),
                        1, 2, 2, 3);
          break;
        case WEST:
          table->attach(*(this->partner_button.back()),
                        0, 1, 1, 2);
          break;
        case EAST:
          table->attach(*(this->partner_button.back()),
                        2, 3, 1, 2);
          break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "GenscherSelection::init()\n"
                        "  wrong position 'CENTER' "
                        << "(" << static_cast<Table*>(this->parent)->position(player) << ")");
        break;
        } // switch(this->table->position(player))
      } // for (p < this->genscher->game().playerno())
      vbox->add(*table);

      this->no_partner_change_button = Gtk::manage(new Gtk::Button("no partner change"));
      this->ui->translations->add(*(this->no_partner_change_button),
                                  ::translation("no partner change"));
      vbox->add(*(this->no_partner_change_button));

      this->no_partner_change_button->signal_clicked().connect(sigc::bind<Player const* const>(sigc::mem_fun(*this, &GenscherSelection::select_partner_event), NULL));
      this->show_all_children();

      return ;
    } // void GenscherSelection::init()

  /**
   **
   ** show the window
   **
   ** @param     genscher   the player who has the genscher
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **
   **/
  Player const*
    GenscherSelection::select_partner(Player const& genscher)
    {
      COUT << "select_partner" << endl;
      this->genscher = &genscher;
      if (!this->is_realized())
        this->realize();

      // Note: NULL is used for no selection
      this->partner = this->genscher;

      this->partner_button[genscher.no()]->hide();

      this->present();

      while (!this->ui->thrower
             && (this->partner == this->genscher)
             && this->is_visible())
        ::ui->wait();

      this->hide();

      if (this->partner == this->genscher)
        this->partner = NULL;

      this->genscher = NULL;

      return this->partner;
    } // Player const* GenscherSelection::select_partner(Player const& genscher)

  /**
   **
   ** a partner has been selected
   **
   ** @param     player   the player who has been selected
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   **/
  void
    GenscherSelection::select_partner_event(Player const* const player)
    {
      this->partner = player;

      return ;
    } // void GenscherSelection::select_partner_event(Player const* const player)

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
    GenscherSelection::name_changed(Player const& player)
    {
      if (!this->is_realized())
        return ;

      dynamic_cast<Gtk::Label*>(this->partner_button[player.no()]->get_child()
                               )->set_text(player.name());

      return ;
    } // void GenscherSelection::name_changed(Player const& player)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
