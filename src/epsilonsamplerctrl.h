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
#ifndef EPSILONSAMPLERCTRL_H
#define EPSILONSAMPLERCTRL_H

#include "baserobotctrl.h"

/**
 * Implements the epsilon sampler strategie
 * @author Jens Wawerla
 */
class CEpsilonSamplerCtrl : public ABaseRobotCtrl
{
  public:
    /**
     * Default constructor
     * @param robot to control
     */
    CEpsilonSamplerCtrl( ARobot* robot );
    /** Default destructor */
    virtual ~CEpsilonSamplerCtrl();
    /**
     * Update function called by the RAPI
     * @param dt size of time step [s]
     */
    virtual void updateData( float dt );

  protected:
    /**
     * Should we leave the current patch or not
     * @return true if we should leave, false otherwise
     */
    virtual bool patchLeavingDecision();
    /**
     * Action selects a patch at random
     * @return action result
     */
    virtual tActionResult actionSelectPatch();
    /**
     * This function gets called by the controller every time puck is collected
     */
    virtual void puckCollectedEvent();
    /**
     * Event called when a patch is entered
     */
    virtual void patchEnteringEvent();

  private:
    /** Longterm gain rate filter constant */
    float mTauLongtermGainRate;
    /** Longterm gain rate */
    float mLongtermGainRate;
    /** First derivative of longterm gain rate */
    float mLongtermGainRateDot;
    /** Pucks collected in current patch */
    unsigned int mNumPucksCollectedInPatch;
};

#endif
