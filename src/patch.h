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

#include "pose2d.h"

using namespace Rapi;

/**
 * Defines a patch
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CPatch
{
  public:
    /**
     * Default constructor
     * @param pose of patch
     */
    CPatch(CPose2d pose);
    /** Default destructor */
    ~CPatch();
    /**
     * Gets the pose of the patch
     * @return pose
     */
    CPose2d getPose() const;

  private:
    /** Pose of patch */
    CPose2d mPose;
};

#endif
