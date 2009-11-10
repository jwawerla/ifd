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

#ifndef PATCH_INTERFACE_H
#define PATCH_INTERFACE_H

#include "pose2d.h"

using namespace Rapi;

/**
 * Interface class for patches, this exposes the interface to a patch that
 * is used by the robots
 * @author Jens Wawerla
 */
class IPatch
{
  public:
    /** Default destructor */
    virtual ~IPatch() {};
    /**
     * Gets the pose of the patch
     * @return pose
     */
    virtual CPose2d getPose() const = 0;
    /**
     * Gets the radius of a patch
     * @return [m]
     */
    virtual float getRadius() const = 0;
    /**
     * Removes a puck from a patch, because it was consumed by a forager
     * @param puck to be removed
     */
    virtual void puckConsumed(Stg::Model* puck) = 0;
    /**
     * Gets the name of the patch
     * @return patch name
     */
    virtual std::string getName() = 0;

  protected:
    /** Default constructor */
    IPatch() {};
};

#endif
