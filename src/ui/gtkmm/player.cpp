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

#include "player.h"
#include "aiconfig.h"
#include "ui.h"
#include "translations.h"

#include "../../party/party.h"
#include "../../player/player.h"
#include "../../player/aiconfig.h"
#include "../../player/namegen.h"
#include "../../player/ai/ai.h"
#ifdef USE_NETWORK
#include "../../network/server.h"
#endif
#include "../../misc/setting.h"

#include "widgets/stock_button.h"
#include "widgets/filemenu_extension.h"
#include "widgets/filemenu_file.h"
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/box.h>
#include <gtkmm/alignment.h>

namespace UI_GTKMM_NS {

  /**
   ** constructor
   **
   ** @param     players    parent object
   ** @param     playerno   number of the player
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  Players::Player::Player(Players* const players, unsigned const playerno) :
    Base(players),
    sigc::trackable(),
    players(players),
    aiconfig_(NULL),
    playerno_(playerno),
    player_bak(this->player().clone()),
    name_label(NULL),
    name_entry(NULL),
    random_name_button(NULL),
    voice_button(NULL),
    voice_menu(NULL),
    type_button(),
    reset_button(NULL)
  {
    this->init();

    return ;
  } // Players::Player::Player(Base* const parent)

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
   ** @version   0.5.4
   **
   **/
  Players::Player::~Player()
  {
    delete this->aiconfig_;

    delete this->player_bak;

    for (vector<Gtk::RadioButton*>::iterator type = this->type_button.begin();
	 type != this->type_button.end();
	 type++)
      delete static_cast<PlayerType*>((*type)->steal_data("type"));

    return ;
  } // Players::Player::~Player()

  /**
   ** init all subelements
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
    Players::Player::init()
    {
      this->aiconfig_ = new AiConfig(this);

      this->name_label = Gtk::manage(new Gtk::Label("name"));
      this->name_entry = Gtk::manage(new Gtk::Entry());

      this->random_name_button = Gtk::manage(new Gtk::Button("random name"));
      this->ui->translations->add(*this->random_name_button,
				  ::translation("random name"));

      this->voice_button = Gtk::manage(new Gtk::Button("voice"));
      this->voice_menu
        = Gtk::manage(new Gtk::FileMenuFilterFile(sigc::mem_fun(*this, &Players::Player::voice_selected), "re." + ::setting(Setting::SOUND_EXTENSION)));

      this->reset_button = Gtk::manage(new Gtk::StockButton(Gtk::Stock::UNDO,
							    "reset"));
      this->ui->translations->add(*this->reset_button,
				  ::translation("reset"));

      { // the player types
	Gtk::RadioButton::Group group;
	// the type is 'int', because there are compilation errors with
	// '::Player::Type'
	// Human
	this->type_button.push_back(Gtk::manage(new Gtk::RadioButton(group,
								     "human")));
	this->type_button.back()->set_data("type",
					   new PlayerType(::Player::HUMAN));
	// Ai
	this->type_button.push_back(Gtk::manage(new Gtk::RadioButton(group,
								     "ai")));
	this->type_button.back()->set_data("type",
					   new PlayerType(::Player::AI));
#ifndef RELEASE
	// AiDummy
	this->type_button.push_back(Gtk::manage(new Gtk::RadioButton(group,
								     "dummy ai")));
	this->type_button.back()->set_data("type",
					   new PlayerType(::Player::AI_DUMMY));
	// AiRandom
	this->type_button.push_back(Gtk::manage(new Gtk::RadioButton(group,
								     "random ai")));
	this->type_button.back()->set_data("type",
					   new PlayerType(::Player::AI_RANDOM));
#endif
#ifdef USE_NETWORK
	// network
	this->type_button.push_back(Gtk::manage(new Gtk::RadioButton(group,
								     "network")));
	this->type_button.back()->set_data("type",
					   new PlayerType(::Player::NETWORK));
#endif

	for (vector<Gtk::RadioButton*>::iterator type = this->type_button.begin();
	     type != this->type_button.end();
	     type++)
	  this->ui->translations->add(**type,
				      ::translation(*static_cast<PlayerType*>((*type)->get_data("type"))));
      } // the player types

      { // signals
      //this->name_entry->signal_changed().connect(sigc::mem_fun(*this, &Player::name_change));
      this->name_entry->signal_focus_out_event().connect(sigc::mem_fun(*this, &Player::name_changed_event));
      this->random_name_button->signal_clicked().connect(sigc::mem_fun(*this, &Player::random_name));

      this->voice_button->signal_clicked().connect(sigc::mem_fun(*this->voice_menu, &Gtk::FileMenu::show));
      for (list<string>::const_iterator
           d = ::setting.data_directories().begin();
           d != ::setting.data_directories().end();
           ++d) {
        this->voice_menu->add_directory(*d + "/"
                                        + ::setting(Setting::SOUNDS_DIRECTORY));
      }

      for (vector<Gtk::RadioButton*>::iterator type = this->type_button.begin();
           type != this->type_button.end();
           type++)
        (*type)->signal_toggled().connect(sigc::bind<PlayerType const>(sigc::mem_fun(*this, &Player::type_change),
                                                                       *static_cast<PlayerType*>((*type)->get_data("type"))));

      this->reset_button->signal_clicked().connect(sigc::mem_fun(*this, &Player::reset));
      } // signals

      return ;
    } // void Players::Player::init()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the player
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  ::Player const&
    Players::Player::player() const
    {
      return this->ui->party().player(this->playerno());
    } // ::Player const& Players::Player::player() const

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the player
   **
   ** @version   0.6.6
   **
   ** @author    Diether Knof
   **
   **/
  ::Player&
    Players::Player::player()
    {
      return this->ui->party().player(this->playerno());
    } // ::Player& Players::Player::player()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the aiconfig
   **
   ** @version   0.6.6
   **
   ** @author    Diether Knof
   **
   **/
  Players::Player::AiConfig&
    Players::Player::aiconfig()
    {
      return *this->aiconfig_;
    } // Players::Player::AiConfig& Players::Player::aiconfig()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the playernumber
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  unsigned
    Players::Player::playerno() const
    {
      return this->playerno_;
    } // unsigned Players::Player::playerno() const

