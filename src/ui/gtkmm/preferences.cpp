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

#include "preferences.h"
#include "ui.h"
#include "translations.h"
#include "cards.h"
#include "main_window.h"
#include "menu.h"
#include "utils.h"

#include "../../misc/setting.h"
#ifdef WINDOWS
#include "../../utils/string.h"
#endif

#include "widgets/stock_button.h"
#include "widgets/labelspinbutton.h"
#include "widgets/filemenu_extension.h"
#include "widgets/filemenu_file.h"
#include <gtkmm/stock.h>
#include <gtkmm/notebook.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/alignment.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/fontselection.h>
#include <gtkmm/colorselection.h>
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
  Preferences::Preferences(Base* const parent) :
    Base(parent),
    StickyDialog("Preferences", false),
    reset_button(NULL),
    close_button(NULL),
    group_notebook(NULL),
    type_bool(),
    type_unsigned(),
    type_string(),
    type_string_label(),
    sorting_tcolor_image(),
    sorting_up_button(),
    sorting_down_button(),
    sorted_button(NULL),
    unsorted_button(NULL),
    mix_cards_button(NULL),
    language_menu(NULL),
    background_menu(NULL),
    cardset_menu(NULL),
    cards_back_menu(NULL),
    font_selector(),
    color_selector(),
#ifndef RELEASE
    update_translation_button(NULL),
