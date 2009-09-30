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
#ifndef STGPATCHVIS_H
#define STGPATCHVIS_H

#include <stage.hh>


/**
 * Visualizes patches
 * @author Jens Wawerla <jwawerla@sfu.ca>
 */
class CPatchVis : public Stg::Visualizer
{
  public:
    /** Default constructor */
    CPatchVis( float radius );
    /** Default destructor */
    ~CPatchVis();

    virtual void Visualize( Stg::Model* displaymod, Stg::Camera* cam );

  protected:
    /**
     * Draws a circle
     * @param x center of circle
     * @param y center of circle
     * @param z center of circle
     * @param radius of circle
     * @param steps of discretization
    */
    void drawCircle( float x, float y, float z, float radius, float steps );

  private:
    /** Radius of patch [m] */
    float mRadius;
};

#endif
