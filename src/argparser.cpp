/***************************************************************************
 * Project: IFD                                                            *
 * Author:  Jens Wawerla (jwawerla@sfu.ca)                                 *
 * $Id: $
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 **************************************************************************/
#include "argparser.h"
#include <stdlib.h>

//-----------------------------------------------------------------------------
CArgParser::CArgParser( std::string args )
{
  mArgs = args;
}
//-----------------------------------------------------------------------------
CArgParser::~CArgParser()
{
}
//-----------------------------------------------------------------------------
bool CArgParser::read( const std::string name, int& value )
{
  std::string valueStr;

  valueStr = findValueString( name );
  if ( valueStr.empty() )
    return false; // entry not found

  value = atoi( valueStr.c_str() );
  return true; // entry found
}
//-----------------------------------------------------------------------------
bool CArgParser::read( const std::string name, float& value )
{
  std::string valueStr;

  valueStr = findValueString( name );
  if ( valueStr.empty() )
    return false; // entry not found

  value = atof( valueStr.c_str() );
  return true; // entry found
}
//-----------------------------------------------------------------------------
bool CArgParser::read( const std::string name, double& value )
{
  std::string valueStr;

  valueStr = findValueString( name );
  if ( valueStr.empty() )
    return false; // entry not found

  value = atof( valueStr.c_str() );
  return true; // entry found
}
//-----------------------------------------------------------------------------
std::string CArgParser::findValueString( const std::string name )
{
  std::string valueStr;
  size_t startPos;
  size_t endPos;

  valueStr.clear();

  // find start position of entry name
  startPos = mArgs.find( name );

  // does this name exist in our args string ?
  if ( startPos != std::string::npos ) {
    startPos += name.length();
    // the end of the value is marked with a blank
    endPos = mArgs.find_first_of( " ", startPos );
    // or nothing if this is the last entry
    if ( endPos == std::string::npos )
      endPos = mArgs.length();
    // copy value
    valueStr = mArgs.substr( startPos, endPos );
  }
  return valueStr;
}
//-----------------------------------------------------------------------------

