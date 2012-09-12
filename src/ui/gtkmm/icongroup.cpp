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

#include "icongroup.h"
#include "table.h"
#include "ui.h"

#include "cards.h"
#include "icons.h"
#include "hand.h"
#include "name.h"

#include "../../game/game.h"
#include "../../party/rule.h"
#include "../../player/player.h"
#include "../../player/human/human.h"
#include "../../misc/setting.h"

namespace UI_GTKMM_NS {

  /**
   ** Constructor
   **
   ** @param     table   the table
   ** @param     player   the player
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  Icongroup::Icongroup(Table* const table, Player const* const player) :
    HTIN(table, player)
  {
    return ;
  } // Icongroup::Icongroup(Table* const table, Player const* const player)

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
  Icongroup::~Icongroup()
  {
    return ;
  } // Icongroup::~Icongroup()

  /**
   ** draws the icongroup
   **
   ** @param     update   whether to update the area
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  void
    Icongroup::draw(bool const update)
    {
      this->draw_announcement(update);
      this->draw_swines(update);
      this->draw_team(update);

      return ;
    } // void Icongroup::draw(bool const update = true)

  /**
   ** draws the team/gametype icon
   **
   ** @param     update   whether to update the area
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @todo	silent marriage
   **
   ** @version   0.6.3
   **/
  void
    Icongroup::draw_team(bool const update)
    {
      if(!(::game_status & GAMESTATUS::GAME))
        return ;

      Game const& game = this->player->game();

      if (   (::game_status < GAMESTATUS::GAME_RESERVATION)
          && !(   game.exists_soloplayer()
               && (this->player->no() == game.soloplayer().no()) )  )
        return ;

      Player const* const soloplayer
        = (game.exists_soloplayer()
           ? &game.soloplayer()
           : ((game.type() == GAMETYPE::NORMAL)
              ? NULL
              : &game.player_current()));

      // the values for the icon of maximal size
      Gdk::Rectangle max_outline;
      // the possible icons (from before, needed for overwriting old icons)
      vector<Glib::RefPtr<Gdk::Pixbuf> > possible_icons;

      // take the team of the game
      Team const team = game.teaminfo_for_humans(*this->player);

      Glib::RefPtr<Gdk::Pixbuf> icon;

      if (::game_status == GAMESTATUS::GAME_RESERVATION) {
        for (int t = GAMETYPE::FIRST; t <= GAMETYPE::LAST; ++t)
          if (t != GAMETYPE::POVERTY)
            possible_icons.push_back(this->ui->icons->icon(static_cast<GameType>(t),
                                                           this->rotation()));
        possible_icons.push_back(this->ui->icons->icon(Icons::POVERTY,
                                                       this->rotation()));

        if (this->player->type() == Player::HUMAN) {
          ::Reservation const& reservation = this->player->reservation();
          switch (reservation.game_type) {
          case GAMETYPE::NORMAL:
            icon = this->ui->icons->icon(TEAM::RE, this->rotation());
            break;
          case GAMETYPE::POVERTY:
            {
            Icons::Type icon_type = Icons::POVERTY;
            switch (this->player->hand().numberoftrumps()) {
            case 0:
              icon_type = Icons::POVERTY_TRUMPS_0;
              break;
            case 1:
              icon_type = Icons::POVERTY_TRUMPS_1;
              break;
            case 2:
              icon_type = Icons::POVERTY_TRUMPS_2;
              break;
            case 3:
            case 4:
            case 5:
              icon_type = Icons::POVERTY_TRUMPS_3;
              break;
            case UINT_MAX:
              icon_type = Icons::POVERTY;
              break;
            default:
              DEBUG_ASSERTION(false,
                              "IconGroup::draw_team():\n"
                              "  gametype: poverty\n"
                              "  number of poverty cards for player " << this-player->no() << ' ' << this->player->name() << " invalid: "
                              << this->player->hand().numberoftrumps() << '\n'
                              << "reservation:\n"
                              << this->player->reservation() << '\n'
                              << "game status: " << ::game_status);
              icon_type = Icons::POVERTY;
              break;
            } // switch (this->player->hand().numberoftrumps())
            icon = this->ui->icons->icon(icon_type,
                                         this->rotation());
            }
            break;
          case GAMETYPE::GENSCHER:
            icon = this->ui->icons->icon(Icons::GENSCHER,
                                         this->rotation());
            break;
          case GAMETYPE::MARRIAGE:
            icon = this->ui->icons->icon(reservation.marriage_selector,
                                         this->rotation());
            break;
          case GAMETYPE::MARRIAGE_SOLO:
          case GAMETYPE::MARRIAGE_SILENT:
          case GAMETYPE::THROWN_NINES:
          case GAMETYPE::THROWN_KINGS:
          case GAMETYPE::THROWN_NINES_AND_KINGS:
          case GAMETYPE::THROWN_RICHNESS:
          case GAMETYPE::FOX_HIGHEST_TRUMP:
          case GAMETYPE::SOLO_DIAMOND:
          case GAMETYPE::SOLO_JACK:
          case GAMETYPE::SOLO_QUEEN:
          case GAMETYPE::SOLO_KING:
          case GAMETYPE::SOLO_QUEEN_JACK:
          case GAMETYPE::SOLO_KING_JACK:
          case GAMETYPE::SOLO_KING_QUEEN:
          case GAMETYPE::SOLO_KOEHLER:
          case GAMETYPE::SOLO_CLUB:
          case GAMETYPE::SOLO_HEART:
          case GAMETYPE::SOLO_SPADE:
          case GAMETYPE::SOLO_MEATLESS:
            icon = this->ui->icons->icon(reservation.game_type,
                                         this->rotation());
            break;
          } // switch (this->player->reservation().game_type())

        } // if (this->player->type() == Player::HUMAN)

      } else if (   game.is_finished()
                 || (   game.rule()(Rule::SHOW_SOLOPLAYER_IN_GAME)
                     && ::setting(Setting::SHOW_SOLOPLAYER_IN_GAME))) {
        switch (game.type()) {
        case GAMETYPE::NORMAL:
          break;
        case GAMETYPE::POVERTY:
          if (this->player == soloplayer) {
            icon = this->ui->icons->icon(game.type(), this->rotation());
            possible_icons.push_back(icon);
            possible_icons.push_back(this->ui->icons->icon(Icons::POVERTY,
                                                           this->rotation()));
            possible_icons.push_back(icon);
          } else if (   (::game_status >= GAMESTATUS::GAME_START)
                     && (this->player->team() == TEAM::RE) ) {
            icon = this->ui->icons->icon(TEAM::RE, this->rotation());
            possible_icons.push_back(icon);
          }
          break;
        case GAMETYPE::GENSCHER:
          if (this->player == soloplayer)
            icon = this->ui->icons->icon(Icons::GENSCHER,
                                         this->rotation());
          else if (this->player->team() == TEAM::RE)
            icon = this->ui->icons->icon(TEAM::RE, this->rotation());
          else
            icon = this->ui->icons->icon(TEAM::CONTRA, this->rotation());
          break;
        case GAMETYPE::MARRIAGE:
        case GAMETYPE::MARRIAGE_SOLO:
          if (this->player == soloplayer) {
            icon = this->ui->icons->icon(game.marriage_selector(),
                                         this->rotation());
            for (int s = MARRIAGE_SELECTOR::FIRST;
                 s <= MARRIAGE_SELECTOR::LAST;
                 s++)
              possible_icons.push_back(this->ui->icons->icon(MarriageSelector(s),
                                                             this->rotation()));
          } else if (game.marriage_selector() == MARRIAGE_SELECTOR::TEAM_SET) {
            icon = this->ui->icons->icon(team, this->rotation());
          } // if !(this->player == soloplayer)
          break;
        case GAMETYPE::MARRIAGE_SILENT:
          if (this->player == soloplayer) {
            icon = this->ui->icons->icon(game.type(),
                                         this->rotation());
          } else {
            icon.clear();
            possible_icons.push_back(this->ui->icons->icon(TEAM::CONTRA,
                                                           this->rotation()));
          }
        case GAMETYPE::THROWN_NINES:
        case GAMETYPE::THROWN_KINGS:
        case GAMETYPE::THROWN_NINES_AND_KINGS:
        case GAMETYPE::THROWN_RICHNESS:
        case GAMETYPE::FOX_HIGHEST_TRUMP:
        case GAMETYPE::SOLO_DIAMOND:
        case GAMETYPE::SOLO_JACK:
        case GAMETYPE::SOLO_QUEEN:
        case GAMETYPE::SOLO_KING:
        case GAMETYPE::SOLO_QUEEN_JACK:
        case GAMETYPE::SOLO_KING_JACK:
        case GAMETYPE::SOLO_KING_QUEEN:
        case GAMETYPE::SOLO_KOEHLER:
        case GAMETYPE::SOLO_CLUB:
        case GAMETYPE::SOLO_HEART:
        case GAMETYPE::SOLO_SPADE:
        case GAMETYPE::SOLO_MEATLESS:
          if (this->player == soloplayer)
            icon = this->ui->icons->icon(game.type(), this->rotation());
          else
            icon.clear();
          break;
        } // switch(game.type())
      } // if (icon shall be shown for soloplayer)

      // special case: silent marriage
      if (   !icon
          && (game.humanno() == 1)
          && (this->player->type() == Player::HUMAN)
          && (   (   (game.type() == GAMETYPE::MARRIAGE_SILENT)
                  && (*this->player == game.soloplayer()) )
              || (   (game.type() == GAMETYPE::NORMAL)
                  && (this->player->hand().numberofall(Card::CLUB_QUEEN)
                      == game.rule()(Rule::NUMBER_OF_SAME_CARDS)))
             )
         ) {
        icon = this->ui->icons->icon(GAMETYPE::MARRIAGE_SILENT,
                                     this->rotation());
      }

      if (!icon) {
        if (   game.is_finished()
            || (   game.rule()(Rule::SHOW_KNOWN_TEAMS_IN_GAME)
                && ::setting(Setting::SHOW_KNOWN_TEAMS_IN_GAME))) {
          icon = this->ui->icons->icon(team,
                                       this->rotation());
          icon = this->ui->icons->icon(team, this->rotation());
        } // if (show teams)
      } // if (!icon)

      if (   !icon
          && ::setting(Setting::SHOW_AI_INFORMATION_TEAMS)
          && game.rule()(Rule::SHOW_KNOWN_TEAMS_IN_GAME)
          && (game.humanno() == 1)
          && (game.human_player()->teaminfo(*this->player) != TEAM::UNKNOWN)
          && (game.type() == GAMETYPE::NORMAL)
         ) {
        icon = this->ui->icons->icon(::maybe_to_team(game.human_player()->teaminfo(*this->player)), this->rotation());
        if (icon
            && !::is_real(game.human_player()->teaminfo(*this->player))) {
          Glib::RefPtr<Gdk::Pixbuf> copy = icon->copy();
          icon->saturate_and_pixelate(copy, 5, true);
          icon = copy;
        } // if (team is maybe)
      } // if (icon)

      if (icon)
        possible_icons.push_back(icon);
      possible_icons.push_back(this->ui->icons->icon(TEAM::RE,
                                                     this->rotation()));
      possible_icons.push_back(this->ui->icons->icon(TEAM::CONTRA,
                                                     this->rotation()));

      if (update) {
        max_outline.set_width(this->ui->icons->max_width(possible_icons));
        max_outline.set_height(this->ui->icons->max_height(possible_icons));
        max_outline.set_x(this->team_start_x(max_outline.get_width()));
        max_outline.set_y(this->team_start_y(max_outline.get_height()));

        this->table().clear(max_outline);
      } // if (update)

      if (icon) {
        this->table().pixmap->draw_pixbuf(this->table().gc,
                                          icon,
                                          0, 0,
                                          this->team_start_x(icon->get_width()),
                                          this->team_start_y(icon->get_height()),
                                          -1, -1,
                                          Gdk::RGB_DITHER_NONE,
                                          0, 0);
      }

      if (update)
        this->table().update(max_outline);

      return ;
    } // void Icongroup::draw_team(bool const update = true)

