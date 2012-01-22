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
 **********************************************************************/

#ifdef USE_UI_GTKMM

#ifndef GTKMM_GAME_FINISHED_HEADER
#define GTKMM_GAME_FINISHED_HEADER

#include "base.h"
#include "ui.h"
#include "widgets/sticky_dialog.h"

class Player;
namespace Gtk {
  class StockButton;
  class ToggleButton;
} // namespace Gtk

namespace UI_GTKMM_NS {
  class GameSummary;
  class GameReview;

  /**
   **
   ** @brief	the game finished window
   **
   ** @author	Diether Knof
   **
   ** @todo	when selecting a special point, jump to the trick
   ** @todo	when the party is finished, rename the 'next game' button
   **
   **/
  class GameFinished : public Base, public Gtk::StickyDialog {
    public:
      GameFinished(Base* const parent);
      ~GameFinished();

      void game_finished();
      void setting_update(int const type);

      void name_changed(::Player const& player);

    private:
      void init();
      void on_hide();
      void toggle_game_review();

    public:
      GameReview* game_review;

    private:
      Gtk::StockButton* create_bug_report_button;
    public:
      GameSummary* game_summary;
    private:
      Gtk::StockButton* show_party_points;
      Gtk::ToggleButton* show_game_review;

      Gtk::StockButton* next_game_button;

    private: // unused
      GameFinished();
      GameFinished(GameFinished const& game_finished);
      GameFinished& operator=(GameFinished const& game_finished);
  }; // class GameFinished : public Base, public Gtk::StickyDialog

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_GAME_FINISHED_HEADER

#endif // #ifdef USE_UI_GTKMM
