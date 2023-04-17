/*
 * openRatSLAM
 *
 * Recoded by : Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * Date       : 08/04/2019 (dd/mm/yyyy)
 * Institution: Universidade Federal do Maranhão (UFMA).
 * Location   : Sâo Luis - MA - Brazil.
 *
 * Copyright (C) 2012
 *   David Ball (david.ball@qut.edu.au) (1),
 *   Scott Heath (scott.heath@uqconnect.edu.au) (2)
 *
 * RatSLAM algorithm by:
 *   Michael Milford (1) and
 *   Gordon Wyeth (1) ([michael.milford, gordon.wyeth]@qut.edu.au)
 *
 * 1. Queensland University of Technology, Australia
 * 2. The University of Queensland, Australia
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _ratslam_LocalViewScene_
#define _ratslam_LocalViewScene_


#include <boost/property_tree/ptree.hpp>
#include <irrlicht/irrlicht.h>
#include <string>


/*************************************************************************/
/**************************** LocalViewScene *****************************/

namespace ratslam
{
  class LocalViewMatch;

  
  /******************/
  class LocalViewScene
  {
    typedef unsigned char uchar;

    
  public:

    LocalViewScene( std::string configFile, LocalViewMatch *inVt );
    ~LocalViewScene()  {}

    
    bool drawAll();

    void updatePtr( LocalViewMatch *inVt );

    
  private:

    void drawImage( double *image,
                    bool    greyscale,
                    float   x,       float  y,
                    int     width,   int    height,
                    double  scale_x, double scale_y );
    
    void drawImage( const uchar *image,
                    bool   greyscale,
                    float  x,       float  y,
                    int    width,   int    height,
                    double scale_x, double scale_y );


    LocalViewMatch *_vtm;

    irr::IrrlichtDevice       *_device;
    irr::video::IVideoDriver  *_driver;
    irr::scene::ISceneManager *_scene;
    irr::scene::ISceneManager *_sceneVt;

    int _width, _height;
  };
}

#endif
