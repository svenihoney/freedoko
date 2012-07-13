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

#include "icons.h"

#include "ui.h"
#include "cards.h"
#include "utils.h"
#include "splash_screen.h" // ToDo: remove it

#include "../../party/party.h"
#include "../../party/rule.h"
#include "../../game/game.h"
#include "../../misc/setting.h"
#include "../../misc/translations.h"

#include <gtkmm/image.h>

namespace UI_GTKMM_NS {
  unsigned const Icons::NUMBER_OF_TYPES = LAST + 1;

  /**
   ** -> result
   **
   ** @param     gametype   the gametype
   **
   ** @return    icon type for 'gametype'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  Icons::Type
    Icons::type(GameType const gametype)
    {
      switch(gametype) {
      case GAMETYPE::NORMAL:
        return RE;
      case GAMETYPE::THROWN_NINES:
        return THROWN_NINES;
      case GAMETYPE::THROWN_KINGS:
        return THROWN_KINGS;
      case GAMETYPE::THROWN_NINES_AND_KINGS:
        return THROWN_NINES_AND_KINGS;
      case GAMETYPE::THROWN_RICHNESS:
        return THROWN_RICHNESS;
      case GAMETYPE::POVERTY:
        if (::game_status == GAMESTATUS::GAME_RESERVATION) {
          Player const& player = ::party.game().player_current();
          if (player.type() == Player::HUMAN) {
            switch (player.hand().numberoftrumps()) {
            case 0:
              return POVERTY_TRUMPS_0;
            case 1:
              return POVERTY_TRUMPS_1;
            case 2:
              return POVERTY_TRUMPS_2;
            case 3:
            case 4:
            case 5:
              return POVERTY_TRUMPS_3;
            case UINT_MAX:
              return POVERTY;
            default:
              DEBUG_ASSERTION(false,
                              "Icons::icon(gametype):\n"
                              "  gametype: poverty\n"
                              "  number of poverty cards for player " << player.no() << ' ' << player.name() << " invalid: "
                              << player.hand().numberoftrumps());
              return POVERTY;
            } // switch (::party.game().poverty_trumpno_returned())
          } else {
            return POVERTY;
          }
        }
        if (::game_status == GAMESTATUS::GAME_POVERTY_SHIFT) {
          return POVERTY;
        }

        switch (::party.game().poverty_trumpno_returned()) {
        case 0:
          return POVERTY_TRUMPS_0;
        case 1:
          return POVERTY_TRUMPS_1;
        case 2:
          return POVERTY_TRUMPS_2;
        case 3:
        case 4:
        case 5:
          return POVERTY_TRUMPS_3;
        case UINT_MAX:
          return POVERTY;
        default:
          DEBUG_ASSERTION(false,
                          "Icons::icon(gametype):\n"
                          "  gametype: poverty\n"
                          "  number of poverty cards invalid: "
                          << ::party.game().poverty_trumpno_returned());
          return POVERTY;
        } // switch (::party.game().poverty_trumpno_returned())
      case GAMETYPE::FOX_HIGHEST_TRUMP:
        return FOX_HIGHEST_TRUMP;
      case GAMETYPE::GENSCHER:
        return GENSCHER;
      case GAMETYPE::MARRIAGE:
        return MARRIAGE;
      case GAMETYPE::MARRIAGE_SOLO:
      case GAMETYPE::MARRIAGE_SILENT:
        return MARRIAGE_SOLO;
      case GAMETYPE::SOLO_DIAMOND:
        return SOLO_DIAMOND;
      case GAMETYPE::SOLO_JACK:
        return SOLO_JACK;
      case GAMETYPE::SOLO_QUEEN:
        return SOLO_QUEEN;
      case GAMETYPE::SOLO_KING:
        return SOLO_KING;
      case GAMETYPE::SOLO_QUEEN_JACK:
        return SOLO_QUEEN_JACK;
      case GAMETYPE::SOLO_KING_JACK:
        return SOLO_KING_JACK;
      case GAMETYPE::SOLO_KING_QUEEN:
        return SOLO_KING_QUEEN;
      case GAMETYPE::SOLO_KOEHLER:
        return SOLO_KOEHLER;
      case GAMETYPE::SOLO_CLUB:
        return SOLO_CLUB;
      case GAMETYPE::SOLO_HEART:
        return SOLO_HEART;
      case GAMETYPE::SOLO_SPADE:
        return SOLO_SPADE;
      case GAMETYPE::SOLO_MEATLESS:
        return SOLO_MEATLESS;
      default:
        DEBUG_ASSERTION(false,
                        "Icons::type(gametype):\n"
                        "  unknown gametype '" << gametype << "'");
        return NONE;
      } // switch(gametype)
    } // static Icons::Type Icons::type(GameType const gametype)

  /**
   ** -> result
   **
   ** @param     marriage_selector   the marriage selector
   **
   ** @return    icon type for 'marriage_selector'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  Icons::Type
    Icons::type(MarriageSelector const marriage_selector)
    {
      switch(marriage_selector) {
      case MARRIAGE_SELECTOR::SILENT:
        return MARRIAGE_SOLO;
      case MARRIAGE_SELECTOR::TEAM_SET:
        return MARRIAGE;
      case MARRIAGE_SELECTOR::FIRST_FOREIGN:
        return MARRIAGE_FOREIGN;
      case MARRIAGE_SELECTOR::FIRST_TRUMP:
        return MARRIAGE_TRUMP;
      case MARRIAGE_SELECTOR::FIRST_COLOR:
        return MARRIAGE_COLOR;
      case MARRIAGE_SELECTOR::FIRST_CLUB:
        return MARRIAGE_CLUB;
      case MARRIAGE_SELECTOR::FIRST_SPADE:
        return MARRIAGE_SPADE;
      case MARRIAGE_SELECTOR::FIRST_HEART:
        return MARRIAGE_HEART;
      } // switch (marriage_selector)
      DEBUG_ASSERTION(false,
                      "Icons::type(marriage_selector):\n"
                      "  unknown marriage selecter '" << marriage_selector << "'");
      return NONE;
    } // static Icons::Type Icons::type(MarriageSelector const marriage_selector)

  /**
   ** -> result
   **
   ** @param     team   	the team
   **
   ** @return    icon type for 'team'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  Icons::Type
    Icons::type(Team const team)
    {
      switch(team) {
      case TEAM::RE:
        return RE;
      case TEAM::CONTRA:
        return CONTRA;
      default:
        return NONE;
      } // switch(team)
    } // static Icons::Type Icons::type(Team const team)

  /**
   ** -> result
   **
   ** @param     announcement   the announcement
   **
   ** @return    icon type for 'announcement'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  Icons::Type
    Icons::type(Announcement const announcement)
    {
      switch(announcement) {
      case ANNOUNCEMENT::NOANNOUNCEMENT:
        DEBUG_ASSERTION(false,
                        "Icon::icon(announcement, rotation):\n"
                        "  invalid announcement 'NOANNOUNCEMENT'");
        break;
      case ANNOUNCEMENT::NO120:
        return NO_120;
      case ANNOUNCEMENT::NO90:
        return NO_90;
      case ANNOUNCEMENT::NO60:
        return NO_60;
      case ANNOUNCEMENT::NO30:
        return NO_30;
      case ANNOUNCEMENT::NO0:
        return NO_0;
      case ANNOUNCEMENT::REPLY:
#ifdef POSTPONED
        DEBUG_ASSERTION(false,
                        "Icon::icon(announcement, rotation):\n"
                        "  invalid announcement 'REPLY'");
        return NONE;
#else
        return NO_120;
#endif
      case ANNOUNCEMENT::NO120_REPLY:
        return NO_120_REPLY;
      case ANNOUNCEMENT::NO90_REPLY:
        return NO_90_REPLY;
      case ANNOUNCEMENT::NO60_REPLY:
        return NO_60_REPLY;
      case ANNOUNCEMENT::NO30_REPLY:
        return NO_30_REPLY;
      case ANNOUNCEMENT::NO0_REPLY:
        return NO_0_REPLY;
      default:
        DEBUG_ASSERTION(false,
                        "Icons::type(announcement):\n"
                        "  unknown announcement '" << announcement << "'");
        return NONE;
      } // switch(announcement)
      return NONE;
    } // static Icons::Type Icons::type(Announcement const announcement)

  /**
   ** -> result
   **
   ** @param     color   	the color
   **
   ** @return    swines icon type for 'color'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  Icons::Type
    Icons::type_swines(Card::Color const color)
    {
      switch(color) {
      case Card::CLUB:
        return SWINES_CLUB;
      case Card::SPADE:
        return SWINES_SPADE;
      case Card::HEART:
        return SWINES_HEART;
      case Card::DIAMOND:
        return SWINES_DIAMOND;
      default:
        DEBUG_ASSERTION(false,
                        "Icon::swines(color)\n"
                        "  Color is not valid\n");
        return NONE;
      } // switch(color)
    } // static Icons::Type Icons::type_swines(Card::Color const color)

  /**
   ** -> result
   **
   ** @param     color   	the color
   **
   ** @return    hyperswines icon type for 'color'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  Icons::Type
    Icons::type_hyperswines(Card::Color const color)
    {
      switch(color) {
      case Card::CLUB:
        return (this->ui->party().rule()(Rule::WITH_NINES)
                ? HYPERSWINES_CLUB
                : HYPERSWINES_KING_CLUB);
      case Card::SPADE:
        return (this->ui->party().rule()(Rule::WITH_NINES)
                ? HYPERSWINES_SPADE
                : HYPERSWINES_KING_SPADE);
      case Card::HEART:
        return (this->ui->party().rule()(Rule::WITH_NINES)
                ? HYPERSWINES_HEART
                : HYPERSWINES_KING_HEART);
      case Card::DIAMOND:
        return (this->ui->party().rule()(Rule::WITH_NINES)
                ? HYPERSWINES_DIAMOND
                : HYPERSWINES_KING_DIAMOND);
      default:
        DEBUG_ASSERTION(false,
                        "Icon::hyperswines(color)\n"
                        "  Color is not valid\n");
        return NONE;
      } // switch(color)
    } // static Icons::Type Icons::type_hyperswines(Card::Color const color)

  /**
   ** -> result
   **
   ** @param     color   	the color
   **
   ** @return    swines-hyperswines icon type for 'color'
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.0
   **/
  Icons::Type
    Icons::type_swines_hyperswines(Card::Color const color)
    {
      switch(color) {
      case Card::CLUB:
        return (this->ui->party().rule()(Rule::WITH_NINES)
                ? SWINES_HYPERSWINES_CLUB
                : SWINES_HYPERSWINES_KING_CLUB);
      case Card::SPADE:
        return (this->ui->party().rule()(Rule::WITH_NINES)
                ? SWINES_HYPERSWINES_SPADE
                : SWINES_HYPERSWINES_KING_SPADE);
      case Card::HEART:
        return (this->ui->party().rule()(Rule::WITH_NINES)
                ? SWINES_HYPERSWINES_HEART
                : SWINES_HYPERSWINES_KING_HEART);
      case Card::DIAMOND:
        return (this->ui->party().rule()(Rule::WITH_NINES)
                ? SWINES_HYPERSWINES_DIAMOND
                : SWINES_HYPERSWINES_KING_DIAMOND);
      default:
        DEBUG_ASSERTION(false,
                        "Icon::swines_hyperswines(color)\n"
                        "  Color is not valid\n");
        return NONE;
      } // switch(color)
    } // static Icons::Type Icons::type_swines_hyperswines(Card::Color const color)

