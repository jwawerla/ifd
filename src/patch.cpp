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
#include "patch.h"
#include "patchvis.h"
#include "common.h"
#include "argparser.h"
#include <assert.h>

static unsigned int lastPuckId = 0;

//-----------------------------------------------------------------------------
int stgUpdate( Stg::Model* mod, CPatch* patch )
{
  patch->update();
  return 0; // ok
}
//-----------------------------------------------------------------------------
CPatch::CPatch( Stg::Model* mod, Stg::CtrlArgs* args ) : IPatch(),
    MIN_PUCK_PUCK_SPACING( 1.0 ),
    MIN_PUCK_ROBOT_SPACING( 3.5 )
{
  char str[50];
  Stg::Pose pose;
  int tmpInt;

  assert( mod );
  assert( args );

  mPatchManager = CPatchManager::getInstance( mod->GetWorld() );
  mPatchManager->registerPatch( this );
  mStgModel = mod;
  pose = mStgModel->GetPose();
  mPose.mX = pose.x;
  mPose.mY = pose.y;
  mPuckRate = 1.0;
  mMaxPucks = 0;
  mFgInitEmpty = false;
  mRadius = 1.0;

  mLastPuckPlacedTimestamp = 0.0;
  mName = mStgModel->Token();
  mStgModel->AddUpdateCallback(( Stg::stg_model_callback_t ) stgUpdate, this );
  mStgModel->Subscribe();

  /*
  if ( !  mStgModel->GetPropertyFloat( "puck_rate", &mPuckRate, 0.0 ) )
    PRT_WARN1( "Puck renewal rate not found %s\n",
               mStgModel->Token() );

  if ( !  mStgModel->GetPropertyInt( "max_pucks", &mMaxPucks, 0 ) )
    PRT_WARN1( "Maximal number of pucks in patch not found %s\n",
               mStgModel->Token() );

  if ( !  mStgModel->GetPropertyFloat( "radius", &mRadius, 0.0 ) )
    PRT_WARN1( "Radius not found %s\n",
               mStgModel->Token() );

  if ( !  mStgModel->GetPropertyInt( "init_empty", &tmpInt, 1 ) )
    PRT_WARN1( "Init empty flag  not found %s\n",
               mStgModel->Token() );
  mFgInitEmpty = tmpInt;
  */
  
  // First read arguments from world file
  CArgParser fileParser( args->worldfile );
  fileParser.read( CMD_PUCKS, mMaxPucks );
  fileParser.read( CMD_RATE, mPuckRate );
  fileParser.read( CMD_RADIUS, mRadius );
  fileParser.read( CMD_INIT, tmpInt );
  // then read the from command line if they are specified
  CArgParser cmdParser( args->cmdline );
  cmdParser.read( CMD_PUCKS, mMaxPucks );
  cmdParser.read( CMD_RATE, mPuckRate );
  cmdParser.read( CMD_RADIUS, mRadius );
  cmdParser.read( CMD_INIT, tmpInt );
  
  
  //createPucks();
  //mod->AddVisualizer( new CPatchVis( mRadius ), true );

  snprintf( str, 50, "%s %d %f", mName.c_str(), mMaxPucks, mPuckRate );
  mStgModel->Say( str );

}
//-----------------------------------------------------------------------------
CPatch::~CPatch()
{
  //printf( "CPatch::~CPatch() %s\n", mName.c_str() );
}
//-----------------------------------------------------------------------------
void CPatch::createPucks()
{

  char name[25];
  tPuck puck;
  Stg::Pose parentPose;
  Stg::Color color( 0, 0, 1 );
  Stg::World* world;
  world = mStgModel->GetWorld();
  assert( world );

  parentPose = mStgModel->GetPose();
  PRT_MSG2( 4, "Generating patch %s with %d pucks", mName.c_str(), mMaxPucks );
  for ( int i = 0; i < mMaxPucks; i++ ) {
    //puck.mod = world->CreateModel( mStgModel, "model" );
    //puck.mod = world->CreateModel( NULL, "model" );
    sprintf( name, "%d", i + lastPuckId );
    puck.mod = world->GetModel( name );
    assert( puck.mod );
    puck.mod->SetGeom( Stg::Geom( Stg::Pose(), Stg::Size( 0.1, 0.1, 0.4 ) ) );
    //puck.mod->SetParent( mStgModel );
    puck.mod->SetGuiNose( 0 );
    puck.mod->SetGuiMove( 1 );
    puck.mod->SetGuiOutline( 0 );
    puck.mod->SetGuiGrid( 0 );
    puck.mod->SetFiducialReturn( 0 );
    //puck.mod->SetLaserReturn( Stg::LaserVisible );
    puck.mod->SetGripperReturn( 1 );
    puck.mod->SetObstacleReturn( 0 );
    puck.mod->SetRangerReturn( 0 );
    puck.mod->SetPose( Stg::Pose( 30.0 + parentPose.x , 30.0 + parentPose.y, -1.0, 0.0 ) );
    puck.mod->SetColor( color );
    snprintf( name, 25, "puck_%s_%d", mName.c_str(), i );
    puck.mod->SetToken( name );
    puck.mod->SetGuiMove( true );
    mPuckVector.push_back( puck );

    if ( mFgInitEmpty ) {
      mFreePuckList.push_back( puck );
    }
    else {
      placePuck( &puck );
    }
  }
  lastPuckId += mMaxPucks;
}
//-----------------------------------------------------------------------------
void CPatch::update()
{
  float dt;

  dt = ( mStgModel->GetWorld()->SimTimeNow() * 1e-6 ) - mLastPuckPlacedTimestamp;

  if ( dt >= 1.0 / mPuckRate ) {
    if ( mFreePuckList.size() > 0 ) {
      placePuck( &mFreePuckList.front() );
      mFreePuckList.pop_front();
      mLastPuckPlacedTimestamp = mStgModel->GetWorld()->SimTimeNow() * 1e-6;
    }
  }

}
//-----------------------------------------------------------------------------
bool CPatch::isPuckIn( CPose2d pose, float radius )
{
  Stg::Pose stgPose;
  CPose2d p;

  for ( unsigned int i = 0; i < mPuckVector.size(); i++ ) {
    stgPose = mPuckVector[i].mod->GetPose();
    p.mX = stgPose.x;
    p.mY = stgPose.y;
    p.mYaw = stgPose.a;
    if ( pose.distance( p ) < radius )
      return true;  // found a puck that is close
  }

  return false; // no puck found with in radius
}
//-----------------------------------------------------------------------------
void CPatch::placePuck( tPuck* puck )
{
  Stg::Pose parentPose;
  int timeoutCount = 0;
  double x;
  double y;

  parentPose = mStgModel->GetPose();
  do {
    x =  randNo( -mRadius, mRadius );
    y =  randNo( -mRadius, mRadius );
    timeoutCount++;
    if ( timeoutCount > 200 ) {
      PRT_ERR1( "Failed to place pucks in patch %s", mName.c_str() );
      return;
    }
  }
  while ( sqrt( x*x + y*y ) > mRadius ||
          isPuckIn( CPose2d( x + mPose.mX, y + mPose.mY, 0.0 ),
                    MIN_PUCK_PUCK_SPACING ) ||
          mPatchManager->isRobotIn( CPose2d( mPose.mX + x, mPose.mY + y, 0.0 ),
                                    MIN_PUCK_ROBOT_SPACING ) );

  puck->mod->SetPose( Stg::Pose( x + parentPose.x, y + parentPose.y, 0.0, 0.0 ) );
}
//-----------------------------------------------------------------------------
CPose2d CPatch::getPose() const
{
  return mPose;
}
//-----------------------------------------------------------------------------
float CPatch::getRadius() const
{
  return mRadius;
}
//-----------------------------------------------------------------------------
void CPatch::puckConsumed( Stg::Model* puck )
{
  if ( puck == NULL )
    return;

  puck->SetPose( Stg::Pose( 0.0, 0.0, -1.0, 0.0 ) );

  for ( unsigned int i = 0; i < mPuckVector.size(); i++ ) {
    if ( puck == mPuckVector[i].mod ) {
      mFreePuckList.push_back( mPuckVector[i] );
      break;
    }
  }
}
//-----------------------------------------------------------------------------

