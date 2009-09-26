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
#include "baserobotctrl.h"
#include "utilities.h"

const double CRUISE_SPEED = 0.4;
const double AVOID_SPEED = 0.05;
const double AVOID_TURN = 0.5;
const double MIN_FRONT_DISTANCE = 0.6;
const double STOP_DISTANCE = 0.5;
const int AVOID_DURATION = 10;

//-----------------------------------------------------------------------------
ABaseRobotCtrl::ABaseRobotCtrl( ARobot* robot ) : ARobotCtrl( robot )
{
  CLooseStageRobot* looseRobot;
  Stg::Model* stgModel;

  mElapsedStateTime = 0.0;
  mState = START;
  mPrevState = START;
  mCurrentPatch = NULL;
  mAvoidCount = 0;
  mReplanCounter = 0;
  mCounter = 0;
  mPatchList.clear();

  //************************************
  // FSM
  mFsmText[START]    = "start";
  mFsmText[SW_PATCH] = "switch patch";
  mFsmText[FORAGE]   = "forage";

  // init path planer
  mPathPlanner = new CPathPlannerLookup( "circle.pts", "circle.fw",
                                         919.0 / 65.0, 927.0 / 65.0,
                                         -919.0 / 2.0 , 927.0 / 2.0 );

  //****************************************
  // init robot

  looseRobot = ( CLooseStageRobot* ) mRobot;
  //looseRobot->findDevice( mFiducial, "model:0.fiducial:0" );
  looseRobot->findDevice( mLaser, "laser:0" );
  looseRobot->findDevice( mPowerPack, "powerpack:0" );
  looseRobot->findDevice( mTextDisplay, "textdisplay:0" );
  looseRobot->findDevice( mDrivetrain, "position:0" );
  mDrivetrain->setTranslationalAccelerationLimit( CLimit( -INFINITY, INFINITY ) );
  mDrivetrain->setRotationalAccelerationLimit( CLimit( -INFINITY, INFINITY ) );
  if ( rapiError->hasError() ) {
    rapiError->print();
    exit( -1 );
  }

  // get robot pose
  mRobotPose = mDrivetrain->getOdometry()->getPose();

  // Add nearest distance obstacle avoidance to robot
  stgModel = mDrivetrain->getStageModel();
  assert( stgModel );
  //mNd = new CStageNd( stgModel, 0.4, 0.3, 0.2, mRobot->getName() );
  //mNd->addRangeFinder( mLaser );
}
//-----------------------------------------------------------------------------
ABaseRobotCtrl::~ABaseRobotCtrl()
{
}
//-----------------------------------------------------------------------------
void ABaseRobotCtrl::transferWaypointToStage()
{
  CRgbColor color;
  CPose2d pose;
  std::list<CWaypoint2d>::iterator it;
  Stg::ModelPosition::Waypoint stgWp;

  // clear current waypoint list in stage
  mDrivetrain->getStageModel()->waypoints.clear();

  // add current waypoint
  pose =  mCurrentWaypoint.getPose();
  stgWp.pose.x = pose.mX;
  stgWp.pose.y = pose.mY;
  stgWp.pose.a = pose.mYaw;
  stgWp.color = Stg::Color( 0, 1, 0 );  // green
  mDrivetrain->getStageModel()->waypoints.push_back( stgWp );

  for ( it = mWaypointList.begin(); it != mWaypointList.end(); it++ ) {
    pose = ( *it ).getPose();
    color = ( *it ).getColor();
    stgWp.pose.x = pose.mX;
    stgWp.pose.y = pose.mY;
    stgWp.pose.a = pose.mYaw;
    stgWp.color = Stg::Color( color.mRed / 255.0,
                              color.mBlue / 255.0,
                              color.mGreen / 255.0 );
    mDrivetrain->getStageModel()->waypoints.push_back( stgWp );
  }
}
//-----------------------------------------------------------------------------
void ABaseRobotCtrl::addPatch( CPatch patch )
{
  mPatchList.push_back( patch );
}
//-----------------------------------------------------------------------------
bool ABaseRobotCtrl::obstacleAvoid()
{
  unsigned int numSamples;
  bool obstruction = false;
  bool stop = false;

  // find the closest distance to the left and right and check if
  // there's anything in front
  double minleft = 1e6;
  double minright = 1e6;

  numSamples = mLaser->getNumSamples();
  for ( unsigned int i = 0; i < numSamples; i++ ) {

    if (( i > ( numSamples / 4 ) )
        && ( i < ( numSamples - ( numSamples / 4 ) ) )
        && mLaser->mRangeData[i].range < MIN_FRONT_DISTANCE ) {
      obstruction = true;
    }

    if ( mLaser->mRangeData[i].range < STOP_DISTANCE ) {
      stop = true;
    }

    if ( i > numSamples / 2 )
      minleft = std::min( minleft, mLaser->mRangeData[i].range );
    else
      minright = std::min( minright, mLaser->mRangeData[i].range );
  }

  if ( obstruction || stop || ( mAvoidCount > 0 ) ) {

    mDrivetrain->setTranslationalVelocityCmd( stop ? 0.0 : AVOID_SPEED );

    /* once we start avoiding, select a turn direction and stick
       with it for a few iterations */
    if ( mAvoidCount < 1 ) {
      mAvoidCount = random() % AVOID_DURATION + AVOID_DURATION;

      if ( minleft < minright ) {
        mDrivetrain->setRotationalVelocityCmd( -AVOID_TURN );
      }
      else {
        mDrivetrain->setRotationalVelocityCmd( + AVOID_TURN );
      }
    }
    mAvoidCount--;

    return true; // busy avoding obstacles
  }
  return false; // didn't have to avoid anything
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionFollowWaypointList()
{
  float angle;
  float leftDist;
  float rightDist;
  float rightFrontDist;
  bool fgRightWallFollow = false;

  if ( mFgStateChanged ) {
    if ( mWaypointList.size() == 0 ) {
      PRT_ERR0( "Waypoint list empty" );
      return COMPLETED;
    }
    mCurrentWaypoint = mWaypointList.front();
  }

  if ( mRobotPose.distance( mCurrentWaypoint.getPose() ) < 1.0 ) {
    if ( mWaypointList.size() == 0 )
      return COMPLETED;

    mCurrentWaypoint = mWaypointList.front();
    mWaypointList.pop_front();
  }

  if ( not obstacleAvoid() ) {
    rightDist = mLaser->mRangeData[0].range;
    leftDist = mLaser->mRangeData[mLaser->getNumSamples()-1].range;
    rightFrontDist = mLaser->mRangeData[mLaser->getNumSamples() / 4].range;
    //rprintf( "dist %f \n", leftDist + rightDist );

    // check if we are in a corridor
    if ( leftDist + rightDist < 2.7 ) {
      angle = D2R( -5000.0 * ( rightFrontDist - 0.8 ) );
      mDrivetrain->setRotationalVelocityCmd( angle );
      mDrivetrain->setTranslationalVelocityCmd( fabs( CRUISE_SPEED * cos( angle ) ) );
      fgRightWallFollow = true;
      //rprintf("rightWallFollow %f\n", R2D(angle));
    }

    if ( not fgRightWallFollow ) {
      angle = atan2( mCurrentWaypoint.getPose().mY - mRobotPose.mY,
                     mCurrentWaypoint.getPose().mX - mRobotPose.mX );
      angle = normalizeAngle( angle - mRobotPose.mYaw );
      mDrivetrain->setTranslationalVelocityCmd( fabs( CRUISE_SPEED * cos( angle ) ) );
      mDrivetrain->setRotationalVelocityCmd( angle );
    }
  }

  mReplanCounter ++;

  if ( ( mReplanCounter > 100 ) && ( mCurrentPatch ) ) {
    mReplanCounter = 0;
    mPathPlanner->getPathFromTo( mRobotPose, mCurrentPatch->getPose(), mWaypointList );
  }

  return IN_PROGRESS;
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionSelectPatch()
{
  unsigned int r;
  do {
    r = ( int ) randNo( 0, mPatchList.size() - 1 );
  } while ( r >= mPatchList.size() );
  mCurrentPatch = &mPatchList[r];
  //rprintf("assigning patch %d %s\n", r, mCurrentPatch->getPose().toStr().c_str());
  assert( mCurrentPatch );

  return COMPLETED;
}
//-----------------------------------------------------------------------------
void ABaseRobotCtrl::updateData( float dt )
{
  std::list<CWaypoint2d>::iterator it;

  //****************************************
  // Refresh data
  mRobotPose = mDrivetrain->getOdometry()->getPose();
  mElapsedStateTime += dt;
  mStatusStr =  mFsmText[mState];

  switch ( mState ) {
    case START:
      actionSelectPatch();
      mState = SW_PATCH;
      break;

    case SW_PATCH:
      if ( mFgStateChanged ) {
        //rprintf("path %s %s \n", mRobotPose.toStr().c_str(), mCurrentPatch->getPose().toStr().c_str());
        mPathPlanner->getPathFromTo( mRobotPose, mCurrentPatch->getPose(), mWaypointList );
      }

      if ( actionFollowWaypointList() == COMPLETED ) {
        mState = FORAGE;
      }
      break;

    case FORAGE:
      mState = START;
      break;

    default:
      PRT_ERR1( "Unknown state %d ", mState );
      mState = START;
      break;
  } // switch

  // transfer waypoints
  transferWaypointToStage();

  // set text display
  mTextDisplay->setText(  mCurrentPatch->getPose().toStr().c_str() ); //mStatusStr.c_str() );

  // Handle FSM status stuff
  if ( mState != mPrevState ) {
    mPrevState = mState;
    mFgStateChanged = true;
    mElapsedStateTime = 0.0;
  }
  else {
    mFgStateChanged = false;
  }
}
//-----------------------------------------------------------------------------

