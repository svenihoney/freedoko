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
 ********************************************************************/

#include "constants.h"

#include "references_check.h"

#include "../game/game.h"
#include "../player/player.h"
#include "../card/trick.h"
#include "../os/bug_report_replay.h"

#include "../utils/file.h"

#include <fstream>
#include <cstring>

#ifdef BOOST
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#endif

// the references check
ReferencesCheck* references_check = NULL;

/**
 ** constructor
 **
 ** @param     -
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.6
 **/
ReferencesCheck::ReferencesCheck() :
  references_dir_(),
  references_subdir_(),
  report_file_("References.report.csv"),
#ifdef BOOST
  dir_itr(),
#endif
  statistics_()
{ }

/**
 ** constructor
 **
 ** @param     -
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.6
 **/
ReferencesCheck::ReferencesCheck(string const& references_dir) :
  references_dir_(),
  references_subdir_(),
  report_file_("References.report.csv")
{
  this->set_directory(references_dir);

  return ;
} // ReferencesCheck::ReferencesCheck(string references_dir)

/**
 ** destructor
 **
 ** @param     -
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.6
 **/
ReferencesCheck::~ReferencesCheck()
{ }

/**
 ** set the directory
 **
 ** @param     references_dir   the directory with the references
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.6
 **/
void
ReferencesCheck::set_directory(string const& references_dir)
{
  if (!DK::Utils::File::isdirectory(references_dir)) {
    cerr << "ReferencesCheck:set_directory()\n"
      << "'" << references_dir << "' is no directory. Aborting."
      << endl;
    exit(EXIT_FAILURE);
  }
  this->references_dir_ = references_dir;
  if (this->references_dir().empty())
    return ;

#ifdef BOOST
  { // add the subdirectories
    for (this->dir_itr
         = boost::filesystem::directory_iterator(this->references_dir());
         this->dir_itr != boost::filesystem::directory_iterator();
         ++this->dir_itr) {
      try {
        if (this->dir_itr->path().filename() == this->report_file())
          continue;
        if (   boost::filesystem::is_directory(*this->dir_itr)
            && (this->dir_itr->path().filename().string()[0] != '.')) {
          this->references_subdir_.push_back(this->dir_itr->path().string());
        }
      } catch (std::exception const& ex) {
        cerr << dir_itr->path().filename() << " " << ex.what() << endl;
        this->references_dir_ = "";
      }
    } // for (this->dir_itr)
  } // add the subdirectories
#endif

#ifdef BOOST
  { // create the statistics file
    ofstream ostr((this->references_dir() + "/" + this->report_file()).c_str());
    if (ostr.fail()) {
      this->report_file_.clear();
      return ;
    } // if (ostr.fail())

    ostr << "file;"
      << "trick;"
      << "player;"
      << "discrepancy;"
      << "real action;"
      << "ref action;"
      << "comment\n";

    cout << "Created file '"
      << this->references_dir() + "/" + this->report_file()
      << "' for the references report.\n";

    this->statistics_.reset();
  } // load the next reference and create the statistics file

#endif
  // load the next reference
  this->next_reference();

  return ;
} // void ReferencesCheck::set_directory(string references_dir)

/**
 ** open the next file
 **
 ** @param     -
 **
 ** @return    whether another file was loaded
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.6
 **/
bool
ReferencesCheck::next_reference()
{
#ifdef BOOST
  if (this->dir_itr == boost::filesystem::directory_iterator())
    this->dir_itr = boost::filesystem::directory_iterator(this->references_dir());
  else
    ++this->dir_itr;
  for (;
       this->dir_itr != boost::filesystem::directory_iterator();
       ++this->dir_itr) {
    try {
      if (this->dir_itr->path().filename() == this->report_file()) {
        continue;
      }
      if (!boost::filesystem::is_directory(*this->dir_itr)
          && (this->dir_itr->path().filename().string().size()
              >= strlen(".Reference.FreeDoko"))
          && (this->dir_itr->path().filename().string().substr(this->dir_itr->path().filename().string().size()
                                               - strlen(".Reference.FreeDoko"))
              == ".Reference.FreeDoko")
         ) {
        if (this->load_reference()) {
          if  (   (::game_status == GAMESTATUS::PROGRAMSTART)
               || (::game_status == GAMESTATUS::PARTY_INITIAL_LOADED)
               || (::game_status == GAMESTATUS::PARTY_LOADED) ) {
            return true;
          } else {
            if (::party.gameno() > 0)
              throw GAMESTATUS::PARTY_LOADED;
            else
              throw GAMESTATUS::PARTY_NEW;
          }
        }
        return false;
      }
    } catch (std::exception const& ex) {
      cerr << dir_itr->path().filename() << " " << ex.what() << endl;
      this->references_dir_ = "";
    }
  } // for (this->dir_itr)

  { // go to the next subdir
    this->write_statistics();
    this->statistics_.reset();

    if (this->references_subdir_.empty()) {
      throw GAMESTATUS::QUIT;
    }
    string const subdir = this->references_subdir_.front();
    this->references_subdir_.pop_front();
    this->set_directory(subdir);
    return true;
  } // go to the next subdir
#endif

  return false;
} // bool ReferencesCheck::next_reference()

