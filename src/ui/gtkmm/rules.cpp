/**********************************************************************
 *
 *   FreeDoko a Doppelkopf-Game
 *    
 *   Copyright (C) 2001-2011  by Diether Knof and Borg Enders
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

#include "rules.h"
#include "ui.h"
#include "translations.h"
#include "icons.h"
#include "utils.h"

#include "../../party/party.h"
#include "../../party/rule.h"
#include "../../misc/setting.h"
#ifdef USE_NETWORK
#include "../../network/server.h"
#endif
#include "../../utils/string.h"

#include "widgets/labelscalespinbutton.h"
#include "widgets/filemenu.h"
#include "widgets/stock_button.h"
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/label.h>
#include <gtkmm/alignment.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/box.h>
#include <gtkmm/table.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/notebook.h>
#include <gtkmm/sizegroup.h>
#include <gtkmm/frame.h>
#include <gtkmm/messagedialog.h>

// additional scaling for the icons of the notebook
#define ICONSCALING 0.7

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
   ** @version   0.6.6
   **/
  Rules::Rules(Base* const parent) :
    Base(parent),
    StickyDialog("Rules", false),
    type_bool(),
    type_unsigned(),
    counting(),
    reset_button(NULL),
    load_button(NULL),
    save_button(NULL),
    save_file_chooser(NULL),
    group_notebook(NULL),
    backup_(NULL),
    without_nines_info_shown(false)
  {
    this->ui->add_window(*this);

    this->signal_realize().connect(sigc::mem_fun(*this, &Rules::init));
    this->create_backup();
    this->signal_show().connect(sigc::mem_fun(*this, &Rules::create_backup));

#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
    this->signal_key_press_event().connect(sigc::mem_fun(*this, &Rules::on_key_press_event));
#endif

    return ;
  } // Rules::Rules(Base* const parent)

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
  Rules::~Rules()
  {
    delete this->save_file_chooser;

    for (vector<Gtk::CheckButton*>::iterator widget = this->type_bool.begin();
         widget != this->type_bool.end();
         widget++)
      delete static_cast<Rule::TypeBool*>((*widget)->steal_data("type"));

    for (vector<Gtk::LabelSpinButton*>::iterator widget
         = this->type_unsigned.begin();
         widget != this->type_unsigned.end();
         widget++)
      delete static_cast<Rule::TypeUnsigned*>((*widget)->steal_data("type"));

    for (vector<Gtk::RadioButton*>::iterator widget
         = this->counting.begin();
         widget != this->counting.end();
         widget++)
      delete static_cast<Rule::TypeUnsigned*>((*widget)->steal_data("counting"));

    delete this->backup_;

    return ;
  } // Rules::~Rules()

  /**
   ** creates all subelements
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @version   0.6.6
   **
   ** @author    Diether Knof
   **
   ** @todo	make the horizontal size always great enough
   ** @todo	visual difference between the groups (two background colours)
   **/
  void
    Rules::init()
    {
      this->set_icon(this->ui->icon);
      this->ui->translations->add(*this, ::translation("FreeDoko %ttitle%",
                                                       ::translation("~rules")));

      this->reset_button
        = Gtk::manage(new Gtk::StockButton(Gtk::Stock::UNDO, "reset"));
      this->ui->translations->add(*this->reset_button,
                                  ::translation("reset"));
      this->load_button
        = Gtk::manage(new Gtk::StockButton(Gtk::Stock::OPEN, "load"));
      this->ui->translations->add(*this->load_button,
                                  ::translation("load"));
      this->save_button
        = Gtk::manage(new Gtk::StockButton(Gtk::Stock::SAVE_AS, "save"));
      this->ui->translations->add(*this->save_button,
                                  ::translation("save"));

      { // file chooser
        this->save_file_chooser = new Gtk::FileChooserDialog("Save rules",
                                                             Gtk::FILE_CHOOSER_ACTION_SAVE);
        this->ui->translations->add(*this->save_file_chooser, ::translation("~save rules"));

        Gtk::Button* cancel_button
          = this->save_file_chooser->add_button(Gtk::Stock::CANCEL,
                                                Gtk::RESPONSE_CANCEL);
        Gtk::Button* ok_button
          = this->save_file_chooser->add_button(Gtk::Stock::SAVE,
                                                Gtk::RESPONSE_OK);


        { // signals
          this->save_file_chooser->signal_file_activated().connect(sigc::mem_fun(*this, &Rules::save));
          this->save_file_chooser->signal_file_activated().connect(sigc::mem_fun(*this->save_file_chooser, &Widget::hide));

          ok_button->signal_clicked().connect(sigc::mem_fun(*this, &Rules::save));
          ok_button->signal_clicked().connect(sigc::mem_fun(*this->save_file_chooser, &Widget::hide));
          cancel_button->signal_clicked().connect(sigc::mem_fun(*this->save_file_chooser, &Widget::hide));
        } // signals
        {
          string dir = (::setting(Setting::PRIVATE_DATA_DIRECTORY)
                        + "/"
                        + ::setting(Setting::RULES_DIRECTORY)
                        + "/"
                       );
#ifdef WINDOWS
          DK::Utils::String::replace_all(dir, "/", "\\");
#endif
          this->save_file_chooser->set_current_folder(dir);
        }
      } // file chooser

      { // create the buttons
        for (int t = Rule::BOOL_FIRST; t <= Rule::BOOL_LAST; t++) {
          this->type_bool.push_back(Gtk::manage(new Gtk::CheckButton(::name(Rule::TypeBool(t)))));
          this->type_bool.back()->set_data("type", new Rule::TypeBool(Rule::TypeBool(t)));
          this->ui->translations->add(*this->type_bool.back(),
                                      ::translation(Rule::TypeBool(t)));
          this->type_bool.back()->signal_toggled().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Rules::change),
                                                                                 t)
                                                          );
        } // for (t \in Rule::BOOL)
        for (int t = Rule::UNSIGNED_FIRST; t <= Rule::UNSIGNED_LAST; t++) {
          switch (t) {
          case Rule::SHOW_TRICKS_NUMBER:
          case Rule::MARRIAGE_DETERMINATION:
          case Rule::ANNOUNCEMENT_NO_120:
          case Rule::ANNOUNCEMENT_NO_90:
          case Rule::ANNOUNCEMENT_NO_60:
          case Rule::ANNOUNCEMENT_NO_30:
          case Rule::ANNOUNCEMENT_NO_0:
          case Rule::ANNOUNCEMENT_LAST:
            this->type_unsigned.push_back(Gtk::manage(new Gtk::LabelScaleSpinButton("type")));
            break;
          default:
            this->type_unsigned.push_back(Gtk::manage(new Gtk::LabelSpinButton("type")));
            break;
          } // switch(t)
          this->type_unsigned.back()->set_data("type", new Rule::TypeUnsigned(Rule::TypeUnsigned(t)));
          this->ui->translations->add(this->type_unsigned.back()->get_label(),
                                      ::translation("%ttranslation%: ",
                                                    ::translation(Rule::TypeUnsigned(t))));
          this->type_unsigned.back()->signal_value_changed().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Rules::change),
                                                                                           t)
                                                                    );
        } // for (t \in Rule::UNSIGNED)
        { // counting
          Gtk::RadioButton::Group counting_group;
          this->counting.push_back(Gtk::manage(new Gtk::RadioButton(counting_group, ::name(COUNTING_NS::PLUS))));
          this->ui->translations->add(*this->counting.back(),
                                      ::translation(COUNTING_NS::PLUS));
          this->counting.back()->set_data("counting",
                                          new Counting(COUNTING_NS::PLUS));
          this->counting.back()->signal_toggled().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Rules::change), Rule::COUNTING));

          this->counting.push_back(Gtk::manage(new Gtk::RadioButton(counting_group, ::name(COUNTING_NS::MINUS))));
          this->ui->translations->add(*this->counting.back(),
                                      ::translation(COUNTING_NS::MINUS));
          this->counting.back()->set_data("counting",
                                          new Counting(COUNTING_NS::MINUS));
          this->counting.back()->signal_toggled().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Rules::change), Rule::COUNTING));

          this->counting.push_back(Gtk::manage(new Gtk::RadioButton(counting_group, ::name(COUNTING_NS::PLUSMINUS))));
          this->ui->translations->add(*this->counting.back(),
                                      ::translation(COUNTING_NS::PLUSMINUS));
          this->counting.back()->set_data("counting",
                                          new Counting(COUNTING_NS::PLUSMINUS));
          this->counting.back()->signal_toggled().connect(sigc::bind<int const>(sigc::mem_fun(*this, &Rules::change), Rule::COUNTING));
        } // counting
        { // same size of some widgets
          static Glib::RefPtr<Gtk::SizeGroup> announcement_scale_size_group
            = Gtk::SizeGroup::create(Gtk::SIZE_GROUP_BOTH);
          announcement_scale_size_group->add_widget(dynamic_cast<Gtk::LabelScaleSpinButton*>(this->type_unsigned[Rule::ANNOUNCEMENT_NO_120 - Rule::UNSIGNED_FIRST])->get_scale());
          announcement_scale_size_group->add_widget(dynamic_cast<Gtk::LabelScaleSpinButton*>(this->type_unsigned[Rule::ANNOUNCEMENT_NO_90  - Rule::UNSIGNED_FIRST])->get_scale());
          announcement_scale_size_group->add_widget(dynamic_cast<Gtk::LabelScaleSpinButton*>(this->type_unsigned[Rule::ANNOUNCEMENT_NO_60  - Rule::UNSIGNED_FIRST])->get_scale());
          announcement_scale_size_group->add_widget(dynamic_cast<Gtk::LabelScaleSpinButton*>(this->type_unsigned[Rule::ANNOUNCEMENT_NO_30  - Rule::UNSIGNED_FIRST])->get_scale());
          announcement_scale_size_group->add_widget(dynamic_cast<Gtk::LabelScaleSpinButton*>(this->type_unsigned[Rule::ANNOUNCEMENT_NO_0   - Rule::UNSIGNED_FIRST])->get_scale());
          announcement_scale_size_group->add_widget(dynamic_cast<Gtk::LabelScaleSpinButton*>(this->type_unsigned[Rule::ANNOUNCEMENT_LAST   - Rule::UNSIGNED_FIRST])->get_scale());

        } // same size of some widgets
      } // create the buttons


      { // layout
        { // action area

          Gtk::StockButton* close_button
            = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
          this->ui->translations->add(*close_button,
                                      ::translation("close"));

          this->add_action_widget(*close_button, Gtk::RESPONSE_CLOSE);
          close_button->grab_focus();
          close_button->signal_clicked().connect(sigc::mem_fun(*this,
                                                               &Gtk::Widget::hide));

        } // action area

        { // layout for the rules
          this->group_notebook = Gtk::manage(new Gtk::Notebook());
          this->group_notebook->signal_switch_page().connect(sigc::mem_fun(*this, &Rules::group_notebook_page_changed));

          this->group_notebook->set_tab_pos(Gtk::POS_LEFT);
          //this->group_notebook->set_scrollable(true);
          this->group_notebook->popup_enable();
          this->group_notebook->set_property("homogeneous", true);
          this->get_vbox()->add(*this->group_notebook);

          { // Tournament
            Gtk::VBox* tournament_box = this->add_group("tournament");
            tournament_box->set_spacing(3 ex);

            { // with nines
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              tournament_box->add(*alignment);
              Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox2);

              vbox2->add(*this->type_bool[Rule::WITH_NINES - Rule::BOOL_FIRST]);
            } // with nines
#ifdef POSTPONED
            { // number of players
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              vbox->add(*alignment);
              Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox2);

              vbox2->add(*this->type_unsigned[Rule::NUMBER_OF_PLAYERS - Rule::UNSIGNED_FIRST]);
              vbox2->add(*this->type_unsigned[Rule::NUMBER_OF_PLAYERS_IN_GAME - Rule::UNSIGNED_FIRST]);
            } // number of players
#endif
            { // lower box
              Gtk::Box* hbox = Gtk::manage(new Gtk::HBox(false, 2 em));
              { // limits
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                hbox->add(*alignment);
                Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 2 ex));
                alignment->add(*vbox2);

                {
                  Gtk::VBox* vbox3
                    = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
                  vbox2->add(*vbox3);
                  vbox3->add(*this->type_bool[Rule::NUMBER_OF_ROUNDS_LIMITED - Rule::BOOL_FIRST]);
                  vbox3->add(*this->type_unsigned[Rule::NUMBER_OF_ROUNDS - Rule::UNSIGNED_FIRST]);
                }
                {
                  Gtk::VBox* vbox3
                    = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
                  vbox2->add(*vbox3);
                  vbox3->add(*this->type_bool[Rule::POINTS_LIMITED - Rule::BOOL_FIRST]);
                  vbox3->add(*this->type_unsigned[Rule::POINTS - Rule::UNSIGNED_FIRST]);
                }
              } // limits
              { // duty soli
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                hbox->pack_end(*alignment, Gtk::PACK_SHRINK, 2 ex);
                Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 2 ex));
                alignment->add(*vbox2);

                {
                  Gtk::VBox* vbox3
                    = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
                  vbox2->add(*vbox3);
                  vbox3->add(*this->type_unsigned[Rule::NUMBER_OF_DUTY_SOLI - Rule::UNSIGNED_FIRST]);
                  vbox3->add(*this->type_unsigned[Rule::NUMBER_OF_DUTY_COLOR_SOLI - Rule::UNSIGNED_FIRST]);
                  vbox3->add(*this->type_unsigned[Rule::NUMBER_OF_DUTY_PICTURE_SOLI - Rule::UNSIGNED_FIRST]);
                }
                {
                  Gtk::VBox* vbox3
                    = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
                  vbox2->add(*vbox3);
                  vbox3->add(*this->type_bool[Rule::OFFER_DUTY_SOLO - Rule::BOOL_FIRST]);
                }
              } // duty soli
              tournament_box->add(*hbox);
            } // lower box
          } // Tournament
          { // counting
            Gtk::VBox* vbox = this->add_group("counting");
            Gtk::HBox* counting_box = Gtk::manage(new Gtk::HBox(false, 4 em));
            { // counting type
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              Gtk::VBox* vbox_count = Gtk::manage(new Gtk::VBox(false, 2 ex));
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                Gtk::Box* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
                {
                  Gtk::Alignment* alignment
                    = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                     Gtk::ALIGN_CENTER,
                                                     0, 0));
                  alignment->set_border_width(1 ex / 2);
                  Gtk::Label* label = Gtk::manage(new Gtk::Label("counting"));
                  this->ui->translations->add(*label,
                                              ::translation(Rule::COUNTING));
                  alignment->add(*label);
                  vbox->add(*alignment);
                }

                for (vector<Gtk::RadioButton*>::iterator widget
                     = this->counting.begin();
                     widget != this->counting.end();
                     ++widget)
                  vbox->add(**widget);

                alignment->add(*vbox);
                vbox_count->add(*alignment);
              }
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                alignment->set_border_width(1 ex / 2);
                alignment->add(*this->type_bool[Rule::SOLO_ALWAYS_COUNTS_TRIPLE - Rule::BOOL_FIRST]);
                vbox_count->add(*alignment);
              }
              alignment->add(*vbox_count);
              counting_box->add(*alignment);
            } // counting type
            { // Bock
              Gtk::VBox* bock_box = Gtk::manage(new Gtk::VBox(false, 0 ex));
              bock_box->set_spacing(2 ex);
              counting_box->add(*bock_box);

              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                bock_box->add(*alignment);

                alignment->add(*this->type_bool[Rule::BOCK - Rule::BOOL_FIRST]);
              }
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                bock_box->add(*alignment);

                alignment->add(*this->type_bool[Rule::BOCK_APPEND - Rule::BOOL_FIRST]);
              }
              { // conditions
                Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(true, 1 ex / 2));
                bock_box->add(*vbox);

                vbox->add(*this->type_bool[Rule::BOCK_120 - Rule::BOOL_FIRST]);
                vbox->add(*this->type_bool[Rule::BOCK_SOLO_LOST - Rule::BOOL_FIRST]);
                vbox->add(*this->type_bool[Rule::BOCK_RE_LOST - Rule::BOOL_FIRST]);
                vbox->add(*this->type_bool[Rule::BOCK_HEART_TRICK - Rule::BOOL_FIRST]);
                vbox->add(*this->type_bool[Rule::BOCK_BLACK - Rule::BOOL_FIRST]);
              } // conditions
            } // Bock
            Gtk::Alignment* alignment
              = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                               Gtk::ALIGN_CENTER,
                                               1, 1));
            alignment->add(*counting_box);
            vbox->add(*alignment);
          } // counting
          { // Information
            Gtk::VBox* vbox0 = this->add_group("information",
                                               Gtk::manage(new Gtk::Image(Gtk::Stock::DIALOG_INFO,
                                                                          Gtk::ICON_SIZE_LARGE_TOOLBAR)));

            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              vbox0->add(*alignment);
              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox);

              vbox->add(*this->type_unsigned[Rule::SHOW_TRICKS_NUMBER - Rule::UNSIGNED_FIRST]);
              vbox->add(*this->type_bool[Rule::SHOW_TRICKS_IN_TRICKPILES - Rule::BOOL_FIRST]);
              vbox->add(*this->type_bool[Rule::SHOW_TRICKPILES_POINTS - Rule::BOOL_FIRST]);
            }
            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              vbox0->add(*alignment);
              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox);

              vbox->add(*this->type_bool[Rule::SHOW_SOLOPLAYER_IN_GAME - Rule::BOOL_FIRST]);
              vbox->add(*this->type_bool[Rule::SHOW_KNOWN_TEAMS_IN_GAME - Rule::BOOL_FIRST]);
              vbox->add(*this->type_bool[Rule::SHOW_IF_VALID - Rule::BOOL_FIRST]);
              vbox->add(*this->type_bool[Rule::SHOW_ALL_HANDS - Rule::BOOL_FIRST]);
            }
            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              vbox0->add(*alignment);
              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox);

              vbox->add(*this->type_bool[Rule::CARD_HINT - Rule::BOOL_FIRST]);
              vbox->add(*this->type_bool[Rule::TAKE_BACK_TRICKS - Rule::BOOL_FIRST]);
            }
          } // Information
          { // Solo
            Gtk::VBox* solo_box = this->add_group("solo",
                                                  this->ui->icons->new_managed_image(Icons::SOLO_KOEHLER, ICONSCALING));
            solo_box->set_spacing(2 ex);

            { // solo types
              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex));
              solo_box->add(*vbox);
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                vbox->add(*alignment);

                alignment->add(*this->type_bool[Rule::SOLO - Rule::BOOL_FIRST]);
              }
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                vbox->add(*alignment);
                {
                  Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 1 em));
                  alignment->add(*hbox);
                  {
                    Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(true, 1 ex / 2));
                    hbox->add(*vbox2);

                    vbox2->add(*this->type_bool[Rule::SOLO_COLOR - Rule::BOOL_FIRST]);
                    vbox2->add(*this->type_bool[Rule::SOLO_KOEHLER - Rule::BOOL_FIRST]);
                    vbox2->add(*this->type_bool[Rule::SOLO_MEATLESS - Rule::BOOL_FIRST]);
                  }
                  {
                    Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(true, 1 ex / 2));
                    hbox->add(*vbox2);

                    vbox2->add(*this->type_bool[Rule::SOLO_JACK - Rule::BOOL_FIRST]);
                    vbox2->add(*this->type_bool[Rule::SOLO_QUEEN - Rule::BOOL_FIRST]);
                    vbox2->add(*this->type_bool[Rule::SOLO_KING - Rule::BOOL_FIRST]);
                  }
                  {
                    Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(true, 1 ex / 2));
                    hbox->add(*vbox2);

                    vbox2->add(*this->type_bool[Rule::SOLO_QUEEN_JACK - Rule::BOOL_FIRST]);
                    vbox2->add(*this->type_bool[Rule::SOLO_KING_JACK - Rule::BOOL_FIRST]);
                    vbox2->add(*this->type_bool[Rule::SOLO_KING_QUEEN - Rule::BOOL_FIRST]);
                  }
                }
              }
            } // solo types
            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              solo_box->pack_end(*alignment, Gtk::PACK_SHRINK, 0 ex);

              alignment->add(*this->type_bool[Rule::THROWING_BEFORE_SOLO - Rule::BOOL_FIRST]);
            }
            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              solo_box->pack_end(*alignment, Gtk::PACK_SHRINK, 2 ex);

              alignment->add(*this->type_bool[Rule::LUSTSOLO_PLAYER_LEADS - Rule::BOOL_FIRST]);
            }
          } // Solo
          { // Poverty
            Gtk::Box* poverty_box
              = this->add_group("poverty",
                                this->ui->icons->new_managed_image(Icons::POVERTY, ICONSCALING));

            {
              Gtk::Alignment* alignment = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                                         Gtk::ALIGN_CENTER,
                                                                         0, 0));
              poverty_box->add(*alignment);
              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox);

              vbox->add(*this->type_bool[Rule::POVERTY - Rule::BOOL_FIRST]);
              vbox->add(*indent(*this->type_bool[Rule::POVERTY_SHIFT - Rule::BOOL_FIRST]));
              vbox->add(*indent(*this->type_bool[Rule::POVERTY_SHIFT_ONLY_TRUMP - Rule::BOOL_FIRST]));
              vbox->add(*indent(*this->type_bool[Rule::POVERTY_FOX_DO_NOT_COUNT - Rule::BOOL_FIRST]));
              vbox->add(*indent(*this->type_bool[Rule::POVERTY_FOX_SHIFT_EXTRA - Rule::BOOL_FIRST], 2));
              vbox->add(*indent(*this->type_bool[Rule::POVERTY_FOX_SHIFT_OPEN - Rule::BOOL_FIRST]));
              vbox->add(*indent(*this->type_bool[Rule::THROW_WITH_ONE_TRUMP - Rule::BOOL_FIRST]));
            }
            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              poverty_box->add(*alignment);
              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox);

              vbox->add(*this->type_bool[Rule::THROW_WHEN_FOX_HIGHEST_TRUMP - Rule::BOOL_FIRST]);
              vbox->add(*this->type_bool[Rule::THROW_WITH_NINES - Rule::BOOL_FIRST]);
              vbox->add(*this->type_unsigned[Rule::MIN_NUMBER_OF_THROWING_NINES - Rule::UNSIGNED_FIRST]);
              vbox->add(*this->type_bool[Rule::THROW_WITH_KINGS - Rule::BOOL_FIRST]);
              vbox->add(*this->type_unsigned[Rule::MIN_NUMBER_OF_THROWING_KINGS - Rule::UNSIGNED_FIRST]);
              vbox->add(*this->type_bool[Rule::THROW_WITH_NINES_AND_KINGS - Rule::BOOL_FIRST]);
              vbox->add(*this->type_unsigned[Rule::MIN_NUMBER_OF_THROWING_NINES_AND_KINGS - Rule::UNSIGNED_FIRST]);
              vbox->add(*this->type_bool[Rule::THROW_WITH_RICHNESS - Rule::BOOL_FIRST]);
              vbox->add(*this->type_unsigned[Rule::MIN_RICHNESS_FOR_THROWING - Rule::UNSIGNED_FIRST]);
            }
          } // Poverty
          { // Marriage
            Gtk::VBox* marriage_box = this->add_group("marriage",
                                                      this->ui->icons->new_managed_image(Icons::MARRIAGE, ICONSCALING));

            { // marriage
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                marriage_box->add(*alignment);

                alignment->add(*this->type_bool[Rule::MARRIAGE_BEFORE_POVERTY - Rule::BOOL_FIRST]);
              }
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                marriage_box->add(*alignment);
                Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
                alignment->add(*vbox2);

                vbox2->add(*this->type_bool[Rule::MARRIAGE_FIRST_FOREIGN - Rule::BOOL_FIRST]);
                vbox2->add(*this->type_bool[Rule::MARRIAGE_FIRST_COLOR - Rule::BOOL_FIRST]);
                vbox2->add(*this->type_bool[Rule::MARRIAGE_FIRST_SINGLE_COLOR - Rule::BOOL_FIRST]);
                vbox2->add(*this->type_bool[Rule::MARRIAGE_FIRST_TRUMP - Rule::BOOL_FIRST]);
              }
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                marriage_box->add(*alignment);

                alignment->add(*this->type_bool[Rule::MARRIAGE_FIRST_ONE_DECIDES - Rule::BOOL_FIRST]);
              }
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                marriage_box->add(*alignment);

                alignment->add(*this->type_unsigned[Rule::MARRIAGE_DETERMINATION - Rule::UNSIGNED_FIRST]);
              }
            } // marriage
            { // genscher
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              marriage_box->pack_end(*alignment, Gtk::PACK_SHRINK, 2 ex);

              alignment->add(*this->type_bool[Rule::GENSCHER - Rule::BOOL_FIRST]);
            } // genscher
          } // Marriage
          { // Announcements
            Gtk::Box* announcements_box
              = this->add_group("announcements",
                                this->ui->icons->new_managed_image(Icons::NO_120, ICONSCALING));
            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              announcements_box->add(*alignment);

              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox);

              vbox->pack_start(*this->type_bool[Rule::KNOCKING - Rule::BOOL_FIRST]);
            }

            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              announcements_box->add(*alignment);

              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox);

              {
                Gtk::Alignment* a
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_RIGHT,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                vbox->add(*a);
                Gtk::Label* label
                  = Gtk::manage(new Gtk::Label("announcement with maximal cards"));
                this->ui->translations->add(*label,
                                            ::translation("announcement with maximal cards"));
                a->add(*label);

              }

              vbox->pack_start(*this->type_unsigned[Rule::ANNOUNCEMENT_NO_120 - Rule::UNSIGNED_FIRST]);
              vbox->pack_start(*this->type_unsigned[Rule::ANNOUNCEMENT_NO_90 - Rule::UNSIGNED_FIRST]);
              vbox->pack_start(*this->type_unsigned[Rule::ANNOUNCEMENT_NO_60 - Rule::UNSIGNED_FIRST]);
              vbox->pack_start(*this->type_unsigned[Rule::ANNOUNCEMENT_NO_30 - Rule::UNSIGNED_FIRST]);
              vbox->pack_start(*this->type_unsigned[Rule::ANNOUNCEMENT_NO_0 - Rule::UNSIGNED_FIRST]);
              vbox->pack_start(*this->type_unsigned[Rule::ANNOUNCEMENT_LAST - Rule::UNSIGNED_FIRST]);
            }
            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              announcements_box->add(*alignment);

              Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox);

              vbox->pack_start(*this->type_bool[Rule::ANNOUNCEMENT_TILL_FULL_TRICK - Rule::BOOL_FIRST]);
              vbox->pack_start(*this->type_bool[Rule::ANNOUNCEMENT_REPLY - Rule::BOOL_FIRST]);
              vbox->pack_start(*this->type_bool[Rule::ANNOUNCEMENT_STRICT_LIMIT - Rule::BOOL_FIRST]);
              vbox->pack_start(*this->type_bool[Rule::ANNOUNCEMENT_RE_DOUBLES - Rule::BOOL_FIRST]);
              vbox->pack_start(*indent(*this->type_bool[Rule::ANNOUNCEMENT_CONTRA_DOUBLES_AGAINST_RE - Rule::BOOL_FIRST]));
              vbox->pack_start(*this->type_bool[Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS - Rule::BOOL_FIRST]);
              vbox->pack_start(*indent(*this->type_bool[Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS_ONLY_FIRST - Rule::BOOL_FIRST]));
              vbox->pack_start(*indent(*this->type_bool[Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS_ONLY_RE_CONTRA - Rule::BOOL_FIRST]));
              vbox->pack_start(*indent(*this->type_bool[Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS_IN_MARRIAGE - Rule::BOOL_FIRST]));
            }
          } // Announcements
          { // Dollen
            Gtk::VBox* vbox = this->add_group("dollen",
                                              this->ui->icons->new_managed_image(Icons::DOLLEN, ICONSCALING));
            vbox->set_spacing(1 ex / 2);

            {
              vbox->add(*this->type_bool[Rule::DOLLEN - Rule::BOOL_FIRST]);
              vbox->add(*indent(*this->type_bool[Rule::DOLLEN_SECOND_OVER_FIRST - Rule::BOOL_FIRST]));
              vbox->add(*indent(*this->type_bool[Rule::DOLLEN_CONTRARY_IN_LAST_TRICK - Rule::BOOL_FIRST], 2));
              vbox->add(*indent(*this->type_bool[Rule::DOLLEN_FIRST_OVER_SECOND_WITH_SWINES - Rule::BOOL_FIRST], 2));
            }
          } // Dollen
          { // Swines
            Gtk::Table* table = Gtk::manage(new Gtk::Table(6, 2, true));
            this->add_group("swines",
                            this->ui->icons->new_managed_image(Icons::SWINES,
                                                               ICONSCALING)
                           )->add(*table);

            table->set_col_spacings(2 em);
            table->set_row_spacings(1 ex / 2);

            {
              table->attach(*this->type_bool[Rule::SWINES - Rule::BOOL_FIRST],
                            0, 1, 0, 1);
              table->attach(*indent(*this->type_bool[Rule::SWINES_IN_SOLO - Rule::BOOL_FIRST]),
                            0, 1, 1, 2);
              table->attach(*indent(*this->type_bool[Rule::SWINES_IN_POVERTY - Rule::BOOL_FIRST]),
                            0, 1, 2, 3);
              table->attach(*indent(*this->type_bool[Rule::SWINES_ANNOUNCEMENT_BEGIN - Rule::BOOL_FIRST]),
                            0, 1, 3, 4);
              table->attach(*indent(*this->type_bool[Rule::SWINE_ONLY_SECOND - Rule::BOOL_FIRST]),
                            0, 1, 4, 5);
              table->attach(*this->type_bool[Rule::HYPERSWINES - Rule::BOOL_FIRST],
                            1, 2, 0, 1);
              table->attach(*this->type_bool[Rule::HYPERSWINES_IN_SOLO - Rule::BOOL_FIRST],
                            1, 2, 1, 2);
              table->attach(*this->type_bool[Rule::HYPERSWINES_IN_POVERTY - Rule::BOOL_FIRST],
                            1, 2, 2, 3);
              table->attach(*this->type_bool[Rule::HYPERSWINES_ANNOUNCEMENT_BEGIN - Rule::BOOL_FIRST],
                            1, 2, 3, 4);
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                alignment->add(*this->type_bool[Rule::SWINES_AND_HYPERSWINES_JOINT - Rule::BOOL_FIRST]);
                table->attach(*alignment, 0, 2, 5, 6);
              }
            }
          } // Swines
          { // Extrapoints
            Gtk::VBox* vbox = this->add_group("extrapoints",
                                              this->ui->icons->new_managed_image(Icons::DOPPELKOPF, ICONSCALING));

            {
              Gtk::HBox* hbox = Gtk::manage(new Gtk::HBox(false, 1 em));
              hbox->set_spacing(2 em);
              vbox->add(*hbox);
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                hbox->add(*alignment);
                Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
                alignment->add(*vbox2);

                vbox2->add(*this->type_bool[Rule::EXTRAPOINT_CATCH_FOX - Rule::BOOL_FIRST]);
                vbox2->add(*indent(*this->type_bool[Rule::EXTRAPOINT_CATCH_FOX_LAST_TRICK - Rule::BOOL_FIRST]));
                vbox2->add(*this->type_bool[Rule::EXTRAPOINT_FOX_LAST_TRICK - Rule::BOOL_FIRST]);
                vbox2->add(*indent(*this->type_bool[Rule::EXTRAPOINT_DOUBLE_FOX_LAST_TRICK - Rule::BOOL_FIRST]));
              }
              {
                Gtk::Alignment* alignment
                  = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                   Gtk::ALIGN_CENTER,
                                                   0, 0));
                hbox->add(*alignment);
                Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
                alignment->add(*vbox2);

                vbox2->add(*this->type_bool[Rule::EXTRAPOINT_CHARLIE - Rule::BOOL_FIRST]);
                vbox2->add(*indent(*this->type_bool[Rule::EXTRAPOINT_CATCH_CHARLIE - Rule::BOOL_FIRST]));
                vbox2->add(*indent(*this->type_bool[Rule::EXTRAPOINT_DOUBLE_CHARLIE - Rule::BOOL_FIRST]));
                vbox2->add(*indent(*this->type_bool[Rule::EXTRAPOINT_CATCH_DOUBLE_CHARLIE - Rule::BOOL_FIRST]));
                vbox2->add(*indent(*this->type_bool[Rule::EXTRAPOINT_CATCH_CHARLIE_ONLY_WITH_DIAMOND_QUEEN - Rule::BOOL_FIRST]));
              }
            }
            {
              Gtk::Alignment* alignment
                = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                                 Gtk::ALIGN_CENTER,
                                                 0, 0));
              vbox->add(*alignment);
              Gtk::VBox* vbox2 = Gtk::manage(new Gtk::VBox(false, 1 ex / 2));
              alignment->add(*vbox2);

              vbox2->add(*this->type_bool[Rule::EXTRAPOINT_DOLLE_JABS_DOLLE - Rule::BOOL_FIRST]);
              vbox2->add(*this->type_bool[Rule::EXTRAPOINT_HEART_TRICK - Rule::BOOL_FIRST]);
            }
          } // Extrapoints
        } // layout for the rules

        { // put the buttons in the container
          Gtk::Alignment* alignment
            = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER,
                                             Gtk::ALIGN_TOP,
                                             0, 0));
          this->get_vbox()->pack_end(*alignment, Gtk::PACK_SHRINK, 1 ex);

          Gtk::Box* file_box = Gtk::manage(new Gtk::HButtonBox());
          file_box->set_border_width(1 ex / 2);
          file_box->set_spacing(1 em);
          alignment->add(*file_box);

          file_box->add(*this->reset_button);
          file_box->add(*this->load_button);
          file_box->add(*this->save_button);
        } // put the buttons in the container

      } // layout

