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
#include <assert.h>


//-----------------------------------------------------------------------------
int stgUpdate( Stg::Model* mod, CPatch* patch )
{
  patch->update();
  return 0; // ok
}
//-----------------------------------------------------------------------------
CPatch::CPatch( CPose2d pose, std::string name ) : IPatch(),
    MIN_PUCK_PUCK_SPACING( 0.6 ),
    MIN_PUCK_ROBOT_SPACING( 1.5 )
{
  mPatchManager = CPatchManager::getInstance( NULL );
  mPose = pose;
  mName = name;
  mRadius = 2.5;
  mLastPuckPlacedTimestamp = 0.0;

  mPatchManager->registerPatch( this );
}
//-----------------------------------------------------------------------------
CPatch::CPatch( Stg::Model* mod ) : IPatch(),
    MIN_PUCK_PUCK_SPACING( 0.6 ),
    MIN_PUCK_ROBOT_SPACING( 3.5 )
{
  Stg::Pose pose;

  assert( mod );
  mPatchManager = CPatchManager::getInstance( mod->GetWorld() );
  mPatchManager->registerPatch( this );
  mStgModel = mod;
  pose = mStgModel->GetPose();
  mPose.mX = pose.x;
  mPose.mY = pose.y;

  mLastPuckPlacedTimestamp = 0;
  mName = mStgModel->Token();
  mStgModel->AddUpdateCallback(( Stg::stg_model_callback_t ) stgUpdate, this );
  mStgModel->Subscribe();

  if ( !  mStgModel->GetPropertyFloat( "puck_rate", &mPuckRate, 0.0 ) )
    PRT_WARN1( "Puck renewal rate not found %s\n",
               mStgModel->Token() );

  if ( !  mStgModel->GetPropertyInt( "max_pucks", &mMaxPucks, 0 ) )
    PRT_WARN1( "Maximal number of pucks in patch not found %s\n",
               mStgModel->Token() );

  if ( !  mStgModel->GetPropertyFloat( "radius", &mRadius, 0.0 ) )
    PRT_WARN1( "Radius not found %s\n",
               mStgModel->Token() );

  createPucks();
}
//-----------------------------------------------------------------------------
CPatch::~CPatch()
{
  printf( "CPatch::~CPatch() %s\n", mName.c_str() );
}
//-----------------------------------------------------------------------------
void CPatch::createPucks()
{
  Stg::Pose pose;
  char name[25];
  tPuck puck;
  Stg::Color color( 0, 0, 1 );
  Stg::Geom geom;
  Stg::World* world;
  world = mStgModel->GetWorld();
  assert( world );

  //PRT_MSG1( 4, "Generating %d pucks ", mMaxPucks );
  //printf("Generating %d pucks \n", mMaxPucks);
  for ( int i = 0; i < mMaxPucks; i++ ) {
    puck.mod = world->CreateModel( mStgModel, "model" );
    //puck.mod = world->CreateModel( NULL, "model" );
    assert( puck.mod );
    geom = puck.mod->GetGeom();
    geom.size.x = 0.1;
    geom.size.y = 0.1;
    geom.size.z = 0.4;
    puck.mod->SetGeom( geom );
    pose = puck.mod->GetPose();
    pose.x = 30 - mPose.mX;
    pose.y = 30 - mPose.mY;
    puck.mod->vis.gripper_return = true;
    puck.mod->SetPose( pose );
    puck.mod->SetColor( color );
    snprintf( name, 25, "puck_%s_%d", mName.c_str(), i );
    puck.mod->SetToken( name );
    puck.mod->SetGuiMove( true );
    mFreePuckList.push_back( puck );
    mPuckList.push_back( puck );
  }
}
//-----------------------------------------------------------------------------
void CPatch::update()
{
  float dt;

  dt = mStgModel->GetWorld()->SimTimeNow() - mLastPuckPlacedTimestamp;

  if ( dt >= 1.0 / mPuckRate ) {
    if ( mFreePuckList.size() > 0 ) {
      placePuck( &mFreePuckList.front() );
      mFreePuckList.pop_front();
      mLastPuckPlacedTimestamp = mStgModel->GetWorld()->SimTimeNow();
    }
  }
}
//-----------------------------------------------------------------------------
bool CPatch::isPuckIn( CPose2d pose, float radius )
{
  Stg::Pose stgPose;
  CPose2d p;

  for ( unsigned int i = 0; i < mPuckList.size(); i++ ) {
    stgPose = mPuckList[i].mod->GetPose();
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
  int timeoutCount = 0;
  Stg::Pose pose;
  float x;
  float y;

  do {
    x =  randNo( -mRadius, mRadius );
    y =  randNo( -mRadius, mRadius );
    timeoutCount++;
    if (timeoutCount > 100) {
      PRT_ERR1("Failed to place pucks in patch %s", mName.c_str() );
      return;
    }
  }
  while ( isPuckIn( CPose2d( x, y, 0.0 ), MIN_PUCK_PUCK_SPACING ) ||
          mPatchManager->isRobotIn( CPose2d( mPose.mX + x, mPose.mY + y, 0.0 ),
                                    MIN_PUCK_ROBOT_SPACING ) );

  pose = puck->mod->GetPose();
  pose.x = x;
  pose.y = y;
  puck->mod->SetPose( pose );
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


