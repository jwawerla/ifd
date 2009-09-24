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
  bool rightWallFollow = false;

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
    //rprintf( "left %f %f\n", leftDist, rightDist );

    // check if we are in a corridor
    if ( leftDist + rightDist < 2.5 ) {
      //if ( rightDist > 0.7 ) {
      angle = D2R( -10.0 * ( rightFrontDist - 0.8 ) );
      mDrivetrain->setRotationalVelocityCmd( angle );
      mDrivetrain->setTranslationalVelocityCmd( fabs( CRUISE_SPEED * cos( angle ) ) );
      rightWallFollow = true;
      //rprintf("rightWallFollow %f\n", R2D(angle));
      //}
    }

    if ( not rightWallFollow ) {
      angle = atan2( mCurrentWaypoint.getPose().mY - mRobotPose.mY,
                     mCurrentWaypoint.getPose().mX - mRobotPose.mX );
      angle = normalizeAngle( angle - mRobotPose.mYaw );
      mDrivetrain->setTranslationalVelocityCmd( fabs( CRUISE_SPEED * cos( angle ) ) );
      mDrivetrain->setRotationalVelocityCmd( angle );
    }
  }

  mReplanCounter ++;

  if (( mReplanCounter > 100 ) && ( mCurrentPatch ) ) {
    mReplanCounter = 0;
    //if ( mCurrentPatch )
    //  mPathPlanner->getPath( mRobotPose, mCurrentPatch->getPose(), mWaypointList );
  }

  return IN_PROGRESS;
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionSelectPatch()
{
  int r;

  r = ( int ) randNo( 0, mPatchList.size() - 1 );
  mCurrentPatch = &mPatchList[r];
  //rprintf("assigning patch %d %s\n", r, mCurrentPatch->getPose().toStr().c_str());
  assert( mCurrentPatch );

  return COMPLETED;
}
/*
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionFollowWaypointList()
{
  if ( mFgStateChanged ) {
    if ( not mWaypointList.empty() ) {
      mCurrentWaypoint = mWaypointList.front();
      mNd->setGoal( mCurrentWaypoint.getPose() );
      mNd->setEpsilonAngle( D2R( 180.0 ) );
      mNd->setEpsilonDistance( 1.0 );
    }
  }

  if ( mNd->isStalled() ) {
    mCounter ++;

    if ( mCounter > 40 ) {
      // we are stalled, so plan a new path
      //planPathTo( mCurrentDestination->getLocation(), CLOSE_SPACING );
      // and set the new waypoint if we have one otherwise set final destination
      if ( not mWaypointList.empty() ) {
        mCurrentWaypoint = mWaypointList.front();
        mWaypointList.pop_front();
        mNd->setGoal( mCurrentWaypoint.getPose() );
        mNd->setEpsilonAngle( D2R( 180.0 ) );
        mNd->setEpsilonDistance( 1.0 );
      }
      else {
        mNd->setGoal( mCurrentWaypoint.getPose() );
        mNd->setEpsilonAngle( D2R( 180.0 ) );
        mNd->setEpsilonDistance( 1.0 );
      }
    }
  }
  else
    mCounter = 0;

  // check if we are the goal
  if (( mNd->atGoal() ) ||
      ( mNd->hasCrossedPathNormal() ) ) {
    if ( mWaypointList.empty() ) {
      mDrivetrain->stop();
      return COMPLETED;
    }
    else {
      mCurrentWaypoint = mWaypointList.front();
      mWaypointList.pop_front();
      mNd->setGoal( mCurrentWaypoint.getPose() );
      // did we just pop the last waypoint ?
      if ( mWaypointList.empty() ) {
        mNd->setEpsilonAngle( D2R( 25.0 ) );
        mNd->setEpsilonDistance( 0.5 );
      }
      else {
        mNd->setEpsilonAngle( D2R( 180.0 ) );
        mNd->setEpsilonDistance( 1.0 );
      }
    }
  }

  // update ND and transfer velocity commands to drivetrain
  mNd->update( mRobot->getCurrentTime(), mRobotPose, mRobotVelocity );
  mDrivetrain->setVelocityCmd( mNd->getRecommendedVelocity() );

  return IN_PROGRESS;
}
*/
//-----------------------------------------------------------------------------
void ABaseRobotCtrl::updateData( float dt )
{
  std::list<CWaypoint2d>::iterator it;

  //****************************************
  // Refresh data
  mRobotPose = mDrivetrain->getOdometry()->getPose();
  mElapsedStateTime += dt;
  mStatusStr =  mFsmText[mState];

rprintf("time %f \n", mTextDisplay->getTimeStamp());
rprintf("robot %s %s\n", mRobotPose.toStr().c_str(), mDrivetrain->getVelocityCmd().toStr().c_str());
  switch ( mState ) {
    case START:
      actionSelectPatch();
      mState = SW_PATCH;
      break;

    case SW_PATCH:
      if ( mFgStateChanged ) {
        rprintf("path %s %s \n", mRobotPose.toStr().c_str(), mCurrentPatch->getPose().toStr().c_str());
        mPathPlanner->getPathFromTo( mRobotPose, mCurrentPatch->getPose(), mWaypointList );
      }

      if ( actionFollowWaypointList() == COMPLETED ) {
        mState = FORAGE;
      }
      break;

    case FORAGE:
      //mState = START;
      break;

    default:
      PRT_ERR1( "Unknown state %d ", mState );
      mState = START;
      break;
  } // switch

  // transfer waypoints
  transferWaypointToStage();

  // set text display
  mTextDisplay->setText( mStatusStr.c_str() );

  // Handle FSM status stuff
  if ( mState != mPrevState ) {
    mPrevState = mState;
    mFgStateChanged = true;
    mElapsedStateTime = 0.0;
  }
}
//-----------------------------------------------------------------------------

//#define VAR(V,init) __typeof(init) V=(init)
//#define FOR_EACH(I,C) for(VAR(I,(C).begin());I!=(C).end();I++)
/*
  template <class T, class C>
  void EraseAll( T thing, C& cont )
  { cont.erase( std::remove( cont.begin(), cont.end(), thing ), cont.end() );
*/