#ifndef RELEASE
#ifdef DKNOF
      { // test, whether all rules-buttons are packed in a container
        for (int t = Rule::BOOL_FIRST; t <= Rule::BOOL_LAST; t++) {
          if (t == Rule::MUTATE)
            continue;
          if (!this->type_bool[t - Rule::BOOL_FIRST]->get_parent())
            cerr << "GTKMM_DOKO::Rules::Rules():\n"
                                                 << "  rule '" << name(Rule::TypeBool(t)) << "' not packed\n";
        }
        for (int t = Rule::UNSIGNED_FIRST; t <= Rule::UNSIGNED_LAST; t++) {
          if (t == Rule::UNSUPPORTED)
            continue;
#ifndef OUTDATED
          if (   (t == Rule::NUMBER_OF_PLAYERS)
              || (t == Rule::NUMBER_OF_PLAYERS_IN_GAME)
              || (t == Rule::BOCK_MULTIPLIER) )
            continue;
#endif
          if (!this->type_unsigned[t - Rule::UNSIGNED_FIRST]->get_parent())
            cerr << "GTKMM_DOKO::Rules::Rules():\n"
                                                 << "  rule '" << name(Rule::TypeUnsigned(t)) << "' not packed\n";
        }
      } // test, whether all rule-buttons are packed in a container
