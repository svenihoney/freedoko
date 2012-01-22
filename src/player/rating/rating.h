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

#ifndef CLASS_RATING_HEADER
#define CLASS_RATING_HEADER

#include "../../misc/translation.h"

/**
 ** base class for ratings
 ** A rating takes multiple values and returns a rated value.
 ** An example is the 'max' rater, that returns the maximum of the given values.
 **/
class Rating
{
  public:
    // rating types
    enum Type {
      TYPE_FIRST,
      MAX = TYPE_FIRST,
      MIN,
      SECOND_MAX,
      SECOND_MIN,
      MEDIAN,
      AVERAGE,
      AVERAGE_PHYSICAL,
      LINEAR,
      LINEAR_REVERSE,
      TYPE_LAST = LINEAR_REVERSE
    }; // enum Type

    class Max;
    class Min;
    class SecondMax;
    class SecondMin;
    class Median;
    class Average;
    class AveragePhysical;
    class Linear;
    class LinearReverse;

    // returns a rating of the type 'type'
    static Rating* new_(Type const type);

  public:
    // constructor
    Rating(Type const type);
    // destructor
    virtual ~Rating();

    // write the rating in the stream
    void write(ostream& ostr) const;

    // adds a value
    void add(int const value);
    // returns the rated value
    virtual int value() const = 0;
    // deletes the worst 'n' values
    void delete_worst(unsigned const n);

  protected:
    // sort the values (changes mutable values vector)
    void sort() const;

  protected:
    // all given values
    mutable vector<int> values;

  private:
    // the rating type
    PRIV_VAR_R(Type, type);

  private: // unused
    Rating();
}; // class Rating

// output of the rating
ostream& operator<<(ostream& ostr, Rating const& rating);

// returns the name of the rating type
string name(Rating::Type const type);
WRITE_NAME(Rating::Type);

inline
Translator::Translation translation(Rating::Type const& rating)
{ return Translator::Translation(::translator,
				 "AiConfig::Rating::"
				 + ::name(rating)); }

#endif // #ifndef CLASS_RATING_HEADER