#endif
    backup_(NULL)
    {
      this->ui->add_window(*this);

#ifdef WORKAROUND
      { // name: change to utf8
        Glib::ustring const name
          = UI_GTKMM::to_utf8(::setting(Setting::NAME));
        ::setting.set(Setting::NAME, name);	
      } // name: change to utf8
#endif

      this->group_notebook = Gtk::manage(new Gtk::Notebook());
      this->group_notebook->set_tab_pos(Gtk::POS_LEFT);

      this->create_backup();

      this->signal_realize().connect(sigc::mem_fun(*this, &Preferences::init));
      this->signal_show().connect(sigc::mem_fun(*this, &Preferences::update_all));

#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
    this->signal_key_press_event().connect(sigc::mem_fun(*this, &Preferences::on_key_press_event));
#endif

      return ;
    } // Preferences::Preferences(Base* const parent)

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
  Preferences::~Preferences()
  {
    for (vector<Gtk::CheckButton*>::iterator widget = this->type_bool.begin();
         widget != this->type_bool.end();
         widget++)
      delete static_cast<Setting::TypeBool*>((*widget)->steal_data("type"));

    delete this->language_menu;
    for (vector<Gtk::FontSelectionDialog*>::iterator
         selector = this->font_selector.begin();
         selector != this->font_selector.end();
         selector++) {
      delete static_cast<int*>((*selector)->steal_data("type"));
      delete *selector;
    } // for (s \in this->font_selector)
    for (vector<Gtk::ColorSelectionDialog*>::iterator
         selector = this->color_selector.begin();
         selector != this->color_selector.end();
         selector++) {
      delete static_cast<int*>((*selector)->steal_data("type"));
      delete *selector;
    } // for (s \in this->color_selector)

    delete this->backup_;

    return ;
  } // Preferences::~Preferences()

  /**
   **
   ** creates all subelements
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
    Preferences::init()
    {
      this->ui->translations->add(*this, ::translation("FreeDoko %ttitle%",
                                                       ::translation("~preferences")));
      this->set_icon(this->ui->icon);

      this->group_notebook->signal_switch_page().connect(sigc::mem_fun(*this, &Preferences::group_notebook_page_changed));

      this->reset_button
        = Gtk::manage(new Gtk::StockButton(Gtk::Stock::UNDO, "reset"));
      this->add_action_widget(*(this->reset_button), Gtk::RESPONSE_REJECT);
      this->reset_button->show_all();
      this->close_button
        = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
      this->add_action_widget(*(this->close_button), Gtk::RESPONSE_CLOSE);
      this->close_button->show_all();

      this->language_menu = new Gtk::Menu();
#ifdef POSTPONED
      this->ui->translations->add(*(this->language_menu),
                                  ::translation("language"));
#endif
      this->background_menu
        = Gtk::manage(new Gtk::FileMenuFilterExtension(sigc::mem_fun(*this, &Preferences::background_selected),
                                                       ::setting(Setting::GRAPHIC_EXTENSION)));
      this->cardset_menu
        = Gtk::manage(new Gtk::FileMenuFilterFile(sigc::mem_fun(*this, &Preferences::cardset_selected), ::setting.value(Setting::CARDS_DIRECTORY) + "/" + ::name(Card::CLUB) + "/" + ::name(Card::QUEEN) + "." + ::setting(Setting::GRAPHIC_EXTENSION)));
      this->cards_back_menu
        = Gtk::manage(new Gtk::FileMenuFilterExtension(sigc::mem_fun(*this, &Preferences::cards_back_selected),
                                                       ::setting(Setting::GRAPHIC_EXTENSION)));

      this->close_button->grab_default();

      this->get_vbox()->add(*(this->group_notebook));

      this->ui->translations->add(*(this->reset_button->label),
                                  ::translation("reset"));
      this->ui->translations->add(*(this->close_button->label),
                                  ::translation("close"));

#ifndef RELEASE
      this->update_translation_button
        = Gtk::manage(new Gtk::Button("update"));
      this->ui->translations->add(*this->update_translation_button,
                                  ::translation("update"));
      this->update_translation_button->signal_clicked().connect(sigc::mem_fun(*this, &Preferences::update_translation));
#endif

      { // create the buttons
        for (int t = Setting::BOOL_FIRST; t <= Setting::BOOL_LAST; t++) {
          this->type_bool.push_back(Gtk::manage(new Gtk::CheckButton(::name(Setting::TypeBool(t)))));
          this->type_bool.back()->set_data("type", new Setting::TypeBool(Setting::TypeBool(t)));
          this->ui->translations->add(*(this->type_bool.back()),
                                      ::translation(Setting::TypeBool(t)));
          this->type_bool.back()->signal_toggled().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Preferences::change),
                                                                                 t)
                                                          );
        } // for (t \in Setting::BOOL)
        for (int t = Setting::UNSIGNED_FIRST;
             t <= Setting::UNSIGNED_LAST;
             t++) {
          this->type_unsigned.push_back(Gtk::manage(new Gtk::LabelSpinButton(::name(Setting::TypeUnsigned(t)))));
          this->type_unsigned.back()->set_data("type", new Setting::TypeUnsigned(Setting::TypeUnsigned(t)));
          this->ui->translations->add(this->type_unsigned.back()->get_label(),
                                      ::translation("%ttranslation%: ",
                                                    ::translation(Setting::TypeUnsigned(t))));
          this->type_unsigned.back()->signal_value_changed().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Preferences::change),
                                                                                           t)
                                                                    );
          switch (static_cast<Setting::TypeUnsigned const>(t)) {
          case Setting::CARD_PLAY_DELAY:
          case Setting::FULL_TRICK_CLOSE_DELAY:
          case Setting::GAMETYPE_WINDOW_CLOSE_DELAY:
          case Setting::MARRIAGE_WINDOW_CLOSE_DELAY:
          case Setting::GENSCHER_WINDOW_CLOSE_DELAY:
          case Setting::ANNOUNCEMENT_WINDOW_CLOSE_DELAY:
          case Setting::SWINES_WINDOW_CLOSE_DELAY:
            // make the precision 1/1000 seconds
            this->type_unsigned.back()->get_spin_button().set_digits(1);
            this->type_unsigned.back()->get_spin_button().set_increments(0.1, 1);
            break;
#ifdef USE_THREADS
          case Setting::THREADS_MAX:
#endif
          case Setting::TABLE_ROTATION:
          case Setting::CARDS_HEIGHT:
            break;
          } // switch(t)
        } // for (t \in Setting::UNSIGNED)
        for (int t = Setting::STRING_FIRST; t <= Setting::STRING_LAST; t++) {
          this->type_string_label.push_back(Gtk::manage(new Gtk::Label("type")));

          this->type_string_label.back()->set_data("type", new Setting::TypeString(Setting::TypeString(t)));

          this->ui->translations->add(*(this->type_string_label.back()),
                                      ::translation(Setting::TypeString(Setting::TypeString(t))) + ":");

          switch (t) {
          case Setting::NAME:
            this->type_string.push_back(Gtk::manage(new Gtk::Entry()));
            dynamic_cast<Gtk::Entry*>(this->type_string.back())->signal_focus_out_event().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Preferences::focus_out_event), t));
            break;

          case Setting::LANGUAGE:
            this->type_string.push_back(Gtk::manage(new Gtk::Button("type")));
            dynamic_cast<Gtk::Button*>(this->type_string.back())->signal_clicked().connect(sigc::mem_fun(*(this->language_menu), &Widget::show_all));
            //	  dynamic_cast<Gtk::Button*>(this->type_string.back())->signal_clicked().connect(sigc::mem_fun(*this, &Preferences::language_menu_create));
            dynamic_cast<Gtk::Button*>(this->type_string.back())->signal_clicked().connect(sigc::bind<bool>(sigc::mem_fun(*this, &Preferences::language_menu_create), true));
            // a direct signal for popping up the menu does not work
            //dynamic_cast<Gtk::Button*>(this->type_string.back())->signal_clicked().connect(sigc::bind<guint, guint32>(sigc::mem_fun(*(this->language_menu), &Gtk::Menu::popup), 0, 0));
            break;

          case Setting::CARDSET:
            this->type_string.push_back(Gtk::manage(new Gtk::Button("type")));
            dynamic_cast<Gtk::Button*>(this->type_string.back())->signal_clicked().connect(sigc::mem_fun(*(this->cardset_menu), &Gtk::FileMenu::show));
            for (list<string>::const_iterator
                 d = ::setting.data_directories().begin();
                 d != ::setting.data_directories().end();
                 ++d) {
              this->cardset_menu->add_directory(*d + "/"
                                                + ::setting(Setting::CARDSETS_DIRECTORY));
            }
            break;

          case Setting::CARDS_BACK:
            this->type_string.push_back(Gtk::manage(new Gtk::Button("type")));
            dynamic_cast<Gtk::Button*>(this->type_string.back())->signal_clicked().connect(sigc::mem_fun(*(this->cards_back_menu), &Gtk::FileMenu::show));
            for (list<string>::const_iterator
                 d = ::setting.data_directories().begin();
                 d != ::setting.data_directories().end();
                 ++d)
              this->cards_back_menu->add_directory(*d + "/"
                                                   + ::setting(Setting::CARDS_BACK_DIRECTORY));
            break;

          case Setting::BACKGROUND:
            this->type_string.push_back(Gtk::manage(new Gtk::Button("type")));
            dynamic_cast<Gtk::Button*>(this->type_string.back())->signal_clicked().connect(sigc::mem_fun(*(this->background_menu), &Gtk::FileMenu::show));
            for (list<string>::const_iterator
                 d = ::setting.data_directories().begin();
                 d != ::setting.data_directories().end();
                 ++d)
              this->background_menu->add_directory(*d + "/"
                                                   + ::setting(Setting::BACKGROUNDS_DIRECTORY));
            break;

          case Setting::NAME_FONT:
          case Setting::TRICKPILE_POINTS_FONT:
            this->type_string.push_back(Gtk::manage(new Gtk::StockButton(Gtk::Stock::SELECT_FONT, "type", false)));
            this->font_selector.push_back(new Gtk::FontSelectionDialog("font selector"));
            this->font_selector.back()->set_data("type", new int(t));
            this->type_string.back()->set_data("font selector", this->font_selector.back());
            this->ui->translations->add(*(this->font_selector.back()),
                                        ::translation(Setting::TypeString(t)));
            dynamic_cast<Gtk::Button*>(this->type_string.back())->signal_clicked().connect(sigc::mem_fun0(*(this->font_selector.back()), &Gtk::Window::present));
            break;

          case Setting::NAME_FONT_COLOR:
          case Setting::NAME_ACTIVE_FONT_COLOR:
          case Setting::NAME_RESERVATION_FONT_COLOR:
          case Setting::TRICKPILE_POINTS_FONT_COLOR:
          case Setting::POVERTY_SHIFT_ARROW_COLOR:
            this->type_string.push_back(Gtk::manage(new Gtk::StockButton(Gtk::Stock::SELECT_COLOR, "type", false)));
            this->color_selector.push_back(new Gtk::ColorSelectionDialog("color selector"));
            this->color_selector.back()->set_data("type", new int(t));
            this->type_string.back()->set_data("color selector", this->color_selector.back());
            this->ui->translations->add(*(this->color_selector.back()),
                                        ::translation(Setting::TypeString(t)));
            dynamic_cast<Gtk::StockButton*>(this->type_string.back())->signal_clicked().connect(sigc::mem_fun0(*(this->color_selector.back()), &Gtk::Window::present));
            break;

#ifdef PLAY_SOUND_COMMAND
          case Setting::PLAY_SOUND_COMMAND:
#endif
          case Setting::BROWSER_COMMAND:
            this->type_string.push_back(Gtk::manage(new Gtk::Entry()));
            dynamic_cast<Gtk::Entry*>(this->type_string.back())->signal_focus_out_event().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Preferences::focus_out_event), t));
            break;

          } // switch(t)
          this->type_string.back()->set_data("type", new Setting::TypeString(Setting::TypeString(t)));
          if ( (t != Setting::CARDSET)
              && (t != Setting::CARDS_BACK)
              && (t != Setting::BACKGROUND) ) {
            if (dynamic_cast<Gtk::StockButton*>(this->type_string.back()) != NULL)
              this->ui->translations->add(*(dynamic_cast<Gtk::StockButton*>(this->type_string.back())),
                                          ::translation(Setting::TypeString(t)));
            else if (dynamic_cast<Gtk::Button*>(this->type_string.back()) != NULL)
              this->ui->translations->add(*(dynamic_cast<Gtk::Button*>(this->type_string.back())),
                                          ::translation(Setting::TypeString(t)));
          } // if (t != CARDSET | CARDS_BACK | BACKGROUND)
        } // for (t \in Setting::STRING)
      } // create the buttons

      { // set the pages of the notebook
#define ADD_ALIGNMENT \
        Gtk::Alignment* alignment \
        = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, \
                                         Gtk::ALIGN_CENTER, \
                                         0, 0)); \
        vbox->add(*alignment)
#define ADD_ALIGNMENT_HBOX \
        ADD_ALIGNMENT; \
        Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 1 ex)); \
        alignment->add(*hbox)
#define ADD_ALIGNMENT_VBOX2 \
        ADD_ALIGNMENT; \
        Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 0)); \
        alignment->add(*vbox2)

        { // General
          Gtk::VBox* vbox = this->add_group_vbox("general");

          { // name
            ADD_ALIGNMENT_HBOX;
            hbox->add(*(this->type_string_label[Setting::NAME
                        - Setting::STRING_FIRST]));
            hbox->add(*(this->type_string[Setting::NAME
                        - Setting::STRING_FIRST]));
          } // name
          { // language
            ADD_ALIGNMENT_HBOX;
            hbox->add(*(this->type_string_label[Setting::LANGUAGE
                        - Setting::STRING_FIRST]));
            hbox->add(*(this->type_string[Setting::LANGUAGE
                        - Setting::STRING_FIRST]));
#ifndef RELEASE
            hbox->add(*this->update_translation_button);
#endif
          } // language
          { // sound
            ADD_ALIGNMENT;
            alignment->add(*(this->type_bool[Setting::SOUND
                             - Setting::BOOL_FIRST]));
          } // sound
#ifdef USE_THREADS
          { // threads
            ADD_ALIGNMENT;
            alignment->add(*(this->type_unsigned[Setting::THREADS_MAX
                             - Setting::UNSIGNED_FIRST]));
          } // threads
#endif
          { // show bug report button in game finished window
            ADD_ALIGNMENT;
            alignment->add(*(this->type_bool[Setting::SHOW_BUG_REPORT_BUTTON_IN_GAME_FINISHED_WINDOW
                             - Setting::BOOL_FIRST]));
          } // show bug report button in game finished window
          { // save bug reports on desktop
            ADD_ALIGNMENT;
            alignment->add(*(this->type_bool[Setting::SAVE_BUG_REPORTS_ON_DESKTOP
                             - Setting::BOOL_FIRST]));
          } // save bug reports on desktop
          { // save party changes
            ADD_ALIGNMENT;
            alignment->add(*(this->type_bool[Setting::SAVE_PARTY_CHANGES
                             - Setting::BOOL_FIRST]));
          } // save party changes
#ifdef USE_SOUND_COMMAND
          { // play sound command
            ADD_ALIGNMENT_HBOX;
            hbox->add(*(this->type_string_label[Setting::PLAY_SOUND_COMMAND
                        - Setting::STRING_FIRST]));
            hbox->add(*(this->type_string[Setting::PLAY_SOUND_COMMAND
                        - Setting::STRING_FIRST]));
          } // play sound command
#endif
          { // browser
            ADD_ALIGNMENT_HBOX;
            hbox->add(*(this->type_string_label[Setting::BROWSER_COMMAND
                        - Setting::STRING_FIRST]));
            hbox->add(*(this->type_string[Setting::BROWSER_COMMAND
                        - Setting::STRING_FIRST]));
          } // browser
        } // General
        { // Behaviour
          Gtk::VBox* vbox = this->add_group_vbox("behaviour");

          vbox->add(*(this->type_bool[Setting::ANNOUNCE_IN_TABLE
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_unsigned[Setting::CARD_PLAY_DELAY
                      - Setting::UNSIGNED_FIRST]));
        } // Behaviour
        { // Assistance
          Gtk::VBox* vbox = this->add_group_vbox("assistance");

          vbox->add(*(this->type_bool[Setting::SHOW_SOLOPLAYER_IN_GAME
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_bool[Setting::ANNOUNCE_SWINES_AUTOMATICALLY
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_bool[Setting::SHOW_IF_VALID
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_bool[Setting::EMPHASIZE_VALID_CARDS
                      - Setting::BOOL_FIRST]));
        } // Assistance
        { // Help
          Gtk::VBox* vbox = this->add_group_vbox("help");

          vbox->add(*(this->type_bool[Setting::SHOW_ALL_HANDS
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_bool[Setting::SHOW_AI_INFORMATION_HANDS
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_bool[Setting::SHOW_KNOWN_TEAMS_IN_GAME
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_bool[Setting::SHOW_AI_INFORMATION_TEAMS
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_bool[Setting::SHOW_TRICKPILES_POINTS
                      - Setting::BOOL_FIRST]));
          vbox->add(*(this->type_bool[Setting::AUTOMATIC_CARD_SUGGESTION
                      - Setting::BOOL_FIRST]));
        } // Help
        { // information windows
          Gtk::Notebook* subnotebook
            = this->add_group_notebook("information windows");

          { // gametype window
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "gametype");

            vbox->add(*(this->type_bool[Setting::SHOW_GAMETYPE_WINDOW
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::CLOSE_GAMETYPE_WINDOW_AUTOMATICALLY
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_unsigned[Setting::GAMETYPE_WINDOW_CLOSE_DELAY
                        - Setting::UNSIGNED_FIRST]));
          } // gametype window
          { // full trick window
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "full trick");

            vbox->add(*(this->type_bool[Setting::SHOW_FULL_TRICK_WINDOW
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::SHOW_FULL_TRICK_WINDOW_IF_SPECIAL_POINTS
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::CLOSE_FULL_TRICK_AUTOMATICALLY
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_unsigned[Setting::FULL_TRICK_CLOSE_DELAY
                        - Setting::UNSIGNED_FIRST]));
          } // full trick window
          { // marriage finding window
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "marriage");

            vbox->add(*(this->type_bool[Setting::SHOW_MARRIAGE_WINDOW
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::CLOSE_MARRIAGE_WINDOW_AUTOMATICALLY
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_unsigned[Setting::MARRIAGE_WINDOW_CLOSE_DELAY
                        - Setting::UNSIGNED_FIRST]));
          } // marriage finding window
          { // genscher window
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "genscher");

            vbox->add(*(this->type_bool[Setting::SHOW_GENSCHER_WINDOW
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::CLOSE_GENSCHER_WINDOW_AUTOMATICALLY
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_unsigned[Setting::GENSCHER_WINDOW_CLOSE_DELAY
                        - Setting::UNSIGNED_FIRST]));
          } // genscher window
          { // announcement window
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "announcement");

            vbox->add(*(this->type_bool[Setting::SHOW_ANNOUNCEMENT_WINDOW
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::CLOSE_ANNOUNCEMENT_WINDOW_AUTOMATICALLY
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_unsigned[Setting::ANNOUNCEMENT_WINDOW_CLOSE_DELAY
                        - Setting::UNSIGNED_FIRST]));
          } // announcement window
          { // swines window
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "swines");

            vbox->add(*(this->type_bool[Setting::SHOW_SWINES_WINDOW
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::CLOSE_SWINES_WINDOW_AUTOMATICALLY
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_unsigned[Setting::SWINES_WINDOW_CLOSE_DELAY
                        - Setting::UNSIGNED_FIRST]));
          } // swines window
          { // network window
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "network");

            vbox->add(*(this->type_bool[Setting::NETWORK_CONNECTIONS_URGENCY_HINT
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::NETWORK_CHAT_URGENCY_HINT
                        - Setting::BOOL_FIRST]));
          } // network window
        } // Information windows
        this->init_cardsorder();
        { // Appearance
          Gtk::Notebook* subnotebook
            = this->add_group_notebook("appearance");

          { // table
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "table");

            vbox->add(*(this->type_bool[Setting::OWN_HAND_ON_TABLE_BOTTOM
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_unsigned[Setting::TABLE_ROTATION
                        - Setting::UNSIGNED_FIRST]));
            vbox->add(*(this->type_bool[Setting::ROTATE_TRICK_CARDS
                        - Setting::BOOL_FIRST]));
          } // table
          { // Cards
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "cards");

            vbox->add(*(this->type_bool[Setting::ORIGINAL_CARDS_SIZE
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_unsigned[Setting::CARDS_HEIGHT
                        - Setting::UNSIGNED_FIRST]));
          } // Cards
          { // Theme
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "theme");

            { // cardset
              Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 1 ex));
              vbox->add(*hbox);

              hbox->pack_start(*(this->type_string_label[Setting::CARDSET
                                 - Setting::STRING_FIRST]), false, true);
              hbox->pack_end(*(this->type_string[Setting::CARDSET
                               - Setting::STRING_FIRST]), true, true);
            } // cardset
            { // cards back
              Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 1 ex));
              vbox->add(*hbox);

              hbox->pack_start(*(this->type_string_label[Setting::CARDS_BACK
                                 - Setting::STRING_FIRST]), false, true);
              hbox->pack_end(*(this->type_string[Setting::CARDS_BACK
                               - Setting::STRING_FIRST]), true, true);
            } // cards back
            { // background
              Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 1 ex));
              vbox->add(*hbox);

              hbox->pack_start(*(this->type_string_label[Setting::BACKGROUND
                                 - Setting::STRING_FIRST]), false, true);
              hbox->pack_end(*(this->type_string[Setting::BACKGROUND
                               - Setting::STRING_FIRST]), true, true);
            } // background
          } // Theme
          { // Fonts and colors
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "fonts and colors");

            { // the name
              ADD_ALIGNMENT_VBOX2;

              vbox2->add(*(this->type_string[Setting::NAME_FONT
                           - Setting::STRING_FIRST]));
              vbox2->add(*(this->type_string[Setting::NAME_FONT_COLOR
                           - Setting::STRING_FIRST]));
              vbox2->add(*(this->type_string[Setting::NAME_ACTIVE_FONT_COLOR
                           - Setting::STRING_FIRST]));
              vbox2->add(*(this->type_string[Setting::NAME_RESERVATION_FONT_COLOR
                           - Setting::STRING_FIRST]));
            } // the name

            { // the trickpile points
              ADD_ALIGNMENT_VBOX2;

              vbox2->add(*(this->type_string[Setting::TRICKPILE_POINTS_FONT
                           - Setting::STRING_FIRST]));
              vbox2->add(*(this->type_string[Setting::TRICKPILE_POINTS_FONT_COLOR
                           - Setting::STRING_FIRST]));

            } // the trickpile points

            { // poverty arrow
              ADD_ALIGNMENT;

              alignment->add(*(this->type_string[Setting::POVERTY_SHIFT_ARROW_COLOR
                               - Setting::STRING_FIRST]));
            } // poverty arrow
          } // Fonts and colors
          { // Splash screen
            Gtk::VBox* vbox = this->add_subgroup_vbox(*subnotebook,
                                                      "splash screen");

            vbox->add(*(this->type_bool[Setting::SHOW_SPLASH_SCREEN
                        - Setting::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Setting::SPLASH_SCREEN_TRANSPARENT
                        - Setting::BOOL_FIRST]));
          } // Splash screen
        } // Appearance

#undef ADD_ALIGNMENT
#undef ADD_ALIGNMENT_HBOX
#undef ADD_ALIGNMENT_VBOX2
      } // set the pages of the notebook

#ifndef RELEASE
#ifdef DKNOF
      { // test, whether all settings-buttons are packed in a container
        for (int t = Setting::BOOL_FIRST; t <= Setting::BOOL_LAST; t++) {
          if (!this->type_bool[t - Setting::BOOL_FIRST]->get_parent()
              && (t != Setting::AUTOMATIC_SAVINGS) )
            cerr << "GTKMM_DOKO::Setting::Setting():\n"
                                                     << "  setting '" << name(Setting::TypeBool(t)) << "' not packed\n";
        }
        for (int t = Setting::UNSIGNED_FIRST; t <= Setting::UNSIGNED_LAST; t++) {
          if (!this->type_unsigned[t - Setting::UNSIGNED_FIRST]->get_parent())
            cerr << "GTKMM_DOKO::Setting::Setting():\n"
                                                     << "  setting '" << name(Setting::TypeUnsigned(t))
                                                                                                       << "' not packed\n";
        }
#ifdef POSTPONED
        for (int t = Setting::STRING_FIRST; t <= Setting::STRING_LAST; t++) {
          if (!this->type_string[t - Setting::STRING_FIRST]->get_parent())
            cerr << "GTKMM_DOKO::Setting::Setting():\n"
                                                     << "  setting '" << name(Setting::TypeString(t))
                                                                                                     << "' not packed\n";
        }
#endif
      } // test, whether all setting-buttons are packed in a container
#endif
#endif

      { // signals
        this->reset_button->signal_clicked().connect(sigc::mem_fun(*this,
                                                                   &Preferences::reset)
                                                    );
        this->close_button->signal_clicked().connect(sigc::mem_fun(*this,
                                                                   &Gtk::Widget::hide)
                                                    );
        this->close_button->signal_clicked().connect(sigc::mem_fun(*this,
                                                                   &Preferences::save)
                                                    );


        for (vector<Gtk::FontSelectionDialog*>::iterator
             selector = this->font_selector.begin();
             selector != this->font_selector.end();
             selector++) {
          (*selector)->get_ok_button()->signal_clicked().connect(sigc::bind<Gtk::FontSelectionDialog* const>(sigc::mem_fun(*this, &Preferences::font_accepted), *selector));
          (*selector)->get_cancel_button()->signal_clicked().connect(sigc::bind<Gtk::FontSelectionDialog* const>(sigc::mem_fun(*this, &Preferences::font_declined), *selector));
        } // for (selector \in this->font_selector)

        for (vector<Gtk::ColorSelectionDialog*>::iterator
             selector = this->color_selector.begin();
             selector != this->color_selector.end();
             selector++) {
          Setting::TypeString const type
            = *(static_cast<Setting::TypeString*>((*selector)->get_data("type")));

          (*selector)->get_ok_button()->signal_clicked().connect(sigc::bind<Gtk::ColorSelectionDialog* const>(sigc::mem_fun(*this, &Preferences::color_accepted), *selector));
          (*selector)->get_cancel_button()->signal_clicked().connect(sigc::bind<Gtk::ColorSelectionDialog* const>(sigc::mem_fun(*this, &Preferences::color_declined), *selector));
          (*selector)->get_colorsel()->signal_color_changed().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Preferences::change), type));
        } // for (selector \in this->color_selector)
      } // signals

      this->update_all();

      this->show_all_children();

#ifdef RELEASE
#ifndef WINDOWS
#ifndef LINUX
      this->type_bool[Setting::SAVE_BUG_REPORTS_ON_DESKTOP - Setting::BOOL_FIRST]->hide();
#endif
#endif
#endif // #ifdef RELEASE

      return ;
    } // void Preferences::init()

  /**
   ** adds to the group notebook a page with a vbox for group 'name'.
   **
   ** @param     name   name of the group
   **
   ** @return    created vbox for the settings of the group
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  Gtk::VBox*
    Preferences::add_group_vbox(string const& name)
    {
      Gtk::Alignment* alignment
        = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
                                         0, 0));
      this->add_group(name, *alignment);

      Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex));
      alignment->add(*vbox);

      return vbox;
    } // Gtk::VBox* Preferences::add_group_vbox(string const& name)

  /**
   ** adds to the group notebook a subnotebook for group 'name'.
   **
   ** @param     name   name of the group
   **
   ** @return    created notebook for the subgroups
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  Gtk::Notebook*
    Preferences::add_group_notebook(string const& name)
    {
      Gtk::Notebook* notebook = Gtk::manage(new Gtk::Notebook());
      this->add_group(name, *notebook);

      return notebook;
    } // Gtk::Notebook* Preferences::add_group_notebook(string name)

  /**
   ** adds to the group notebook a page of 'widget' for group 'name'.
   **
   ** @param     name     name of the group
   ** @param     widget   widget contianing the group
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  void
    Preferences::add_group(string const& name, Gtk::Widget& widget)
    {
      widget.show();
      widget.set_data("group name",
                      new Translator::Translation(::translation("Setting::Group::" + name)));

      Gtk::Label* label = Gtk::manage(new Gtk::Label(name));
      label->show();
      this->ui->translations->add(*label, ::translation("Setting::Group::" + name));

      this->group_notebook->append_page(widget, *label);
      this->group_notebook->set_tab_label_packing(widget, true, true, Gtk::PACK_START);

      return ;
    } // void Preferences::add_group(string name, Gtk::Widget& widget)

  /**
   ** adds to the subgroup notebook a page with a vbox for group 'name'.
   **
   ** @param     subnotebook   notebook to add the group
   ** @param     name   	name of the group
   **
   ** @return    created vbox for the settings of the group
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  Gtk::VBox*
    Preferences::add_subgroup_vbox(Gtk::Notebook& subnotebook,
                                   string const& name)
    {
      Gtk::Alignment* alignment
        = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
                                         0, 0));
      alignment->show();
      alignment->set_data("group name",
                          new Translator::Translation(::translation("Setting::Group::" + name)));

      Gtk::Label* label = Gtk::manage(new Gtk::Label(name));
      label->show();
      this->ui->translations->add(*label, ::translation("Setting::Group::" + name));

      subnotebook.append_page(*alignment, *label);
      subnotebook.set_tab_label_packing(*alignment, true, true,
                                        Gtk::PACK_START);


      Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex));
      alignment->add(*vbox);

      return vbox;
    } // Gtk::VBox* Preferences::add_subgroup_vbox(Gtk::Notebook& subnotebook, string name)

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
    Preferences::create_backup()
    {
      delete this->backup_;
      this->backup_ = new Setting(::setting);

      return ;
    } // void Preferences::create_backup()

  /**
   **
   ** -> result
   **
   ** @param     -
   **
   ** @return    the backup
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  Setting const&
    Preferences::backup() const
    {
      DEBUG_ASSERTION((this->backup_ != NULL),
                      "Setting::backup():\n"
                      "  'this->backup_' == NULL");

      return *this->backup_;
    } // Setting const& Preferences::backup() const

  /**
   **
   ** save the settings
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
    Preferences::save()
    {
    if (::setting(Setting::AUTOMATIC_SAVINGS))
      ::setting.save();

      return ;
    } // void Preferences::save()

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
    Preferences::reset()
    {
      ::setting = this->backup();

      return ;
    } // void Preferences::reset()

  /**
   **
   ** update the sensitivity of all widgets
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
    Preferences::sensitivity_update()
    {
      for (vector<Gtk::CheckButton*>::iterator widget = this->type_bool.begin();
           widget != this->type_bool.end();
           widget++)
        (*widget)->set_sensitive(::setting.dependencies(*(static_cast<Setting::TypeBool*>((*widget)->get_data("type")))));
      for (vector<Gtk::LabelSpinButton*>::iterator widget
           = this->type_unsigned.begin();
           widget != this->type_unsigned.end();
           widget++) {
        Setting::TypeUnsigned const type 
          = *static_cast<Setting::TypeUnsigned*>((*widget)->get_data("type"));

        (*widget)->set_sensitive(::setting.dependencies(type));
        if (type == Setting::CARDS_HEIGHT) {
          // make such a limit, so that a right-click (maximum) does not lead
          // to a limit problem
          (*widget)->set_range(::setting.min(type),
                               10 * this->ui->cards->height_original());
        } else {
          (*widget)->set_range(::setting.min(type),
                               ::setting.max(type));
        }
      } // for (widget \in this->type_unsigned)

      return ;
    } // void Preferences::sensitivity_update()

  /**
   **
   ** update all
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
    Preferences::update_all()
    {
      for (int t = Setting::BOOL_FIRST; t <= Setting::BOOL_LAST; t++)
        this->update(t, false);
      for (int t = Setting::UNSIGNED_FIRST; t <= Setting::UNSIGNED_LAST; t++)
        this->update(t, false);
      for (int t = Setting::STRING_FIRST; t <= Setting::STRING_LAST; t++)
        this->update(t, false);
      this->update(Setting::CARDS_ORDER, false);

      this->sensitivity_update();

      this->reset_button->set_sensitive(this->backup() != ::setting);

      return ;
    } // void Preferences::update_all()

  /**
   **
   ** update the setting 'type'
   **
   ** @param     type   		the type of the setting
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::setting_update(int const type)
    {
      this->update(type);
      this->sensitivity_update();

      return ;
    } // void Preferences::setting_update(int const type)

  /**
   **
   ** update the setting 'type'
   **
   ** @param     type   		the type of the setting
   ** @param     update_sensitivity   whether the sensitivity shall be updated
   ** 					(default: true)
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   ** @todo	style of the font and color buttons
   **
   **/
  void
    Preferences::update(int const type, bool const update_sensitivity)
    {
      if (!this->is_visible())
        return ;

      if (update_sensitivity)
        this->sensitivity_update();

      if ((type >= Setting::BOOL_FIRST)
          && (type <= Setting::BOOL_LAST)) {
        this->type_bool[type - Setting::BOOL_FIRST]->set_active(::setting.value(Setting::TypeBool(type)));
        switch (static_cast<Setting::TypeBool const>(type)) {
        case Setting::EMPHASIZE_VALID_CARDS:
          this->ui->main_window->menu->emphasize_valid_cards->set_active(::setting.value(Setting::TypeBool(type)));
          break;
        case Setting::SHOW_ALL_HANDS:
          this->ui->main_window->menu->show_all_hands->set_active(::setting.value(Setting::TypeBool(type)));
          break;
        default:
          break;
        } // switch(type)
      } else if ((type >= Setting::UNSIGNED_FIRST)
                 && (type <= Setting::UNSIGNED_LAST)) {
        switch (static_cast<Setting::TypeUnsigned const>(type)) {
        case Setting::CARD_PLAY_DELAY:
        case Setting::FULL_TRICK_CLOSE_DELAY:
        case Setting::GAMETYPE_WINDOW_CLOSE_DELAY:
        case Setting::MARRIAGE_WINDOW_CLOSE_DELAY:
        case Setting::GENSCHER_WINDOW_CLOSE_DELAY:
        case Setting::ANNOUNCEMENT_WINDOW_CLOSE_DELAY:
        case Setting::SWINES_WINDOW_CLOSE_DELAY:
          // make one second to correspond with '1.0'
          this->type_unsigned[type - Setting::UNSIGNED_FIRST]->set_value((1.0 / 1000) * ::setting.value(static_cast<Setting::TypeUnsigned>(type)));
          break;
#ifdef USE_THREADS
        case Setting::THREADS_MAX:
#endif
        case Setting::TABLE_ROTATION:
        case Setting::CARDS_HEIGHT:
          this->type_unsigned[type - Setting::UNSIGNED_FIRST]->set_value(::setting.value(static_cast<Setting::TypeUnsigned>(type)));
          break;
        } // switch(type)
      } else if ((type >= Setting::STRING_FIRST)
                 && (type <= Setting::STRING_LAST)) {
        switch(type) {
        case Setting::NAME:
          dynamic_cast<Gtk::Entry*>(this->type_string[type - Setting::STRING_FIRST])->set_text(::setting(Setting::TypeString(type)));
          break;
        case Setting::LANGUAGE:
          this->language_menu_create(false);
          if (::setting.value(Setting::TypeString(type)) == "")
            this->ui->translations->change(*(dynamic_cast<Gtk::Button*>(this->type_string[type - Setting::STRING_FIRST])),
                                           ::translation("default"));
          else
            this->ui->translations->change(*(dynamic_cast<Gtk::Button*>(this->type_string[type - Setting::STRING_FIRST])),
                                           ::translation("%stext%",
                                                         ::translator.name()));
          break;
        case Setting::CARDSET:
          this->cards_back_menu->clear_directories();
          for (list<string>::const_iterator
               d = ::setting.data_directories().begin();
               d != ::setting.data_directories().end();
               ++d)
            this->cards_back_menu->add_directory(*d + "/"
                                                 + ::setting(Setting::CARDS_BACK_DIRECTORY));
        case Setting::CARDS_BACK:
        case Setting::BACKGROUND:
          dynamic_cast<Gtk::Button*>(this->type_string[type - Setting::STRING_FIRST])->set_label(::setting.value(Setting::TypeString(type)));
          break;
        case Setting::NAME_FONT:
        case Setting::TRICKPILE_POINTS_FONT:
          static_cast<Gtk::FontSelectionDialog*>(this->type_string[type - Setting::STRING_FIRST]->get_data("font selector"))->set_font_name(::setting(Setting::TypeString(type)));
#ifdef POSTPONED
          // ToDo
          // don't work :-(
          { // change the style
            Glib::RefPtr<Gtk::Style> style
              = this->type_string[type - Setting::STRING_FIRST]->get_style();

            style->set_font(Pango::FontDescription(::setting(Setting::TypeString(type))));

            this->type_string[type - Setting::STRING_FIRST]->set_style(style);
          } // change the style
#endif // #ifdef POSTPONED
          break;

        case Setting::NAME_FONT_COLOR:
        case Setting::NAME_ACTIVE_FONT_COLOR:
        case Setting::NAME_RESERVATION_FONT_COLOR:
        case Setting::TRICKPILE_POINTS_FONT_COLOR:
        case Setting::POVERTY_SHIFT_ARROW_COLOR:
          static_cast<Gtk::ColorSelectionDialog*>(this->type_string[type - Setting::STRING_FIRST]->get_data("color selector"))->get_colorsel()->set_current_color(Gdk::Color(::setting(Setting::TypeString(type))));
#ifdef POSTPONED
          // change the color of the text in the buttons
          Glib::RefPtr<Gtk::Style> style
            = this->type_string[type - Setting::STRING_FIRST]->get_style();
          // = static_cast<Gtk::Button*>(this->type_string[type - Setting::STRING_FIRST])->get_child()->get_style();

          Gdk::Color color(::setting(Setting::TypeString(type)));
          this->ui->colormap->alloc_color(color);
          style->set_fg(Gtk::STATE_NORMAL, color);
          style->set_text(Gtk::STATE_NORMAL, color);
          style->set_text_aa(Gtk::STATE_NORMAL, color);
          style->set_base(Gtk::STATE_NORMAL, color);

          this->type_string[type - Setting::STRING_FIRST]->set_style(style);
          static_cast<Gtk::Button*>(this->type_string[type - Setting::STRING_FIRST])->get_child()->set_style(style);
#endif // #ifdef POSTPONED
          break;

#ifdef PLAY_SOUND_COMMAND
        case Setting::PLAY_SOUND_COMMAND:
#endif
        case Setting::BROWSER_COMMAND:
          dynamic_cast<Gtk::Entry*>(this->type_string[type - Setting::STRING_FIRST])->set_text(::setting(Setting::TypeString(type)));
          break;
        } // switch(type)


      } // if (type \in TypeString)
      else if (type == Setting::CARDS_ORDER)
        this->update_cards_order();
      else 
        DEBUG_ASSERTION(false,
                        "Preferences::update(type, update_sensitivity):\n"
                        "  type '" << type << "' unknown.");

      if (update_sensitivity)
        this->reset_button->set_sensitive(this->backup() != ::setting);

      this->ui->main_window->menu->update_themes_sensitivity();

      return ;
    } // void Preferences::update(int const type, bool const update_sensitivity = true)

  /**
   **
   ** a setting has been changed by the user
   **
   ** @param     type   the type of the setting
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::change(int const type)
    {
      if ((type >= Setting::BOOL_FIRST)
          && (type <= Setting::BOOL_LAST))
        ::setting.set(Setting::TypeBool(type),
                      this->type_bool[type - Setting::BOOL_FIRST]->get_active());
      else if ((type >= Setting::UNSIGNED_FIRST)
               && (type <= Setting::UNSIGNED_LAST)) {
        if (this->type_unsigned[type - Setting::UNSIGNED_FIRST]->is_realized()) {
          switch (static_cast<Setting::TypeUnsigned const>(type)) {
          case Setting::CARD_PLAY_DELAY:
          case Setting::FULL_TRICK_CLOSE_DELAY:
          case Setting::GAMETYPE_WINDOW_CLOSE_DELAY:
          case Setting::MARRIAGE_WINDOW_CLOSE_DELAY:
          case Setting::GENSCHER_WINDOW_CLOSE_DELAY:
          case Setting::ANNOUNCEMENT_WINDOW_CLOSE_DELAY:
          case Setting::SWINES_WINDOW_CLOSE_DELAY:
            // make one second to correspond with '1.0'
            ::setting.set(static_cast<Setting::TypeUnsigned>(type),
                          static_cast<int>(1000 * this->type_unsigned[type - Setting::UNSIGNED_FIRST]->get_value()));
            break;
#ifdef USE_THREADS
          case Setting::THREADS_MAX:
#endif
          case Setting::TABLE_ROTATION:
          case Setting::CARDS_HEIGHT:
            ::setting.set(static_cast<Setting::TypeUnsigned>(type),
                          this->type_unsigned[type - Setting::UNSIGNED_FIRST]->get_value_as_int());
            break;
          } // switch(type)
        } // if (type is realized)
      }
      else if ((type >= Setting::STRING_FIRST)
               && (type <= Setting::STRING_LAST)) {
        switch(type) {
        case Setting::NAME:
          ::setting.set(static_cast<Setting::TypeString>(type),
                        dynamic_cast<Gtk::Entry*>(this->type_string[type - Setting::STRING_FIRST])->get_text());
          break;
        case Setting::LANGUAGE:
          break;
        case Setting::CARDSET:
          // -> Preferences::cardset_selected()
          break;
        case Setting::CARDS_BACK:
          // -> Preferences::cards_back_selected()
          break;
        case Setting::BACKGROUND:
          // -> Preferences::background_selected()
          break;
        case Setting::NAME_FONT:
        case Setting::TRICKPILE_POINTS_FONT:
          ::setting.set(static_cast<Setting::TypeString>(type),
                        static_cast<Gtk::FontSelectionDialog*>(this->type_string[type - Setting::STRING_FIRST]->get_data("font selector"))->get_font_name());
          break;
        case Setting::NAME_FONT_COLOR:
        case Setting::NAME_ACTIVE_FONT_COLOR:
        case Setting::NAME_RESERVATION_FONT_COLOR:
        case Setting::TRICKPILE_POINTS_FONT_COLOR:
        case Setting::POVERTY_SHIFT_ARROW_COLOR:
          ::setting.set(static_cast<Setting::TypeString>(type),
                        colorname(static_cast<Gtk::ColorSelectionDialog*>(this->type_string[type - Setting::STRING_FIRST]->get_data("color selector"))->get_colorsel()->get_current_color()));
          break;

#ifdef PLAY_SOUND_COMMAND
        case Setting::PLAY_SOUND_COMMAND:
#endif
        case Setting::BROWSER_COMMAND:
          ::setting.set(static_cast<Setting::TypeString>(type),
                        dynamic_cast<Gtk::Entry*>(this->type_string[type - Setting::STRING_FIRST])->get_text());
          break;
        } // switch(type)
      } // if (type == ())
      // cards order is managed in 'setting.cards_order.cpp'

      return ;
    } // void Preferences::change(int const type)

  /**
   ** a setting has been changed by the user
   **
   ** @param     focus   ignored
   ** @param     type   the type of the setting
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  bool
    Preferences::focus_out_event(GdkEventFocus* focus, int const type)
    {
      this->change(type);

      return true;
    } // bool Preferences::focus_out_event(GdkEventFocus*, int type)

  /**
   **
   ** create and show the language menu
   **
   ** @param     popup   whether to popup the menu (default: true)
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::language_menu_create(bool const popup)
    {
#if 0
      if (this->language_menu == NULL)
        return ;
#endif

      { // remove all children
        Glib::ListHandle<Widget*> children
          = this->language_menu->get_children();

        if (!children.empty()) {
          for (Glib::ListHandle<Widget*>::iterator child = children.begin();
               child != children.end();
               child++)
            this->language_menu->remove(**child);
        } // if (!children.empty())
      } // remove all children

      // the language menu consists of the default button, a separator and 
      // the languages (taken from '::language'
      Gtk::MenuItem* item = NULL;

      // memory leak: the translation created in 'Ui::->translations->add' is not removed
      item = Gtk::manage(new Gtk::MenuItem(""));
      this->ui->translations->add(*(dynamic_cast<Gtk::Label*>(item->get_child())),
                                  ::translation("default"));
      item->signal_activate().connect(sigc::bind<string const>(sigc::mem_fun(*this, &Preferences::language_selected), ""));
      this->language_menu->add(*item);
      item = Gtk::manage(new Gtk::SeparatorMenuItem());
      this->language_menu->add(*item);

      for (vector<string>::const_iterator name = Translator::names.begin();
           name != Translator::names.end();
           name++) {
        item = Gtk::manage(new Gtk::MenuItem(*name));
        item->signal_activate().connect(sigc::bind<string const>(sigc::mem_fun(*this, &Preferences::language_selected), *name));
        this->language_menu->add(*item);
      } // for (language \in Translator::names)

      if (popup)
        this->language_menu->popup(0, 0);

      this->language_menu->show_all();

      return ;
    } // void Preferences::language_menu_create(bool const popup = true)

  /**
   **
   ** change of the language
   **
   ** @param     language   new language
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::language_selected(string const language)
    {
      ::setting.set(Setting::LANGUAGE, language);

      return ;
    } // void Preferences::language_selected(string const language)

  /**
   **
   ** popup the background menu
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
    Preferences::background_menu_popup()
    {
      this->background_menu->show();
      //this->background_menu->popup(0, 0);

      return ;
    } // void Preferences::background_menu_popup()

  /**
   **
   ** change of the background
   **
   ** @param     background   new background
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::background_selected(string const background)
    {
      ::setting.set(Setting::BACKGROUND,
                    string(background,
                           0,
                           background.size() - 1
                           - ::setting(Setting::GRAPHIC_EXTENSION).size()));

      return ;
    } // void Preferences::background_selected(string const background)

  /**
   **
   ** change of the cardset
   **
   ** @param     cardset   new cardset
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::cardset_selected(string const cardset)
    {
      ::setting.set(Setting::CARDSET, cardset);

      return ;
    } // void Preferences::cardset_selected(string const cardset)

  /**
   **
   ** change of the cards_back
   **
   ** @param     cards_back   new cards_back
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::cards_back_selected(string const cards_back)
    {
      ::setting.set(Setting::CARDS_BACK,
                    string(cards_back,
                           0,
                           cards_back.size() - 1
                           - ::setting(Setting::GRAPHIC_EXTENSION).size()));


      return ;
    } // void Preferences::cards_back_selected(string const cards_back)

  /**
   **
   ** the font of the setting 'type' has been accepted
   **
   ** @param     font_selector   the font selector
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::font_accepted(Gtk::FontSelectionDialog* const font_selector)
    {
      Setting::TypeString const type
        = *(static_cast<Setting::TypeString*>(font_selector->get_data("type")));

#ifdef POSTPONED
      Gtk::FontSelection* const fontsel =
        font_selector->get_font_selection();

#endif

      font_selector->hide();
      this->change(type);

      return ;
    } // void Preferences::font_accepted(Gtk::FontSelectionDialog* const font_selector)

  /**
   **
   ** the font of the setting 'type' has been declined
   **
   ** @param     font_selector   the font selector
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::font_declined(Gtk::FontSelectionDialog* const font_selector)
    {
      font_selector->hide();

      return ;
    } // void Preferences::font_declined(Gtk::FontSelectionDialog* const font_selector)

  /**
   **
   ** the color of the setting 'type' has been accepted
   **
   ** @param     color_selector   the color selector
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::color_accepted(Gtk::ColorSelectionDialog* const color_selector)
    {
      Gtk::ColorSelection* const colorsel = color_selector->get_colorsel();

      colorsel->set_previous_color(colorsel->get_current_color());

      color_selector->hide();

      Setting::TypeString const type
        = *(static_cast<Setting::TypeString*>(color_selector->get_data("type")));
      this->change(type);

      return ;
    } // void Preferences::color_accepted(Gtk::ColorSelectionDialog* const color_selector)

  /**
   **
   ** the color of the setting 'type' has been declined
   **
   ** @param     color_selector   the color selector
   **
   ** @return    -
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::color_declined(Gtk::ColorSelectionDialog* const color_selector)
    {
      Gtk::ColorSelection* const colorsel = color_selector->get_colorsel();

      colorsel->set_current_color(colorsel->get_previous_color());

      color_selector->hide();

      Setting::TypeString const type
        = *(static_cast<Setting::TypeString*>(color_selector->get_data("type")));
      this->change(type);

      return ;
    } // void Preferences::color_declined(Gtk::ColorSelectionDialog* const color_selector)

#ifndef RELEASE
  /**
   **
   ** update the translation
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.7
   **
   ** @author    Diether Knof
   **
   **/
  void
    Preferences::update_translation()
    {
      ::translator.load();

      return ;
    } // void Preferences::update_translation()
