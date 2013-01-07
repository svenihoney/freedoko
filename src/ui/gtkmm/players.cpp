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

#include "players.h"
#include "player.h"
#include "aiconfig.h"
#include "players_db.h"
#include "ui.h"
#include "translations.h"
#include "utils.h"
#include "widgets/stock_button.h"

#include "../../party/party.h"
#include "../../player/ai/ai.h"
#include "../../player/aiconfig.h"
#include "../../misc/translator.h"

#include "widgets/labelspinbutton.h"
#include <gtkmm/table.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/stock.h>
#include <gtkmm/alignment.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/entry.h>
#include <gtkmm/notebook.h>

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
  Players::Players(Base* const parent) :
    Base(parent),
  StickyDialog("Players", false),
  player_(),
    players_notebook(NULL)
  {
    this->ui->add_window(*this);

    this->signal_realize().connect(sigc::mem_fun(*this, &Players::init));
    this->signal_show().connect(sigc::mem_fun(*this, &Players::sensitivity_update));
#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
    this->signal_key_press_event().connect(sigc::mem_fun(*this, &Players::on_key_press_event));
#endif

    this->create_backup();

    return ;
  } // Players::Players(Base* const parent)

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
  Players::~Players()
  {
    for (vector<Player*>::iterator player = this->player_.begin();
	 player != this->player_.end();
	 player++)
      delete *player;

    return ;
  } // Players::~Players()

  /**
   ** creates all subelements
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **
   ** @todo	support for more than four players
   **/
  void
    Players::init()
    {
      this->set_icon(this->ui->icon);
      this->ui->translations->add(*this, ::translation("FreeDoko %ttitle%",
						      ::translation("~players")));

      { // set default size
	Gdk::Geometry geometry;
	geometry.min_width = 70 ex;
	geometry.min_height = 10 ex;

	this->set_geometry_hints(*this, geometry, Gdk::HINT_MIN_SIZE);
      } // set default size

      for (unsigned p = 0; p < ::party.playerno(); p++)
	this->player_.push_back(new Player(this, p));

      Gtk::StockButton* close_button
	= Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
      close_button->signal_clicked().connect(sigc::mem_fun(*this, &Gtk::Widget::hide));
      this->ui->translations->add(*close_button,
				  ::translation("close"));

      { // layout
	{ // action area
	  this->add_action_widget(*close_button, Gtk::RESPONSE_CLOSE);
	  close_button->grab_focus();
	} // action area

	{ // players configurations
	  this->players_notebook = Gtk::manage(new Gtk::Notebook());
	  this->players_notebook->set_tab_pos(Gtk::POS_LEFT);
	  this->get_vbox()->add(*this->players_notebook);

	  vector<Gtk::Notebook*> player_notebook;
	  for (unsigned p = 0; p < ::party.playerno(); ++p) {
	    Players::Player& player = this->player(p);

	    Gtk::VBox* player_box = Gtk::manage(new Gtk::VBox(false, 3 ex / 2));

	    this->players_notebook->append_page(*player_box,
						*(player.name_label));

	    { // buttons
	      Gtk::Alignment* alignment
		= Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
						 Gtk::ALIGN_CENTER,
						 0, 0));
	      player_box->pack_end(*alignment, false, true);
	      alignment->set_border_width(1 ex / 2);
	      Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(true, 0));
	      alignment->add(*hbox);
	      { // reset button
		hbox->add(*player.reset_button);
	      } // reset button
	      { // empty space
		hbox->add(*(Gtk::manage(new Gtk::Alignment())));
	      } // empty space
	      { // statistics button
		Gtk::Button* statistics_button
		  = Gtk::manage(new Gtk::Button("statistics"));
		this->ui->translations->add(*statistics_button,
					    ::translation("statistics"));
		hbox->add(*statistics_button);
		statistics_button->signal_clicked().connect(sigc::mem_fun0(*(this->ui->players_db), &Gtk::Window::present));
	      } // statistics button
	    } // buttons


	    player_notebook.push_back(Gtk::manage(new Gtk::Notebook()));
	    player_box->pack_start(*(player_notebook.back()), true, true);
	    { // general page
	      Gtk::Label* general_label
		= Gtk::manage(new Gtk::Label("general"));
	      this->ui->translations->add(*general_label,
					  ::translation("general"));

	      Gtk::Alignment* alignment
		= Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
						 Gtk::ALIGN_CENTER,
						 0, 0));
	      player_notebook.back()->append_page(*alignment, *general_label);

	      Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 2 ex));
	      vbox->set_border_width(1 ex);
	      alignment->add(*vbox);

	      { // the name widgets
		Gtk::Alignment* alignment
		  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
						   0, 0)); 
		vbox->add(*alignment);

		Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 10));
		alignment->add(*hbox);

		Gtk::Label* name_label = Gtk::manage(new Gtk::Label("name:"));
		this->ui->translations->add(*name_label,
					    ::translation("%ttranslation%: ",
							  ::translation("name")));

		hbox->add(*name_label);
		player.name_entry->set_width_chars(12);
		hbox->add(*player.name_entry);
		hbox->add(*player.random_name_button);
	      } // the name widgets
	      { // the voice widgets
		Gtk::Alignment* alignment
		  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
						   0, 0)); 
		vbox->add(*alignment);

		Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 10));
		alignment->add(*hbox);

		Gtk::Label* voice_label = Gtk::manage(new Gtk::Label("voice:"));
		this->ui->translations->add(*voice_label,
					    ::translation("%ttranslation%: ",
							  ::translation("voice")));

		hbox->add(*voice_label);
		hbox->add(*player.voice_button);
	      } // the voice widgets
	      { // the type widgets
		Gtk::Alignment* alignment
		  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
						   0, 0)); 
		vbox->add(*alignment);

		Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
		alignment->add(*vbox);

		for (vector<Gtk::RadioButton*>::iterator type
		     = player.type_button.begin();
		     type != player.type_button.end();
		     type++)
		  vbox->add(**type);
	      } // the type widgets
	      { // the ai difficulty
		vbox->add(*player.aiconfig().difficulty_container);
	      } // the ai difficulty
	    } // general page
	    { // aiconfig
	      Gtk::Label* aiconfig_label
		= Gtk::manage(new Gtk::Label("aiconfig"));
	      this->ui->translations->add(*aiconfig_label,
					  ::translation("aiconfig"));

	      player_notebook.back()->append_page(*player.aiconfig().container,
						  *aiconfig_label);

	    } // aiconfig
	  } // for (p < ::party.playerno())

	  // set the signals for the notebook changes

	  for (vector<Gtk::Notebook*>::iterator notebook1
	       = player_notebook.begin();
	       notebook1 != player_notebook.end();
	       notebook1++)
	    for (vector<Gtk::Notebook*>::iterator notebook2
		 = player_notebook.begin();
		 notebook2 != player_notebook.end();
		 notebook2++) {
	      if (notebook1 == notebook2)
		continue;

	      (*notebook1)->signal_switch_page().connect(sigc::bind<Gtk::Notebook*>(sigc::mem_fun(*this, &Players::notebook_switch_page_signal), *notebook2));
	    }

	  for (vector<Player*>::iterator player1 = this->player_.begin();
	       player1 != this->player_.end();
	       player1++) {
	    for (vector<Player*>::iterator player2 = this->player_.begin();
		 player2 != this->player_.end();
		 player2++) {
	      if (player1 == player2)
		continue;

	      (*player1)->aiconfig().notebook->signal_switch_page().connect(sigc::bind<Gtk::Notebook*>(sigc::mem_fun(*this, &Players::notebook_switch_page_signal), (*player2)->aiconfig().notebook));

	      for (unsigned i = 0;
		   i < (*player1)->aiconfig().notebooks.size();
		   ++i)
		(*player1)->aiconfig().notebooks[i]->signal_switch_page().connect(sigc::bind<Gtk::Notebook*>(sigc::mem_fun(*this, &Players::notebook_switch_page_signal), (*player2)->aiconfig().notebooks[i]));
	    } // for (player2)
	  } // for (player1)
	} // players configurations
      } // layout

      this->show_all_children();

      this->update();

      return ;
    } // void Players::init()

  /**
   ** -> result
   **
   ** @param     playerno   the player number
   **
   ** @return    the player configuration of player 'playerno'
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  Players::Player&
    Players::player(unsigned const playerno)
    {
      DEBUG_ASSERTION((this->player_.size() > playerno),
		      "Players::player(playerno):\n"
		      "  playerno too great: "
		      << playerno << " >= " << this->player_.size());

      return *(this->player_[playerno]);
    } // Players::Player& Players::player(unsigned const playerno)

  /**
   ** creates a backup
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
    Players::create_backup()
    {
      for (vector<Player*>::iterator player = this->player_.begin();
	   player != this->player_.end();
	   player++)
	(*player)->create_backup();

      return ;
    } // void Players::create_backup()

  /**
   ** resets the settings
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **
   ** @todo	set the elements
   **/
  void
    Players::reset()
    {
      if (!this->is_visible())
	return ;

      for (vector<Player*>::iterator player = this->player_.begin();
	   player != this->player_.end();
	   player++)
	(*player)->reset();
      this->update();

      return ;
    } // void Players::reset()

  /**
   ** update the sensitivity of all widgets
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
    Players::sensitivity_update()
    {
      if (!this->is_visible())
	return ;

      for (vector<Player*>::iterator player = this->player_.begin();
	   player != this->player_.end();
	   player++)
	(*player)->sensitivity_update();

      return ;
    } // void Players::sensitivity_update()

  /**
   ** update all widgets
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
    Players::update()
    {
      if (!this->is_visible())
	return ;

      for (vector<Player*>::iterator player = this->player_.begin();
	   player != this->player_.end();
	   player++)
	(*player)->update();

      return ;
    } // void Players::update()

  /**
   ** the players 'player_a' and 'player_b' have been switched
   **
   ** @param     player_a   first player
   ** @param     player_b   second player
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **/
  void
    Players::players_switched(::Player const& player_a,
			      ::Player const& player_b)
    {
      if (!this->is_visible())
	return ;

#ifdef POSTPONED
      std::swap(this->player(player_a.no()), this->player(player_a.no()));
      // Problem: cannot swap notebook pages
#else
      this->player(player_a.no()).update();
      this->player(player_b.no()).update();
#endif

      return ;
    } // void Players::players_switched(Player player_a, Player player_b)

  /**
   ** update 'player'
   **
   ** @param     player   player to update
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    Players::player_update(::Player const& player)
    {
      if (!this->is_visible())
	return ;

      this->player(player.no()).update();

      return ;
    } // void Players::player_update(::Player const& player)

  /**
   ** update the name of 'player'
   **
   ** @param     player   player with the new name
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    Players::name_update(::Player const& player)
    {
      if (!this->is_visible())
	return ;

      this->player(player.no()).name_update();

      return ;
    } // void Players::name_update(::Player player)

  /**
   ** update the voice of 'player'
   **
   ** @param     player   player with the new voice
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  void
    Players::voice_update(::Player const& player)
    {
      if (!this->is_visible())
	return ;

      this->player(player.no()).voice_update();

      return ;
    } // void Players::voice_update(::Player player)

  /**
   ** update the aiconfig
   **
   ** @param     aiconfig   changed aiconfig
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.8
   **/
  void
    Players::aiconfig_update(::Aiconfig const& aiconfig)
    {
      if (!this->is_visible())
	return ;

      for (vector<Player*>::iterator p = this->player_.begin();
           p != this->player_.end();
           ++p)
          (*p)->aiconfig_update(aiconfig);

      return ;
    } // void Players::aiconfig_update(::Aiconfig aiconfig)

  /**
   ** attaches a group named 'name'
   ** the row is added at 'table';
   ** the row is 'current_row', which is incremented;
   **
   ** @param     name   		the name of the group (untranslated)
   ** @param     table   		the table to add the row
   ** @param     current_row   	the row number to insert the row
   ** @param     depth   		the depth of the group
   ** @param     dependent_button   the button which hides/shows the row
   **
   ** @return    the button for the group
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  Gtk::ToggleButton*
    Players::add_group(string const& name,
		       Gtk::Table* table, int& current_row,
		       int const depth,
		       Gtk::ToggleButton* dependent_button)
    {
      Gtk::Alignment* alignment
	= Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP,
					 0, 0));
      alignment->set_border_width(1 ex / 2);

      table->attach(*alignment, 0, 1, current_row, current_row + 1,
		    Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK,
		    depth em);

      Gtk::CheckButton* group_button
	= Gtk::manage(new Gtk::CheckButton(name));
      this->ui->translations->add(*group_button, ::translation(name));

      alignment->add(*group_button);

      if (dependent_button)
	set_signal_toggle_view(alignment, dependent_button);


      current_row += 1;

      return group_button;
    } // Gtk::ToggleButton* Players::add_group(Aiconfig::TypeBool const type, Gtk::Table* table, int& row, int const depth, Gtk::ToggleButton* dependent_button)

  /**
   ** attaches the aiconfig 'type'
   ** the row for 'type' is added at 'table';
   ** the row is 'current_row', which is incremented;
   ** the showing is dependent on the state of 'dependent_button'
   **
   ** @param     type   		the aiconfig type to add a row for
   ** @param     table   		the table to add the row
   ** @param     current_row   	the row number to insert the row
   ** @param     dependent_button   the button which hides/shows the row
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  void
    Players::add_aiconfig(int const type,
			  Gtk::Table* table, int& current_row,
			  Gtk::ToggleButton* dependent_button)
    {
      Gtk::Alignment* alignment
	= Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT, Gtk::ALIGN_CENTER,
					 0, 0));

      table->attach(*alignment, 0, 1, current_row, current_row + 1,
		    Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);

      Gtk::Label* label = NULL;
      if ((type >= Aiconfig::BOOL_FIRST)
	  && (type <= Aiconfig::BOOL_LAST)) {
	label = Gtk::manage(new Gtk::Label(::name(static_cast<Aiconfig::TypeBool>(type))));
	this->ui->translations->add(*label, ::translation(static_cast<Aiconfig::TypeBool>(type)));
      } else if ((type >= Aiconfig::INT_FIRST)
		 && (type <= Aiconfig::INT_LAST)) {
	label = Gtk::manage(new Gtk::Label(::name(static_cast<Aiconfig::TypeInt>(type))));
	this->ui->translations->add(*label, ::translation(static_cast<Aiconfig::TypeInt>(type)));
      } else {
	DEBUG_ASSERTION(false,
			"Players::add_aiconfig():\n"
			"  type '" << type << "' not known as Aiconfig::Type*");
      } // if (type \in Aiconfig::Type*)

      alignment->add(*label);

      Gtk::Table* table2 = Gtk::manage(new Gtk::Table(1, ::party.playerno(),
						      true));
      table->attach(*table2, 1, 2, current_row, current_row + 1,
		    Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK);
      table2->set_col_spacings(1 em / 2);
      table2->set_row_spacing(0, 1 ex / 2);

      for (unsigned p = 0; p < ::party.playerno(); ++p) {
	Gtk::Alignment* alignment
	  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
					   1, 0));
	table2->attach(*alignment, p, p + 1, 0, 1,
		       Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK,
		       0, 1 ex / 4);

	Gtk::Widget* widget = NULL;
	if ((type >= Aiconfig::BOOL_FIRST)
	    && (type <= Aiconfig::BOOL_LAST)) {
	  widget = this->player(p).aiconfig().type_bool[type - Aiconfig::BOOL_FIRST];
	} else if ((type >= Aiconfig::INT_FIRST)
		   && (type <= Aiconfig::INT_LAST)) {
	  widget = this->player(p).aiconfig().type_int[type - Aiconfig::INT_FIRST];
	} else {
	  DEBUG_ASSERTION(false,
			  "Players::add_aiconfig():\n"
			  "  type '" << type << "' not known as Aiconfig::Type*");
	} // if (type \in Aiconfig::Type*)
	alignment->add(*widget);
      } // for (p < ::party.playerno())

      set_signal_toggle_view(alignment, dependent_button);
      set_signal_toggle_view(table2, dependent_button);

      current_row += 1;

      return ;
    } // void Players::add_aiconfig(int const type, Gtk::Table* table, int& current_row, Gtk::ToggleButton* dependent_widget)

  /**
   ** signal: the page of the notebook is changed
   **
   ** @param     page      current page
   ** @param     pageno    current page number
   ** @param     notebook   notebook to change the page to 'pageno'
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  void
    Players::notebook_switch_page_signal(GtkNotebookPage* page,
					 guint pageno,
					 Gtk::Notebook* notebook)
    {
      // Note: the signal for this member is binded in 'Players'
      // because it is called by the other 'AiConfig's which have to be
      // created first.

      if (static_cast<unsigned>(notebook->get_current_page()) != pageno)
	notebook->set_current_page(pageno);

      return ;
    } // void Players::notebook_switch_page_signal(GtkNotebookPage* page, guint pageno, Gtk::Notebook* notebook)

  /**
   ** a key has been pressed
   ** C-o: output of the active player on 'stdout'
   **
   ** @param     key   the key
   **
   ** @return    whether the key was managed
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **/
  bool
    Players::on_key_press_event(GdkEventKey* key)
    {
      bool managed = false;

      if ((key->state & ~GDK_SHIFT_MASK) == GDK_CONTROL_MASK) {
	switch (key->keyval) {
	case GDK_o: { // ouput of the player
	  if (this->players_notebook->get_current_page() == -1)
	    break;
	  bool const write_database_bak = ::Player::write_database;
	  ::Player::write_database = false;
	  cout << this->player(this->players_notebook->get_current_page()).player();
	  ::Player::write_database = write_database_bak;
	  managed = true;
	} break;
	case GDK_u: // update the entries
	  if (this->players_notebook->get_current_page() == -1)
	    break;
	  this->player(this->players_notebook->get_current_page()).update();
	  managed = true;
	  break;
	} // switch (key->keyval)
      } // if (key->state == GDK_CONTROL_MASK)

      return (managed
	      || this->StickyDialog::on_key_press_event(key)
	      || this->ui->key_press(key));
    } // bool Players::on_key_press_event(GdkEventKey* key)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
