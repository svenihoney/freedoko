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

#include "aiconfig.h"
#include "ui.h"
#include "translations.h"

#include "../../party/party.h"
#include "../../party/rule.h"
#include "../../player/aiconfig.h"
#include "../../misc/setting.h"
#include "../../utils/string.h"
#ifdef USE_NETWORK
#include "../../network/server.h"
#endif

#include "widgets/labelscalespinbutton.h"
#include "widgets/filemenu.h"
#include "widgets/stock_button.h"
#include "widgets/label_card_selector.h"
#include <gtkmm/stock.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/table.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/optionmenu.h>
#include <gtkmm/notebook.h>
#include <gtkmm/frame.h>
#include <gtkmm/textview.h>
#include <gtkmm/main.h>

namespace UI_GTKMM_NS {

  /**
   ** Constructor
   **
   ** @param     player   the corresponding (Gtk-)Player
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  Players::Player::AiConfig::AiConfig(Player* const player) :
    Base(player),
    sigc::trackable(),
    player(player),
    container(NULL),
    notebook(NULL),
    notebooks(),
    difficulty_container(NULL),
    difficulty_label(NULL),
    difficulty_button(NULL),
    difficulty_selector(NULL),
    adopt_button(NULL),
    allot_button(NULL),
    load_button(NULL),
    save_button(NULL),
    save_file_chooser(NULL),
    aitype(),
    rating(),
    future_limit(),
    aitype_as_previous(),
    type_bool(),
    type_int(),
    type_card(),
    heuristics_default_button(),
    heuristics_model(NULL),
    heuristics_list(),
    heuristics_treeview(),
    heuristic_up_button(),
    heuristic_down_button(),
    heuristic_description(),
    heuristics_signals_active(false)
  {
    this->init();

    return ;
  } // Players::Player::AiConfig::AiConfig(Player* const player)

  /**
   ** Destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  Players::Player::AiConfig::~AiConfig()
  {
    for (vector<Gtk::OptionMenu*>::iterator i = this->aitype.begin();
         i != this->aitype.end();
         i++) {
      for (Gtk::Menu_Helpers::MenuList::iterator
           t = (*i)->get_menu()->items().begin();
           t != (*i)->get_menu()->items().end();
           ++t) {
        delete static_cast<unsigned*>(t->steal_data("trick number"));
        delete static_cast<AiType*>(t->steal_data("aitype"));
      }
    }
    for (vector<Gtk::OptionMenu*>::iterator i = this->rating.begin();
         i != this->rating.end();
         i++) {
      for (Gtk::Menu_Helpers::MenuList::iterator
           t = (*i)->get_menu()->items().begin();
           t != (*i)->get_menu()->items().end();
           ++t) {
        delete static_cast<unsigned*>(t->steal_data("trick number"));
        delete static_cast<Rating::Type*>(t->steal_data("rating"));
      }
    }

    for (vector<Gtk::CheckButton*>::iterator i = this->type_bool.begin();
         i != this->type_bool.end();
         i++)
      delete static_cast<string*>((*i)->steal_data("type"));

    for (vector<Gtk::LabelCardSelector*>::iterator i = this->type_card.begin();
         i != this->type_card.end();
         i++)
      delete static_cast<string*>((*i)->steal_data("type"));

    for (vector<Gtk::LabelSpinButton*>::iterator i
         = this->type_int.begin();
         i != this->type_int.end();
         i++)
      delete static_cast<string*>((*i)->steal_data("type"));

    return ;
  } // Players::Player::AiConfig::~AiConfig()

  /**
   ** creates all subelements
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
    Players::Player::AiConfig::init()
    {
      { // create the widgets
        { // buttons
          this->difficulty_label
            = Gtk::manage(new Gtk::Label("difficulty"));
          this->ui->translations->add(*this->difficulty_label,
                                      ::translation("difficulty") + ": ");
          this->difficulty_button
            = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLEAR, "hardcoded"));
          this->ui->translations->add(*this->difficulty_button,
                                      ::translation("hardcoded"));
          this->difficulty_selector
            = Gtk::manage(new Gtk::LabelTypeSelector< ::Aiconfig::Difficulty>());
          this->ui->translations->add(this->difficulty_selector->get_label(),
                                      ::translation("AiConfig::difficulty")
                                      + ": ");
          for (int difficulty = ::Aiconfig::DIFFICULTY_FIRST;
               difficulty <= ::Aiconfig::DIFFICULTY_LAST;
               difficulty++) {
            this->difficulty_selector->add(static_cast< ::Aiconfig::Difficulty>(difficulty));
          }

          this->adopt_button
            = Gtk::manage(new Gtk::StockButton("adopt"));
          this->ui->translations->add(*this->adopt_button,
                                      ::translation("adopt"));
          this->allot_button
            = Gtk::manage(new Gtk::StockButton("allot"));
          this->ui->translations->add(*this->allot_button,
                                      ::translation("allot"));
          this->load_button
            = Gtk::manage(new Gtk::StockButton(Gtk::Stock::OPEN, "load"));
          this->ui->translations->add(*this->load_button,
                                      ::translation("load"));
          this->save_button
            = Gtk::manage(new Gtk::StockButton(Gtk::Stock::SAVE_AS, "save"));
          this->ui->translations->add(*this->save_button,
                                      ::translation("save"));

          { // file chooser
            this->save_file_chooser = new Gtk::FileChooserDialog("Save aiconfig",
                                                                 Gtk::FILE_CHOOSER_ACTION_SAVE);
            this->ui->translations->add(*this->save_file_chooser,
                                        ::translation("~save aiconfig of %splayer%",
                                                      this->player->player().name()));

            Gtk::Button* cancel_button
              = this->save_file_chooser->add_button(Gtk::Stock::CANCEL,
                                                    Gtk::RESPONSE_CANCEL);
            Gtk::Button* ok_button
              = this->save_file_chooser->add_button(Gtk::Stock::SAVE,
                                                    Gtk::RESPONSE_OK);


            { // signals
              this->save_file_chooser->signal_file_activated().connect(sigc::mem_fun(*this, &AiConfig::save));
              this->save_file_chooser->signal_file_activated().connect(sigc::mem_fun(*this->save_file_chooser, &Widget::hide));

              ok_button->signal_clicked().connect(sigc::mem_fun(*this, &AiConfig::save));
              ok_button->signal_clicked().connect(sigc::mem_fun(*this->save_file_chooser, &Widget::hide));
              cancel_button->signal_clicked().connect(sigc::mem_fun(*this->save_file_chooser, &Widget::hide));
            } // signals
            {
              string dir = (::setting(Setting::PRIVATE_DATA_DIRECTORY)
                            + "/"
                            + ::setting(Setting::AI_DIRECTORY)
                            + "/"
                           );
#ifdef WINDOWS
              DK::Utils::String::replace_all(dir, "/", "\\");
#endif
              this->save_file_chooser->set_current_folder(dir);
            }
          } // file chooser
        } // buttons


        for (int t = Aiconfig::BOOL_FIRST;
             t <= Aiconfig::BOOL_LAST;
             t++) {
          this->type_bool.push_back(Gtk::manage(new Gtk::CheckButton(::name(Aiconfig::Heuristic(t)))));
          this->type_bool.back()->set_data("type", new Aiconfig::TypeBool(Aiconfig::TypeBool(t)));
          this->ui->translations->add(*this->type_bool.back(),
                                      ::translation(Aiconfig::TypeBool(t)));
          this->type_bool.back()->signal_toggled().connect(sigc::bind<int const>(sigc::mem_fun(*this, &AiConfig::change),
                                                                                 t)
                                                          );
        } // for (t \in Aiconfig::TypeBool)
        for (int t = Aiconfig::INT_FIRST;
             t <= Aiconfig::INT_LAST;
             t++) {
          switch (t) {
          case Aiconfig::REMEMBERTRICKS:
          case Aiconfig::LASTFEHLCREATION:
          case Aiconfig::LAST_TRICKS_WITHOUT_HEURISTICS:
          case Aiconfig::FIRST_TRICK_FOR_TRUMP_POINTS_OPTIMIZATION:
            this->type_int.push_back(Gtk::manage(new Gtk::LabelScaleSpinButton(::name(Aiconfig::TypeInt(t)))));
            break;
          case Aiconfig::LIMIT_THROW_FEHL:
          case Aiconfig::LIMITQUEEN:
          case Aiconfig::LIMITDOLLE:
          case Aiconfig::ANNOUNCELIMIT:
          case Aiconfig::ANNOUNCELIMITDEC:
          case Aiconfig::ANNOUNCECONFIG:
          case Aiconfig::ANNOUNCELIMITREPLY:
          case Aiconfig::ANNOUNCECONFIGREPLY:
          case Aiconfig::TAKEPOVERTY:
          case Aiconfig::SINGLESOLO:
          case Aiconfig::DOUBLESOLO:
          case Aiconfig::TRIPLESOLO:
          case Aiconfig::COLORSOLO:
          case Aiconfig::MEATLESS:
            this->type_int.push_back(Gtk::manage(new Gtk::LabelSpinButton(::name(Aiconfig::TypeInt(t)))));
            break;
          } // switch(t)
          this->type_int.back()->set_data("type", new Aiconfig::TypeInt(Aiconfig::TypeInt(t)));
          this->ui->translations->add(this->type_int.back()->get_label(),
                                      ::translation(Aiconfig::TypeInt(t))
                                      + ": ");
          this->type_int.back()->signal_value_changed().connect(sigc::bind<int const>(sigc::mem_fun(*this, &AiConfig::change),
                                                                                           t)
                                                                    );
        } // for (t \in Aiconfig::TypeInt)
        for (int t = Aiconfig::CARD_FIRST;
             t <= Aiconfig::CARD_LAST;
             t++) {
          Aiconfig::TypeCard const type = static_cast<Aiconfig::TypeCard>(t);
          switch (type) {
          case Aiconfig::LIMITTHROWING:
          case Aiconfig::LIMITHIGH:
          case Aiconfig::TRUMPLIMIT_NORMAL:
          case Aiconfig::LOWEST_TRUMPLIMIT_NORMAL:
            this->type_card.push_back(Gtk::manage(new Gtk::LabelCardSelector(::name(type))));
            this->ui->translations->add(this->type_card.back()->get_label(),
                                        ::translation(type));
            break;
          case Aiconfig::TRUMPLIMIT_SOLOCOLOR:
          case Aiconfig::LOWEST_TRUMPLIMIT_SOLOCOLOR:
          case Aiconfig::TRUMPLIMIT_SOLOJACK:
          case Aiconfig::LOWEST_TRUMPLIMIT_SOLOJACK:
          case Aiconfig::TRUMPLIMIT_SOLOQUEEN:
          case Aiconfig::LOWEST_TRUMPLIMIT_SOLOQUEEN:
          case Aiconfig::TRUMPLIMIT_SOLOKING:
          case Aiconfig::LOWEST_TRUMPLIMIT_SOLOKING:
          case Aiconfig::TRUMPLIMIT_SOLOJACKKING:
          case Aiconfig::LOWEST_TRUMPLIMIT_SOLOJACKKING:
          case Aiconfig::TRUMPLIMIT_SOLOJACKQUEEN:
          case Aiconfig::LOWEST_TRUMPLIMIT_SOLOJACKQUEEN:
          case Aiconfig::TRUMPLIMIT_SOLOQUEENKING:
          case Aiconfig::LOWEST_TRUMPLIMIT_SOLOQUEENKING:
          case Aiconfig::TRUMPLIMIT_SOLOKOEHLER:
          case Aiconfig::LOWEST_TRUMPLIMIT_SOLOKOEHLER:
          case Aiconfig::TRUMPLIMIT_MEATLESS:
          case Aiconfig::LOWEST_TRUMPLIMIT_MEATLESS:
            // the translation is added by a separate label
            this->type_card.push_back(Gtk::manage(new Gtk::LabelCardSelector()));
            break;
          default:
            DEBUG_ASSERTION(false,
                            "UI_GTKMM::AiConfig::init()\n"
                            "  Aiconfig::TypeCard '" << type << "' "
                            "not taken into account.");
            break;
          } // switch(type)
          this->type_card.back()->set_data("type",
                                           new Aiconfig::TypeCard(type));
          for (list<Card>::const_iterator
               c = this->aiconfig().valid_cards(type).begin();
               c != this->aiconfig().valid_cards(type).end();
               ++c)
            this->type_card.back()->add(*c);
          this->type_card.back()->signal_value_changed().connect(sigc::bind<int const>(sigc::mem_fun(*this, &AiConfig::change), t) );
        } // for (t \in Aiconfig::TypeCard)
      } // create the widgets

      this->create_container();

      { // the signals
        this->difficulty_button->signal_clicked().connect(sigc::mem_fun(*this, &AiConfig::hardcoded));
        this->difficulty_selector->signal_value_changed().connect(sigc::mem_fun(*this, &AiConfig::difficulty_changed));
        this->adopt_button->signal_clicked().connect(sigc::mem_fun(*this, &AiConfig::adopt));
        this->allot_button->signal_clicked().connect(sigc::mem_fun(*this, &AiConfig::allot));
        this->load_button->signal_clicked().connect(sigc::mem_fun(*this, &AiConfig::load));
        this->save_button->signal_clicked().connect(sigc::mem_fun(*(this->save_file_chooser), &Widget::show));

        for (std::map<HeuristicsMap::Key, Gtk::ToggleButton*>::iterator
             m = this->heuristics_default_button.begin();
             m != this->heuristics_default_button.end();
             ++m)
          m->second->signal_toggled().connect(sigc::bind<HeuristicsMap::Key const>(sigc::mem_fun(*this, &AiConfig::heuristics_default_button_toggled_event), m->first));
      } // the signals

      this->language_update();
      this->update();
      this->sensitivity_update();

      this->heuristics_signals_active = true;

      return ;
    } // void Players::Player::AiConfig::init()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    a container with the configuration (in notebooks)
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  void
    Players::Player::AiConfig::create_container()
    {
      this->container
        = Gtk::manage(new Gtk::VBox(false, 1 ex));

      { // the difficulty
        Gtk::Alignment* alignment
          = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                           Gtk::ALIGN_CENTER,
                                           0, 1));
        Gtk::Box* box = Gtk::manage(new Gtk::HBox(false, 0));

        box->add(*this->difficulty_label);
        box->add(*this->difficulty_selector);
        alignment->add(*box);

        this->difficulty_container = alignment;
      } // the difficulty

      { // the settings
        this->notebook = Gtk::manage(new Gtk::Notebook());
        this->notebook->set_scrollable(true);

        { // the notebook pages
          { // data buttons
            Gtk::Label* label = Gtk::manage(new Gtk::Label(""));
            this->ui->translations->add(*label,
                                        ::translation("AiConfig::Group::data"));

            Gtk::Alignment* alignment
              = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                               Gtk::ALIGN_CENTER,
                                               0, 0));
            alignment->set_border_width(1 em);

            Gtk::Table* table = Gtk::manage(new Gtk::Table(2, 2, true));
            table->set_col_spacings(2 em);
            table->set_row_spacings(1 ex);
            table->set_border_width(1 ex);

            //table->attach(*this->difficulty_button, 0, 1, 0, 1);
            // the difficulty selector is put in the 'player' page
            // (see players.cpp)

            table->attach(*this->adopt_button,        0, 1, 0, 1);
            table->attach(*this->allot_button,        1, 2, 0, 1);
            table->attach(*this->load_button,         0, 1, 1, 2);
            table->attach(*this->save_button,         1, 2, 1, 2);

            alignment->add(*table);
            this->notebook->append_page(*alignment, *label);
          } // data buttons
          { // simple settings
            Gtk::Label* label = Gtk::manage(new Gtk::Label("simple"));
            this->ui->translations->add(*label,
                                        ::translation("AiConfig::Group::simple"));

            Gtk::Alignment* alignment
              = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                               Gtk::ALIGN_CENTER,
                                               0, 0));
            alignment->set_border_width(1 em);

            Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 em));

            vbox->add(*(this->type_bool[Aiconfig::TEAMS_KNOWN - Aiconfig::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Aiconfig::HANDS_KNOWN - Aiconfig::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Aiconfig::ESTIMATE_HANDS - Aiconfig::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Aiconfig::TRUSTING - Aiconfig::BOOL_FIRST]));
            vbox->add(*(this->type_bool[Aiconfig::AGGRESSIVE - Aiconfig::BOOL_FIRST]));
            vbox->add(*(this->type_int[Aiconfig::REMEMBERTRICKS - Aiconfig::INT_FIRST]));

            alignment->add(*vbox);
            this->notebook->append_page(*alignment, *label);
          } // simple settings
          { // aitype
            Gtk::Label* label = Gtk::manage(new Gtk::Label("aitype"));
            this->ui->translations->add(*label,
                                        ::translation("AiConfig::Group::aitype"));

            Gtk::Alignment* alignment
              = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                               Gtk::ALIGN_CENTER,
                                               0, 0));
            alignment->set_border_width(1 em);

            // ToDo: table -> ListStore
            Gtk::Table* table = Gtk::manage(new Gtk::Table(this->ui->party().rule()(Rule::MAX_NUMBER_OF_TRICKS_IN_GAME) + 2, 5));
            { // titles
              Gtk::Label* label;
#ifdef POSTPONED
              label = Gtk::manage(new Gtk::Label("as previous"));
              this->ui->translations->add(*label, ::translation("as previous"));
              table->attach(*label, 0, 1, 0, 1);
#endif

              label = Gtk::manage(new Gtk::Label("trick"));
              this->ui->translations->add(*label, ::translation("trick"));
              table->attach(*label, 1, 2, 0, 1);

              label = Gtk::manage(new Gtk::Label("aitype"));
              this->ui->translations->add(*label, ::translation("AiConfig::aitype"));
              table->attach(*label, 2, 3, 0, 1);

              label = Gtk::manage(new Gtk::Label("rating"));
              this->ui->translations->add(*label, ::translation("AiConfig::rating"));
              table->attach(*label, 3, 4, 0, 1);

              label = Gtk::manage(new Gtk::Label("future limit"));
              this->ui->translations->add(*label, ::translation("AiConfig::future limit"));
              table->attach(*label, 4, 5, 0, 1);

              label = Gtk::manage(new Gtk::Label(""));
              table->attach(*label, 0, 5, 1, 2);
            } // titles
            for (unsigned t = 0;
                 t < this->ui->party().rule()(Rule::MAX_NUMBER_OF_TRICKS_IN_GAME); 
                 ++t) {
              { // aitype as previous
                this->aitype_as_previous.push_back(Gtk::manage(new Gtk::CheckButton()));
                if (t > 0) {
                  this->aitype_as_previous.back()->set_active(   (this->aiconfig().aitype(t) != this->aiconfig().aitype(t - 1))
                                                              || (this->aiconfig().rating(t) != this->aiconfig().rating(t - 1))
                                                              || (this->aiconfig().future_limit(t) != this->aiconfig().future_limit(t - 1))
                                                             );
                  table->attach(*this->aitype_as_previous.back(), 0, 1, t + 2, t + 3);
                  this->aitype_as_previous.back()->signal_toggled().connect(sigc::bind<unsigned const>(sigc::mem_fun(*this, &AiConfig::aitype_as_previous_changed), t));
                } // if (t > 0)
              } // aitype as previous

              { // label
                Gtk::Label* label = Gtk::manage(new Gtk::Label( " " + DK::Utils::String::to_string(t + 1) + ": "));
                table->attach(*label, 1, 2, t + 2, t + 3);
              } // label

              { // aitype menu
                Gtk::Menu *menu = Gtk::manage(new Gtk::Menu());

                for (int a = AITYPE::AITYPE_FIRST;
                     a <= AITYPE::AITYPE_LAST;
                     ++a) {
                  if (a == AITYPE::NO_CHOOSEBESTCARD)
                    continue;

                  Gtk::MenuItem *item
                    = Gtk::manage(new Gtk::MenuItem("ai type"));
                  this->ui->translations->add(*(dynamic_cast<Gtk::Label*>(item->get_child())),
                                              ::translation(static_cast<AiType>(a)));
                  item->set_data("aitype", new AiType(AiType(a)));
                  item->set_data("trick number", new unsigned(t));
                  menu->append(*item);
                } // for (a \in AITYPE)
                this->aitype.push_back(Gtk::manage(new Gtk::OptionMenu()));
                this->aitype.back()->set_menu(*menu);
                this->aitype.back()->set_history(this->aiconfig().aitype(t) - 1);

                table->attach(*this->aitype.back(), 2, 3, t + 2, t + 3);
                this->aitype.back()->signal_changed().connect(sigc::bind<unsigned const>(sigc::mem_fun(*this, &AiConfig::aitype_changed), t));


              } // aitype menu
              { // rating menu
                Gtk::Menu *menu = Gtk::manage(new Gtk::Menu());

                for (int r = Rating::TYPE_FIRST;
                     r <= Rating::TYPE_LAST;
                     ++r) {
                  Gtk::MenuItem *item
                    = Gtk::manage(new Gtk::MenuItem("rating"));
                  this->ui->translations->add(*(dynamic_cast<Gtk::Label*>(item->get_child())),
                                              ::translation(static_cast<Rating::Type>(r)));
                  item->set_data("rating", new Rating::Type(Rating::Type(r)));
                  item->set_data("trick number", new unsigned(t));
                  menu->append(*item);
                } // for (r \in Rating::Type)
                this->rating.push_back(Gtk::manage(new Gtk::OptionMenu()));
                this->rating.back()->set_menu(*menu);

                this->rating.back()->set_history(this->aiconfig().rating(t));

                table->attach(*this->rating.back(), 3, 4, t + 2, t + 3);
                this->rating.back()->signal_changed().connect(sigc::bind<unsigned const>(sigc::mem_fun(*this, &AiConfig::rating_changed), t));
              } // rating menu
              { // future limit
                this->future_limit.push_back(Gtk::manage(new Gtk::SpinButton()));
                this->future_limit.back()->set_range(1, 1e9);
                this->future_limit.back()->set_increments(1, 1000);
                this->future_limit.back()->set_numeric(true);
                table->attach(*this->future_limit.back(), 4, 5, t + 2, t + 3);

                this->future_limit.back()->signal_value_changed().connect(sigc::bind<unsigned const>(sigc::mem_fun(*this, &AiConfig::future_limit_changed), t));
              } // future limit
            } // for (t < MAX_NUMBER_OF_TRICKS_IN_GAME)

            alignment->add(*table);
            this->notebook->append_page(*alignment, *label);
          } // aitype
          { // game decision
            Gtk::Label* label = Gtk::manage(new Gtk::Label("game decision"));
            this->ui->translations->add(*label,
                                        ::translation("AiConfig::Group::game decision"));

            Gtk::Alignment* alignment
              = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                               Gtk::ALIGN_CENTER,
                                               0, 0));
            alignment->set_border_width(1 em);

            Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 em));

            vbox->add(*this->type_int[Aiconfig::TAKEPOVERTY - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::SINGLESOLO - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::DOUBLESOLO - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::TRIPLESOLO - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::COLORSOLO - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::MEATLESS - Aiconfig::INT_FIRST]);

            alignment->add(*vbox);
            this->notebook->append_page(*alignment, *label);
          } // game decision
          { // announcement
            Gtk::Label* label = Gtk::manage(new Gtk::Label("announcement"));
            this->ui->translations->add(*label,
                                        ::translation("AiConfig::Group::announcement"));

            Gtk::Alignment* alignment
              = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                               Gtk::ALIGN_CENTER,
                                               0, 0));
            alignment->set_border_width(1 em);

            Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 em));

            vbox->add(*(this->type_int[Aiconfig::ANNOUNCELIMIT - Aiconfig::INT_FIRST]));
            vbox->add(*(this->type_int[Aiconfig::ANNOUNCELIMITDEC - Aiconfig::INT_FIRST]));
            vbox->add(*(this->type_int[Aiconfig::ANNOUNCECONFIG - Aiconfig::INT_FIRST]));
            vbox->add(*(this->type_int[Aiconfig::ANNOUNCELIMITREPLY - Aiconfig::INT_FIRST]));
            vbox->add(*(this->type_int[Aiconfig::ANNOUNCECONFIGREPLY - Aiconfig::INT_FIRST]));

            alignment->add(*vbox);
            this->notebook->append_page(*alignment, *label);
          } // announcement
          { // trick decision
            Gtk::Label* label = Gtk::manage(new Gtk::Label("trick decision"));
            this->ui->translations->add(*label,
                                        ::translation("AiConfig::Group::trick decision"));

            Gtk::Alignment* alignment
              = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                               Gtk::ALIGN_CENTER,
                                               0, 0));
            alignment->set_border_width(1 em);

            Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 em));

            vbox->add(*this->type_bool[Aiconfig::FEHLCREATIONONFIRSTCARD - Aiconfig::BOOL_FIRST]);
            vbox->add(*this->type_int[Aiconfig::LAST_TRICKS_WITHOUT_HEURISTICS - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::FIRST_TRICK_FOR_TRUMP_POINTS_OPTIMIZATION - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::LASTFEHLCREATION - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::LIMIT_THROW_FEHL - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::LIMITQUEEN - Aiconfig::INT_FIRST]);
            vbox->add(*this->type_int[Aiconfig::LIMITDOLLE - Aiconfig::INT_FIRST]);

            alignment->add(*vbox);
            this->notebook->append_page(*alignment, *label);
          } // trick decision
          { // card limits
            Gtk::Label* label = Gtk::manage(new Gtk::Label("card limits"));
            this->ui->translations->add(*label,
                                        ::translation("AiConfig::Group::card limits"));

            Gtk::Notebook* card_limits_notebook
              = Gtk::manage(new Gtk::Notebook());
            this->notebooks.push_back(card_limits_notebook);

            { // normal game
              Gtk::Label* label = Gtk::manage(new Gtk::Label("normal game"));
              this->ui->translations->add(*label,
                                          ::translation("AiConfig::CardLimit::Group::normal game"));

              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              alignment->set_border_width(1 em);

              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));

              vbox->add(*this->type_card[Aiconfig::LIMITTHROWING - Aiconfig::CARD_FIRST]);
              vbox->add(*this->type_card[Aiconfig::TRUMPLIMIT_NORMAL - Aiconfig::CARD_FIRST]);
              vbox->add(*this->type_card[Aiconfig::LOWEST_TRUMPLIMIT_NORMAL - Aiconfig::CARD_FIRST]);
              vbox->add(*this->type_card[Aiconfig::LIMITHIGH - Aiconfig::CARD_FIRST]);

              alignment->add(*vbox);
              card_limits_notebook->append_page(*alignment, *label);
            } // normal game
            { // other solo
              Gtk::Label* label = Gtk::manage(new Gtk::Label("solo"));
              this->ui->translations->add(*label,
                                          ::translation("AiConfig::CardLimit::Group::solo"));

              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              alignment->set_border_width(1 em);
              Gtk::Table* table = Gtk::manage(new Gtk::Table(1, 3, false));
              table->set_row_spacings(1 ex / 2);
              table->set_col_spacings(1 em / 2);
              { // the table
                Gtk::Label* label;
                Gtk::Alignment* alignment;
                int row = 0;

                // title
                label = Gtk::manage(new Gtk::Label("trump limit"));
                this->ui->translations->add(*label,
                                            ::translation("AiConfig::CardLimit::Group::trump limit"));
                table->attach(*label, 1, 2, row, row + 1);
                label = Gtk::manage(new Gtk::Label("lowest trump limit"));
                this->ui->translations->add(*label,
                                            ::translation("AiConfig::CardLimit::Group::lowest trump limit"));
                table->attach(*label, 2, 3, row, row + 1);
                row += 1;
                table->set_row_spacing(0, 3 ex / 2);
                // color solo
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("color solo"));
                this->ui->translations->add(*label,
                                            ::translation("GameType::Group::Solo::color")
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_SOLOCOLOR - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_SOLOCOLOR - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
                // jack solo
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("jack solo"));
                this->ui->translations->add(*label,
                                            ::translation(GAMETYPE::SOLO_JACK)
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_SOLOJACK - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_SOLOJACK - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
                // queen solo
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("queen solo"));
                this->ui->translations->add(*label,
                                            ::translation(GAMETYPE::SOLO_QUEEN)
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_SOLOQUEEN - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_SOLOQUEEN - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
                // king solo
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("king solo"));
                this->ui->translations->add(*label,
                                            ::translation(GAMETYPE::SOLO_KING)
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_SOLOKING - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_SOLOKING - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
                // queen-jack solo
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("queen-jack solo"));
                this->ui->translations->add(*label,
                                            ::translation(GAMETYPE::SOLO_QUEEN_JACK)
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_SOLOJACKQUEEN - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_SOLOJACKQUEEN - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
                // king-jack solo
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("king-jack solo"));
                this->ui->translations->add(*label,
                                            ::translation(GAMETYPE::SOLO_KING_JACK)
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_SOLOJACKKING - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_SOLOJACKKING - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
                // king-queen solo
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("queen-king solo"));
                this->ui->translations->add(*label,
                                            ::translation(GAMETYPE::SOLO_KING_QUEEN)
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_SOLOQUEENKING - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_SOLOQUEENKING - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
                // koehler
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("koehler solo"));
                this->ui->translations->add(*label,
                                            ::translation(GAMETYPE::SOLO_KOEHLER)
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_SOLOKOEHLER - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_SOLOKOEHLER - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
                // meatless
                table->resize(row + 1, 3);
                label = Gtk::manage(new Gtk::Label("meatless solo"));
                this->ui->translations->add(*label,
                                            ::translation(GAMETYPE::SOLO_MEATLESS)
                                            + ": ");
                alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                           Gtk::ALIGN_CENTER,
                                                           0, 0));
                alignment->add(*label);
                table->attach(*alignment, 0, 1, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::TRUMPLIMIT_MEATLESS - ::Aiconfig::CARD_FIRST],
                              1, 2, row, row + 1);
                table->attach(*this->type_card[::Aiconfig::LOWEST_TRUMPLIMIT_MEATLESS - ::Aiconfig::CARD_FIRST],
                              2, 3, row, row + 1);
                row += 1;
              } // the table
              alignment->add(*table);
              card_limits_notebook->append_page(*alignment, *label);

            } // other solo

            this->notebook->append_page(*card_limits_notebook, *label);
          } // card limits
          { // heuristics
            Gtk::Label* label = Gtk::manage(new Gtk::Label("heuristics"));
            this->ui->translations->add(*label,
                                        ::translation("AiConfig::Group::heuristics"));

            Gtk::Notebook* heuristics_notebook
              = Gtk::manage(new Gtk::Notebook());
            this->notebooks.push_back(heuristics_notebook);
            this->heuristics_model = new HeuristicsModel();


            { // normal game
#if 0
              Gtk::Label* label
                = Gtk::manage(new Gtk::Label("heuristics normal"));
              this->ui->translations->add(*label,
                                          ::translation(HeuristicsMap::NORMAL));


              heuristics_notebook->append_page(*this->create_heuristic_treeview(HeuristicsMap::DEFAULT, HeuristicsMap::RE),
                                               *label);
#else
              Gtk::Notebook* normal_notebook
                = Gtk::manage(new Gtk::Notebook());
              this->notebooks.push_back(normal_notebook);
              this->add_heuristic_treeviews_re_contra(heuristics_notebook,
                                                      HeuristicsMap::NORMAL);
#endif
            } // normal game
            { // marriage
              Gtk::Label* label = Gtk::manage(new Gtk::Label("marriage"));
              this->ui->translations->add(*label,
                                          ::translation("AiConfig::HeuristicsMap::GametypeGroup::marriage"));

              Gtk::Notebook* marriage_notebook
                = Gtk::manage(new Gtk::Notebook());
              this->notebooks.push_back(marriage_notebook);

              this->add_heuristic_treeviews_re_contra(marriage_notebook,
                                                      HeuristicsMap::MARRIAGE_UNDETERMINED);
              this->add_heuristic_treeviews_re_contra(marriage_notebook,
                                                      HeuristicsMap::MARRIAGE_SILENT);

              heuristics_notebook->append_page(*marriage_notebook,
                                               *label);
            } // marriage
            { // poverty
              this->add_heuristic_treeviews_special_re_contra(heuristics_notebook,
                                                              HeuristicsMap::POVERTY);
            } // poverty
            { // soli
              Gtk::Label* label = Gtk::manage(new Gtk::Label("soli"));
              this->ui->translations->add(*label,
                                          ::translation("AiConfig::HeuristicsMap::GametypeGroup::soli"));

              Gtk::Notebook* soli_notebook = Gtk::manage(new Gtk::Notebook());
              this->notebooks.push_back(soli_notebook);

              this->add_heuristic_treeviews_re_contra(soli_notebook,
                                                      HeuristicsMap::SOLI_COLOR);
              this->add_heuristic_treeviews_re_contra(soli_notebook,
                                                      HeuristicsMap::SOLI_SINGLE_PICTURE);
              this->add_heuristic_treeviews_re_contra(soli_notebook,
                                                      HeuristicsMap::SOLI_DOUBLE_PICTURE);
              this->add_heuristic_treeviews_re_contra(soli_notebook,
                                                      HeuristicsMap::SOLO_KOEHLER);
              this->add_heuristic_treeviews_re_contra(soli_notebook,
                                                      HeuristicsMap::SOLO_MEATLESS);

              heuristics_notebook->append_page(*soli_notebook,
                                               *label);
            } // soli

            this->notebook->append_page(*heuristics_notebook, *label);
          } // heuristics
        } // the notebook pages

        this->container->pack_start(*this->notebook, true, true);
      } // the settings

#ifndef RELEASE
#ifdef DKNOF
      { // test, whether all configuration buttons are packed in a container
        for (int t = Aiconfig::BOOL_FIRST; t <= Aiconfig::BOOL_LAST; ++t)
          if (!this->type_bool[t - Aiconfig::BOOL_FIRST]->get_parent())
            cerr << "UI_GTKMM::Players::Player::AiConfig::create_container()\n"
              << "  config '" << name(Aiconfig::TypeBool(t)) << "' not packed\n";
        for (int t = Aiconfig::INT_FIRST; t <= Aiconfig::INT_LAST; ++t)
          if (!this->type_int[t - Aiconfig::INT_FIRST]->get_parent())
            cerr << "UI_GTKMM::Players::Player::AiConfig::create_container()\n"
              << "  config '" << name(Aiconfig::TypeInt(t)) << "' not packed\n";
        for (int t = Aiconfig::CARD_FIRST; t <= Aiconfig::CARD_LAST; t++)
          if (!this->type_card[t - Aiconfig::CARD_FIRST]->get_parent())
            cerr << "UI_GTKMM::Players::Player::AiConfig::create_container()\n"
              << "  config '" << name(Aiconfig::TypeCard(t)) << "' not packed\n";
      } // test, whether all configuration buttons are packed in a container
#endif
#endif

      return ;
    } // void Players::Player::AiConfig::create_container()

  /**
   ** adds a treeview for the heuristic group to the notbook
   **
   ** @param     notebook           notebook to add the treeview to
   ** @param     gametype_group     gametype group
   ** @param     playertype_group   playertype group
   ** @param     label_text         label translation
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::add_heuristic_treeview(Gtk::Notebook* notebook,
                                                      HeuristicsMap::GametypeGroup const
                                                      gametype_group,
                                                      HeuristicsMap::PlayerTypeGroup const
                                                      playertype_group,
                                                      Translator::Translation const& label_text)
    {
      Gtk::Label* label = Gtk::manage(new Gtk::Label("label"));
      this->ui->translations->add(*label, label_text);

      notebook->append_page(*this->create_heuristic_treeview(gametype_group,
                                                             playertype_group),
                            *label);
      return ;
    } // void Players::Player::AiConfig::add_heuristic_treeview(Gtk::Notebook* notebook, HeuristicsMap::GametypeGroup gametype_group, HeuristicsMap::PlayerTypeGroup playertype_group, Translator::Translation label)

  /**
   ** adds a treeview for the heuristic group to the notbook
   ** a subpage for the re player and the contra player
   **
   ** @param     notebook           notebook to add the treeview to
   ** @param     gametype_group     gametype group
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::add_heuristic_treeviews_re_contra(Gtk::Notebook* notebook,
                                                                 HeuristicsMap::GametypeGroup const gametype_group)
    {
      Gtk::Label* label = Gtk::manage(new Gtk::Label("label"));
      this->ui->translations->add(*label,
                                  ::translation(gametype_group));

      Gtk::Notebook* subnotebook = Gtk::manage(new Gtk::Notebook());
      this->notebooks.push_back(subnotebook);

      this->add_heuristic_treeview(subnotebook,
                                   gametype_group,
                                   HeuristicsMap::RE,
                                   ::translation(HeuristicsMap::RE));
      this->add_heuristic_treeview(subnotebook,
                                   gametype_group,
                                   HeuristicsMap::CONTRA,
                                   ::translation(HeuristicsMap::CONTRA));

      notebook->append_page(*subnotebook, *label);

      return ;
    } // void Players::Player::AiConfig::add_heuristic_treeviews_re_contra(Gtk::Notebook* notebook, HeuristicsMap::GametypeGroup gametype_group)

  /**
   ** adds a treeview for the heuristic group to the notbook
   ** a subpage for the special player, the re player and the contra player
   **
   ** @param     notebook           notebook to add the treeview to
   ** @param     gametype_group     gametype group
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::add_heuristic_treeviews_special_re_contra(Gtk::Notebook* notebook,
                                                                         HeuristicsMap::GametypeGroup const gametype_group)
    {
      Gtk::Label* label = Gtk::manage(new Gtk::Label("label"));
      this->ui->translations->add(*label,
                                  ::translation(gametype_group));

      Gtk::Notebook* subnotebook = Gtk::manage(new Gtk::Notebook());
      this->notebooks.push_back(subnotebook);

      this->add_heuristic_treeview(subnotebook,
                                   gametype_group,
                                   HeuristicsMap::SPECIAL,
                                   ::translation(HeuristicsMap::SPECIAL));
      this->add_heuristic_treeview(subnotebook,
                                   gametype_group,
                                   HeuristicsMap::RE,
                                   ::translation(HeuristicsMap::RE));
      this->add_heuristic_treeview(subnotebook,
                                   gametype_group,
                                   HeuristicsMap::CONTRA,
                                   ::translation(HeuristicsMap::CONTRA));

      notebook->append_page(*subnotebook, *label);

      return ;
    } // void Players::Player::AiConfig::add_heuristic_treeviews_special_re_contra(Gtk::Notebook* notebook, HeuristicsMap::GametypeGroup gametype_group)

  /**
   ** creates a treeview for the given gametype/playertype combination
   **
   ** @param     gametype_group     gametype group
   ** @param     playertype_group   playertype group
   **
   ** @return    a scrolled window containing the treeview
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  Gtk::Widget*
    Players::Player::AiConfig::create_heuristic_treeview(HeuristicsMap::GametypeGroup const
                                                         gametype_group,
                                                         HeuristicsMap::PlayerTypeGroup const
                                                         playertype_group)
    {
      Gtk::Box* box = Gtk::manage(new Gtk::VBox(false, 0));
      box->signal_realize().connect(sigc::bind<Gtk::Box* const, HeuristicsMap::GametypeGroup const, HeuristicsMap::PlayerTypeGroup const>(sigc::mem_fun(*this,
                                                                                                                                                        &Players::Player::AiConfig::init_heuristic_treeview), box, gametype_group, playertype_group));

      return box;
    } // Gtk::Widget* Players::Player::AiConfig::create_heuristic_treeview(HeuristicsMap::GametypeGroup gametype_group, HeuristicsMap::PlayerTypeGroup playertype_group)

  /**
   ** creates a treeview for the given gametype/playertype combination
   **
   ** @param     gametype_group     gametype group
   ** @param     playertype_group   playertype group
   **
   ** @return    a scrolled window containing the treeview
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::init_heuristic_treeview(Gtk::Box* const box,
                                                       HeuristicsMap::GametypeGroup const
                                                       gametype_group,
                                                       HeuristicsMap::PlayerTypeGroup const
                                                       playertype_group)
    {
      HeuristicsMap::Key key(gametype_group, playertype_group);

      { // default button
        this->heuristics_default_button[key]
          = Gtk::manage(new Gtk::CheckButton("default"));
        Gtk::ToggleButton& default_button
          = *this->heuristics_default_button[key];
        this->ui->translations->add(default_button,
                                    ::translation("heuristics as in normal game"));

        if (   (gametype_group != HeuristicsMap::NORMAL)
            && (key != HeuristicsMap::Key::DEFAULT) ) {
          Gtk::Alignment* alignment
            = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                             Gtk::ALIGN_CENTER));
          alignment->set_border_width(1 ex);
          alignment->add(default_button);
          box->pack_start(*alignment, Gtk::PACK_SHRINK);
        } // if (key != HeuristicsMap::Key::DEFAULT)
      } // default button

      { // table
        Gtk::Box* box_treeview = Gtk::manage(new Gtk::HBox(false, 0));
        { // treeview
          HeuristicsModel*& model = this->heuristics_model;
          Glib::RefPtr<Gtk::ListStore>& store = this->heuristics_list[key];
          Gtk::TreeView*& treeview = heuristics_treeview[key];

          vector<Aiconfig::HeuristicState> const& states
            = this->aiconfig_const().heuristic_states(key);

          store = Gtk::ListStore::create(*model);
          treeview = Gtk::manage(new Gtk::TreeView(store));
          treeview->set_property("reorderable", true);
          treeview->set_property("headers_visible", false);

          treeview->append_column_editable("active", model->active);
          treeview->get_column_cell_renderer(0)->set_property("xalign", 0.5);
          treeview->append_column("heuristic", model->heuristic_name);
          treeview->get_column_cell_renderer(1)->set_property("xalign", 0);

          { // create the rows
            for (vector<Aiconfig::HeuristicState>::const_iterator
                 s = states.begin();
                 s != states.end();
                 ++s) {
              Gtk::TreeModel::Row row = *(store->append());

              Aiconfig::Heuristic const heuristic
                = static_cast<Aiconfig::Heuristic>(s->heuristic);
              if (!Aiconfig::is_real(heuristic))
                continue;

              row[model->active] = this->aiconfig().value(key, s->heuristic);
              row[model->heuristic] = heuristic;
              row[model->heuristic_name] = ::translation(heuristic).translation();
              row[model->gametype_group] = gametype_group;
              row[model->playertype_group] = playertype_group;
            } // for (s \in states
          } // create the rows

          { // events
            store->signal_row_changed().connect(sigc::mem_fun(*this, &AiConfig::heuristics_row_changed_event));
            store->signal_row_deleted().connect(sigc::bind<Glib::RefPtr<Gtk::ListStore> >(sigc::mem_fun(*this, &AiConfig::heuristics_row_deleted_event), store));
            treeview->get_selection()->signal_changed().connect(sigc::bind<HeuristicsMap::Key const>(sigc::mem_fun(*this, &AiConfig::update_heuristic_up_down_buttons), key));
            treeview->get_selection()->signal_changed().connect(sigc::bind<HeuristicsMap::Key const>(sigc::mem_fun(*this, &AiConfig::update_heuristic_description), key));
          } // events

          Gtk::ScrolledWindow* scrolled_window
            = Gtk::manage(new Gtk::ScrolledWindow);
          scrolled_window->set_policy(Gtk::POLICY_NEVER,
                                      Gtk::POLICY_AUTOMATIC);
          scrolled_window->add(*treeview);

          box_treeview->pack_start(*scrolled_window, Gtk::PACK_SHRINK);
        } // treeview
        { // right column
          Gtk::Box* box3
            = Gtk::manage(new Gtk::VBox(false, 1 ex));
          box3->set_border_width(1 ex);
          { // move up/down buttons
            Gtk::Alignment* alignment
              = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_LEFT,
                                               Gtk::ALIGN_CENTER,
                                               0, 0));

            Gtk::Box* button_box
              = Gtk::manage(new Gtk::VButtonBox(Gtk::BUTTONBOX_DEFAULT_STYLE,
                                                1 ex));

            this->heuristic_up_button[key]
              = Gtk::manage(new Gtk::StockButton(Gtk::Stock::GO_UP, "up"));
            this->ui->translations->add(*this->heuristic_up_button[key]->label,
                                        ::translation("move up"));
            this->heuristic_down_button[key]
              = Gtk::manage(new Gtk::StockButton(Gtk::Stock::GO_DOWN, "down"));
            this->ui->translations->add(*this->heuristic_down_button[key]->label,
                                        ::translation("move down"));

            button_box->add(*this->heuristic_up_button[key]);
            button_box->add(*this->heuristic_down_button[key]);

            alignment->add(*button_box);
            box3->pack_start(*alignment, Gtk::PACK_SHRINK);

            this->update_heuristic_up_down_buttons(key);
            this->heuristic_up_button[key]->signal_clicked().connect(sigc::bind<HeuristicsMap::Key>(sigc::mem_fun(*this, &AiConfig::heuristic_move_up_event), key));
            this->heuristic_down_button[key]->signal_clicked().connect(sigc::bind<HeuristicsMap::Key>(sigc::mem_fun(*this, &AiConfig::heuristic_move_down_event), key));
          } // move up/down buttons
          { // description
            Gtk::TextView* description_view = Gtk::manage(new Gtk::TextView());
            this->heuristic_description[key] = description_view;

            description_view->set_editable(false);
            description_view->set_wrap_mode(Gtk::WRAP_WORD);

            description_view->get_buffer()->set_text("description");
            this->ui->translations->add(*description_view, ::translation(""));

            Gtk::ScrolledWindow* scrolled_window
              = Gtk::manage(new Gtk::ScrolledWindow);
            scrolled_window->set_policy(Gtk::POLICY_NEVER,
                                        Gtk::POLICY_AUTOMATIC);
            scrolled_window->add(*description_view);
            box3->add(*scrolled_window);
          } // description
          box_treeview->add(*box3);
        } // right column
        box->add(*box_treeview);
      } // table
      box->show_all();

      return ;
    } // void Players::Player::AiConfig::init_heuristic_treeview(Gtk::Box* box, HeuristicsMap::GametypeGroup gametype_group, HeuristicsMap::PlayerTypeGroup playertype_group)

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the ai configuration
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  ::Aiconfig&
    Players::Player::AiConfig::aiconfig()
    {
      return dynamic_cast<Aiconfig&>(this->player->player());
    } // ::Aiconfig& Players::Player::AiConfig::aiconfig()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the ai configuration (const version)
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  ::Aiconfig const&
    Players::Player::AiConfig::aiconfig_const() const
    {
      return dynamic_cast<Aiconfig&>(this->player->player());
    } // ::Aiconfig const& Players::Player::AiConfig::aiconfig_const() const

  /**
   ** sets the aiconfig to the selected difficulty
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::difficulty_changed()
    {
      ::Aiconfig::Difficulty const difficulty
        = this->difficulty_selector->type();
      if (difficulty == ::Aiconfig::CUSTOM)
        return ;

      if (difficulty == this->aiconfig().difficulty())
        return ;

      for (unsigned t = 1; t < this->aitype.size(); ++t)
        this->aitype_as_previous[t]->set_active(true);

      this->aiconfig().set_to_difficulty(difficulty);
      this->update();

      for (unsigned t = 1; t < this->aitype.size(); ++t)
        if (   (this->aiconfig().aitype(t)
                == this->aiconfig().aitype(t - 1))
            && (this->aiconfig().rating(t)
                == this->aiconfig().rating(t - 1))
            && (this->aiconfig().future_limit(t)
                == this->aiconfig().future_limit(t - 1)) )
          this->aitype_as_previous[t]->set_active(false);

      return ;
    } // void Players::Player::AiConfig::difficulty_changed()

  /**
   ** the aitype has changed
   **
   ** @param     trickno   	the trick number of the changed type
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.7
   **/
  void
    Players::Player::AiConfig::aitype_changed(unsigned const trickno)
    {
      this->aiconfig().enter_update();

      AiType const aitype
        = *static_cast<AiType*>(this->aitype[trickno]->get_menu()->get_active()->get_data("aitype"));
      this->aiconfig().set_aitype(trickno, aitype);

      if (   (trickno < this->aitype_as_previous.size() - 1)
          && !this->aitype_as_previous[trickno + 1]->get_active()) {
        this->aitype[trickno + 1]->set_history(this->aitype[trickno]->get_history());
      }

      this->aiconfig().leave_update();

      if (trickno == 0)
        this->sensitivity_update();

      return ;
    } // void Players::Player::AiConfig::aitype_changed(unsigned const trickno)

