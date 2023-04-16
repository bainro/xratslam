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
#include "LocalViewScene.hh"

#include <GL/gl.h>
#include <GL/glext.h>

#include "ratslam/utils.h"
#include "ratslam/local_view_match.h"

// Mesmu: debug.
#include <stdio.h>
#include <iostream>

namespace pt = boost::property_tree;

using namespace ratslam;
using namespace std;


/*************************************************************************/
/**************************** LocalViewScene *****************************/

/*******************/
/*** constructor ***/

LocalViewScene::LocalViewScene( string configFile, LocalViewMatch *inVt )
{
  pt::ptree settings, settingsDrawer;
  read_ini( configFile, settings );
  get_setting_child( settingsDrawer, settings, "draw", true );

  get_setting_from_ptree( _width,  settingsDrawer, "vt_window_width",  640 );
  get_setting_from_ptree( _height, settingsDrawer, "vt_window_height", 480 );

  updatePtr( inVt );

  //  auto edt = irr::video::EDT_SOFTWARE;
  //  auto edt = irr::video::EDT_OPENGL;
  
  auto dimension = irr::core::dimension2d<irr::u32>( _width, _height );
  _device = irr::createDevice( irr::video::EDT_OPENGL,
                               dimension,
                               32,
                               false,
                               false,
                               false
                               );
  _device->setWindowCaption( L"openRatSLAM Local View" );

  _driver  = _device->getVideoDriver();
  _scene   = _device->getSceneManager();
  _sceneVt = _scene->createNewSceneManager( false );
}
  

/****************/
/*** draw All ***/
bool
LocalViewScene::drawAll()
{
  auto bgColor = irr::video::SColor( 255, 0, 0, 0 );

  if ( ! _device->run() || ! _driver->beginScene( true, true, bgColor ) )
  {
    return false;
  }
  
  // TODO not always true for greyscale
  double scaleImg = (double)_width / _vtm->IMAGE_WIDTH;
  drawImage( (const uchar *)_vtm->view_rgb,
             _vtm->greyscale,
             -1.0f, +1.0f,
             _vtm->IMAGE_WIDTH,
             _vtm->IMAGE_HEIGHT,
             +scaleImg,
             -scaleImg
             );

  // Draw current template.
  double scaleVtX = (double) _width  / _vtm->TEMPLATE_X_SIZE;
  double scaleVtY = (double) _height / _vtm->TEMPLATE_Y_SIZE / 4;
  drawImage( (double*) &_vtm->templates[_vtm->current_vt].data[0],
             true,
             -1.0f, 0.0f,
             _vtm->TEMPLATE_X_SIZE,
             _vtm->TEMPLATE_Y_SIZE,
             +scaleVtX,
             -scaleVtY
             );

  // Draw viewed template.
  drawImage( (double*) &_vtm->current_view[0],
             true,
             -1.0f, -0.5f,
             _vtm->TEMPLATE_X_SIZE,
             _vtm->TEMPLATE_Y_SIZE,
             +scaleVtX,
             -scaleVtY
             );
  
  _sceneVt->drawAll();
  _driver->endScene();
}
  

/******************/
/*** update Ptr ***/
void
LocalViewScene::updatePtr( LocalViewMatch *inVt )
{
  _vtm = inVt;
}


/******************/
/*** draw Image ***/
void
LocalViewScene::drawImage( double *image,
                           bool    greyscale,
                           float   x,      float  y,
                           int     width,  int    height,
                           double  scaleX, double scaleY )
{
  int textureSize = width * height * (greyscale ? 1 : 3);
  uchar texture[textureSize];

  /*
  // Debug.
  cout << "LocalViewScene::draw_image_1():"<< endl;
  cout << "  (x,y)    : "<< x <<", "<< y << endl;
  cout << "  width    : "<< width        << endl;
  cout << "  height   : "<< height       << endl;
  cout << "  greyscale: "<< greyscale    << endl;
  cout << "  scale    : "<< scaleX <<", "<< scaleY << endl;
  cout << endl;
  */
  
  // Copy 'image' content (double) to 'texture' (uchar).
  double *src = image;
  uchar  *dst = texture;
  uchar  *end = texture + textureSize;
  while ( dst < end )
  {
    *dst++ = (uchar)( *src++ * 255.0 );
  }

  drawImage( texture, greyscale, x, y, width, height, scaleX, scaleY );
}


/******************/
/*** draw Image ***/
void
LocalViewScene::drawImage( const uchar *image,
                           bool   greyscale,
                           float  x,      float  y,
                           int    width,  int    height,
                           double scaleX, double scaleY )
{
  /*
  // Debug.
  cout << "LocalViewScene::draw_image_2():"<< endl;
  cout << "  (x,y)    : "<< x <<", "<< y << endl;
  cout << "  width    : "<< width        << endl;
  cout << "  height   : "<< height       << endl;
  cout << "  greyscale: "<< greyscale    << endl;
  cout << "  scale    : "<< scaleX <<", "<< scaleY << endl;
  cout << endl;
  */
  
  glRasterPos2f( x, y );
  glPixelZoom( scaleX, scaleY );
  if ( greyscale )
    glDrawPixels( width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, image );
  else
    glDrawPixels( width, height, GL_BGR, GL_UNSIGNED_BYTE, image );
}
