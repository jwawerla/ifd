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
#include "pathplannerlookup.h"
#include "printerror.h"
#include "utilities.h"

//-----------------------------------------------------------------------------
CPathPlannerLookup::CPathPlannerLookup( std::string pointsFile,
                                        std::string tableFile,
                                        float pixelPerMeterX,
                                        float pixelPerMeterY,
                                        float offsetX,
                                        float offsetY )
{
  int point;
  int rows;
  int ret;
  float x, y;
  char* str =  new char[80];

  mMaxPlanLength = 1000;
  mPixelPerMeterX = pixelPerMeterX;
  mPixelPerMeterY = pixelPerMeterY;

  mFpTable = fopen( tableFile.c_str(), "r" );
  if ( mFpTable == NULL ) {
    PRT_ERR1( "Failed to open lookup table file %s", tableFile.c_str() );
    exit(-1);
  }

  mFpPoints = fopen( pointsFile.c_str(), "r" );
  if ( mFpPoints == NULL ) {
    PRT_ERR1( "Failed to open points file %s", pointsFile.c_str() );
    exit(-1);
  }

  // load list of points and convert to metric coordinates
  while ( not feof( mFpPoints ) ) {

    ret = fscanf( mFpPoints, "%f %f\n", &x, &y );

    if ( ret != 2 ) {
      PRT_ERR1( "Points file has wrong format %s", pointsFile.c_str() );
      exit(-1);
    }

    // convert to metric coordinate system
    x = x + offsetX;
    y = -y + offsetY;
    x = x / pixelPerMeterX;
    y = y / pixelPerMeterY;
    mWaypointList.push_back( new CWaypoint2d( x, y ) );

  } // while
  mNumPoints = mWaypointList.size();

  // create lookup table
  mLookupTable = new int*[mNumPoints];
  for ( unsigned int i = 0; i < mNumPoints; i++ )
    mLookupTable[i] = new int[mNumPoints];

  // read first line with general info
  ret = fscanf(mFpTable, "%d points, %d chars/row, %s indexing.\n", &point, &rows, str);

  // load lookup table
  for ( unsigned int x = 0; x < mNumPoints; x++ ) {
    for ( unsigned int y = 0; y < mNumPoints; y++ ) {
      ret = fscanf( mFpTable, "%d", &point );
      if (point >= (int) mNumPoints) {
        PRT_ERR1("Lookup table %s corrupted", tableFile.c_str() );
        exit(-1);
      }
      mLookupTable[x][y] = point;
    }
  }

  // close files
  if ( mFpTable ) {
    fclose( mFpTable );
    mFpTable = NULL;
  }

  if ( mFpPoints ) {
    fclose( mFpPoints );
    mFpPoints = NULL;
  }
}
//-----------------------------------------------------------------------------
CPathPlannerLookup::~CPathPlannerLookup()
{
  for ( unsigned int i = 0; i < mNumPoints; i++ )
    delete[] mLookupTable[i];
}
//-----------------------------------------------------------------------------
int CPathPlannerLookup::findClosesWaypoint( const CPose2d pose )
{
  int idx = -1;
  CPose2d wpPose;
  float dist;
  float minDist = INFINITY;

  for ( unsigned int i = 0; i < mWaypointList.size(); i++ ) {
    wpPose = mWaypointList[i]->getPose();
    dist = wpPose.distance( pose );
    if ( dist < minDist ) {
      minDist = dist;
      idx = i;
    }
  }

  if (idx == -1) {
    PRT_ERR1("No waypoint found for %s", pose.toStr().c_str());
  }
  return idx;
}
//-----------------------------------------------------------------------------
void CPathPlannerLookup::getPathFromTo( const CPose2d start, const CPose2d end,
                                        std::list<CWaypoint2d> &wpList )
{
  unsigned int count = 0;
  std::list<CWaypoint2d>::iterator it;
  int tmpIdx;
  int startIdx;
  int endIdx;

  // clear waypoint list, just in case
  wpList.clear();

  // find index for start and end points
  startIdx = findClosesWaypoint( start );
  endIdx = findClosesWaypoint( end );
  tmpIdx = endIdx;

  do {
    tmpIdx = mLookupTable[startIdx][tmpIdx];
    // finished yet ?
    if ( (tmpIdx == -1) || ( tmpIdx == startIdx ) ) {
      break;
    }

    wpList.push_front( *mWaypointList[tmpIdx] );
    count ++;
  }
  while ( count < mMaxPlanLength );

  // add start waypoint and end point
  wpList.push_front( *mWaypointList[startIdx] );
  wpList.push_back( *mWaypointList[endIdx] );

  //for (it = wpList.begin(); it != wpList.end(); it++) {
  //   (*it).print();
  //}
}
//-----------------------------------------------------------------------------


