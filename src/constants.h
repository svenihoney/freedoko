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
 **********************************************************************/

#ifndef HEADER_CONSTANTS
#define HEADER_CONSTANTS

// using the microsoft visual C++ compiler
#ifdef _MSC_VER
#define MS_VISUAL_C
#define WINDOWS
#endif

// C++ include

#include <typeinfo>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

// for temporary output
// ToDo: use null_str in the release
#define COUT cout
#define CLOG cout << __FILE__ << '#' << __LINE__ << "  "

#define LOGreturn if(CLOG << "return" << endl, true) return


#include <vector>
#include <list>
#include <queue>
#include <map>
#include <set>
#include <string>

using namespace std;


// C functions
#include <cmath>
#include <climits>

#if defined(_WIN32)
#define USE_REGISTRY
#endif

#if defined(_WIN32)
#ifdef USE_REGISTRY
#include <windows.h>
#undef min
#undef max
#endif
#endif


#ifndef MS_VISUAL_C
#include <dirent.h>
#endif


#include "utils/version.h"


// Whether GTKMM can be used.
// When not defined, FreeDoko should compile and run without any GTKMM library
// this makro shall be defined through the compiler (in the makefile)
//#define GTKMM


// code that is outdated -- leave this macro undefined
#undef OUTDATED
// code, that is postponed -- leave this macro undefined
#undef POSTPONED

// code that is deprecated -- leave this macro defined
#define DEPRECATED

// code that is workaround -- leave this macro defined
#define WORKAROUND

// code, that is an alternative -- leave this macro undefined
#undef ALTERNATIVE


extern char const* const GPL_string;
extern DK::Utils::Version const* version;
extern std::vector<DK::Utils::Version const*> all_versions;
#define CONTACT "freedoko@users.sourceforge.net"
#define PROGRAM_NAME "FreeDoko"


// In the release-version there are some elements hidden
//#define RELEASE
//#define VERSION_DESCRIPTION "preversion"

#ifdef RELEASE
#define NO_CRITICAL_CHECKS
#endif


// whether to check the runtime
#if !defined(RELEASE) && !defined(USE_THREADS)
#define CHECK_RUNTIME
#endif


namespace FAST_NS {
  enum BIT {
    NONE	         = 0,
    PARTY_START          = 1,                         // automatical start party
    PLAYER               = PARTY_START          << 1, // change human players to ais
    RANDOM_AI            = PLAYER               << 1, // set the players to the random ai
    PAUSE                = RANDOM_AI            << 1, // skip pauses
    GAMEPLAY_INFORMATION = PAUSE                << 1, // skip gameplay information dialogs
    GAME_FINISHED        = GAMEPLAY_INFORMATION << 1, // skip game finished dialog
    PARTY_FINISHED        = GAME_FINISHED       << 1, // skip party finished dialog
    SHOW_ALL_HANDS       = PARTY_FINISHED        << 1, // show all hands
    HIDE_BUG_REPORT_WINDOW       = SHOW_ALL_HANDS << 1, // do not show the bug report replay window
    QUIT_WHEN_FINISHED   = HIDE_BUG_REPORT_WINDOW << 1,  // quit when finished
    SEED_INFO             = QUIT_WHEN_FINISHED << 1 // only write the seed on stdout
  }; // enum BIT
  extern int const ALL;
} // namespace FAST_NS
// All players are AI, there is no pause and no 'full trick'-window
extern int FAST_PLAY;

// the starting seed
extern unsigned const SEED_START;
// the starting player
extern unsigned const STARTPLAYER;

// some utils (RAND(int), MARK(), expand_filename(string), Null_Str
#include "utils.h"


#ifndef WINDOWS
// PlaySound exists only under MS Windows
#undef USE_SOUND_PLAYSOUND
#endif


// whether an assertion shall create a segmentation fault (for a core dump)
// (define it in 'Makefile.local')
//#define ASSERTION_GENERATES_SEGFAULT

#ifdef RELEASE
// if DEBUG_NO_OUTPUT is defined, debugging information are not written
#define DEBUG_NO_OUTPUT
#endif

// whether to use debug informations
#define DEBUG

#ifndef DEBUG
// if DEBUG_NO is defined, the internal checks are skipped
// this speeds up the program up to factor 4
#define DEBUG_NO
#endif

// debugging utils
#include "debug.h"

// creates a segmentation fault
#define SEGFAULT if (true) { cerr << "Created segmentation fault:\n" \
  << __FILE__ << " # " << __LINE__ << endl; \
(*reinterpret_cast<volatile int*>(NULL) = 0); } else

#endif // #ifndef HEADER_CONSTANTS