  /**
   ** the rating has changed
   **
   ** @param     trickno   	the trick number of the changed type
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::rating_changed(unsigned const trickno)
    {
      this->aiconfig().enter_update();

      Rating::Type const rating
        = *static_cast<Rating::Type*>(this->rating[trickno]->get_menu()->get_active()->get_data("rating"));
      this->aiconfig().set_rating(trickno, rating);

      if (   (trickno < this->aitype_as_previous.size() - 1)
          && !this->aitype_as_previous[trickno + 1]->get_active()) {
        this->rating[trickno + 1]->set_history(this->rating[trickno]->get_history());
      }

      this->aiconfig().leave_update();

      return ;
    } // void Players::Player::AiConfig::rating_changed(unsigned const trickno)

  /**
   ** the future limit has changed
   **
   ** @param     trickno   	the trick number of the changed future limit
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.7
   **/
  void
    Players::Player::AiConfig::future_limit_changed(unsigned const trickno)
    {
      this->aiconfig().enter_update();

      unsigned const future_limit
        = static_cast<unsigned>(this->future_limit[trickno]->get_value());
      this->aiconfig().set_future_limit(trickno, future_limit);

      if ( (trickno < this->aitype_as_previous.size() - 1)
          && !this->aitype_as_previous[trickno + 1]->get_active())
        this->future_limit[trickno + 1]->set_value(future_limit);

      this->aiconfig().leave_update();

      return ;
    } // void Players::Player::AiConfig::future_limit_changed(unsigned const trickno)