/**
 ** open the reference
 **
 ** @param     -
 **
 ** @return    whether the loading was successful
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.6
 **/
bool
ReferencesCheck::load_reference()
{
#ifdef BOOST
  if (this->dir_itr == boost::filesystem::directory_iterator())
    return false;

  OS_NS::BugReportReplay* bug_report_replay
    = new OS_NS::BugReportReplay(this->dir_itr->path().string());
  // ToDo: check for auto action end
  if (!bug_report_replay->loaded()) {
    cerr << "Error loading the reference '" << this->dir_itr->path().string()
      << "'" << endl;
    if (!this->report_file().empty()) {
      ofstream ostr((this->references_dir() + "/"
                     + this->report_file()).c_str(),
                    ios::app);
      if (!ostr.fail())
        ostr << this->dir_itr->path().string() << ";loading error\n";
    } // if (output set)
    delete bug_report_replay;
    return false;
  }
#endif

  this->statistics_.no_files += 1;

  return true;
} // bool ReferencesCheck::load_reference()

/**
 ** check the action of the reference against the action of the game
 **
 ** @param     game           the game
 ** @param     game_action    the action from the game
 ** @param     check_action   the action to check
 ** @param     comment        the comment of the check
 ** @param     discrepancy    the discrepancy between the two actions
 ** @param     last_check     whether this is the last check
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.6
 **/
void
ReferencesCheck::check(Game const& game,
                       GameplayAction const& game_action,
                       GameplayAction const& check_action,
                       string const& comment,
                       GameplayAction::Discrepancy const& discrepancy,
                       bool const last_check)
{
  this->statistics_.no_checks += 1;
  if (!discrepancy)
    this->statistics_.no_success += 1;

  string output;
  { // create the result line
    // format:
    //   seed (trick, player); OK; ; real action; text
    //   seed (trick, player); discrepancy; real action; check action; text
    // semicolon separated list:
    // 1) seed
    // 2) trick
    // 3) player
    // 4) discrepancy
    // 5) real gameplay action / empty
    // 6) gameplay action from the reference
    // 7) comment (optional)
    ostringstream ostr;
#ifdef BOOST
    ostr << setfill('0') << setw(6) << this->dir_itr->path().filename().string()
      << ';' << setw(2) << game.trick_current_no() << ';'
      << check_action.player << ';';
#endif
    if (!discrepancy)
      ostr << "OK";
    else
      ostr << discrepancy;
    ostr << ';' << game_action.str()
      << ';' << check_action.str();
    if (!comment.empty())
      ostr << ';' << comment;
    output = ostr.str();
  } // create the result line

  // print the result line
  cout << "check: " << output << endl;

  { // save the result line
    if (this->references_dir().empty()
        && !this->report_file().empty())
      this->set_directory(".");
    if (!this->report_file().empty()) {
      ofstream ostr((this->references_dir() + "/"
                     + this->report_file()).c_str(),
                    ios::app);
      if (ostr.fail()) {
        this->report_file_.clear();
        return ;
      } // if (ostr.fail())
      ostr << output << '\n';
    } // if (output set)
  } // save the result line

  if (discrepancy || last_check) {
    throw GAMESTATUS::PARTY_NEW;
    exit(discrepancy);
  }

  return ;
} // void ReferencesCheck::check(Game game, GameplayAction game_action, GameplayAction check_action, string comment, GameplayAction::Discrepancy discrepancy, bool last_check)

/**
 ** write the statistics
 **
 ** @param     -
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.6
 **/
void
ReferencesCheck::write_statistics() const
{
  // print the statistics
  cout << '\n'
    << "reference checks\n"
    << "  files:   "
    << setw(static_cast<int>(ceil(log10(this->statistics().no_checks))))
    << this->statistics().no_files << '\n'
    << "  checks:  "
    << this->statistics().no_checks << '\n'
    << "  success: "
    << setw(static_cast<int>(ceil(log10(this->statistics().no_checks))))
    << this->statistics().no_success << '\n'
    << "  failed:  "
    << setw(static_cast<int>(ceil(log10(this->statistics().no_checks))))
    << (this->statistics().no_checks
        - this->statistics().no_success) << '\n'
    << '\n';

  { // save the result line
    if (this->references_dir().empty()
        && !this->report_file().empty()) {
      return ;
    }
    if (!this->report_file().empty()) {
      ofstream ostr((this->references_dir() + "/"
                     + this->report_file()).c_str(),
                    ios::app);
      if (ostr.fail()) {
        return ;
      } // if (ostr.fail())
      ostr << '\n'
        << "files;"
        << this->statistics().no_files << '\n'
        << "checks;"
        << this->statistics().no_checks << '\n'
        << "success;"
        << this->statistics().no_success << '\n'
        << "failed;"
        << (this->statistics().no_checks
            - this->statistics().no_success) << '\n';
    } // if (output set)
  } // save the result line

  return ;
} // ReferencesCheck::write_statistics() const
