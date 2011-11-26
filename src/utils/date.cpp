//////////////////////////////////////////////////////////////////////////////
//
//   Copyright (C) 2002  by Diether Knof
//
//   This program is free software; you can redistribute it and/or 
//   modify it under the terms of the GNU General Public License as 
//   published by the Free Software Foundation; either version 2 of 
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details. 
//   You can find this license in the file 'gpl.txt'.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
//   MA  02111-1307  USA
//
//  Contact:
//    Diether Knof dknof@gmx.de
//
//////////////////////////////////////////////////////////////////////////////

#include "date.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
using std::ostream;
using std::istream;
using std::ifstream;
using std::string;

namespace DK {
  namespace Utils {
    // converts a monthname to the month number
    unsigned monthtou(std::string const monthname);

    /**
     ** Constructor
     **
     ** @param     istr   the input stream
     **
     ** @return    pointer to a new created date
     **	           NULL in the case of an error
     ** 
     ** @author    Diether Knof
     **
     ** @version   2006-10-11
     **/
    Date*
      Date::new_(istream& istr)
    {
      int year = 0;
      unsigned month = 0;
      unsigned day = 0;

      istr >> year;
      if (istr.get() != '-')
	return NULL;
      if (istr.peek() == '0')
	istr.get();
      istr >> month;
      if (istr.get() != '-')
	return NULL;
      if (istr.peek() == '0')
	istr.get();
      istr >> day;

      if (istr.fail())
	return NULL;

      return (new Date(year, month, day));
    } // static Date* Date::new_(string const filename)

    /**
     ** Constructor
     **
     ** @param     string   string to read the version von
     **
     ** @return    pointer to a new created date
     **	           NULL in the case of an error
     ** 
     ** @author    Diether Knof
     **
     ** @version   2006-10-11
     **/
    Date*
      Date::new_(string const& s)
    {
      std::istringstream istr(s.c_str());

      return new_(istr);
    } // static Date* Date::new_(string const& s)

    /**
     ** Constructor
     ** takes the compilation time
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    Date::Date() :
#ifdef POSTPONED
      // for the format: day.month.year (ex: 08.12.2003)
      year(atoi(__DATE__)),
      month(atoi(__DATE__ + 3)),
      day(atoi(__DATE__ + 6))
#else
	// for the foramt: month day year (ex: Dec  8 2003)
	year(atoi(__DATE__ + 7)),
      month(monthtou(std::string(__DATE__, 3))),
      day(atoi(__DATE__ + 4))
#endif
    {
    } // Date::Date()

    /**
     ** Constructor
     **
     ** @param     year    the year
     ** @param     month   the month
     ** @param     day     the day
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    Date::Date(int const year, unsigned const month, unsigned const day) :
      year(year),
      month(month),
      day(day)
    {
    } // Date::Date(int const year, unsigned const month, unsigned const day)

    /**
     ** Constructor
     **
     ** @param     date   date to copy
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    Date::Date(Date const& date) :
      year(date.year),
      month(date.month),
      day(date.day)
    {
    } // Date::Date(Date const& date)

    /**
     ** Destructor
     **
     ** @param     -
     **
     ** @return    -
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    Date::~Date()
    {
    } // Date::~Date()

    /**
     ** write the date to the stream
     **
     ** @param     ostr   output stream to write to
     **
     ** @return    the output stream
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    ostream&
      Date::write(ostream& ostr) const
      {
	ostr << this->year << '-'
	  << (this->month < 10 ? "0" : "") << this->month << '-'
	  << (this->day < 10 ? "0" : "") << this->day;

	return ostr;
      } // ostream& Date::write(ostream& ostr) const

    /**
     ** conversion to a string
     **
     ** @param     -
     **
     ** @return    the corresponding string
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    Date::operator string const() const
    {
      std::ostringstream ostr;

      ostr << *this;

      return ostr.str();
    } // Date::operator string string() const

    /**
     ** write 'date' to stream 'ostr'
     **
     ** @param     ostr   output stream to write to
     ** @param     date   the date
     **
     ** @return    the output stream
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    ostream&
      operator<<(ostream& ostr, Date const& date)
      {
	date.write(ostr);

	return ostr;
      } // ostream& operator<<(ostream& ostr, Date const& date)

    /**
     ** -> result
     **
     ** @param     d1   first date
     ** @param     d2   second date
     **
     ** @return    whether the dates are equal
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    bool
      operator==(Date const& d1, Date const& d2)
      {
	return (   (d1.day == d2.day)
		&& (d1.month == d2.month)
		&& (d1.year == d2.year));
      } // bool operator==(Date const& d1, Date const& d2)

    /**
     ** -> result
     **
     ** @param     d1   first date
     ** @param     d2   second date
     **
     ** @return    whether 'd1' is older than 'd2'
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-08
     **/
    bool
      operator<(Date const& d1, Date const& d2)
      {
	if (d1.year < d2.year)
	  return true;
	if (d1.year > d2.year)
	  return false;

	// now we have 'd1.year == d2.year'
	if (d1.month < d2.month)
	  return true;
	if (d1.month > d2.month)
	  return false;

	// now we have 'd1.month == d2.month'
	if (d1.day < d2.day)
	  return true;
	if (d1.day > d2.day)
	  return false;

	return false;
      } // bool operator<(Date const& d1, Date const& d2)

    /**
     ** -> result
     **
     ** @param     monthname   name of the month (english, short form, ex: Dec)
     **
     ** @return    number of the month 'monthname',
     **	           0 on failure
     ** 
     ** @author    Diether Knof
     **
     ** @version   2003-12-09
     **/
    unsigned
      monthtou(std::string const monthname)
      {
	if (monthname == "Jan")
	  return 1;
	if (monthname == "Feb")
	  return 2;
	if (monthname == "Mar")
	  return 3;
	if (monthname == "Apr")
	  return 4;
	if (monthname == "May")
	  return 5;
	if (monthname == "Jun")
	  return 6;
	if (monthname == "Jul")
	  return 7;
	if (monthname == "Aug")
	  return 8;
	if (monthname == "Sep")
	  return 9;
	if (monthname == "Oct")
	  return 10;
	if (monthname == "Nov")
	  return 11;
	if (monthname == "Dec")
	  return 12;

	// Error
	return 0;
      } // unsigned monthtou(std::string const monthname)
  } // namespace Utils
} // namespace DK