  /**
   ** the aitype has changed
   **
   ** @param     trickno   	the trick number of the changed type
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::aitype_as_previous_changed(unsigned const trickno)
    {
      DEBUG_ASSERTION(trickno > 0,
                      "AiConfig::aitype_as_previous_changed(trickno):\n"
                      "  'trickno' == 0");

      bool const active = this->aitype_as_previous[trickno]->get_active();
      this->aitype[trickno]->set_sensitive(active);
      this->rating[trickno]->set_sensitive(active);
      this->future_limit[trickno]->set_sensitive(active);

      if (!active) {
        this->aitype[trickno]->set_history(this->aitype[trickno - 1]->get_history());
        this->rating[trickno]->set_history(this->rating[trickno - 1]->get_history());
        this->future_limit[trickno]->set_value(this->future_limit[trickno - 1]->get_value());
      } // if (!active)

      return ;
    } // void Players::Player::AiConfig::aitype_as_previous_changed(unsigned const trickno)

  /**
   ** a aiconfig has been changed by the user
   **
   ** @param     type   the type of the aiconfig
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    Players::Player::AiConfig::change(int const type)
    {
      if ((type >= Aiconfig::BOOL_FIRST)
          && (type <= Aiconfig::BOOL_LAST))
        this->aiconfig().set(Aiconfig::TypeBool(type),
                             this->type_bool[type - Aiconfig::BOOL_FIRST]->get_active());
      else if ((type >= Aiconfig::INT_FIRST)
               && (type <= Aiconfig::INT_LAST)) {
        if (this->type_int[type - Aiconfig::INT_FIRST]->is_realized())
          this->aiconfig().set(Aiconfig::TypeInt(type),
                               this->type_int[type - Aiconfig::INT_FIRST]->get_value_as_int());
      }
      else if ((type >= Aiconfig::CARD_FIRST)
               && (type <= Aiconfig::CARD_LAST)) {
        if (this->type_card[type - Aiconfig::CARD_FIRST]->is_realized())
          this->aiconfig().set(Aiconfig::TypeCard(type),
                               this->type_card[type - Aiconfig::CARD_FIRST]->card());
      }
      else 
        DEBUG_ASSERTION(false,
                        "AiConfig::change(type):\n"
                        "  type '" << type << "' unknown.");

      this->difficulty_selector->set_type(this->aiconfig().difficulty());

      return ;
    } // void Players::Player::AiConfig::change(int const type)

  /**
   ** a row has changed
   **
   ** @param     path    path to the changed row
   ** @param     row     changed row
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::heuristics_row_changed_event(Gtk::TreeModel::Path const& path,
                                                            Gtk::TreeModel::iterator const& row)

    {
      if (!this->heuristics_signals_active)
        return ;

      HeuristicsModel& model = *this->heuristics_model;

      HeuristicsMap::Key const key(static_cast<HeuristicsMap::GametypeGroup>(static_cast<int>((*row)[model.gametype_group])),
                                   static_cast<HeuristicsMap::PlayerTypeGroup>(static_cast<int>((*row)[model.playertype_group])));

      this->aiconfig().set(key,
                           static_cast<Aiconfig::Heuristic>(static_cast<int>((*row)[model.heuristic])),
                           (*row)[model.active]);

      this->update_heuristics_default_button_state(key);

      return ;
    } // void Players::Player::AiConfig::heuristics_row_changed_event(Gtk::TreeModel::Path path, Gtk::TreeModel::iterator row, Glib::RefPtr<Gtk::ListStore> store)

  /**
   ** a row has been deleted
   ** This is used for recalculating the order of the rows.
   ** Assumptions are:
   ** * this is function is called last in the reordering
   ** * the indices of 'TreeModel::Path::get_indices()' are so that the first
   **   one indicates the row number (counted from 0 on)
   **
   ** @param     path    path to the changed row
   ** @param     store   corresponding store
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **
   ** @bug      too many uncertain assumptions (see description)
   **/
  void
    Players::Player::AiConfig::heuristics_row_deleted_event(Gtk::TreeModel::Path const& path,
                                                            Glib::RefPtr<Gtk::ListStore> store)
    {
      if (!this->heuristics_signals_active)
        return ;

      HeuristicsModel& model = *this->heuristics_model;

      Gtk::TreeModel::iterator row = store->get_iter("0");
      HeuristicsMap::Key const key(static_cast<HeuristicsMap::GametypeGroup>(static_cast<int>((*row)[model.gametype_group])),
                                   static_cast<HeuristicsMap::PlayerTypeGroup>(static_cast<int>((*row)[model.playertype_group])));
      for (; row; ++row) {
        this->aiconfig().move(key,
                              static_cast<Aiconfig::Heuristic>(static_cast<int>((*row)[model.heuristic])),
                              store->get_path(row).front()
                             );
      }
      this->update_heuristics_default_button_state(key);

      return ;
    } // void Players::Player::AiConfig::heuristics_row_deleted_event(Gtk::TreeModel::Path path, Glib::RefPtr<Gtk::ListStore> store)