  /**
   ** constructor
   **
   ** @param     parent   the parent object
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Icons::Icons(Base* const parent) :
    Base(parent),
    sigc::trackable(),
    icon_(),
    max_width_(0),
    max_height_(0),
    drawable_ref(parent->ui->splash_screen->get_root_window()),
    managed_image_()
  {
    this->load();

    return ;
  } // Icons::Icons(Base* const parent)

  /**
   ** Destruktor
   **
   ** @param     -
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Icons::~Icons()
  {
    return ;
  } // Icons::~Icons()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    the scaling
   **
   ** @author    Diether Knof
   **
   ** @version   0.6,1
   **/
  double
    Icons::scaling() const
    {
      return this->ui->cards->scaling();
    } // double Icons::scaling() const

  /**
   ** update the scaling
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6,2
   **/
  void
    Icons::update_scaling()
    {
      // the scaling value is already updated in 'Cards'

      // set the scaling of the 'ScaledPixbufRotations'
      for (vector<Gdk::ScaledPixbufRotations>::iterator
           i = this->icon_.begin();
           i != this->icon_.end();
           ++i) {
        i->set_scaling(this->scaling());
      }

      return ;
    } // void Icons::update_scaling()

  /**
   ** update the maximal width and height
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
    Icons::update_max_values()
    {
      this->max_width_ = 0;
      for (vector<Gdk::ScaledPixbufRotations>::const_iterator
           i = this->icon_.begin();
           i != this->icon_.end();
           i++) {
        if (i->get_width() > this->max_width_)
          this->max_width_ = i->get_width();
      }

      this->max_height_ = 0;
      for (vector<Gdk::ScaledPixbufRotations>::const_iterator
           i = this->icon_.begin();
           i != this->icon_.end();
           i++) {
        if (i->get_height() > this->max_height_)
          this->max_height_ = i->get_height();
      }

      return ;
    } // void Icons::update_max_values()


  /**
   ** -> result
   **
   ** @param     rotation   the rotation
   **
   ** @return    maximal icon width
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    Icons::max_width(Rotation const rotation) const
    {
      if (   (rotation == LEFT)
          || (rotation == RIGHT))
        return this->max_height(ROTATION::clockwise(rotation));

      return this->max_width_;
    } // int Icons::max_width(Rotation rotation = UP) const

  /**
   ** -> result
   **
   ** @param     rotation   the rotation
   **
   ** @return    maximal icon height
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    Icons::max_height(Rotation const rotation) const
    {
      if (   (rotation == LEFT)
          || (rotation == RIGHT))
        return this->max_width(ROTATION::clockwise(rotation));

      return this->max_height_;
    } // int Icons::max_height(Rotation rotation = UP) const

  /**
   ** -> result
   **
   ** @param     icons      the icons to look at
   ** @param     rotation   the rotation
   **
   ** @return    maximal icon width of the icons
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    Icons::max_width(vector<Glib::RefPtr<Gdk::Pixbuf> > const& icons,
                     Rotation const rotation) const
    {
      if (   (rotation == LEFT)
          || (rotation == RIGHT))
        return this->max_height(icons, ROTATION::clockwise(rotation));

      int width = 0;

      for (vector<Glib::RefPtr<Gdk::Pixbuf> >::const_iterator
           i = icons.begin();
           i != icons.end();
           i++)
        if ((*i)->get_width() > width)
          width = (*i)->get_width();

      return width;
    } // int Icons::max_width(vector<Glib::RefPtr<Gdk::Pixbuf> > icons, Rotation rotation = UP) const

  /**
   ** -> result
   **
   ** @param     icons      the icons to look at
   ** @param     rotation   the rotation
   **
   ** @return    maximal icon height of the icons
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.7.5
   **/
  int
    Icons::max_height(vector<Glib::RefPtr<Gdk::Pixbuf> > const& icons,
                      Rotation const rotation) const
    {
      if (   (rotation == LEFT)
          || (rotation == RIGHT))
        return this->max_width(icons, ROTATION::clockwise(rotation));

      int height = 0;

      for (vector<Glib::RefPtr<Gdk::Pixbuf> >::const_iterator
           i = icons.begin();
           i != icons.end();
           i++)
        if ((*i)->get_height() > height)
          height = (*i)->get_height();

      return height;
    } // int Icons::max_height(vector<Glib::RefPtr<Gdk::Pixbuf> > icons, Rotation rotation = UP) const

