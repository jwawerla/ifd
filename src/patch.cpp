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
CPatch::CPatch( CPose2d pose, std::string name ) : IPatch()
{
  CPatchManager* manager = CPatchManager::getInstance( NULL );

  mPose = pose;
  mName = name;
  mRadius = 2.5;
  mLastPuckPlacedTimestamp = 0.0;

  manager->registerPatch( this );
}
//-----------------------------------------------------------------------------
CPatch::CPatch( Stg::Model* mod ) : IPatch()
{
  CPatchManager* manager = CPatchManager::getInstance( mod->GetWorld() );

  Stg::Pose pose;

  assert( mod );
  manager->registerPatch( this );
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
    //puck.mod = new Stg::Model(world);
    puck.mod = world->CreateModel( mStgModel, "model" );
    assert( puck.mod );
    geom = puck.mod->GetGeom();
    geom.size.x = 0.3;
    geom.size.y = 0.3;
    geom.size.z = 0.4;
    puck.mod->SetGeom( geom );
    pose = puck.mod->GetPose();
    pose.x = mPose.mX - 30;
    pose.y = mPose.mY + 30;
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
  Stg::Pose pose;
  float x;
  float y;

  do {
    x =  randNo( -mRadius, mRadius );
    y =  randNo( -mRadius, mRadius );
  }
  while ( isPuckIn( CPose2d( x, y, 0.0 ), 0.6 ) );

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


