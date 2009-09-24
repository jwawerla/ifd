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
#ifndef BASEROBOTCTRL_H
#define BASEROBOTCTRL_H

#include "RapiLooseStage"
#include "pathplannerlookup.h"
#include "patch.h"
//#include "stagend.h"
#include <list>
#include <vector>

using namespace Rapi;

/** Type definition for FSM */
typedef enum {START=0, SW_PATCH, FORAGE, NUM_STATES} tState;
/** Type definition for action results */
typedef enum {COMPLETED, IN_PROGRESS, FAILED } tActionResult;

/**
 * Base class for all robots
 * @author Jens Wawerla
 */
class ABaseRobotCtrl : public ARobotCtrl
{
  public:
    /**
     * Default constructor
     */
    ABaseRobotCtrl( ARobot* robot );
    /** Default destructor */
    virtual ~ABaseRobotCtrl();
    /**
     * Update function called by the RAPI
     * @param dt size of time step [s]
     */
    virtual void updateData(float dt);
    /**
     * Adds a patch to the list of patches
     * @param patch to be added
     */
    void addPatch(CPatch patch);

  protected:
    /** Current pose of robot */
    CPose2d mRobotPose;
    /** List of patches */
    std::vector<CPatch> mPatchList;
    /** Flags if the state of the FSM has changed */
    bool mFgStateChanged;
    /** Current patch */
    CPatch* mCurrentPatch;
    /** State of FSM */
    tState mState;
    /** Elapsed time since the current state was entered [s] */
    float mElapsedStateTime;


  private:
    /** Previous state */
    tState mPrevState;
    /** Nearness distance obstacle avoidance */
    //CStageNd* mNd;
    /** Current waypoint */
    CWaypoint2d mCurrentWaypoint;
    /** Path planner */
    CPathPlannerLookup* mPathPlanner;
    /** List of way points */
    std::list<CWaypoint2d> mWaypointList;
    /** Stage position model */
    CLooseStageDrivetrain2dof* mDrivetrain;
    /** Stage laser model */
    CLooseStageLaser* mLaser;
    /** Stage power pack */
    CLooseStagePowerPack* mPowerPack;
    /** Stage fiducial model */
    CLooseStageFiducialFinder* mFiducial;
    /** Text display */
    CLooseStageTextDisplay* mTextDisplay;
    /** General purpose counter */
    int mCounter;
    /** Current velocity of robot */
    CVelocity2d mRobotVelocity;
    /** Counter for obstacle avoidance [steps] */
    int mAvoidCount;
    /** Counter to determine when it is time to replan */
    int mReplanCounter;
    /** Single word describtion of FSM states */
    std::string mFsmText[NUM_STATES];
    /** String for status messages */
    std::string mStatusStr;
    /**
     * Action drives the robot along the waypoint list
     * @return action result
     */
    tActionResult actionFollowWaypointList();
    /**
     * Action selects a patch at random
     * @return action result
     */
    tActionResult actionSelectPatch();
    /**
     * Transfer waypoints to stages position device
     */
    void transferWaypointToStage();
    /**
     * Avoid obstacles and move to current waypoint
     * @return true if still avoiding obstacles, false if no obstacles have
     *         to be avoided
     */
    bool obstacleAvoid();
};

#endif