#endif

  /**
   **
   ** a key has been pressed
   ** C-o: output of the preferences on 'stdout'
   **
   ** @param     key   the key
   **
   ** @return    whether the key was managed
   **
   ** @version   0.6.1
   **
   ** @author    Diether Knof
   **
   **/
  bool
    Preferences::on_key_press_event(GdkEventKey* key)
    {
      bool managed = false;

      if ((key->state & ~GDK_SHIFT_MASK) == GDK_CONTROL_MASK) {
        switch (key->keyval) {
        case GDK_o: // ouput of the preferences
          cout << ::setting;
          managed = true;
          break;
#ifndef RELEASE
        case GDK_t: // update the translation
          this->update_translation();
          managed = true;
          break;
#endif
        } // switch (key->keyval)
      } // if (key->state == GDK_CONTROL_MASK)

      return (managed
              || this->StickyDialog::on_key_press_event(key)
              || this->ui->key_press(key));
    } // bool Preferences::on_key_press_event(GdkEventKey* key)

  /**
   ** the page of the notebook has changed
   ** update the title of the window
   **
   ** @param     page     current page
   ** @param     pageno   number of the active page
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.1
   **/
  void
    Preferences::group_notebook_page_changed(GtkNotebookPage* page,
                                             guint pageno)
    {
      Translator::Translation* group_name
        = static_cast<Translator::Translation*>(this->group_notebook->get_nth_page(pageno)->get_data("group name"));
      this->ui->translations->change(*this, ::translation("FreeDoko %ttitle%",
                                                          ::translation("settings: %tgroup%",
                                                                        *group_name)));

      return ;
    } // void Preferences::notebook_page_changed(GtkNotebookPage* page, guint pageno)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
