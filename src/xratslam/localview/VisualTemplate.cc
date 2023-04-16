/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A RatSlam LocalView implementation.
 *
 * This implementation is based on:
 *   Code:
 *     David Ball (david.ball@qut.edu.au)         (1),
 *     Scott Heath (scott.heath@uqconnect.edu.au) (2)
 *   RatSLAM algorithm by:
 *     Michael Milford (1)
 *     Gordon Wyeth    (1) ([michael.milford, gordon.wyeth]@qut.edu.au)
 *
 *   1. Queensland University of Technology, Australia
 *   2. The University of Queensland, Australia
 *
 * The original code was used as an inspiration to try some personal ideas.
 * And also to understand the original code.
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
 *
 */

#include <xratslam/localview/VisualTemplate.hh>
#include <xratslam/Image.hh>

#include <cmath>
#include <iostream>
#include <fstream>

using namespace xratslam;
using namespace std;


/*************************************************************************/
/**************************** VisualTemplate *****************************/

/******************/
/*** construtor ***/

VisualTemplate::VisualTemplate( int dimX, int dimY )
  : _template( dimX * dimY, 0.0 )
{
  _dimX = dimX;
  _dimY = dimY;
  _id   = 0;
  _mean = -1.0;
}


/**************/
/*** create ***/
VisualTemplate *
VisualTemplate::create( uint id, Image *img,
                        uint cropMinX, uint cropMaxX, 
                        uint cropMinY, uint cropMaxY, 
                        uint dimX,     uint dimY,
                        double normFactor
                        )
{
  int blockSizeX = (cropMaxX - cropMinX) / dimX;
  int blockSizeY = (cropMaxY - cropMinY) / dimY;

  VisualTemplate *newVt = new VisualTemplate( dimX, dimY );
  newVt->_id = id;

  int    stepX  = (img->isGrey ? 1 : 3);
  int    stepY  = stepX * img->width;
  double maxVal = stepX * 255.0 * blockSizeX * blockSizeY;
    
  int dataNext = 0;
  
  int blockY = cropMinY;
  for ( int countY = 0; countY < dimY; countY++, blockY += blockSizeY )
  {
    int blockX = cropMinX;
    for ( int countX = 0; countX < dimX; countX++, blockX += blockSizeX )
    {
      double sum = 0.0;

      uchar *py   = img->data + blockX * stepX + blockY * stepY;
      uchar *endY = py + blockSizeY * stepY;
      for ( ; py < endY; py += stepY )
      {
        uchar *px   = py;
        uchar *endX = px + blockSizeX * stepX;
        while ( px < endX )
          sum += *px++;
      }
      newVt->_template[dataNext++] = sum / maxVal;
    }
  }

  // Normalize image pixels.
  // Obs: need to be done before mean calculation.
  if ( normFactor > 0.0 )
    newVt->normalize( normFactor );
    
  // Mean of new template image elements.
  newVt->calcMean();

  return newVt;
}


/***************/
/*** compare ***/
bool
VisualTemplate::compare( VisualTemplate *vt, int shiftMatch,
                         int stepMatch, double *minDist, double *error )
{
  // This: newVt, vt: from _vtSet.

  bool isCloser = false;
  int  vtSize   = _dimX * _dimY;
  
  // Parameters:
  //
  int offsetSize = 2 * shiftMatch + 1;
  int offsetStep = stepMatch;
  int sizeY      = vtSize - shiftMatch;
  int stepY      = _dimX;
  int sizeX      = _dimX - 2 * shiftMatch;
  
  double *myData = _template.data();
  double *vtData = vt->_template.data();

  for ( int offset = 0; offset < offsetSize; offset += offsetStep )
  {
    double dist = 0.0;
    
    double *vtY  = vtData + offset;
    double *myY  = myData + shiftMatch;
    double *endY = myData + sizeY;
    for ( ; myY < endY; vtY += stepY, myY += stepY )
    {
      // Sum to 'dist' the differences from 'vty' and 'myY' rows.
      double *vtX  = vtY;
      double *myX  = myY;
      double *endX = myX + sizeX;
      while ( myX < endX )
        dist += std::fabs( *myX++ - *vtX++ );

      // fast breaks
      if ( dist > *minDist )
        break;
    }
    
    if ( dist < *minDist )
    {
      *minDist = dist;
      isCloser = true;
    }
  }

  // If all calculated distances are greater than '*minDist',
  // return false.
  if ( ! isCloser )
    return false;

  // Calculate normalized error.
  *error = *minDist / (vtSize - 2 * shiftMatch * _dimY);
  
  return true;
}


/************/
/*** save ***/
bool
VisualTemplate::save( string fileName )
{
  // Template informations.
  ofstream file{ fileName };
  file
    << _id   << endl
    << _dimX << endl
    << _dimY << endl
    << _mean << endl
    ;

  // Template data
  int x = 0;
  for ( double data : _template )
  {
    file << data << " ";
    if ( ++x == _dimX )
    {
      file << endl;
      x = 0;
    }
  }
  file << endl;
  file.close();
  
  return true;
}


/************/
/*** load ***/
VisualTemplate *
VisualTemplate::load( string fileName )
{
  // Open file.
  ifstream file{ fileName };
  if ( ! file.is_open() ) return 0;

  VisualTemplate *vt = new VisualTemplate();
  
  // Template informations.
  file >> vt->_id;
  file >> vt->_dimX;
  file >> vt->_dimY;
  file >> vt->_mean;

  // Template data
  int size = vt->_dimX * vt->_dimY;
  vt->_template.resize( size );
  
  for ( int i = 0; i < size; i++ )
  {
    file >> vt->_template[i];
  }
  file.close();
  
  return vt;
}


/*****************/
/*** normalize ***/
void
VisualTemplate::normalize( double factor )
{
  double scale = factor / calcMean();
  
  // Scaled normalization.
  double *src = _template.data();
  double *end = src + _template.size();
  for ( ; src < end; src++ )
  {
    *src *= scale;
    if      ( *src < 0.0 ) *src = 0.0;
    else if ( *src > 1.0 ) *src = 1.0;
  }
}


/*****************/
/*** calc Mean ***/
double
VisualTemplate::calcMean()
{
  double sum  = 0.0;
  double *src = _template.data();
  double *end = src + _template.size();
  while ( src < end )
    sum += *src++;

  return _mean = (sum / _template.size());
}