  /**
   ** update the toggled state of the 'default' button of 'key'
   **
   ** @param     key      key of the heuristic
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::update_heuristics_default_button_state(HeuristicsMap::Key const& key)
    {
      bool const equal_to_default
        = (this->aiconfig_const().heuristic_states(key)
           == this->aiconfig_const().heuristic_states(HeuristicsMap::Key::DEFAULT));

      this->heuristics_default_button[key]->set_active(equal_to_default);
      this->heuristics_default_button[key]->set_sensitive(!equal_to_default);

      return ;
    } // void Players::Player::AiConfig::update_heuristics_default_button_state(HeuristicsMap::Key const& key)

  /**
   ** the default button for the heuristics of 'key' has been toggled
   **
   ** @param     key      key of the heuristic
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::heuristics_default_button_toggled_event(HeuristicsMap::Key const key)
    {
      if (   this->heuristics_default_button[key]->get_active()
          && (this->aiconfig_const().heuristic_states(key)
              != this->aiconfig_const().heuristic_states(HeuristicsMap::Key::DEFAULT))
        ) {
          this->aiconfig().set_to_default(key);
          this->update();
        }

      return ;
    } // void Players::Player::AiConfig::heuristics_default_button_toggled_event(HeuristicsMap::Key key)

  /**
   ** update the sensitivity of the up and down buttons
   **
   ** @param     key      key of the heuristic
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::update_heuristic_up_down_buttons(HeuristicsMap::Key const& key)
    {
      Glib::RefPtr<Gtk::TreeView::Selection> selection
        = this->heuristics_treeview[key]->get_selection();
      Glib::RefPtr<Gtk::ListStore> store
        = this->heuristics_list[key];

      DEBUG_ASSERTION((selection->count_selected_rows() <= 1),
                      "UI_GTKMM::Players::Player::AiConfig::update_heuristic_up_down_buttons()\n"
                      "  more than one line selected: (" << selection->count_selected_rows() << ") for key " << key.gametype_group << "-" << key.playertype_group);

      if (selection->count_selected_rows() == 0) {
        this->heuristic_up_button[key]->set_sensitive(false);
        this->heuristic_down_button[key]->set_sensitive(false);
        return ;
      } // if (selection->count_selected_rows() == 0)

      //Gtk::TreeModel::Row const& row = *selection->get_selected();
      Gtk::TreeModel::Path const& path = store->get_path(selection->get_selected());
      unsigned const selected_row_no = path.front();
      this->heuristic_up_button[key]->set_sensitive(selected_row_no > 0);
      this->heuristic_down_button[key]->set_sensitive(selected_row_no <
                                                      store->children().size()
                                                      - 1);


      return ;
    } // void Players::Player::AiConfig::update_heuristic_up_down_buttons(HeuristicsMap::Key key)

  /**
   ** update the description of the active heuristic
   **
   ** @param     key      key of the heuristic
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::update_heuristic_description(HeuristicsMap::Key const& key)
    {
      Glib::RefPtr<Gtk::TreeView::Selection> selection
        = this->heuristics_treeview[key]->get_selection();
      Glib::RefPtr<Gtk::ListStore> store
        = this->heuristics_list[key];

      DEBUG_ASSERTION((selection->count_selected_rows() <= 1),
                      "UI_GTKMM::Players::Player::AiConfig::update_heuristic_description()\n"
                      "  more than one line selected: (" << selection->count_selected_rows() << ") for key " << key.gametype_group << "-" << key.playertype_group);

      if (selection->count_selected_rows() == 0) {
        this->ui->translations->change(*this->heuristic_description[key],
                                       ::translation(""));
        return ;
      } // if (selection->count_selected_rows() == 0)

      Gtk::TreeModel::Row const& row = *selection->get_selected();
      this->ui->translations->change(*this->heuristic_description[key],
                                     ::translation_description(static_cast<Aiconfig::Heuristic>(static_cast<int>(row[this->heuristics_model->heuristic]))));

      return ;
    } // void Players::Player::AiConfig::update_heuristic_description(HeuristicsMap::Key key)

  /**
   ** move the selected heuristic up
   **
   ** @param     key      key of the heuristic
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::heuristic_move_up_event(HeuristicsMap::Key const key)
    {
      Gtk::TreeView& treeview = *this->heuristics_treeview[key];
      Glib::RefPtr<Gtk::TreeView::Selection> selection
        = treeview.get_selection();

      DEBUG_ASSERTION((selection->count_selected_rows() == 1),
                      "UI_GTKMM::Players::Player::AiConfig::heuristic_move_up_event()\n"
                      "  not one line selected but '" << selection->count_selected_rows() << "' for key " << key.gametype_group << "-" << key.playertype_group);
      Glib::RefPtr<Gtk::ListStore> store = this->heuristics_list[key];
      Gtk::TreeModel::Path const& path = store->get_path(selection->get_selected());
      unsigned const selected_row_no = path.front();
      DEBUG_ASSERTION((selected_row_no > 0),
                      "UI_GTKMM::Players::Player::AiConfig::heuristic_move_up_event()\n"
                      "  selected row is '0'");

      store->iter_swap(selection->get_selected(),
                       --selection->get_selected());
#ifdef WORKAROUND
      // call this so that the aiconfig gets noted about the change
      this->heuristics_row_deleted_event(path, store);
      // and update the sensitivity of the buttons
      this->update_heuristic_up_down_buttons(key);
#endif
      // make the selected row be visible
      treeview.scroll_to_row(store->get_path(treeview.get_selection()->get_selected()));

      return ;
    } // void Players::Player::AiConfig::heuristic_move_up_event(HeuristicsMap::Key key)

  /**
   ** move the selected heuristic down
   **
   ** @param     key      key of the heuristic
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::heuristic_move_down_event(HeuristicsMap::Key const key)
    {
      Gtk::TreeView& treeview = *this->heuristics_treeview[key];
      Glib::RefPtr<Gtk::TreeView::Selection> selection
        = treeview.get_selection();

      DEBUG_ASSERTION((selection->count_selected_rows() == 1),
                      "UI_GTKMM::Players::Player::AiConfig::heuristic_move_up_event()\n"
                      "  not one line selected but '" << selection->count_selected_rows() << "' for key " << key.gametype_group << "-" << key.playertype_group);

      Glib::RefPtr<Gtk::ListStore> store = this->heuristics_list[key];
      Gtk::TreeModel::Path const& path = store->get_path(selection->get_selected());
      unsigned const selected_row_no = path.front();
      DEBUG_ASSERTION((selected_row_no < store->children().size() - 1),
                      "UI_GTKMM::Players::Player::AiConfig::heuristic_move_up_event()\n"
                      "  selected row is too great: " << selected_row_no
                      << " >= " << store->children().size() << " = num rows");

      store->iter_swap(selection->get_selected(),
                       ++selection->get_selected());
#ifdef WORKAROUND
      // call this so that the aiconfig gets noted about the change
      this->heuristics_row_deleted_event(path, store);
      // and update the sensitivity of the buttons
      this->update_heuristic_up_down_buttons(key);
#endif
      // make the selected row be visible
      treeview.scroll_to_row(store->get_path(treeview.get_selection()->get_selected()));

      return ;
    } // void Players::Player::AiConfig::heuristic_move_down_event(HeuristicsMap::Key key)

  /**
   ** sets the ai to the hardcoded
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::hardcoded()
    {
      for (unsigned t = 1; t < this->aitype.size(); ++t)
        this->aitype_as_previous[t]->set_active(true);

      this->aiconfig().reset_to_hardcoded();
      this->update();

      for (unsigned t = 1; t < this->aitype.size(); ++t)
        if (   (this->aiconfig().aitype(t)
                == this->aiconfig().aitype(t - 1))
            && (this->aiconfig().rating(t)
                == this->aiconfig().rating(t - 1))
            && (this->aiconfig().future_limit(t)
                == this->aiconfig().future_limit(t - 1)) )
          this->aitype_as_previous[t]->set_active(false);


      return ;
    } // void Players::Player::AiConfig::hardcoded()

  /**
   ** creates a menu with the players to adopt from
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
    Players::Player::AiConfig::adopt()
    {

      Gtk::Menu* adopt_menu
        = Gtk::manage(new Gtk::Menu());

      for (unsigned p = 0; p < this->ui->party().playerno(); p++)
        if (p != this->player->playerno()) {
          Gtk::MenuItem* item
            = Gtk::manage(new Gtk::MenuItem(this->player->players->player(p).player().name()));
          item->signal_activate().connect(sigc::bind<unsigned const>(sigc::mem_fun(*this, &AiConfig::adopt_aiconfig), p));
          adopt_menu->append(*item);
        } // if (p != this->player().playerno())

      adopt_menu->show_all();
      adopt_menu->popup(0, 0);

      return ;
    } // void Players::Player::AiConfig::adopt()

  /**
   ** adopt the aiconfig from player 'playerno'
   **
   ** @param     playerno   player to adopt from
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  void
    Players::Player::AiConfig::adopt_aiconfig(unsigned const playerno)
    {
#ifdef RELEASE
      DEBUG_ASSERTION((::game_status == GAMESTATUS::PARTY_NEW),
                      "Players::Player::AiConfig::adopt_aiconfig():\n"
                      "  game status is not party_init: "
                      << ::game_status);
#endif
      for (unsigned t = 1; t < this->aitype.size(); ++t)
        this->aitype_as_previous[t]->set_active(true);

      this->aiconfig()
        = this->player->players->player(playerno).aiconfig().aiconfig();
      this->update();

      for (unsigned t = 1; t < this->aitype.size(); ++t)
        if (   (this->aiconfig().aitype(t)
                == this->aiconfig().aitype(t - 1))
            && (this->aiconfig().rating(t)
                == this->aiconfig().rating(t - 1))
            && (this->aiconfig().future_limit(t)
                == this->aiconfig().future_limit(t - 1)) )
          this->aitype_as_previous[t]->set_active(false);

      return ;
    } // void Players::Player::AiConfig::adopt_aiconfig(unsigned const playerno)

  /**
   ** make all other aiconfigs like this
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
    Players::Player::AiConfig::allot()
    {
      for (unsigned p = 0; p < ::party.playerno(); p++)
        if (p != this->player->playerno())
          this->player->players->player(p).aiconfig().adopt_aiconfig(this->player->playerno());

      return ;
    } // void Players::Player::AiConfig::allot()

  /**
   ** creates a menu with the files in the aiconfig directories
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
    Players::Player::AiConfig::load()
    {

      Gtk::FileMenu* load_menu
        = Gtk::manage(new Gtk::FileMenu(sigc::mem_fun(*this, &AiConfig::load_file)));
      load_menu->add_directory(::setting(Setting::PRIVATE_DATA_DIRECTORY)
                               + "/"
                               + ::setting(Setting::AI_DIRECTORY));
      load_menu->add_directory(::setting(Setting::PUBLIC_DATA_DIRECTORY)
                               + "/"
                               + ::setting(Setting::AI_DIRECTORY));

      load_menu->show();

      return ;
    } // void Players::Player::AiConfig::load()

  /**
   ** loads the aiconfig
   **
   ** @param     filename   the aiconfig file
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    Players::Player::AiConfig::load_file(string const filename)
    {
      for (unsigned t = 1; t < this->aitype.size(); ++t)
        this->aitype_as_previous[t]->set_active(true);

      this->aiconfig().load(filename);
      this->update();

      for (unsigned t = 1; t < this->aitype.size(); ++t)
        if (   (this->aiconfig().aitype(t)
                == this->aiconfig().aitype(t - 1))
            && (this->aiconfig().rating(t)
                == this->aiconfig().rating(t - 1))
            && (this->aiconfig().future_limit(t)
                == this->aiconfig().future_limit(t - 1)) )
          this->aitype_as_previous[t]->set_active(false);

      return ;
    } // void Players::Player::AiConfig::load_file(string const filename)

  /**
   ** saves the aiconfig.
   ** The filename is taken from 'save_file_chooser'.
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
    Players::Player::AiConfig::save()
    {
      this->aiconfig().save(this->save_file_chooser->get_filename()); 

      return ;
    } // void Players::Player::AiConfig::save()

  /**
   ** update the sensitivity of all widgets
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **
   ** @todo     heuristics
   **/
  void
    Players::Player::AiConfig::sensitivity_update()
    {
      bool sensitive = (::game_status == GAMESTATUS::PARTY_NEW);
#ifndef RELEASE
      sensitive = true;
#endif
#ifdef USE_NETWORK
      sensitive &= (this->player->player().type() != ::Player::NETWORK);
#endif

      this->difficulty_button->set_sensitive(sensitive);
      this->adopt_button->set_sensitive(sensitive);
      this->allot_button->set_sensitive(sensitive);
      this->load_button->set_sensitive(sensitive);
      this->save_button->set_sensitive(true);
      this->difficulty_selector->set_sensitive(sensitive);

      for (vector<Gtk::CheckButton*>::iterator t = this->type_bool.begin();
           t != this->type_bool.end();
           t++)
        (*t)->set_sensitive(sensitive);
      for (vector<Gtk::LabelSpinButton*>::iterator
           t = this->type_int.begin();
           t != this->type_int.end();
           t++)
        (*t)->set_sensitive(sensitive);
      for (vector<Gtk::LabelCardSelector*>::iterator
           t = this->type_card.begin();
           t != this->type_card.end();
           t++)
        (*t)->set_sensitive(sensitive);

      for (unsigned t = 0; t < this->aitype.size(); ++t) {
        if (   (t == 0)
            || this->aitype_as_previous[t]->get_active() ) {
          this->aitype[t]->set_sensitive(sensitive);
          this->rating[t]->set_sensitive(::supports_rating(this->aiconfig().aitype(t))
                                         ? sensitive
                                         : false);
          this->future_limit[t]->set_sensitive(sensitive);
          this->aitype_as_previous[t]->set_sensitive(sensitive);
        } else {
          this->aitype[t]->set_sensitive(false);
          this->rating[t]->set_sensitive(false);
          this->future_limit[t]->set_sensitive(false);
        }
      } // for (unsigned t = 1; t < this->aitype.size(); ++t)

      for (std::map<HeuristicsMap::Key, Gtk::ToggleButton*>::iterator
           m = this->heuristics_default_button.begin();
           m != this->heuristics_default_button.end();
           ++m)
        m->second->set_sensitive(sensitive);
      for (std::map<HeuristicsMap::Key, Gtk::TreeView*>::iterator
           m = this->heuristics_treeview.begin();
           m != this->heuristics_treeview.end();
           ++m)
        m->second->set_sensitive(sensitive);

      return ;
    } // void Players::Player::AiConfig::sensitivity_update()