  /**
   **
   ** creates a backup
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Players::Player::create_backup()
    {
      delete this->player_bak;
      this->player_bak = this->player().clone();

      return ;
    } // void Players::Player::create_backup()

  /**
   **
   ** resets the settings
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Players::Player::reset()
    {
      if (this->player_bak)
        this->ui->party().set_player(this->player_bak->clone(), this->playerno());
      this->update();

      return ;
    } // void Players::Player::reset()

  /**
   ** the name has changed
   **
   ** @param     focus   ignored
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.3
   **/
  bool
    Players::Player::name_changed_event(GdkEventFocus* focus)
    {
      if (this->ui->party().player(this->playerno()).name()
          != this->name_entry->get_text())
        this->ui->party().set_playername(this->playerno(),
                                         this->name_entry->get_text());

      return true;
    } // bool Players::Player::name_changed_event(GdkEventFocus*)

  /**
   **
   ** create a random name
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Players::Player::random_name()
    {
      this->ui->party().set_playername(this->playerno(),
                                       ::generate_name(4 + RAND(4)));

      return ;
    } // void Players::Player::random_name()

  /**
   ** a voice is selected
   **
   ** @param     voice   the selected voice
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  void
    Players::Player::voice_selected(string const& voice)
    {
      this->ui->party().set_playervoice(this->playerno(), voice);

      return ;
    } // void Players::Player::voice_selected(string voice)

  /**
   ** the type has changed
   **
   ** @param     type   new type
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.4
   **/
  void
    Players::Player::type_change(PlayerType const type)
    {
      for (vector<Gtk::RadioButton*>::iterator
           typei = this->type_button.begin();
           typei != this->type_button.end();
           typei++) {
        if (*static_cast<PlayerType*>((*typei)->get_data("type"))
            == type) {
          if ((*typei)->get_active())
            this->ui->party().set_playertype(this->playerno(),
                                             type);
          break;
        }
      } // for (typei \in this->type_button)

      return ;
    } // void Players::Player::type_change(PlayerType const type)

  /**
   ** update the aiconfig
   **
   ** @param     aiconfig   aiconfig to update
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.8
   **/
  void
    Players::Player::aiconfig_update(::Aiconfig const& aiconfig)
    {
      this->aiconfig().update(aiconfig);
      return ;
    } // void Players::Player::aiconfig_update(::Aiconfig aiconfig)

  /**
   ** update the sensitivity of all widgets
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.7.2
   **
   ** @author    Diether Knof
   **/
  void
    Players::Player::sensitivity_update()
    {
      bool sensitive = (::game_status == GAMESTATUS::PARTY_NEW);
#ifdef USE_NETWORK
      if (::server
          && ::server->has_parent_connection())
        sensitive &= (this->player().type() != ::Player::NETWORK);
#endif

      this->name_entry->set_sensitive(this->player().type()
                                      != ::Player::NETWORK);
      this->random_name_button->set_sensitive(this->player().type()
                                              != ::Player::NETWORK);

      for (vector<Gtk::RadioButton*>::iterator t = this->type_button.begin();
           t != this->type_button.end();
           t++) {
        if (*static_cast<PlayerType*>((*t)->get_data("type"))
            == ::Player::NETWORK)
          (*t)->set_sensitive(false);
        else
          (*t)->set_sensitive(sensitive);
      } // for (t \in this->type_button)
      this->reset_button->set_sensitive(sensitive);

      this->aiconfig().sensitivity_update();

      return ;
    } // void Players::Player::sensitivity_update()

  /**
   **
   ** update all widgets
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Players::Player::update()
    {
      this->sensitivity_update();
      this->name_update();
      this->voice_update();
      this->language_update();
      for (vector<Gtk::RadioButton*>::iterator type = this->type_button.begin();
           type != this->type_button.end();
           type++)
        if (*static_cast<PlayerType*>((*type)->get_data("type"))
            == this->player().type()) {
          (*type)->set_active();
          break;
        }

      this->aiconfig().update();

      return ;
    } // void Players::Player::update()

  /**
   **
   ** update the name
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   ** @todo	block 'Entry::change_signal()'
   **
   **/
  void
    Players::Player::name_update()
    {
      // ToDo: block 'Entry::change_signal()'
      // Now, the entry is first cleared (that leads to an inserting of the 
      // default name), afterwards the new name is inserted.
      // So in the end the name is the old name followed by the default name.

      // workaround
      static bool in_change = false;
      if (!in_change) {
        in_change = true;
        this->name_entry->set_text(this->player().name());
        this->name_label->set_text(this->player().name());
        in_change = false;
      } // if (!in_change)
      this->aiconfig().name_update();

      return ;
    } // void Players::Player::name_update()

  /**
   ** update the voice
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  void
    Players::Player::voice_update()
    {
      this->voice_button->set_label(this->player().voice());
      return ;
    } // void Players::Player::voice_update()

  /**
   ** update the language
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  void
    Players::Player::language_update()
    {
      this->voice_menu->clear_directories();
      for (list<string>::const_iterator
           d = ::setting.data_directories().begin();
           d != ::setting.data_directories().end();
           ++d) {
        this->voice_menu->add_directory(*d + "/"
                                        + ::setting(Setting::SOUNDS_DIRECTORY));
      }
      return ;
    } // void Players::Player::language_update()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
