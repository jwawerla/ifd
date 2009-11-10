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
#include "epsilonsamplerctrl.h"

//-----------------------------------------------------------------------------
CEpsilonSamplerCtrl::CEpsilonSamplerCtrl( ARobot* robot ) :
    ABaseRobotCtrl( robot )
{
  mDataLogger->addVar( &mLongtermGainRate, "longtermGainRate", 5 );

  mTauLongtermGainRate = 0.001;
  mLongtermGainRate = 0.0;
  mNumPucksCollectedInPatch = 0;
}
//-----------------------------------------------------------------------------
CEpsilonSamplerCtrl::~CEpsilonSamplerCtrl()
{
}
//-----------------------------------------------------------------------------
void CEpsilonSamplerCtrl::updateData( float dt )
{
  float gainRate;

  ABaseRobotCtrl::updateData( dt );

  //mGainRate = mNumPucksCollectedInPatch / mRobot->getCurrentTime();
  //mLongtermGainRate += mTauLongtermGainRate * ( mGainRate - mLongtermGainRate );

  gainRate = mNumPucksCollectedInPatch / (140.0 + mPatchResidenceTime);

  mLongtermGainRateDot = (gainRate - mLongtermGainRate)/dt;
  mLongtermGainRate = gainRate;
}
//-----------------------------------------------------------------------------
bool CEpsilonSamplerCtrl::patchLeavingDecision()
{
  if ( mPatchResidenceTime < 120.0 )
    return false; // stay in current patch, we haven't spend enough time yet

  if (mLongtermGainRateDot > 0.0  )
    return false; // stay in current patch

  return true; // leave patch
}
//-----------------------------------------------------------------------------
tActionResult CEpsilonSamplerCtrl::actionSelectPatch()
{
  if ( mCurrentPatch == mPatchVector[1] )
    mCurrentPatch = mPatchVector[0];
  else
    mCurrentPatch = mPatchVector[1];

  rprintf( "SWITCH PATCH to %s after %f sec at %0.1f\n",
           mCurrentPatch->getName().c_str(), mPatchResidenceTime,
           mRobot->getCurrentTime() );
  return COMPLETED;
}
//-----------------------------------------------------------------------------
void CEpsilonSamplerCtrl::puckCollectedEvent()
{
  mNumPucksCollectedInPatch++;
}
//-----------------------------------------------------------------------------
void CEpsilonSamplerCtrl::patchEnteringEvent()
{
  ABaseRobotCtrl::patchEnteringEvent();
  mNumPucksCollectedInPatch = 0;
}
//-----------------------------------------------------------------------------


