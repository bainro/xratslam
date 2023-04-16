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

#ifndef _xratslam_VisualTemplate_
#define _xratslam_VisualTemplate_


#include <vector>
#include <string>


/*************************************************************************/
/**************************** VisualTemplate *****************************/

namespace xratslam
{
  class Image;

  
  /******************/
  class VisualTemplate
  {
  public:

    typedef unsigned char       uchar;
    typedef unsigned int        uint;
    typedef std::vector<double> Data;

    
  public:

    /** Instantiate a VisualTemplate with given dimensions.
     */
    VisualTemplate( int dimX, int dimY );


    /** Create a visual template with dimensions (dimX, dimY) based
     * on the given image.
     */
    static VisualTemplate *create( uint id, Image *img,
                                   uint cropMinX, uint cropMaxX, 
                                   uint cropMinY, uint cropMaxY, 
                                   uint dimX,     uint dimY,
                                   double normFactor
                                   );

    // Template identifier.
    void setId( uint id )  { _id = id; }
    uint getId()           { return _id; }

    // Dimensions.
    uint dimX()  { return _dimX; }
    uint dimY()  { return _dimY; }
    //    uint size()  { return _dimX * _dimY; }

    /** Access to template elements.
     * ????
     */
    double *data()  { return _template.data(); }
    
    
    /** Return the mean of template image elements.
     */
    double mean()  { return _mean; }


    /** Compare this template with 'vt'.
     * Todo: describe parameters.
     * Return true if the distance between 'vt' and 'this' is less than
     * given '*minDist' distance. In this case, '*minDist' is filled with
     * the calculated distance.
     *
     * Obs: this: newVt, vt: from _vtSet.
     * Obs2: it is assumed that 'this' and 'vt' have the same dimensions.
     *       It is not checked!
     */
    bool compare( VisualTemplate *vt, int shiftMatch, int stepMatch,
                  double *minDist, double *error );

    /** Save template.
     */
    bool save( std::string fileName );
    
    /** Load template.
     */
    static VisualTemplate *load( std::string fileName );


  private:

    VisualTemplate()  {}

    /** Normalize template pixels.
     */
    void normalize( double factor );
    
    /** Calculate the mean of template image elements.
     * The resulting mean is stored in '_mean' attribute.
     */
    double calcMean();
    
    uint   _id;            // Template identifier.
    uint   _dimX, _dimY;   // Template dimensions.
    Data   _template;      // Template data.
    double _mean;          // Mean (math) of template elements.
  };
}

#endif
