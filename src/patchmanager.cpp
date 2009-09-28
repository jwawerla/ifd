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
#include "patchmanager.h"
#include <stdio.h>
#include <assert.h>

int stgUpdate( Stg::Model* mod, CPatchManager* manager )
{
  printf("stgUpdate\n");
  return 0; // ok
}

//-----------------------------------------------------------------------------
CPatchManager::CPatchManager(Stg::Model* mod)
{
  assert(mod);
  //mod->AddUpdateCallback(( Stg::stg_model_callback_t ) stgUpdate, this );
  printf("CPatchManager::CPatchManager()\n"); 
}
//-----------------------------------------------------------------------------
CPatchManager::~CPatchManager()
{
}
//-----------------------------------------------------------------------------
CPatchManager* CPatchManager::getInstance(Stg::World* world)
{
  Stg::Model* patchManagerCtrlMod;
  static CPatchManager* instance = NULL;

  if (instance == NULL) {
    assert(world);
    patchManagerCtrlMod = world->GetModel( "patchManagerCtrl");

    if ( patchManagerCtrlMod ) {
      instance = (CPatchManager*)(patchManagerCtrlMod->GetProperty("patchmanager") );
    }
   if (instance == NULL) {
      instance = new CPatchManager(patchManagerCtrlMod);
      if ( patchManagerCtrlMod )
        patchManagerCtrlMod->SetProperty("patchmanager", (void*)instance);
    }
  }
  return instance;
}
//-----------------------------------------------------------------------------
void CPatchManager::registerPatch( IPatch* patch )
{
  mPatchList.push_back(patch);
}
//-----------------------------------------------------------------------------
unsigned int CPatchManager::getNumPatches() const
{
  return mPatchList.size();
}
//-----------------------------------------------------------------------------
IPatch* CPatchManager::getPatch(unsigned int idx)
{
  if (idx < mPatchList.size() )
    return mPatchList[idx];

  return NULL; // patch not found
}
//-----------------------------------------------------------------------------


