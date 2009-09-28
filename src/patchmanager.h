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
#ifndef PATCHMANAGER_H
#define PATCHMANAGER_H

#include <vector>
#include <stage.hh>
#include "patchinterface.h"


/**
 * The patch manager keeps track of all available patches
 * @author Jens Wawerla
 */
class CPatchManager
{
  public:

    /** Default destructor */
    ~CPatchManager();
    /**
     * Singelton access method, gets the only instance of this class
     * @param world stage world
     * @return instance of this class
     */
    static CPatchManager* getInstance(Stg::World* world);
    /**
     * Registers a patch with the manager
     * @param patch to be registered
     */
    void registerPatch( IPatch* patch );
    /**
     * Gets the number of available patches
     * @return number of patches
     */
    unsigned int getNumPatches() const;
    /**
     * Gets the patch of index i or NULL if the patch does not exist
     * @param idx of patch to get
     * @return patch or NULL
     */
    IPatch* getPatch(unsigned int idx);

  protected:
    /**
     * Default constructor
     * @param mod Stage model
     */
    CPatchManager(Stg::Model* mod);

    /** List of registered patches */
    std::vector<IPatch*> mPatchList;
};


#endif
