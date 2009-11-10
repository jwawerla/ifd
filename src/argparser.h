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
#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>

/**
 * Simple parser for command line parameter strings comming from Stage
 * @author Jens Wawerla
 */
class CArgParser
{
  public:
    /**
     * Default constructor
     * @param args argument string
     */
    CArgParser( std::string args );
    /** Default destructor */
    ~CArgParser();
    /**
     * Read the value of an entry
     * @param name of entry (-x=)
     * @return return value
     * @return true if found, false otherwise
     */
    bool read( const std::string name, int& value );
    bool read( const std::string name, double& value );
    bool read( const std::string name, float& value );
  protected:
    /**
     * Finds the value string for a given parameter name
     * @param name of entry
     * @return value string
     */
    std::string findValueString( const std::string name );

  private:
    /** Argument string */
    std::string mArgs;
};

#endif
