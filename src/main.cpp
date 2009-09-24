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

#include "RapiLooseStage"
#include "baserobotctrl.h"

//-----------------------------------------------------------------------------
extern "C" int Init ( Stg::Model* mod )
{
  static bool init = false;


  Rapi::CLooseStageRobot* robot;
  ABaseRobotCtrl* robotCtrl;

  if ( not init )
  {
    init = true;
    printf ( "-----------------------------------\n" );
    printf ( "IFD \n" );
    printf ( "  build %s %s \n", __DATE__, __TIME__ );
    printf ( "  compiled against RAPI version %s (%s) build %s\n", RAPI_VERSION(),
             RAPI_GIT_VERSION(), RAPI_BUILD() );
    printf ( "\n" );

    //************************************
    // init general stuff
    ErrorInit ( 2, false );
    initRandomNumberGenerator();
  }
  //************************************
  // create robot and its controller
  robot = new Rapi::CLooseStageRobot ( mod );
  robotCtrl = new ABaseRobotCtrl ( robot );

  robotCtrl->addPatch ( CPatch ( CPose2d ( 0, 26,0 ) ) );
  robotCtrl->addPatch ( CPatch ( CPose2d ( 17, 17,0 ) ) );
  robotCtrl->addPatch ( CPatch ( CPose2d ( 26,  0,0 ) ) );
  robotCtrl->addPatch ( CPatch ( CPose2d ( 17, -26,0 ) ) );
  //robotCtrl->addPatch ( CPatch ( CPose2d ( 0, -26,0 ) ) );
  //robotCtrl->addPatch ( CPatch ( CPose2d ( -17, -17,0 ) ) );
  robotCtrl->addPatch ( CPatch ( CPose2d ( -26,  0,0 ) ) );
  robotCtrl->addPatch ( CPatch ( CPose2d ( 17, -17,0 ) ) );

  return 0; // ok

}
//-----------------------------------------------------------------------------

