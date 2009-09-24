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
#ifndef PATHPLANNERLOOKUP_H
#define PATHPLANNERLOOKUP_H

#include "waypoint2d.h"
#include <string>
#include <stdio.h>
#include <vector>
#include <list>

using namespace Rapi;

/**
 * Path planner based on Adam Lein's shortest paths lookup table. The origin
 * of the coordinate system is placed in the center of the bitmap.
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CPathPlannerLookup
{
  public:
    /**
     * Default constructor
     * @param pointsFile filename of points list
     * @param tableFile  filename of lookup table
     * @param pixelPerMeterX pixel per meter in X direction [px/m]
     * @param pixelPerMeterY pixel per meter in X direction [px/m]
     * @param offsetX X direction offset of the origin [pixel]
     * @param offsetY Y direction offset of the origin [pixel]
     */
    CPathPlannerLookup( std::string pointsFile,
                        std::string tableFile,
                        float pixelPerMeterX,
                        float pixelPerMeterY,
                        float offsetX,
                        float offsetY
                      );
    /** Default destructor */
    ~CPathPlannerLookup();
    /**
     * Gets a plan from start to end
     * @param start position
     * @param end position
     * @param wpList list of way points
     */
    void getPathFromTo( const CPose2d start, const CPose2d end,
                        std::list<CWaypoint2d> &wpList );


  protected:
    /**
     * Finds the closest waypoint for a given coordinate and returns the
     * index of that wayppoint
     * @param pose to find waypoint for
     * @return index of waypoint
     */
    int findClosesWaypoint( const CPose2d pose );

  private:
    /** Maximal number of waypoints in a planned path */
    unsigned int mMaxPlanLength;
    /** List of way points */
    std::vector<CWaypoint2d*> mWaypointList;
    /** Number of points same as mWaypointList.size() */
    unsigned int mNumPoints;
    /** Lookup table */
    int** mLookupTable;
    /** File descriptor for points file */
    FILE* mFpPoints;
    /** File descriptor for lookup table file */
    FILE* mFpTable;
    /** pixel per meter in X direction [px/m] */
    float mPixelPerMeterX;
    /** pixel per meter in Y direction [px/m] */
    float mPixelPerMeterY;

};

#endif