  /**
   ** update the aiconfig if it irefers to 'aiconfig'
   **
   ** @param     aiconfig   aiconfig to update
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **/
  void
    Players::Player::AiConfig::update(::Aiconfig const& aiconfig)
    {
      if (&this->aiconfig() == &aiconfig)
        this->update();
      return ;
    } // void Players::Player::AiConfig::update(::Aiconfig aiconfig)

  /**
   ** update all widgets
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.2
   **/
  void
    Players::Player::AiConfig::update()
    {
      this->difficulty_selector->set_type(this->aiconfig().difficulty());
      this->difficulty_description_update();

      for (unsigned t = 0; t < this->aitype.size(); ++t) {
        if ((t < this->aitype.size() - 1)
            && (   (this->aiconfig().aitype(t + 1)
                    != this->aiconfig().aitype(t))
                || (this->aiconfig().future_limit(t + 1)
                    != this->aiconfig().future_limit(t))))
          this->aitype_as_previous[t + 1]->set_active(true);
        // Here '- 1' since the aitype 'NO_CHOOSEBESTCARD' is not shown.
        this->aitype[t]->set_history(this->aiconfig().aitype(t) - 1);
        this->rating[t]->set_history(this->aiconfig().rating(t));
        this->future_limit[t]->set_value(this->aiconfig().future_limit(t));
      }

      for (vector<Gtk::CheckButton*>::iterator widget = this->type_bool.begin();
           widget != this->type_bool.end();
           widget++)
        (*widget)->set_active(this->aiconfig().value(*static_cast<Aiconfig::TypeBool*>((*widget)->get_data("type"))));
      for (vector<Gtk::LabelSpinButton*>::iterator widget
           = this->type_int.begin();
           widget != this->type_int.end();
           widget++) {
        Aiconfig::TypeInt type
          = *static_cast<Aiconfig::TypeInt*>((*widget)->get_data("type"));
        (*widget)->set_range(this->aiconfig().min(type),
                             this->aiconfig().max(type));
        (*widget)->set_value(this->aiconfig().value(type));
      } // for (widget \in this->type_int)
      for (vector<Gtk::LabelCardSelector*>::iterator
           widget = this->type_card.begin();
           widget != this->type_card.end();
           widget++) {
        Aiconfig::TypeCard type
          = *static_cast<Aiconfig::TypeCard*>((*widget)->get_data("type"));
        (*widget)->set_card(this->aiconfig().value(type));
      } // for (widget \in this->type_card)

      { // heuristics
        this->heuristics_signals_active = false;
        HeuristicsModel& model = *this->heuristics_model;
        for (std::map<HeuristicsMap::Key, Glib::RefPtr<Gtk::ListStore> >::iterator
             m = this->heuristics_list.begin();
             m != this->heuristics_list.end();
             ++m) {
          HeuristicsMap::Key const& key = m->first;
          vector<Aiconfig::HeuristicState> const& states
            = this->aiconfig_const().heuristic_states(key);
          Glib::RefPtr<Gtk::ListStore>& store = m->second;

          { // create the rows
            store->clear();
            for (vector<Aiconfig::HeuristicState>::const_iterator
                 s = states.begin();
                 s != states.end();
                 ++s) {
              Gtk::TreeModel::Row row = *store->append();

              row[model.active] = this->aiconfig().value(key, s->heuristic);
              row[model.heuristic] = s->heuristic;
              row[model.heuristic_name] = ::translation(s->heuristic).translation();
              row[model.gametype_group] = key.gametype_group;
              row[model.playertype_group] = key.playertype_group;

            } // for (s \in states
          } // create the rows

          this->update_heuristics_default_button_state(key);
        } // for (m \in this->heuristics_list)
        this->heuristics_signals_active = true;
      } // heuristics

      return ;
    } // void Players::Player::AiConfig::update()