  /**
   ** -> result
   **
   ** @param     type   	the type
   ** @param     rotation   the rotation
   **
   ** @return    the pixbuf of 'icon'
   **		(with rotation 'rotation')
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Glib::RefPtr<Gdk::Pixbuf> const&
    Icons::icon(Type const type, Rotation const rotation)
    {
      return this->icon_[type][rotation];
    } // Glib::RefPtr<Gdk::Pixbuf> Icons::icon(Type type, Rotation rotation = UP)

  /**
   ** -> result
   **
   ** @param     type   icon type
   **			(default: NONE)
   ** @param     scaling   additional scaling
   **			(default: 1)
   **
   ** @return    new image with the icon 'type' that is managed
   **		(that is size and theme)
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  Gtk::Image*
    Icons::new_managed_image(Type const type, double const scaling)
    {
      Glib::RefPtr<Gdk::Pixbuf> icon = this->icon(type);
      Gtk::Image* image = Gtk::manage(new Gtk::Image(icon));
      image->set_data("type", new Type(type));
      image->set_data("scaling", new double(scaling));
      if (scaling != 1)
        image->set(icon->scale_simple(static_cast<int>(icon->get_width() * scaling),
                                      static_cast<int>(icon->get_height() * scaling),
                                      Gdk::INTERP_TILES));

      image->signal_unrealize().connect(sigc::bind<Gtk::Image const* const>(sigc::mem_fun(*this, &Icons::remove_managed), image));

      this->managed_image_.push_back(image);

      return image;
    } // Gtk::Image* Icons::new_managed_image(Type const type = NONE, double const scaling = 1)

  /**
   ** changes the type of 'image'
   **
   ** @param     image   image to change the type of
   ** @param     type   new type
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  void
    Icons::change_managed(Gtk::Image* const image, Type const type)
    {
      DEBUG_ASSERTION(image->get_data("type"),
                      "Icons::change_managed:\n"
                      "  image has no data 'type'");
      // ToDo: Test whether the image is in the vector

      delete static_cast<Type*>(image->steal_data("type"));
      image->set_data("type", new Type(type));

      if (type == Icons::NONE)
        image->clear();
      else
        image->set(this->icon(type));

      return ;
    } // void Icons::change_managed(Gtk::Image* const image, Type const type)

  /**
   ** changes the scaling of 'image'
   **
   ** @param     image   image to change the type of
   ** @param     scaling   new scaling
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  void
    Icons::change_scaling(Gtk::Image* const image, double const scaling)
    {
      DEBUG_ASSERTION(image->get_data("scaling"),
                      "Icons::change_scaling:\n"
                      "  image has no data 'scaling'");
      // ToDo: Test whether the image is in the vector

      delete static_cast<double*>(image->steal_data("scaling"));
      image->set_data("scaling", new double(scaling));

      Type const type = *(static_cast<Type*>(image->get_data("type")));
      if (type == Icons::NONE)
        return ;

      if (scaling == 1)
        image->set(this->icon(type));
      else {
        Glib::RefPtr<Gdk::Pixbuf> icon = this->icon(type);
        image->set(icon->scale_simple(static_cast<int>(icon->get_width() * scaling),
                                      static_cast<int>(icon->get_height() * scaling),
                                      Gdk::INTERP_TILES));
      }

      return ;
    } // void Icons::change_scaling(Gtk::Image* const image, double const scaling)

  /**
   ** remove 'image' from the managed image list
   **
   ** @param     image   image to remove
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  void
    Icons::remove_managed(Gtk::Image const* const image)
    {
      for (vector<Gtk::Image*>::iterator i = this->managed_image_.begin();
           i != this->managed_image_.end();
          )
        if (*i == image) {
          delete static_cast<Type*>((*i)->steal_data("type"));
          delete static_cast<double*>((*i)->steal_data("scaling"));
          i = this->managed_image_.erase(i);
          break;
        } else {
          ++i;
        }

      return ;
    } // void Icons::remove_managed(Gtk::Image const* const image)

  /**
   ** update the managed images
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.6
   **/
  void
    Icons::update_managed()
    {
      for (vector<Gtk::Image*>::iterator i = this->managed_image_.begin();
           i != this->managed_image_.end();
           ++i) {
        Type const type = *(static_cast<Type*>((*i)->get_data("type")));
        if (type == Icons::NONE)
          continue;

        double const scaling = *(static_cast<double*>((*i)->get_data("scaling")));
        if ( (scaling == 1)
            || (scaling == 0) )
          (*i)->set(this->icon(type));
        else {
          Glib::RefPtr<Gdk::Pixbuf> icon = this->icon(type);
          (*i)->set(icon->scale_simple(static_cast<int>(icon->get_width() * scaling),
                                       static_cast<int>(icon->get_height() * scaling),
                                       Gdk::INTERP_TILES));
        }
      }

      return ;
    } // void Icons::update_managed()

