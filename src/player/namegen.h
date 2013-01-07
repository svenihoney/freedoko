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

#ifndef CLASS_NAMEGENERATOR_HEADER
#define CLASS_NAMEGENERATOR_HEADER

/// creates a string of vocals and consonants with given length
string generate_name(int len);


class Namegenerator
{
  public:
    Namegenerator();

    ~Namegenerator();

    /// creates a string of vocals and consonants with given length i
    string getname(int i);


  private:
    int num_char;         // number of used characters
    vector<char> chars;   // array of used characters
    vector<int> prob;     // table with probability for next char depending on last char (size num_char*num_char)
    // for each char the sum of table values should be 100
    int char_num;         // number of last seleected character

    char nextletter();
}; // class Namegenerator

#endif // #ifndef CLASS_NAMEGENERATOR_HEADER
