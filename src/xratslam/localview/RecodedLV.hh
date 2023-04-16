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

#ifndef _xratslam_RecodedLV_
#define _xratslam_RecodedLV_

#include <xratslam/LocalView.hh>
#include <xratslam/localview/VisualTemplateSet.hh>


/*************************************************************************/
/******************************* RecodedLV *******************************/

namespace xratslam
{
  class VisualTemplate;

  
  /*************/
  class RecodedLV
    : public LocalView
  {
  public:

    typedef unsigned char uchar;
    typedef unsigned int  uint;
    
    
  public:
    
    RecodedLV( std::string configFile );
    ~RecodedLV() {}

    
    bool save( std::string dir, std::string conf, int counter ) override;
    bool load( std::string dir, std::string conf, int counter ) override;
    int  getSize() override { return _vtSet.size(); }

    
  private:

    /** See: xratslam::LocalView class.
     */
    void onFeed( Image  *image, uint *vtId, double *relRad ) override;

    /** Save object parameters.
     */
    bool saveConfig( std::string configFile, int iteration );
    
    /** Load object parameters.
     */
    bool loadConfig( std::string configFile, int iteration );

   /** Save internal data (template set).
    */
    bool saveData( std::string dir );
    
    /** Load internal states from given file.
     */
    bool loadData( std::string dir );

    // int saveTemplateSet( std::string dir, int iteration ) override;
    // int loadTemplateSet( std::string dir, int iteration ) override;
    

    // TODO: pass templated to be compared as parameter.
    //
    // compare a visual template to all the stored templates, allowing for
    // slen pixel shifts in each direction
    // returns the matching template and the MSE
    void compare( VisualTemplate *newVt, double &vtError, int &vtMatchId );

    /*
    void comparePanoramic( VisualTemplate *newVt,
                           double &vtError, int &vtMatchId );
    */
    
    void compareStandard ( VisualTemplate *newVt,
                           int shiftMatch , int stepMatch,
                           double &vtError, int &vtMatchId );


    int _vtShiftMatch;
    int _vtStepMatch;

    int    _vtPanoramic;
    double _vtNormFactor;  // Normalization factor.
    
    double _vtThreshold;   // Match threshold.
    int    _imageVtMinX;
    int    _imageVtMaxX;
    int    _imageVtMinY;
    int    _imageVtMaxY;
    int    _templateDimX;
    int    _templateDimY;

    VisualTemplateSet _vtSet;
    int               _maxVtId;  // '_vtSet' can have more than is used.

    int    _activeVtId;
    int    _prevVtId;
    double _vtRelativeRad;

    // Storage related.
    //
    int _lastSavedId;   // Number of already saved templates.
  };
}

#endif
