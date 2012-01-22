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

#ifndef GTKMM_GAME_REVIEW_HEADER
#define GTKMM_GAME_REVIEW_HEADER

#include "base.h"

class Game;
class Trick;

#include <gdk/gdkevents.h>

namespace UI_GTKMM_NS {
  class GameFinished;

  /**
   **
   ** @brief	window for reviewing the game (trickwise)
   **
   ** @author	Diether Knof
   **
   **/
  class GameReview : public Base {
    public:
      GameReview(GameFinished* const game_finished);
      ~GameReview();

      void set_game(Game const& game);

      ::Trick const& trick() const;
       bool trick_visible() const;

      void previous_trick();
      void following_trick();
      void set_trickno(unsigned const trickno);

      bool key_press(GdkEventKey const* const key);

    private:
      PRIV_VAR_R(unsigned, trickno);

    private: // unused
      GameReview();
      GameReview(GameReview const&);
      GameReview& operator=(GameReview const&);
  }; // class GameReview : public Base

} // namespace UI_GTKMM_NS

#endif // #ifdef GTKMM_GAME_REVIEW_HEADER

#endif // #ifdef USE_UI_GTKMM
