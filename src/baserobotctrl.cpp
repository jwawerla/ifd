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
const double AVOID_TURN = 0.75;
const double MIN_FRONT_DISTANCE = 0.6;
const double STOP_DISTANCE = 0.5;
const int AVOID_DURATION = 10; // 10

//-----------------------------------------------------------------------------
ABaseRobotCtrl::ABaseRobotCtrl( ARobot* robot ) : ARobotCtrl( robot )
{
  CLooseStageRobot* looseRobot;
  Stg::ModelBlobfinder* stgBlobfinder;

  mElapsedStateTime = 0.0;
  mState = START;
  mPrevState = START;
  mCurrentPatch = NULL;
  mAvoidCount = 0;
  mReplanCounter = 0;
  mCounter = 0;
  mNumPucksCollected = 0;
  mSwitchingTime = 0.0;
  mRightFrontDistance = 0.0;
  mPatchResidenceTime = 0.0;
  mPatchVector.clear();
  mFgProgress = true;
  mProgressTimer = new CTimer( mRobot );

  //************************************
  // FSM
  mFsmText[START]               = "start";
  mFsmText[SW_PATCH]            = "switch patch";
  mFsmText[FORAGE]              = "forage";
  mFsmText[CHOOSE_PATCH]        = "choose";
  mFsmText[ROTATE90]            = "rotate90";
  mFsmText[RETURN_TO_PATCH]     = "return to patch";
  mFsmText[RETURN_TO_PATCH_RND] = "return to patch rnd";

  // init path planer
  mPathPlanner = new CPathPlannerLookup( "circle.pts", "circle.fw",
                                         919.0 / 65.0, 927.0 / 65.0,
                                         -919.0 / 2.0 , 927.0 / 2.0 );

  //****************************************
  // init robot

  looseRobot = ( CLooseStageRobot* ) mRobot;
  //looseRobot->findDevice( mFiducial, "model:0.fiducial:0" );
  looseRobot->findDevice( mBlobFinder, "blobfinder:0" );
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

  // make blobfinder only track puck colors
  stgBlobfinder = mBlobFinder->getStageModel();
  stgBlobfinder->RemoveAllColors();
  stgBlobfinder->AddColor( Stg::Color( 0, 0, 1, 1 ) );

  // construct a gripper
  mGripper = ( Stg::ModelGripper* ) looseRobot->getStageModel()->GetChild( "gripper:0" );
  assert( mGripper );
  mGripper->Subscribe();

  // get robot pose
  mRobotPose = mDrivetrain->getOdometry()->getPose();

  mDataLogger = CDataLogger::getInstance("ifd.log", OVERWRITE);
  mDataLogger->addVar(&mNumPucksCollected, "collected");
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
void ABaseRobotCtrl::addPatch( IPatch* patch )
{
  mPatchVector.push_back( patch );
}
//-----------------------------------------------------------------------------
CPose2d ABaseRobotCtrl::getPose() const
{
  return mRobotPose;
}
//-----------------------------------------------------------------------------
bool ABaseRobotCtrl::inPatch() const
{
  if ( mCurrentPatch ) {
    if ( mRobotPose.distance( mCurrentPatch->getPose() ) <
         mCurrentPatch->getRadius() - 0.2 )
      return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool ABaseRobotCtrl::isMakingProgress()
{
  if ( mFgStateChanged ) {
    mFgProgress = true;
    mRobotProgressPose = mRobotPose;
    mProgressTimer->start();
  }

  if ( mProgressTimer->getElapsedTime() > 60.0 ) {
    if ( mRobotPose.distance( mRobotProgressPose ) > 0.2 )
      mFgProgress = true;
    else
      mFgProgress = false;
    mRobotProgressPose = mRobotPose;
    mProgressTimer->start();
  }
  return mFgProgress;
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

    //rprintf("avoiding obstacles \n");
    return true; // busy avoding obstacles
  }
  return false; // didn't have to avoid anything
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionFollowWaypointList()
{
  float diffRightFrontDist;
  float angle;
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

    rightFrontDist = mLaser->mRangeData[mLaser->getNumSamples() / 4].range;
    diffRightFrontDist = rightFrontDist - mRightFrontDistance;
    mRightFrontDistance = rightFrontDist;

    // check if we are in a corridor
    if ( inCorridor() ) {
      angle = D2R( -250.0 * ( rightFrontDist - 0.8 ) - 1.0 * diffRightFrontDist );
      //printf("%f  %f -> %f \n", rightFrontDist -0.8, diffRightFrontDist, R2D(angle));
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

  // since way points are very sparse, it is a good idea to replan every
  // once in a while to make sure we are not in a local minima
  mReplanCounter ++;
  //if (( mReplanCounter > 100 ) && ( mCurrentPatch ) ) {
  // mReplanCounter = 0;
  // mPathPlanner->getPathFromTo( mRobotPose, mCurrentPatch->getPose(),
  //                               mWaypointList );
  //}

  return IN_PROGRESS;
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionReturnToPatch()
{
  float angle;

  if ( mFgStateChanged ) {
    angle = atan2( mCurrentPatch->getPose().mY - mRobotPose.mY,
                   mCurrentPatch->getPose().mX - mRobotPose.mX );
    mHeading = randNo( angle - ( PI / 2.0 ), angle + ( PI / 2.0 ) );
  }

  if ( not obstacleAvoid() ) {
    angle = normalizeAngle( mHeading - mRobotPose.mYaw );
    mDrivetrain->setVelocityCmd( fabs( CRUISE_SPEED * cos( angle ) ), angle );
  }
  if ( inPatch() )
    return COMPLETED;

  return IN_PROGRESS;
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionForage(float dt)
{
  Stg::Model* puck = NULL;
  bool fgCloseToPuck = false;
  unsigned int center;
  float turnRate;
  float speed;

  mPatchResidenceTime += dt;

  if ( mBlobFinder->getNumReadings() > 0 ) {
    center = ( mBlobFinder->mBlobData[0].right +
               mBlobFinder->mBlobData[0].left ) / 2;
    turnRate = ( center - ( mBlobFinder->getScanWidth() / 2.0 ) ) / -150.0;
    //printf( "turnRate %f %d %d %f \n", R2D( turnRate ),
    //mBlobFinder->getScanWidth(), center, mBlobFinder->mBlobData[0].range );
    if ( mBlobFinder->mBlobData[0].range < 0.85 )
      fgCloseToPuck = true;

    if ( fabs( turnRate ) < 0.05 )
      speed = 0.3;
    else
      speed = 0.0; //0.05;
    mDrivetrain->setVelocityCmd( speed, turnRate );

    if ( mGripper->GetConfig().beam[0] )
      puck = mGripper->GetConfig().beam[0];
    if ( mGripper->GetConfig().beam[1] )
      puck = mGripper->GetConfig().beam[1];

    if ( puck ) {
      mNumPucksCollected ++;
      puckCollectedEvent();
      mCurrentPatch->puckConsumed( puck );
    }
  }
  else {
    // nothing in view, go straight
    mDrivetrain->setVelocityCmd( CRUISE_SPEED, 0.0 );
  }

  if ( not fgCloseToPuck ) {
    obstacleAvoid();
  }

  if ( mDrivetrain->isStalled() )
    mDrivetrain->setVelocityCmd( -0.1, 0.0 );

  return IN_PROGRESS;
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionRotate90()
{
  float angle;

  if ( mFgStateChanged ) {
    if ( drand48() < 0.5 )
      angle = D2R( -90.0 );
    else
      angle = D2R( 90.0 );
    mHeading = normalizeAngle( mRobotPose.mYaw + angle );
  }
  angle = normalizeAngle( mHeading - mRobotPose.mYaw );
  if ( angle > 0 )
    mDrivetrain->setVelocityCmd( 0.0, 0.75 );
  else
    mDrivetrain->setVelocityCmd( 0.0, -0.75 );

  if ( fabs( angle ) < 0.05 )
    return COMPLETED;

  return IN_PROGRESS;
}
//-----------------------------------------------------------------------------
void ABaseRobotCtrl::puckCollectedEvent()
{
  // nothing to do
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionSelectInitialPatch()
{
  unsigned int r;
  do {
    r = ( int ) randNo( 0, mPatchVector.size() - 1 );
  }
  while ( r >= mPatchVector.size() );
  mCurrentPatch = mPatchVector[r];
  //rprintf("assigning patch %d %s\n", r, mCurrentPatch->getPose().toStr().c_str());
  assert( mCurrentPatch );

  return COMPLETED;
}
//-----------------------------------------------------------------------------
tActionResult ABaseRobotCtrl::actionSelectPatch()
{
  unsigned int r;
  do {
    r = ( int ) randNo( 0, mPatchVector.size() - 1 );
  }
  while ( r >= mPatchVector.size() );
  mCurrentPatch = mPatchVector[r];
  //rprintf("assigning patch %d %s\n", r, mCurrentPatch->getPose().toStr().c_str());
  assert( mCurrentPatch );

  return COMPLETED;
}
//-----------------------------------------------------------------------------
void ABaseRobotCtrl::patchEnteringEvent()
{
  mPatchResidenceTime = 0.0;
}
//-----------------------------------------------------------------------------
bool ABaseRobotCtrl::patchLeavingDecision()
{
  return false; // do not leave
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
      actionSelectInitialPatch();
      mState = CHOOSE_PATCH;
      break;

    case CHOOSE_PATCH:
      actionSelectPatch();
      mState = SW_PATCH;
      break;

    case SW_PATCH:
      if ( mFgStateChanged ) {
        mSwitchingTime = 0.0;
        mPathPlanner->getPathFromTo( mRobotPose, mCurrentPatch->getPose(),
                                     mWaypointList );
      }
      mSwitchingTime += dt;
      if (( actionFollowWaypointList() == COMPLETED ) ||
          ( inPatch() ) ) {
        // we are about to enter the patch
        patchEnteringEvent();
        mState = FORAGE;
        rprintf("SW TIME %f \n", mSwitchingTime);
      }
      break;

    case FORAGE:
      if ( actionForage( dt ) == COMPLETED )
        mState = CHOOSE_PATCH;
      if ( not inPatch() )
        mState = RETURN_TO_PATCH_RND;
      if ( not isMakingProgress() )
        mState = ROTATE90;
      if ( patchLeavingDecision() ) {
        mState = CHOOSE_PATCH;
      }
      break;

    case ROTATE90:
      if ( actionRotate90() == COMPLETED )
        mState = FORAGE;
      break;

    case RETURN_TO_PATCH_RND:
      if ( actionReturnToPatch() == COMPLETED )
        mState = FORAGE;
      if (mElapsedStateTime > 20.0)
        mState = RETURN_TO_PATCH;
      break;

    case RETURN_TO_PATCH:
      if (mFgStateChanged) {
           mPathPlanner->getPathFromTo( mRobotPose, mCurrentPatch->getPose(),
                                     mWaypointList );
      }
      if (( actionFollowWaypointList() == COMPLETED ) ||
          ( inPatch() ) ) {
        mState = FORAGE;
      }
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
  else {
    mFgStateChanged = false;
  }

  mDataLogger->write(mRobot->getCurrentTime() );
}
//-----------------------------------------------------------------------------

