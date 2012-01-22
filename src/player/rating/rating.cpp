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
#include "rating.h"
#include "ratings.h"

/**
 ** constructor
 **
 ** @param     type   rating type
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.3
 **/
Rating::Rating(Type const type) :
  values(),
  type_(type)
{ }

/**
 ** destructor
 **
 ** @param     -
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.3
 **/
Rating::~Rating()
{ }

/**
 ** -> result
 **
 ** @param     type   the rating type to return
 **
 ** @return    new rating of type 'type' (to be freed)
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.3
 **/
Rating*
Rating::new_(Type const type)
{
  switch(type) {
  case MAX:
    return new Max();
  case MIN:
    return new Min();
  case SECOND_MAX:
    return new SecondMax();
  case SECOND_MIN:
    return new SecondMin();
  case MEDIAN:
    return new Median();
  case AVERAGE:
    return new Average();
  case AVERAGE_PHYSICAL:
    return new AveragePhysical();
  case LINEAR:
    return new Linear();
  case LINEAR_REVERSE:
    return new LinearReverse();
  } // switch(type)
  return NULL;
} // Rating* Rating::new_(Type const type)

/**
 ** write the rating in the stream
 **
 ** @param     ostr     stream to write into
 ** @param     rating   rating to write
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.9
 **/
ostream&
operator<<(ostream& ostr, Rating const& rating)
{
  rating.write(ostr);
  return ostr;
} // ostream& operator<<(ostream& ostr, Rating const& rating)

/**
 ** write the rating in the stream
 **
 ** @param     ostr  stream to write into
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.9
 **/
void
Rating::write(ostream& ostr) const
{
  ostr << ::name(this->type()) << '\n';
  for (vector<int>::const_iterator i = this->values.begin();
       i != this->values.end();
       ++i)
    ostr << "  " << *i << '\n';

  return ;
} // void Rating::write(ostream& ostr) const

/**
 ** add the value
 **
 ** @param     value   value to add
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.9
 **/
void
Rating::add(int const value)
{
  this->values.push_back(value);

  return ;
} // void Rating::add(int value)

/**
 ** deletes the worst 'n' arguments
 **
 ** @param     n   number of arguments to ignore
 **
 ** @return    -
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.9
 **/
void
Rating::delete_worst(unsigned const n)
{
  if (this->values.size() <= n) {
    this->values.clear();
    return ;
  }
  this->sort();
  this->values.erase(this->values.begin(), this->values.begin() + n);

  return ;
} // void Rating::delete_worst(int n)

/**
 ** sorts the values
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
Rating::sort() const
{
  std::sort(this->values.begin(), this->values.end());
  return ;
} // void Rating::sort() const

/**
 ** -> result
 **
 ** @param     type   rating type
 **
 ** @return    name of the rating type
 **
 ** @author    Diether Knof
 **
 ** @version   0.7.3
 **/
string
name(Rating::Type const type)
{
  switch(type) {
  case Rating::MAX:
    return "maximum";
  case Rating::MIN:
    return "minimum";
  case Rating::SECOND_MAX:
    return "second maximum";
  case Rating::SECOND_MIN:
    return "second minimum";
  case Rating::MEDIAN:
    return "median";
  case Rating::AVERAGE:
    return "average";
  case Rating::AVERAGE_PHYSICAL:
    return "physical average";
  case Rating::LINEAR:
    return "linear";
  case Rating::LINEAR_REVERSE:
    return "linear reverse";
  } // switch(type)
  return "";
} // string name(Rating::Type const type)
