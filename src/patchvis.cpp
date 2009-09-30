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
#include "patchvis.h"



//-----------------------------------------------------------------------------
CPatchVis::CPatchVis( float radius )
    : Visualizer( "Patch", "show_patch" )
{
  mRadius = radius;
}
//-----------------------------------------------------------------------------
CPatchVis::~CPatchVis()
{
}
//-----------------------------------------------------------------------------
void CPatchVis::Visualize( Stg::Model* mod, Stg::Camera* cam )
{
  Stg::Pose pose;

  // push the current patch-centered coordinate frame on the stack
  glPushMatrix();
  // go into global coordinates
  Stg::Gl::pose_inverse_shift ( mod->GetGlobalPose() );

  pose = mod->GetPose();

  mod->PushColor( 1, 0.5, 0, 0.8 );
  drawCircle( pose.x, pose.y, pose.z, mRadius, 20 );
  mod->PopColor();

  glPopMatrix(); // back to patch coords

}
//-----------------------------------------------------------------------------
void CPatchVis::drawCircle( float x, float y, float z, float radius, float steps )
{
  glBegin( GL_LINE_LOOP );
  for ( float a = 0;  a < 2.0*M_PI; a += 2.0 * M_PI / steps )
    glVertex3f( x + radius * sin( a ),
                y + radius * cos( a ),
                z );
  glEnd();
}
//-----------------------------------------------------------------------------