  /**
   ** update the name
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **
   ** @todo	block 'Entry::change_signal()'
   **/
  void
    Players::Player::AiConfig::name_update()
    {
      this->ui->translations->change(*this->save_file_chooser,
                                     ::translation("~save aiconfig of %splayer%",
                                                   this->player->player().name()));

      return ;
    } // void Players::Player::AiConfig::name_update()

  /**
   ** update the description of the difficulty according to the player type
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.4
   **
   ** @bug       the un-sensitivity is not accepted when called before the container has been drawn (gtkmm bug?)
   **/
  void
    Players::Player::AiConfig::difficulty_description_update()
    {
      // change the name of the difficulty button
      switch (this->player->player().type()) {
      case ::Player::UNSET:
        DEBUG_ASSERTION(false,
                        "UI_GTKMM::Players::Player::AiConfig::type_change(type)\n"
                        "  type is 'unset'");
        break;
      case ::Player::HUMAN:
      case ::Player::AI:
      case ::Player::NETWORK:
        this->difficulty_selector->set_sensitive(true);
        this->container->set_sensitive(true);
        switch (this->player->player().type()) {
        case ::Player::HUMAN:
          this->ui->translations->change(*this->difficulty_label,
                                         ::translation("card suggestion") + ": ");
          break;
        case ::Player::AI:
          this->ui->translations->change(*this->difficulty_label,
                                         ::translation("difficulty") + ": ");
          break;
        case ::Player::NETWORK:
          this->ui->translations->change(*this->difficulty_label,
                                         ::translation("replacement ai") + ": ");
          break;
        default:
          break;
        } // switch (this->player->player().type())
        break;
      case ::Player::AI_DUMMY:
      case ::Player::AI_RANDOM:
        this->ui->translations->change(*this->difficulty_label,
                                       ::translation("no ai configuration")
                                       + ": ");
        this->difficulty_selector->set_sensitive(false);
        this->container->set_sensitive(false);
        break;
      } // switch (this->player->player().type())

      return ;
    } // void Players::Player::AiConfig::difficutly_description_update()

  /**
   ** update the language
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **
   ** @todo      update the texts in the difficulty selector without emmitting a signal (-> misc/type_selector.hpp ?)
   **/
  void
    Players::Player::AiConfig::language_update()
    {
#ifndef POSTPONED
      // problem with the signal
      this->difficulty_selector->language_update();
#endif

      for (vector<Gtk::LabelCardSelector*>::iterator
           s = this->type_card.begin();
           s != this->type_card.end();
           ++s)
        (*s)->language_update();

      { // heuristics
        HeuristicsModel& model = *this->heuristics_model;
        for (std::map<HeuristicsMap::Key, Glib::RefPtr<Gtk::ListStore> >::iterator
             m = this->heuristics_list.begin();
             m != this->heuristics_list.end();
             ++m) {
          Gtk::TreeModel::Children children = m->second->children();
          for (Gtk::TreeModel::Children::iterator row = children.begin();
               row != children.end();
               ++row) {
            (*row)[model.heuristic_name]
              = ::translation(static_cast<Aiconfig::Heuristic>(static_cast<int>((*row)[model.heuristic]))).translation();
          }
        } // for (m \in this->heuristics_list)
      } // heuristics

      return ;
    } // void Players::Player::AiConfig::language_update()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