  /**
   ** load the icons
   **
   ** @param     -
   **
   ** @return    -
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  void
    Icons::load()
    {
      if (Glib::file_test(::setting.path(Setting::CARDSET) + "/"
                          + ::setting(Setting::ICONS_DIRECTORY) + "/"
                          + "no_120"
                          + "." + ::setting(Setting::GRAPHIC_EXTENSION),
                          Glib::FILE_TEST_IS_REGULAR)) {

        if (this->icon_.empty()) {
          this->ui->error("Error at loading the icons from the cardset "
                          + ::setting(Setting::CARDSET) + ".\n"
                          + "Perhaps you should validate your 'FreeDokorc'-File "
                          + "(in the directory '"
                          + ::setting.path(Setting::CARDSET)
                          + "').");
        } else { // if !(this->icon_.empty())
          this->ui->information(::translation("Error at loading the icons from the cardset "
                                              + ::setting(Setting::CARDSET) + ".\n"
                                              "Keeping the old icons."),
                                INFORMATION::WARNING);
        } // if !(this->icon_.empty())

        return ;
      } // if (directory is valid)

      if (   (::game_status != GAMESTATUS::PROGRAMSTART)
          && (::game_status != GAMESTATUS::PARTY_INITIAL_LOADED) ) {
        this->ui->add_status_message(::translation("loading the icons"));
      } // if (::game_status != GAMESTATUS::PROGRAMSTART)

      vector<Gdk::ScaledPixbufRotations> icon_new;
      for (unsigned type = 0; type < NUMBER_OF_TYPES; type++) {
        icon_new.push_back(this->load(static_cast<Type>(type)));
        if (   (::game_status == GAMESTATUS::PROGRAMSTART)
            || (::game_status == GAMESTATUS::PARTY_INITIAL_LOADED) )
          this->ui->add_progress(1.0 / NUMBER_OF_TYPES);
      } // for (n \in names)
      if (   (::game_status != GAMESTATUS::PROGRAMSTART)
          && (::game_status != GAMESTATUS::PARTY_INITIAL_LOADED) ) {
        this->ui->remove_status_message(::translation("loading the icons"));
      } // if (::game_status != GAMESTATUS::PROGRAMSTART)

      this->icon_ = icon_new;

      this->update_scaling();
      this->update_managed();
      this->update_max_values();

      return ;
    } // void Icons::load()

  /**
   ** load or construct the icons
   **
   ** @param     type   	the icon type
   **
   ** @return    loaded/constructed icon
   **
   ** @author    Diether Knof
   **
   ** @version   0.7.4
   **/
  Gdk::ScaledPixbufRotations
    Icons::load(Type const type)
    {
      Gdk::ScaledPixbufRotations icon;

      switch(type) {
      case NONE:
        icon = Gdk::ScaledPixbufRotations();
        break;
      case RE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "re" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN);
        break;
      case CONTRA:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "contra" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon) {
          icon = this->construct(Card::CLUB_QUEEN);
          // strike out the icon
          icon = this->construct(Card::CLUB_QUEEN);
        } // if (!icon)
        break;
      case THROWN_NINES:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "thrown_nines" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::NINE),
                                 Card(Card::SPADE, Card::NINE),
                                 Card(Card::HEART, Card::NINE),
                                 Card(Card::DIAMOND, Card::NINE));
        break;
      case THROWN_KINGS:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "thrown_kings" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::KING),
                                 Card(Card::SPADE, Card::KING),
                                 Card(Card::HEART, Card::KING),
                                 Card(Card::DIAMOND, Card::KING));
        break;
      case THROWN_NINES_AND_KINGS:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "thrown_nines_and_kings" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::KING),
                                 Card(Card::SPADE, Card::KING),
                                 Card(Card::HEART, Card::NINE),
                                 Card(Card::DIAMOND, Card::NINE) );
        break;
      case THROWN_RICHNESS:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "thrown_richness" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB,    Card::TEN),
                                 Card(Card::SPADE,   Card::TEN),
                                 Card(Card::HEART,   Card::TEN),
                                 Card(Card::DIAMOND, Card::TEN));
        break;
      case FOX_HIGHEST_TRUMP:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "fox_highest_trump" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::ACE),
                                 Card(Card::DIAMOND, Card::TEN),
                                 Card(Card::DIAMOND, Card::KING),
                                 Card(Card::DIAMOND, Card::NINE));
        break;
      case MARRIAGE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "marriage" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN,
                                 Card::CLUB_QUEEN);
        break;
      case MARRIAGE_SOLO:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "marriage.solo" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "marriage" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN,
                                 Card::CLUB_QUEEN);
        break;
      case MARRIAGE_FOREIGN:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "marriage.foreign" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN,
                                 Card::CLUB_QUEEN,
                                 Card(Card::DIAMOND, Card::JACK),
                                 Card(Card::SPADE, Card::ACE));
        break;
      case MARRIAGE_TRUMP:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "marriage.trump" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN,
                                 Card::CLUB_QUEEN,
                                 Card(Card::DIAMOND, Card::JACK),
                                 Card(Card::SPADE, Card::ACE));
        break;
      case MARRIAGE_COLOR:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "marriage.color" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN,
                                 Card::CLUB_QUEEN,
                                 Card(Card::HEART, Card::ACE),
                                 Card(Card::SPADE, Card::ACE));
        break;
      case MARRIAGE_CLUB:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "marriage.club" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN,
                                 Card::CLUB_QUEEN,
                                 Card(Card::CLUB, Card::ACE),
                                 Card(Card::CLUB, Card::ACE));
        break;
      case MARRIAGE_SPADE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "marriage.spade" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN,
                                 Card::CLUB_QUEEN,
                                 Card(Card::SPADE, Card::ACE),
                                 Card(Card::SPADE, Card::ACE));
        break;
      case MARRIAGE_HEART:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "marriage.heart" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card::CLUB_QUEEN,
                                 Card::CLUB_QUEEN,
                                 Card(Card::HEART, Card::ACE),
                                 Card(Card::HEART, Card::ACE));
        break;
      case GENSCHER:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "genscher" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::KING),
                                 Card(Card::DIAMOND, Card::KING));
        break;
      case POVERTY:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "poverty" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::QUEEN),
                                 Card(Card::SPADE, Card::JACK),
                                 Card(Card::DIAMOND, Card::TEN));
        break;
      case POVERTY_TRUMPS_0:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "poverty.0" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::ACE),
                                 Card(Card::HEART, Card::KING),
                                 Card(Card::SPADE, Card::TEN));
        break;
      case POVERTY_TRUMPS_1:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "poverty.1" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::SPADE, Card::QUEEN),
                                 Card(Card::HEART, Card::NINE),
                                 Card(Card::CLUB, Card::KING));
        break;
      case POVERTY_TRUMPS_2:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "poverty.2" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::QUEEN),
                                 Card(Card::SPADE, Card::JACK),
                                 Card(Card::CLUB, Card::ACE));
        break;
      case POVERTY_TRUMPS_3:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "poverty.3" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::QUEEN),
                                 Card(Card::CLUB, Card::JACK),
                                 Card(Card::DIAMOND, Card::KING));
        break;
      case SWINES_CLUB:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines.club" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::ACE),
                                 Card(Card::CLUB, Card::ACE));
        break;
      case SWINES_SPADE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines.spade" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::SPADE, Card::ACE),
                                 Card(Card::SPADE, Card::ACE));
        break;
      case SWINES_HEART:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines.heart" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::ACE),
                                 Card(Card::HEART, Card::ACE));
        break;
      case SWINES_DIAMOND:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines.diamond" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::ACE),
                                 Card(Card::DIAMOND, Card::ACE));
        break;
      case HYPERSWINES_CLUB:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "hyperswines.club" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::NINE),
                                 Card(Card::CLUB, Card::NINE));
        break;
      case HYPERSWINES_SPADE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "hyperswines.spade" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::SPADE, Card::NINE),
                                 Card(Card::SPADE, Card::NINE));
        break;
      case HYPERSWINES_HEART:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "hyperswines.heart" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::NINE),
                                 Card(Card::HEART, Card::NINE));
        break;
      case HYPERSWINES_DIAMOND:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "hyperswines.diamond" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::NINE),
                                 Card(Card::DIAMOND, Card::NINE));
        break;
      case HYPERSWINES_KING_CLUB:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "hyperswines.king.club" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::KING),
                                 Card(Card::CLUB, Card::KING));
        break;
      case HYPERSWINES_KING_SPADE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "hyperswines.king.spade" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::SPADE, Card::KING),
                                 Card(Card::SPADE, Card::KING));
        break;
      case HYPERSWINES_KING_HEART:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "hyperswines.king.heart" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::KING),
                                 Card(Card::HEART, Card::KING));
        break;
      case HYPERSWINES_KING_DIAMOND:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "hyperswines.king.diamond" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::KING),
                                 Card(Card::DIAMOND, Card::KING));
        break;
      case SWINES_HYPERSWINES_CLUB:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines-hyperswines.club" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines-hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::ACE),
                                 Card(Card::CLUB, Card::NINE));
        break;
      case SWINES_HYPERSWINES_SPADE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines-hyperswines.spade" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines-hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::SPADE, Card::ACE),
                                 Card(Card::SPADE, Card::NINE));
        break;
      case SWINES_HYPERSWINES_HEART:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines-hyperswines.heart" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines-hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::ACE),
                                 Card(Card::HEART, Card::NINE));
        break;
      case SWINES_HYPERSWINES_DIAMOND:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines-hyperswines.diamond" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines-hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::ACE),
                                 Card(Card::DIAMOND, Card::NINE));
        break;
      case SWINES_HYPERSWINES_KING_CLUB:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines-hyperswines.king.club" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines-hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::ACE),
                                 Card(Card::CLUB, Card::KING));
        break;
      case SWINES_HYPERSWINES_KING_SPADE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines-hyperswines.king.spade" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines-hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::SPADE, Card::ACE),
                                 Card(Card::SPADE, Card::KING));
        break;
      case SWINES_HYPERSWINES_KING_HEART:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines-hyperswines.king.heart" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines-hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::ACE),
                                 Card(Card::HEART, Card::KING));
        break;
      case SWINES_HYPERSWINES_KING_DIAMOND:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "swines-hyperswines.king.diamond" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                            + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                            + "swines-hyperswines" + "."
                                            + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::ACE),
                                 Card(Card::DIAMOND, Card::KING));
        break;
      case DOLLEN:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "dollen" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::TEN),
                                 Card(Card::HEART, Card::TEN));
        break;

      case DOPPELKOPF:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "doppelkopf" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::ACE),
                                 Card(Card::SPADE, Card::TEN),
                                 Card(Card::DIAMOND, Card::ACE),
                                 Card(Card::HEART, Card::TEN));
        break;
      case SOLO_CLUB:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.club" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::SPADE, Card::QUEEN),
                                 Card(Card::HEART, Card::JACK),
                                 Card(Card::CLUB, Card::ACE),
                                 Card(Card::CLUB, Card::KING));
        break;
      case SOLO_SPADE:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.spade" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::QUEEN),
                                 Card(Card::DIAMOND, Card::JACK),
                                 Card(Card::SPADE, Card::ACE),
                                 Card(Card::SPADE, Card::KING));
        break;
      case SOLO_HEART:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.heart" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::DIAMOND, Card::QUEEN),
                                 Card(Card::CLUB, Card::JACK),
                                 Card(Card::HEART, Card::ACE),
                                 Card(Card::HEART, Card::KING));
        break;
      case SOLO_DIAMOND:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.diamond" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::HEART, Card::QUEEN),
                                 Card(Card::SPADE, Card::JACK),
                                 Card(Card::DIAMOND, Card::ACE),
                                 Card(Card::DIAMOND, Card::KING));
        break;
      case SOLO_JACK:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.jack" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::JACK),
                                 Card(Card::SPADE, Card::JACK),
                                 Card(Card::HEART, Card::JACK),
                                 Card(Card::DIAMOND, Card::JACK));
        break;
      case SOLO_QUEEN:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.queen" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::QUEEN),
                                 Card(Card::SPADE, Card::QUEEN),
                                 Card(Card::HEART, Card::QUEEN),
                                 Card(Card::DIAMOND, Card::QUEEN));
        break;
      case SOLO_KING:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.king" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::KING),
                                 Card(Card::SPADE, Card::KING),
                                 Card(Card::HEART, Card::KING),
                                 Card(Card::DIAMOND, Card::KING));
        break;
      case SOLO_QUEEN_JACK:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.queen-jack" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::QUEEN),
                                 Card(Card::SPADE, Card::QUEEN),
                                 Card(Card::HEART, Card::JACK),
                                 Card(Card::DIAMOND, Card::JACK));
        break;
      case SOLO_KING_JACK:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.king-jack" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::KING),
                                 Card(Card::SPADE, Card::KING),
                                 Card(Card::HEART, Card::JACK),
                                 Card(Card::DIAMOND, Card::JACK));
        break;
      case SOLO_KING_QUEEN:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.king-queen" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::KING),
                                 Card(Card::SPADE, Card::KING),
                                 Card(Card::HEART, Card::QUEEN),
                                 Card(Card::DIAMOND, Card::QUEEN));
        break;
      case SOLO_KOEHLER:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.koehler" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::KING),
                                 Card(Card::SPADE, Card::KING),
                                 Card(Card::HEART, Card::QUEEN),
                                 Card(Card::DIAMOND, Card::JACK));
        break;
      case SOLO_MEATLESS:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "solo.meatless" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));
        if (!icon)
          icon = this->construct(Card(Card::CLUB, Card::ACE),
                                 Card(Card::SPADE, Card::ACE),
                                 Card(Card::HEART, Card::ACE),
                                 Card(Card::DIAMOND, Card::ACE));
        break;
      case NO_120:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_120" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 120'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO120)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_90:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_90" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 90'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO90)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_60:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_60" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 60'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO60)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_30:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_30" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 30'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO30)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_0:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_0" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 0'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO0)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_120_REPLY:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_120_reply" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 120 reply'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO120_REPLY)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_90_REPLY:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_90_reply" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 90 reply'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO90_REPLY)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_60_REPLY:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_60_reply" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 60 reply'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO60_REPLY)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_30_REPLY:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_30_reply" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 30 reply'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO30_REPLY)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;
      case NO_0_REPLY:
        icon = Gdk::ScaledPixbufRotations(::setting.path(Setting::CARDSET) + "/"
                                          + ::setting.value(Setting::ICONS_DIRECTORY) + "/"
                                          + "no_0_reply" + "."
                                          + ::setting(Setting::GRAPHIC_EXTENSION));

        if (!icon) {
          if (this->icon_.size() <= static_cast<unsigned>(type)) {
            // no icon loaded so far -- exiting
            this->ui->error("Cannot construct the icon for the announcement 'no 0 reply'.\n"
                            "Perhaps you should validate your 'FreeDokorc'-File "
                            "(in the directory '"
                            + ::setting.path(Setting::CARDSET) + "/"
                            + ::setting.value(Setting::ICONS_DIRECTORY)
                            + "').\n"
                            "Exiting.");
          } // if (this->icon_.size() <= type)
          this->ui->information(::translation("Cannot construct the icon for the announcement '%tannouncement%'.\n"
                                              "Keeping the old one.",
                                              ::translation(ANNOUNCEMENT::NO0_REPLY)),
                                INFORMATION::WARNING);
          icon = this->icon(type);
        } // if (!icon)

        break;

      } // switch(type)

      return icon;
    } // Gdk::ScaledPixbufRotations Icons::load(Type type)

  /**
   ** construct an icon from one card
   **
   ** @param     card1	first card
   **
   ** @return    constructed icon
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Gdk::ScaledPixbufRotations
    Icons::construct(Card const card1)
    {
      vector<Card> cards;
      cards.push_back(card1);

      return this->construct(cards);
    } // Gdk::ScaledPixbufRotations Icons::construct(Card const card1)

  /**
   ** construct an icon from two cards
   **
   ** @param     card1	first card
   ** @param     card2	second card
   **
   ** @return    constructed icon
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Gdk::ScaledPixbufRotations
    Icons::construct(Card const card1, Card const card2)
    {
      vector<Card> cards;
      cards.push_back(card1);
      cards.push_back(card2);

      return this->construct(cards);
    } // Gdk::ScaledPixbufRotations Icons::construct(Card const card1, Card const card2)

  /**
   ** construct an icon from six cards
   **
   ** @param     card1	first card
   ** @param     card2	second card
   ** @param     card3	third card
   **
   ** @return    constructed icon
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Gdk::ScaledPixbufRotations
    Icons::construct(Card const card1,
                     Card const card2,
                     Card const card3)
    {
      vector<Card> cards;
      cards.push_back(card1);
      cards.push_back(card2);
      cards.push_back(card3);

      return this->construct(cards);
    } // Gdk::ScaledPixbufRotations Icons::construct(Card const card1, Card const card2, Card const card3)

  /**
   ** construct an icon from six cards
   **
   ** @param     card1	first card
   ** @param     card2	second card
   ** @param     card3	third card
   ** @param     card4	fourth card
   **
   ** @return    constructed icon
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Gdk::ScaledPixbufRotations
    Icons::construct(Card const card1,
                     Card const card2,
                     Card const card3,
                     Card const card4)
    {
      vector<Card> cards;
      cards.push_back(card1);
      cards.push_back(card2);
      cards.push_back(card3);
      cards.push_back(card4);

      return this->construct(cards);
    } // Gdk::ScaledPixbufRotations Icons::construct(Card const card1, Card const card2, Card const card3, Card const card4)

  /**
   ** construct an icon from six cards
   **
   ** @param     card1	first card
   ** @param     card2	second card
   ** @param     card3	third card
   ** @param     card4	fourth card
   ** @param     card5	fifth card
   **
   ** @return    constructed icon
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Gdk::ScaledPixbufRotations
    Icons::construct(Card const card1,
                     Card const card2,
                     Card const card3,
                     Card const card4,
                     Card const card5)
    {
      vector<Card> cards;
      cards.push_back(card1);
      cards.push_back(card2);
      cards.push_back(card3);
      cards.push_back(card4);
      cards.push_back(card5);

      return this->construct(cards);
    } // Gdk::ScaledPixbufRotations Icons::construct(Card const card1, Card const card2, Card const card3, Card const card4, Card const card5)

  /**
   ** construct an icon from six cards
   **
   ** @param     card1	first card
   ** @param     card2	second card
   ** @param     card3	third card
   ** @param     card4	fourth card
   ** @param     card5	fifth card
   ** @param     card6	sixth card
   **
   ** @return    constructed icon
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Gdk::ScaledPixbufRotations
    Icons::construct(Card const card1,
                     Card const card2,
                     Card const card3,
                     Card const card4,
                     Card const card5,
                     Card const card6)
    {
      vector<Card> cards;
      cards.push_back(card1);
      cards.push_back(card2);
      cards.push_back(card3);
      cards.push_back(card4);
      cards.push_back(card5);
      cards.push_back(card6);

      return this->construct(cards);
    } // Gdk::ScaledPixbufRotations Icons::construct(Card const card1, Card const card2, Card const card3, Card const card4, Card const card5, Card const card6)

  /**
   ** construct an icon from the cards
   **
   ** @param     cards   cards
   **
   ** @return    constructed icon
   **
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **
   ** @bug	the transparent regions are copied, too
   **/
  Gdk::ScaledPixbufRotations
    Icons::construct(vector<Card> const& cards)
    {
      // take the half height and overlap with a fourth width

      int const width = (this->ui->cards->width_original()
                         * (4 + (cards.size() - 1))
                         / 4);
      int const height = (this->ui->cards->height_original() / 2);

      Glib::RefPtr<Gdk::Pixbuf> pixbuf
        = Gdk::Pixbuf::create(this->drawable_ref,
                              this->drawable_ref->get_colormap(),
                              0, 0, 0, 0,
                              width, height
                             )->add_alpha(false, 0, 0, 0);

      for (unsigned i = 0; i < cards.size(); i++)
        this->ui->cards->card_original(cards[i])->copy_area(0, 0,
                                                            this->ui->cards->width_original(),
                                                            this->ui->cards->height_original() / 2,
                                                            pixbuf,
                                                            i * this->ui->cards->width_original() / 4, 0);


      return Gdk::ScaledPixbufRotations(pixbuf->scale_simple(width * 4 / 5,
                                                             height * 4 / 5,
                                                             Gdk::INTERP_BILINEAR));
    } // Gdk::ScaledPixbufRotations Icons::construct(vector<Card> const& cards)

} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
