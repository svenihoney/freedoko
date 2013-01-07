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
 *********************************************************************/

#include "constants.h"

#include "namegen.h"

#define DEBUG_NAMEGEN false


#define MRAND(x) (rand() % x)

/**********************************************************************
 *
 **    GenerateName(int len)
 *
 **    Parameters: length of name
 *
 **    Result: name of length len
 *
 **    Version: Alpha
 *
 **    Description:
 *
 *
 **********************************************************************/

string generate_name(int len)
{
  DEBUG_CALLING(DEBUG_NAMEGEN,"generate_name");
  DEBUG_RETURNING(Namegenerator().getname(len),DEBUG_NAMEGEN,"generate_name");
}


/**********************************************************************
 *
 **    Namegenerator::Namegenerator()
 *
 **    Parameters: 
 *
 **    Result: standard initiazilzed table 
 *
 **    Version: Alpha
 *
 **    Description:
 *
 *
 **********************************************************************/

Namegenerator::Namegenerator()
{
  DEBUG_CALLING(DEBUG_NAMEGEN,"Namegenerator::Namegenerator()");

  num_char=26;

  chars=vector<char>(num_char);

  chars[0 ]='a'; chars[1 ]='b'; chars[2 ]='c'; chars[3 ]='d';
  chars[4 ]='e'; chars[5 ]='f'; chars[6 ]='g'; chars[7 ]='h';
  chars[8 ]='i'; chars[9 ]='j'; chars[10]='k'; chars[11]='l';
  chars[12]='m'; chars[13]='n'; chars[14]='o'; chars[15]='p';
  chars[16]='q'; chars[17]='r'; chars[18]='s'; chars[19]='t';
  chars[20]='u'; chars[21]='v'; chars[22]='w'; chars[23]='x';
  chars[24]='y'; chars[25]='z';

  prob=vector<int>(num_char*num_char);

  for (int x=0; x<num_char; x++)
    for (int y=0; y<num_char; y++)
    {
      int p=0;
      if (x==0 || x==4 || x==8 || x==14 || x==20) //vocal
      {
	if (y!=0 && y!=4 && y!=8 && y!=14 && y!=20) // consonant
	{
	  p=4;
	  if (y>20) p++;
	} else 
	{ 
	  p=0;
	}
      } 
      else // consonant
      {
	if (y==0 || y==4 || y==8 || y==14 || y==20) // vocals
	{
	  p=20;

	} else 
	{ 
	  p=0;
	}
      }
      // each line of table is for one letter and contains in the rows the
      // probality that for the letter chars[x] the next letter chars[y] is
      prob[x*num_char+y]=p;
    }

  DEBUG_RETURNING_N(DEBUG_NAMEGEN,"Namegenerator::Namegenerator()");
}

/**********************************************************************
 *
 **    Namegenerator::~Namegenerator()
 *
 **    Parameters: 
 *
 **    Result: 
 *
 **    Version: Alpha
 *
 **    Description:
 *
 *
 **********************************************************************/

Namegenerator::~Namegenerator()
{
  DEBUG_CALLING(DEBUG_NAMEGEN,"Namegenerator::~Namegenerator()");
  DEBUG_RETURNING_N(DEBUG_NAMEGEN,"Namegenerator::~Namegenerator()");
}

/**********************************************************************
 *
 **    string Namegenerator::getname(int i)
 *
 **    Parameters: length of name
 *
 **    Result: name of length len
 *
 **    Version: Alpha
 *
 **    Description:
 *
 *
 **********************************************************************/

string  Namegenerator::getname(int i)
{

  DEBUG_CALLING(DEBUG_NAMEGEN,"Namegenerator::getname(int i)"); 

  string res;

  char_num=MRAND(num_char);
  res+=toupper(chars[char_num]);         
  for (int n = 1; n < i; n++)
    res += nextletter();

  DEBUG_RETURNING(res,DEBUG_NAMEGEN,"Namegenerator::getname(int i)");   

}


/**********************************************************************
 *
 **    char Namegenerator::nextletter()
 *
 **    Parameters: 
 *
 **    Result: next letter for name generation depending on char_num
 *
 **    Version: Alpha
 *
 **    Description:
 *
 *
 **********************************************************************/

char Namegenerator::nextletter()
{
  DEBUG_CALLING(DEBUG_NAMEGEN,"Namegenerator::nextletter()"); 

  int p=MRAND(100);
  int i=0;

  while (p>=prob[num_char*char_num+i] && i<num_char)
  {
    p-=prob[num_char*char_num+i];
    i++;
  }

  // check for array overflow
  // this might happen for the last letter of chars
  if (!(i<num_char)) i=num_char-1;

  char_num=i;// number of chosen next letter

  DEBUG_RETURNING(chars[char_num],DEBUG_NAMEGEN,"Namegenerator::nextletter()"); 

}
