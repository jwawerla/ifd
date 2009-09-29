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
#ifndef IROBOTCTRL_H
#define IROBOTCTRL_H

#include "pose2d.h"

/**
 * Interface to the robot control used by the patch to get the position of the
 * robot and avoid placing puck on top of robots during renewal
 * @author Jens Wawerla (jwawerla@sfu.ca)
 */
class IRobotCtrl
{
  public:
    /** Default destructor */
    virtual ~IRobotCtrl() {};
    /**
     * Gets the robots current pose
     * @return pose
     */
    virtual CPose2d getPose() const = 0;

  protected:
    /** Default constructor */
    IRobotCtrl() {};

};

#endif
