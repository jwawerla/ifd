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
#include "patchinterface.h"
#include "robotctrlinterface.h"
#include <list>
#include <vector>

using namespace Rapi;

/** Type definition for FSM */
typedef enum {START = 0, SW_PATCH, FORAGE, CHOOSE_PATCH,
              ROTATE90, RETURN_TO_PATCH_RND, RETURN_TO_PATCH,
              NUM_STATES} tState;
/** Type definition for action results */
typedef enum {COMPLETED, IN_PROGRESS, FAILED } tActionResult;

/**
 * Base class for all robots
 * @author Jens Wawerla
 */
class ABaseRobotCtrl : public ARobotCtrl, public IRobotCtrl
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
    virtual void updateData( float dt );
    /**
     * Adds a patch to the list of patches
     * @param patch to be added
     */
    void addPatch( IPatch* patch );
    /**
     * Gets the robots current pose
     * @return pose
     */
    virtual CPose2d getPose() const;

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
     * Action randomly forages for pucks in a given patch
     * @param dt time step size [s]
     * @return action result
     */
    virtual tActionResult actionForage( float dt );
    /**
     * This function gets called by the controller every time puck is collected
     */
    virtual void puckCollectedEvent();
    /**
     * Event called when a patch is entered
     */
    virtual void patchEnteringEvent();
    /**
     * Checks if the robot is making progress moving around, or if it is stuck
     * in some behavioural loop
     * @return true if making progress, false if stuck
     */
    bool isMakingProgress();
    /**
     * Checks in robot is currently in a corridor
     * @return true if in corridor, false otherwise
     */
    inline bool inCorridor() {
      if (( mLaser->mRangeData[0].range +
            mLaser->mRangeData[mLaser->getNumSamples()-1].range ) < 2.7 )
        return true;
      return false;
    };
    /**
     * Checks if the robot is within the patch
     * @return true if in patch, false otherwise
     */
    bool inPatch() const;
    /** Current pose of robot */
    CPose2d mRobotPose;
    /** List of patches */
    std::vector<IPatch*> mPatchVector;
    /** Flags if the state of the FSM has changed */
    bool mFgStateChanged;
    /** Current patch */
    IPatch* mCurrentPatch;
    /** State of FSM */
    tState mState;
    /** Elapsed time since the current state was entered [s] */
    float mElapsedStateTime;
    /** Logger */
    CDataLogger* mDataLogger;
    /** Number of pucks collected */
    int mNumPucksCollected;
    /** Patch residence time for the current patch [s] */
    float mPatchResidenceTime;

  private:

    /** Previous state */
    tState mPrevState;
    /** Current waypoint */
    CWaypoint2d mCurrentWaypoint;
    /** Path planner */
    CPathPlannerLookup* mPathPlanner;
    /** List of way points */
    std::list<CWaypoint2d> mWaypointList;
    /** Stage gripper model */
    Stg::ModelGripper* mGripper;
    /** Stage position model */
    CLooseStageDrivetrain2dof* mDrivetrain;
    /** Stage laser model */
    CLooseStageLaser* mLaser;
    /** Stage blobfinder model */
    CLooseStageBlobFinder* mBlobFinder;
    /** Stage power pack */
    CLooseStagePowerPack* mPowerPack;
    /** Stage fiducial model */
    //CLooseStageFiducialFinder* mFiducial;
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
    /** Robot pose to measure progress */
    CPose2d mRobotProgressPose;
    /** Timer for measuring progress */
    CTimer* mProgressTimer;
    /** Flags if we make progress or not */
    bool mFgProgress;
    /** Heading for rotatign 90 deg [rad] */
    float mHeading;
    /** Right front laser reading [m] */
    float mRightFrontDistance;
    /** Last switching time (cost) [s] */
    float mSwitchingTime;
    /**
     * Action selects the inital patch at random
     * @return action result
     */
    virtual tActionResult actionSelectInitialPatch();
    /**
     * Action randomly returns the robot to the current patch
     * @return action result
     */
     tActionResult actionReturnToPatch();
    /**
     * Action drives the robot along the waypoint list
     * @return action result
     */
    tActionResult actionFollowWaypointList();
    /**
     * Action causes the robot to rotate 90 deg
     * @return action result
     */
    tActionResult actionRotate90();
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