  /**
   ** draws the announcement icon
   **
   ** @param     update   whether to update the area
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  void
    Icongroup::draw_announcement(bool const update)
    {
      if (! (   (::game_status & GAMESTATUS::GAME)
             && (::game_status > GAMESTATUS::GAME_RESERVATION)) )
        return ;

      // the values for the icon of maximal size
      Gdk::Rectangle max_outline;

      if (update) {
        vector<Glib::RefPtr<Gdk::Pixbuf> > possible_icons;
        for (unsigned a = ANNOUNCEMENT::NO120; a <= ANNOUNCEMENT::NO0; a++)
          possible_icons.push_back(this->ui->icons->icon(Announcement(a),
                                                         this->rotation()));
        max_outline.set_width(this->ui->icons->max_width(possible_icons));
        max_outline.set_height(this->ui->icons->max_height(possible_icons));
        max_outline.set_x(this->announcement_start_x(max_outline.get_width()));
        max_outline.set_y(this->announcement_start_y(max_outline.get_height()));


        this->table().clear(max_outline);
      } // if (update)

      Announcement announcement = this->player->announcement();

      if (   ::setting(Setting::ANNOUNCE_IN_TABLE)
          && (this->player->type() == Player::HUMAN)
          && (this->mouse_over_next_announcement()
              || (   (this->player->announcement()
                      == ANNOUNCEMENT::NOANNOUNCEMENT)
                  && (this->player->next_announcement()
                      != ANNOUNCEMENT::NOANNOUNCEMENT)))) {
        announcement = this->player->next_announcement();
        if (!announcement)
          announcement = this->player->game().announcement_valid(ANNOUNCEMENT::REPLY, *this->player);
      }

      if (announcement == ANNOUNCEMENT::REPLY)
        announcement = ANNOUNCEMENT::to_reply(this->player->game().announcement_of_team(::opposite(this->player->team())));

      if (this->table().in_game_review()) {
        announcement = ANNOUNCEMENT::NOANNOUNCEMENT;
        for (vector<Game::AnnouncementWithTrickno>::const_reverse_iterator
             a = this->player->game().announcements_of_player(*this->player
                                                             ).rbegin();
             a != this->player->game().announcements_of_player(*this->player
                                                              ).rend();
             ++a)
          if (a->trickno < this->table().game_review_trickno()) {
            announcement = a->announcement;
            break;
          }
      } // if (this->table().in_game_review())
      if (announcement != ANNOUNCEMENT::NOANNOUNCEMENT) {
        Glib::RefPtr<Gdk::Pixbuf> icon
          = this->ui->icons->icon(announcement,
                                  this->rotation());
        if (!this->table().in_game_review()
            && !(is_reply(announcement) && this->player->announcement())
            && (announcement != this->player->announcement())) {
          Glib::RefPtr<Gdk::Pixbuf> copy = icon->copy();
          icon->saturate_and_pixelate(copy, 4, true);
          icon = copy;
        } // if (!card valid)

        if (update) {
          this->table().clear(max_outline);
          // ToDo: remove
          this->name().draw(false);
        }
        this->table().pixmap->draw_pixbuf(this->table().gc,
                                          icon,
                                          0, 0,
                                          this->announcement_start_x(icon->get_width()),
                                          this->announcement_start_y(icon->get_height()),
                                          -1, -1,
                                          Gdk::RGB_DITHER_NONE,
                                          0, 0);
      } // if (this->player->announcement() != ANNOUNCEMENT::NOANNOUNCEMENT)

      if (update) {
        this->table().update(max_outline);
      } // if (update)

      return ;
    } // void Icongroup::draw_announcement(bool const update = true)

  /**
   ** draws the swines/hyperswines icon
   **
   ** @param     update   whether to update the area
   **
   ** @return    -
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  void
    Icongroup::draw_swines(bool const update)
    {
      // no clearing necessary, since the icon can only get bigger

      Glib::RefPtr<Gdk::Pixbuf> icon;

      Gdk::Rectangle max_outline;
      max_outline.set_x(this->swines_start_x(this->ui->icons->max_width(this->rotation())));
      max_outline.set_y(this->swines_start_y(this->ui->icons->max_height(this->rotation())));
      max_outline.set_width(this->ui->icons->max_width(this->rotation()));
      max_outline.set_height(this->ui->icons->max_height(this->rotation()));

      if (   (::game_status == GAMESTATUS::GAME_RESERVATION)
          && (this->player == &this->player->game().player_current())) {
        if (   this->player->reservation().swines
            && this->player->game().swines_announcement_valid(*this->player)
            && this->player->reservation().hyperswines
            && this->player->game().hyperswines_announcement_valid(*this->player))
          icon = this->ui->icons->swines_hyperswines(this->player->game().trumpcolor(),
                                                     this->rotation());
        else if (   this->player->reservation().swines
                 && this->player->game().swines_announcement_valid(*this->player))
          icon = this->ui->icons->swines(this->player->game().trumpcolor(),
                                         this->rotation());
        else if (   this->player->reservation().hyperswines
                 && this->player->game().hyperswines_announcement_valid(*this->player))
          icon = this->ui->icons->hyperswines(this->player->game().trumpcolor(),
                                              this->rotation());
        else if (update) {
          // the values for the icon of maximal size
          this->table().clear(max_outline);
          this->table().update(max_outline);
        }
      } else { // if !(reservation && current player)
        // setting the pixmap, if the player has swines/hyperswines
        if (   this->player->has_swines()
            && this->player->has_hyperswines()) {
          icon = this->ui->icons->swines_hyperswines(this->player->game().trumpcolor(),
                                                     this->rotation());
        } else if (this->player->has_swines()) {
          icon = this->ui->icons->swines(this->player->game().trumpcolor(),
                                         this->rotation());
        } else if (this->player->has_hyperswines()) {
          icon = this->ui->icons->hyperswines(this->player->game().trumpcolor(),
                                              this->rotation());
        } // if !(player has swines/hyperswines)
      } // if !(reservation && current player)

      if (!icon)
        return ;

      this->table().pixmap->draw_pixbuf(this->table().gc,
                                        icon,
                                        0, 0,
                                        this->swines_start_x(icon->get_width()),
                                        this->swines_start_y(icon->get_height()),
                                        -1, -1,
                                        Gdk::RGB_DITHER_NONE,
                                        0, 0);

      if (update) {
        // ToDo: remove
        this->draw(false);
        this->name().draw(false);

        this->table().update(max_outline);
      } // if (update)

      return ;
    } // void Icongroup::draw_swines(bool const update = true)

  /**
   ** -> result
   **
   ** @param     width   the width of the icon
   **
   ** @return    upper left x position of the team/gametype icon
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::team_start_x(int const width) const
    {
      switch(this->position()) {
      case NORTH:
      case SOUTH:
        return (this->hand().start_x()
                + this->hand().width() / 3);
      case WEST:
        return (this->hand().start_x()
                + this->hand().width()
                + this->hand().margin_x());
      case EAST:
        return (this->hand().start_x()
                - this->hand().margin_x()
                - width);
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::team_start_x(width)\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int Icongroup::team_start_x(int width)

  /**
   ** -> result
   **
   ** @param     height   the height of the icon
   **
   ** @return    upper left y position of the team/gametype icon
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::team_start_y(int const height) const
    {
      switch(this->position()) {
      case NORTH:
        return (this->hand().start_y()
                + this->hand().height()
                + this->hand().margin_y());
      case SOUTH:
        return (this->hand().start_y()
                - this->hand().margin_y()
                - height);
      case WEST:
      case EAST:
        return this->hand().start_y();
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::team_start_y(height)\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int Icongroup::team_start_y(int height)

  /**
   ** -> result
   **
   ** @param     width   the width of the icon
   **
   ** @return    upper left x position of the announcement icon
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::announcement_start_x(int const width) const
    {
      switch(this->position()) {
      case NORTH:
      case SOUTH:
        return (this->hand().start_x()
                + this->hand().width() * 2 / 3
                - width / 2);
      case WEST:
        return (this->hand().start_x()
                + this->hand().width()
                + this->hand().margin_x());
      case EAST:
        return (this->hand().start_x()
                - this->hand().margin_x()
                - width);
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::announcement_start_x(width)\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int Icongroup::announcement_start_x(int width)

  /**
   ** -> result
   **
   ** @param     height   the height of the icon
   **
   ** @return    upper left y position of the announcement icon
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::announcement_start_y(int const height) const
    {
      switch(this->position()) {
      case NORTH:
        return (this->hand().start_y()
                + this->hand().height()
                + this->hand().margin_y());
      case SOUTH:
        return (this->hand().start_y()
                - this->hand().margin_y()
                - height);
      case WEST:
      case EAST:
        return (this->hand().start_y()
                + this->hand().height() / 2
                - height / 2);
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::announcement_start_y(height)\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int Icongroup::announcement_start_y(int height)

  /**
   ** -> result
   **
   ** @param     width   the width of the icon
   **
   ** @return    upper left x position of the team icon
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::swines_start_x(int const width) const
    {
      switch(this->position()) {
      case NORTH:
      case SOUTH:
        return (this->hand().start_x()
                + this->hand().width()
                - width);
      case WEST:
        return (this->hand().start_x()
                + this->hand().width()
                + this->hand().margin_x());
      case EAST:
        return (this->hand().start_x()
                - this->hand().margin_x()
                - width);
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::swines_start_x(width)\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int Icongroup::swines_start_x(int width)

  /**
   ** -> result
   **
   ** @param     height   the height of the icon
   **
   ** @return    upper left y position of the swines icon
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::swines_start_y(int const height) const
    {
      switch(this->position()) {
      case NORTH:
        return (this->hand().start_y()
                + this->hand().height()
                + this->hand().margin_y());
      case SOUTH:
        return (this->hand().start_y()
                - this->hand().margin_y()
                - height);
      case WEST:
      case EAST:
        return (this->hand().start_y()
                + this->hand().height()
                - height);
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::swines_start_y(height)\n"
                        "  wrong position 'CENTER' "
                        << "(" << this->position() << ")");
        break;
      } // switch(this->position())

      return 0;
    } // int Icongroup::swines_start_y(int height)

  /**
   ** -> result
   **
   ** @param     position   Position of the icongroup
   **
   ** @return    width of the icongroup at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::width(Position const position) const
    {
      switch(position) {
      case WEST:
      case EAST:
        return this->ui->icons->max_height();
      case CENTER:
      case NORTH:
      case SOUTH:
        DEBUG_ASSERTION(false,
                        "Icongroup::width(" << position << "):\n"
                        "  'position' must be 'WEST' or 'EAST'");
        break;
      } // switch(position)

      return 0;
    } // int Icongroup::width(Position position)

  /**
   ** -> result
   **
   ** @param     position   Position of the icongroup
   **
   ** @return    height of the icongroup at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::height(Position const position) const
    {
      switch(position) {
      case NORTH:
      case SOUTH:
        return this->ui->icons->max_height();
      case WEST:
      case EAST:
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::height(" << position << ")\n"
                        "  'position' must be 'NORTH' or 'SOUTH'");
        break;
      } // switch(position)

      return 0;
    } // int Icongroup::height(Position position)

  /**
   ** -> result
   **
   ** @param     position   Position of the icongroup
   **
   ** @return    width of the icongroup at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::margin_x(Position const position) const
    {
      switch(position) {
      case NORTH:
      case SOUTH:
        DEBUG_ASSERTION(false,
                        "Icongroup::margin_x(position):\n"
                        "  'position' is 'NORTH' or 'SOUTH'");
        break;
      case WEST:
      case EAST:
        return (this->ui->cards->height() / 10);
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::margin_x(position)\n"
                        "  wrong position 'CENTER' "
                        << "(" << position << ")");
        break;
      } // switch(position)

      return 0;
    } // int Icongroup::margin_x(Position position)

  /**
   ** -> result
   **
   ** @param     position   Position of the icongroup
   **
   ** @return    margin_y of the icongroup at 'position'
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.7.5
   **/
  int
    Icongroup::margin_y(Position const position) const
    {
      switch(position) {
      case NORTH:
      case SOUTH:
        return (this->ui->cards->height() / 10);
      case WEST:
      case EAST:
        DEBUG_ASSERTION(false,
                        "Icongroup::margin_y(position):\n"
                        "  'position' is 'WEST' or 'EAST'");
        break;
      case CENTER:
        DEBUG_ASSERTION(false,
                        "Icongroup::margin_y(position)\n"
                        "  wrong position 'CENTER' "
                        << "(" << position << ")");
        break;
      } // switch(position)

      return 0;
    } // int Icongroup::margin_y(Position position)

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    width of the icongroup
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Icongroup::width() const
    {
      return this->width(this->position());
    } // int Icongroup::width()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    height of the icongroup
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Icongroup::height() const
    {
      return this->height(this->position());
    } // int Icongroup::height()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    margin_x of the icongroup
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Icongroup::margin_x() const
    {
      return this->margin_x(this->position());
    } // int Icongroup::margin_x()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    margin_y of the icongroup
   ** 
   ** @author    Diether Knof
   **
   ** @version   0.6.1
   **/
  int
    Icongroup::margin_y() const
    {
      return this->margin_y(this->position());
    } // int Icongroup::margin_y()

  /**
   ** -> result
   **
   ** @param     -
   **
   ** @return    whether the mouse is over the next announcement
   **
   ** @author    Diether Knof
   ** 
   ** @version   0.6.9
   **/
  bool
    Icongroup::mouse_over_next_announcement() const
    {
      if (!::setting(Setting::ANNOUNCE_IN_TABLE))
        return false;

      if (!::is_real(this->player->team()))
        return false;

      Announcement const announcement
        = this->player->next_announcement();

      if (announcement == ANNOUNCEMENT::NOANNOUNCEMENT)
        return false;

      int x, y;
      this->table().get_pointer(x, y);

      Glib::RefPtr<Gdk::Pixbuf> icon
        = this->ui->icons->icon(announcement,
                                this->rotation());
      return (   (x >= this->announcement_start_x(icon->get_width()))
              && (x < (this->announcement_start_x(icon->get_width())
                       + icon->get_width()))
              && (y >= this->announcement_start_y(icon->get_height()))
              && (y < (this->announcement_start_y(icon->get_height())
                       + icon->get_height())));
    } // bool Icongroup::mouse_over_next_announcement() const
} // namespace UI_GTKMM_NS

#endif // #ifdef USE_UI_GTKMM
