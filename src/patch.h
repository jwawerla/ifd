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
#ifndef PATCH_H
#define PATCH_H

#include "RapiLooseStage"
#include "pose2d.h"
#include "patchinterface.h"
#include "patchmanager.h"
#include <list>

using namespace Rapi;

/**
 * Defines a patch
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CPatch : public IPatch
{
    friend int stgUpdate( Stg::Model* mod, CPatch* patch );

  public:

    /**
     * Constructs a patch from a Stage model
     * @param mod stage model
     * @param args command line args form Stage
     */
    CPatch( Stg::Model* mod, Stg::CtrlArgs* args );
    /** Default destructor */
    virtual ~CPatch();
    /**
     * Gets the pose of the patch
     * @return pose
     */
    CPose2d getPose() const;
    /**
     * Gets the radius of a patch
     * @return [m]
     */
    float getRadius() const;
    /**
     * Removes a puck from a patch, because it was consumed by a forager
     * @param puck to be removed
     */
    void puckConsumed(Stg::Model* puck);
    /**
     * Gets the name of the patch
     * @return patch name
     */
    std::string getName() { return mName; };
    
  protected:
    /** Type definition for pucks */
    typedef struct {
      Stg::Model* mod;
    } tPuck;
    /**
     * Updates the patch
     */
    void update();
    /**
     * Place puck randomly
     * @param puck to be placed
     */
    void placePuck( tPuck* puck );
    /**
     * Is there a puck in a circle with in radius of pose
     * @param pose center of circle
     * @param radius of circle [m]
     * @return true if there is a puck, false otherwise
     */
    bool isPuckIn( CPose2d pose, float radius );
    /**
     * Creates the pucks handled by this patch
     */
    void createPucks();


  private:
    /** Minimum Spacing between any puck and a newly placed puck [m] */
    const float MIN_PUCK_PUCK_SPACING;
    /** Minimum Spacing between any robot and a newly placed puck [m] */
    const float MIN_PUCK_ROBOT_SPACING;
    /** Stage model */
    Stg::Model* mStgModel;
    /** Central patch manager */
    CPatchManager* mPatchManager;
    /** Pose of patch */
    CPose2d mPose;
    /** Puck renewal rate [1/s] */
    float mPuckRate;
    /** Upper puck limit */
    int mMaxPucks;
    /** List of pucks */
    std::vector<tPuck> mPuckVector;
    /** List of free pucks, that is pucks not currently in the patch */
    std::list<tPuck> mFreePuckList;
    /** Name of patch */
    std::string mName;
    /** Radius of patch [m] */
    float mRadius;
    /** Time stamp when the last puck was placed */
    float mLastPuckPlacedTimestamp;
    /** Flags if the patch is initially empty or not */
    bool mFgInitEmpty;
};

#endif