#endif
#endif

      { // signals
        this->reset_button->signal_clicked().connect(sigc::mem_fun(*this, &Rules::reset));

        this->load_button->signal_clicked().connect(sigc::mem_fun(*this, &Rules::load));
        this->save_button->signal_clicked().connect(sigc::mem_fun0(*this->save_file_chooser, &Gtk::Window::present));
      } // signals

      this->update_all();

      this->show_all_children();

      this->language_update();

      return ;
    } // void Rules::init()

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
    Rules::create_backup()
    {
      delete this->backup_;
#ifdef WORKAROUND
      this->backup_ = new Rule(::party.rule());
#else
      this->backup_ = new Rule(this->ui->party().rule());
#endif

      this->sensitivity_update();

      return ;
    } // void Rules::create_backup()

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
  Rule const&
    Rules::backup() const
    {
      DEBUG_ASSERTION((this->backup_ != NULL),
                      "Rule::backup():\n"
                      "  'this->backup_' == NULL");

      return *this->backup_;
    } // Rule const& Rules::backup() const

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
    Rules::reset()
    {
      ::party.rule() = this->backup();
      this->update_all();

      return ;
    } // void Rules::reset()

  /**
   **
   ** creates a menu with the files in the rules directories
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
    Rules::load()
    {
      Gtk::FileMenu* load_menu
        = Gtk::manage(new Gtk::FileMenu(sigc::mem_fun(*this, &Rules::load_file)));
      load_menu->add_directory(::setting(Setting::PRIVATE_DATA_DIRECTORY)
                               + "/"
                               + ::setting(Setting::RULES_DIRECTORY));
      load_menu->add_directory(::setting(Setting::PUBLIC_DATA_DIRECTORY)
                               + "/"
                               + ::setting(Setting::RULES_DIRECTORY));

      load_menu->show();

      return ;
    } // void Rules::load()

  /**
   **
   ** loads the rules
   **
   ** @param     filename   the rules file
   **
   ** @return    -
   **
   ** @version   0.5.4
   **
   ** @author    Diether Knof
   **
   **/
  void
    Rules::load_file(string const filename)
    {
      ::party.rule().load(filename);
      this->update_all();

      return ;
    } // void Rules::load_file(string const filename)

  /**
   **
   ** saves the rules.
   ** The filename is taken from 'save_file_selector'.
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
    Rules::save()
    {
      if (::setting(Setting::AUTOMATIC_SAVINGS))
        this->ui->party().rule().save(this->save_file_chooser->get_filename()); 

      return ;
    } // void Rules::save()

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
    Rules::sensitivity_update()
    {
      if (!this->is_realized())
        return ;

      bool sensitive = (::game_status == GAMESTATUS::PARTY_NEW);

#ifndef RELEASE
      sensitive = true;
#endif

#ifdef USE_NETWORK
      if (::server
          && ::server->has_parent_connection())
        sensitive = false;
#endif

      for (int i = 0; i < this->group_notebook->get_n_pages(); ++i)
        this->group_notebook->get_nth_page(i)->set_sensitive(sensitive);

      if (sensitive) {
        for (vector<Gtk::CheckButton*>::iterator
             widget = this->type_bool.begin();
             widget != this->type_bool.end();
             widget++)
          (*widget)->set_sensitive(this->ui->party().rule().dependencies(*static_cast<Rule::TypeBool*>((*widget)->get_data("type"))));
        for (vector<Gtk::LabelSpinButton*>::iterator widget
             = this->type_unsigned.begin();
             widget != this->type_unsigned.end();
             widget++) {
          Rule::TypeUnsigned const type 
            = *static_cast<Rule::TypeUnsigned*>((*widget)->get_data("type"));

          (*widget)->set_sensitive(this->ui->party().rule().dependencies(type));
          (*widget)->set_range(this->ui->party().rule().min(type),
                               this->ui->party().rule().max(type));
        } // for (widget \in this->type_unsigned)

        for (vector<Gtk::RadioButton*>::iterator widget
             = this->counting.begin();
             widget != this->counting.end();
             widget++)
          (*widget)->set_sensitive(true);

        this->load_button->set_sensitive(true);
        this->reset_button->set_sensitive(this->backup()
                                          != this->ui->party().rule());

      } else { // if (sensitive)
        for (vector<Gtk::CheckButton*>::iterator
             widget = this->type_bool.begin();
             widget != this->type_bool.end();
             widget++)
          (*widget)->set_sensitive(false);
        for (vector<Gtk::LabelSpinButton*>::iterator widget
             = this->type_unsigned.begin();
             widget != this->type_unsigned.end();
             widget++)
          (*widget)->set_sensitive(false);

        for (vector<Gtk::RadioButton*>::iterator widget
             = this->counting.begin();
             widget != this->counting.end();
             widget++)
          (*widget)->set_sensitive(false);

        this->load_button->set_sensitive(false);
        this->reset_button->set_sensitive(false);
      } // if (sensitive)

#ifdef RELEASE
      this->type_bool[Rule::TAKE_BACK_TRICKS - Rule::BOOL_FIRST]->set_sensitive(false);
      this->type_unsigned[Rule::NUMBER_OF_PLAYERS - Rule::UNSIGNED_FIRST]->set_sensitive(false);
      this->type_unsigned[Rule::NUMBER_OF_PLAYERS_IN_GAME - Rule::UNSIGNED_FIRST]->set_sensitive(false);
#endif // #ifdef RELEASE

      return ;
    } // void Rules::sensitivity_update()

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
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    Rules::update(int const type, bool const update_sensitivity)
    {
      if (!this->is_realized())
        return ;

      if (update_sensitivity)
        this->sensitivity_update();

      Rule const& rule = this->ui->party().rule();

      if ((type >= Rule::BOOL_FIRST)
          && (type <= Rule::BOOL_LAST)) {
        this->type_bool[type - Rule::BOOL_FIRST]->set_active(rule.value(Rule::TypeBool(type)));
      } else if ((type >= Rule::UNSIGNED_FIRST)
                 && (type <= Rule::UNSIGNED_LAST)) {
        this->type_unsigned[type - Rule::UNSIGNED_FIRST]->set_range(rule.min(Rule::TypeUnsigned(type)),
                                                                    rule.max(Rule::TypeUnsigned(type)));
        this->type_unsigned[type - Rule::UNSIGNED_FIRST]->set_value(rule.value(Rule::TypeUnsigned(type)));
      } else if (type == Rule::COUNTING) {
        this->counting[rule(Rule::COUNTING)]->set_active(true);
      } else  {
        DEBUG_ASSERTION(false,
                        "Rules::update(type, update_sensitivity):\n"
                        "  type '" << type << "' unknown.");
      }

      if (type == Rule::KNOCKING)
        this->update_announcement_text();

      if (update_sensitivity)
        this->reset_button->set_sensitive(this->backup() != rule);

      return ;
    } // void Rules::update(int const type, bool const update_sensitivity = true)

  /**
   **
   ** a rule has been changed by the user
   **
   ** @param     type   the type of the rule
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.5.4
   **/
  void
    Rules::change(int const type)
    {
      Rule& rule = ::party.rule();

      if ( (type == Rule::WITH_NINES)
          && (this->type_bool[type - Rule::BOOL_FIRST]->get_active() 
              != rule(Rule::WITH_NINES))
          && (this->without_nines_info_shown == false)
         ) {
        // show an information window for rule problems
        // @bug    memory bug: dialog is not destroyed (but is only created at most once)
        Gtk::MessageDialog* information
          = new Gtk::MessageDialog(!rule(Rule::WITH_NINES)
                                   ? ::translation("Rule::Information::with nines").translation()
                                   : ::translation("Rule::Information::without nines").translation(),
                                   false,
                                   Gtk::MESSAGE_INFO,
                                   Gtk::BUTTONS_NONE,
                                   false);
        information->set_icon(this->ui->icon);
        information->set_title(!rule(Rule::WITH_NINES)
                               ? ::translation("Rule::Information::Title::with nines").translation()
                               : ::translation("Rule::Information::Title::without nines").translation());

        { // adjust rules button
          Gtk::Button* adjust_rules_button
            = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "adjust rules"));
          this->ui->translations->add(*adjust_rules_button, ::translation("adjust rules"));
          information->add_action_widget(*adjust_rules_button, Gtk::RESPONSE_CLOSE);
          adjust_rules_button->show_all();

          adjust_rules_button->signal_clicked().connect(sigc::mem_fun(*this,
                                                                      !rule(Rule::WITH_NINES)
                                                                      ? &Rules::adjust_to_with_nines
                                                                      : &Rules::adjust_to_without_nines));
          adjust_rules_button->signal_clicked().connect(sigc::mem_fun(*information,
                                                                      &Gtk::Widget::hide)
                                                       );
        } // adjust rules button

        { // close button
          Gtk::Button* close_button
            = Gtk::manage(new Gtk::StockButton(Gtk::Stock::CLOSE, "close"));
          this->ui->translations->add(*close_button, ::translation("close"));
          information->add_action_widget(*close_button, Gtk::RESPONSE_CLOSE);
          close_button->show_all();

          close_button->signal_clicked().connect(sigc::mem_fun(*information,
                                                               &Gtk::Widget::hide)
                                                );
        } // close button

        information->set_transient_for(*this);

        information->present();

        this->without_nines_info_shown = true;
      } // if ('with nines' changed)

      if ((type >= Rule::BOOL_FIRST)
          && (type <= Rule::BOOL_LAST)) {
        rule.set(Rule::TypeBool(type),
                 this->type_bool[type - Rule::BOOL_FIRST]->get_active());
      } else if ((type >= Rule::UNSIGNED_FIRST)
                 && (type <= Rule::UNSIGNED_LAST)) {
        if (this->type_unsigned[type - Rule::UNSIGNED_FIRST]->is_realized())
          rule.set(Rule::TypeUnsigned(type),
                   this->type_unsigned[type - Rule::UNSIGNED_FIRST]->get_value_as_int());
      } else if (type == Rule::COUNTING) {
        for (int c = COUNTING_NS::PLUS; c <= COUNTING_NS::PLUSMINUS; ++c)
          if (this->counting[c]->get_active())
            rule.set(Rule::COUNTING, static_cast<Counting>(c));
      } else 
        DEBUG_ASSERTION(false,
                        "Rules::change(type):\n"
                        "  type '" << type << "' unknown.");

      return ;
    } // void Rules::change(int const type)

  /**
   **
   ** updates all widgets
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
    Rules::update_all()
    {
      if (!this->is_realized())
        return ;

      this->sensitivity_update();
      for (int t = Rule::BOOL_FIRST; t <= Rule::BOOL_LAST; t++)
        this->update(t, false);
      for (int t = Rule::UNSIGNED_FIRST; t <= Rule::UNSIGNED_LAST; t++)
        this->update(t, false);
      this->update(Rule::COUNTING, false);

      this->reset_button->set_sensitive(this->backup() != this->ui->party().rule());

      return ;
    } // void Rules::update_all()

  /**
   ** adjust the rules to the game with nines
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.10
   **/
  void
    Rules::adjust_to_with_nines()
    {
      if (!this->is_realized())
        return ;

      Rule& rule = ::party.rule();

      rule.set(Rule::ANNOUNCEMENT_NO_120, 11);
      rule.set(Rule::ANNOUNCEMENT_NO_90, 9);
      rule.set(Rule::ANNOUNCEMENT_NO_60, 6);
      rule.set(Rule::ANNOUNCEMENT_NO_30, 3);
      rule.set(Rule::ANNOUNCEMENT_NO_0, 3);

      return ;
    } // void Rules::adjust_to_with_nines()

  /**
   ** adjust the rules to the game without nines
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
    Rules::adjust_to_without_nines()
    {
      if (!this->is_realized())
        return ;

      Rule& rule = ::party.rule();

      rule.set(Rule::ANNOUNCEMENT_NO_120, 9);
      rule.set(Rule::ANNOUNCEMENT_NO_90, 7);
      rule.set(Rule::ANNOUNCEMENT_NO_60, 5);
      rule.set(Rule::ANNOUNCEMENT_NO_30, 3);
      rule.set(Rule::ANNOUNCEMENT_NO_0, 3);

      return ;
    } // void Rules::adjust_to_without_nines()

  /**
   ** adds to the group notebook a page for group 'name'.
   **
   ** @param     name   name of the group
   ** @param     image   image before the label
   **			(default: NULL)
   **
   ** @return    created vbox for the rules of the group
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  Gtk::VBox*
    Rules::add_group(string const& name, Gtk::Image* image)
    {
      Gtk::Alignment* alignment
        = Gtk::manage(new Gtk::Alignment(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
                                         0, 0));
      Gtk::VBox* vbox = Gtk::manage(new Gtk::VBox(false, 2 ex));
      alignment->add(*vbox);
      vbox->set_border_width(1 em);

      Gtk::HBox* bookmark = Gtk::manage(new Gtk::HBox(false, 1 em / 2));
      if (image)
        bookmark->add(*image);
      if (!name.empty()) {
        Gtk::Label* label = Gtk::manage(new Gtk::Label(name));
        this->ui->translations->add(*label, ::translation("Rule::Group::" + name));
        bookmark->add(*label);
      } // if (name)
      bookmark->show_all_children();

      alignment->set_data("group name",
                          new Translator::Translation(::translation("Rule::Group::" + name)));

      Gtk::Label* menu_label = Gtk::manage(new Gtk::Label(name));
      this->ui->translations->add(*menu_label, ::translation("Rule::Group::" + name));
      this->group_notebook->append_page(*alignment, *bookmark, *menu_label);
      this->group_notebook->set_tab_label_packing(*alignment, true, true, Gtk::PACK_START);

      return vbox;
    } // Gtk::VBox* Rules::add_group(string name, Gtk::Image* image = NULL)

  /**
   ** the page of the notebook has changed
   ** update the title of the window
   **
   ** @param     page     selected page
   ** @param     pageno   selected page number
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  void
    Rules::group_notebook_page_changed(GtkNotebookPage* page, guint pageno)
    {
      Translator::Translation* group_name
        = static_cast<Translator::Translation*>(this->group_notebook->get_nth_page(pageno)->get_data("group name"));
      //= static_cast<Translator::Translation*>(this->group_notebook->get_current()->get_child()->get_data("group name"));
      this->ui->translations->change(*this, ::translation("FreeDoko %ttitle%",
                                                          ::translation("~rules: %tgroup%",
                                                                        *group_name)));

      return ;
    } // void Rules::notebook_page_changed(GtkNotebookPage* page, guint pageno)

  /**
   ** a key has been pressed
   ** C-o: output of the rules on 'stdout'
   **
   ** @param     key   the key
   **
   ** @return    whether the key was managed
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.3
   **/
  bool
    Rules::on_key_press_event(GdkEventKey* key)
    {
      bool managed = false;

      if ((key->state & ~GDK_SHIFT_MASK) == GDK_CONTROL_MASK) {
        switch (key->keyval) {
        case GDK_o: // ouput of the rules
          cout << this->ui->party().rule() << endl;
          managed = true;
          break;
        case GDK_u: // update the rules
          this->update_all();
          managed = true;
          break;
        } // switch (key->keyval)
      } // if (key->state == GDK_CONTROL_MASK)

      return (managed
              || this->StickyDialog::on_key_press_event(key)
              || this->ui->key_press(key));
    } // bool Rules::on_key_press_event(GdkEventKey* key)


  /**
   ** Update the announcement texts (with or without knocking)
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
    Rules::update_announcement_text()
    {
      if (!this->is_realized())
        return ;

      this->ui->translations->change(this->type_unsigned[Rule::ANNOUNCEMENT_NO_120 - Rule::UNSIGNED_FIRST]->get_label(),
                                     ::translation(Rule::ANNOUNCEMENT_NO_120,
                                                   this->ui->party().rule()(Rule::KNOCKING) ? "knocking" : ""));
      this->ui->translations->change(*this->type_bool[Rule::ANNOUNCEMENT_RE_DOUBLES - Rule::BOOL_FIRST],
                                     ::translation(Rule::ANNOUNCEMENT_RE_DOUBLES,
                                                   this->ui->party().rule()(Rule::KNOCKING) ? "knocking" : ""));

      this->ui->translations->change(*this->type_bool[Rule::ANNOUNCEMENT_CONTRA_DOUBLES_AGAINST_RE - Rule::BOOL_FIRST],
                                     ::translation(Rule::ANNOUNCEMENT_CONTRA_DOUBLES_AGAINST_RE,
                                                   this->ui->party().rule()(Rule::KNOCKING) ? "knocking" : ""));

      this->ui->translations->change(*this->type_bool[Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS_ONLY_RE_CONTRA - Rule::BOOL_FIRST],
                                     ::translation(Rule::ANNOUNCEMENT_FIRST_TRICK_THIRTY_POINTS_ONLY_RE_CONTRA,
                                                   this->ui->party().rule()(Rule::KNOCKING) ? "knocking" : ""));

      return ;
    } // void Rules::update_announcement_text()

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
