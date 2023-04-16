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
#include "xratslam/localview/RecodedLV.hh"
#include "xratslam/localview/VisualTemplate.hh"
#include "xratslam/Image.hh"
#include "xratslam/Utils.hh"

#include <iostream>
#include <fstream>

#include <float.h>  // DBL_MAX.
#include <math.h>   // abs().


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************* RecodedLV *******************************/

/*******************/
/*** constructor ***/

RecodedLV::RecodedLV( string configFile )
  : LocalView( "RecodedLV" ),
    _vtSet( 10000 )
{
  _vtRelativeRad = 0;

  // Read ratslam settings from 'configFile'.
  Utils::PTree rss;
  Utils::getChildFromFile( rss, configFile, "ratslam" );

  // Read ratslam local view settings.
  Utils::getFromPtree( _vtShiftMatch, rss, "vt_shift_match"    , 25    );
  Utils::getFromPtree( _vtStepMatch , rss, "vt_step_match"     ,  5    );
  Utils::getFromPtree( _vtPanoramic , rss, "vt_panoramic"      ,  0    );
  Utils::getFromPtree( _vtNormFactor, rss, "vt_normalisation"  ,  0.0  );
  Utils::getFromPtree( _vtThreshold , rss, "vt_match_threshold",  0.03 );
  Utils::getFromPtree( _templateDimX, rss, "template_x_size"   , -1    );
  Utils::getFromPtree( _templateDimY, rss, "template_y_size"   , -1    );
  Utils::getFromPtree( _imageVtMinX , rss, "image_crop_x_min"  ,  0    );
  Utils::getFromPtree( _imageVtMaxX , rss, "image_crop_x_max"  , -1    );
  Utils::getFromPtree( _imageVtMinY , rss, "image_crop_y_min"  ,  0    );
  Utils::getFromPtree( _imageVtMaxY , rss, "image_crop_y_max"  , -1    );

  _maxVtId     = -1;
  _activeVtId  =  0;
  _prevVtId    =  0;
  _lastSavedId = -1;  // No templated was saved, yet.
}


/************/
/*** save ***/
bool
RecodedLV::save( string dir, string conf, int counter )
{
  return saveConfig( conf, counter ) && saveData( dir );
}


/************/
/*** load ***/
bool
RecodedLV::load( string dir, string conf, int counter )
{
  return loadConfig( conf, counter ) && loadData( dir );
}


/***************/
/*** on Feed ***/
void
RecodedLV::onFeed( Image *image, uint *vtId, double *relRad )
{
  static int count = 0;
  
  if ( ! image || ! image->data )
    return;

  if ( _imageVtMaxX == -1 ) _imageVtMaxX = image->width  - 1;
  if ( _imageVtMaxY == -1 ) _imageVtMaxY = image->height - 1;

  if ( _templateDimX == -1 ) _templateDimX = image->width;
  if ( _templateDimY == -1 ) _templateDimY = image->height;

  // Create a new Visual Template based on 'viewRgb' and destructs the
  // original 'viewRgb' image.
  VisualTemplate *newVt =
    VisualTemplate::create( _vtSet.size(), image,
                            _imageVtMinX, _imageVtMaxX,
                            _imageVtMinY, _imageVtMaxY,
                            _templateDimX, _templateDimY,
                            _vtNormFactor
                            );

  // Should consume 'image->data'.
  delete image->data;

  // Compare new template with all stored templates.
  int    vtMatchId;
  double vtError = DBL_MAX;
  compare( newVt, vtError, vtMatchId );

  if ( vtError <= _vtThreshold )
  {
    // If 'newVt' matchs, activate the matched VT.
    if ( _activeVtId != vtMatchId )
      _prevVtId = _activeVtId;
    
    _activeVtId = vtMatchId;
    delete newVt;
  }
  else
  {
    // If there is no match, insert new created VT into '_vtSet'.
    _vtRelativeRad = 0;

    _vtSet.add( newVt );
    
    _maxVtId = newVt->getId();
    
    _prevVtId   = _activeVtId;
    _activeVtId = newVt->getId();
  }

  // Resulting values.
  *vtId   = _activeVtId;
  *relRad = _vtRelativeRad;
}


/*******************/
/*** save Config ***/
bool
RecodedLV::saveConfig( string configFile, int iteration )
{
  ofstream file{ configFile };
  if ( ! file.is_open() ) return false;

  // Base class attributes.
  file
    << "templId   = " << _templId   << endl
    << "relRadius = " << _relRadius << endl
    ;

  // Attributes.
  file
    << "VtShiftMatch  = " << _vtShiftMatch  << endl
    << "VtStepMatch   = " << _vtStepMatch   << endl
    << "VtPanoramic   = " << _vtPanoramic   << endl
    << "VtNormFactor  = " << _vtNormFactor  << endl
    << "VtThreshold   = " << _vtThreshold   << endl
    << "ImageVtMinX   = " << _imageVtMinX   << endl
    << "ImageVtMaxX   = " << _imageVtMaxX   << endl
    << "imageVtMinY   = " << _imageVtMinY   << endl
    << "imageVtMaxY   = " << _imageVtMaxY   << endl
    << "TemplateDimX  = " << _templateDimX  << endl
    << "TemplateDimY  = " << _templateDimY  << endl
    << "MaxVtId       = " << _maxVtId       << endl
    << "ActiveVtId    = " << _activeVtId    << endl
    << "PrevVtId      = " << _prevVtId      << endl
    << "VtRelativeRad = " << _vtRelativeRad << endl
    ;
  file.close();

  return true;
}


/*******************/
/*** load Config ***/
bool
RecodedLV::loadConfig( string configFile, int iteration )
{
  ifstream file{ configFile };
  if ( ! file.is_open() ) return false;

  // Used to read labels an 'equal' signs.
  string str;

  // Base class attributes.
  file >> str >> str; file >> _templId  ;
  file >> str >> str; file >> _relRadius;
  
  // Attributes.
  file >> str >> str; file >> _vtShiftMatch ;
  file >> str >> str; file >> _vtStepMatch  ;
  file >> str >> str; file >> _vtPanoramic  ;
  file >> str >> str; file >> _vtNormFactor ;
  file >> str >> str; file >> _vtThreshold  ;
  file >> str >> str; file >> _imageVtMinX  ;
  file >> str >> str; file >> _imageVtMaxX  ;
  file >> str >> str; file >> _imageVtMinY  ;
  file >> str >> str; file >> _imageVtMaxY  ;
  file >> str >> str; file >> _templateDimX ;
  file >> str >> str; file >> _templateDimY ;
  file >> str >> str; file >> _maxVtId      ;
  file >> str >> str; file >> _activeVtId   ;
  file >> str >> str; file >> _prevVtId     ;
  file >> str >> str; file >> _vtRelativeRad;

  file.close();
  return true;
}


/*****************/
/*** save Data ***/
bool
RecodedLV::saveData( string dir )
{
  int lastId = _vtSet.save( dir, _lastSavedId );
  if ( lastId < 0 )
    return false;
  
  _lastSavedId = lastId;
  return true;
}


/*****************/
/*** load Data ***/
bool
RecodedLV::loadData( std::string dir )
{
  // Note that 'loadConfig()' should be called before, so '_maxVtId'
  // informs what templates should be loaded.
  //

  int maxId = _vtSet.load( dir, _maxVtId );
  cout <<"RecodedLV::loadData() - maxId: "<< maxId << endl;
  if ( maxId < 0 )
    return false;
  
  _maxVtId = maxId;
  return true;
}


/***************/
/*** compare ***/
void
RecodedLV::compare( VisualTemplate *newVt, double &vtError,
                    int &vtMatchId )
{
  if ( _vtSet.empty() )
  {
    vtError = DBL_MAX;
    return;
  }

  // Compatibility with old implementations.
  if ( _vtPanoramic )
  {
    cerr <<"RecodedLV::compare() - comparePanoramic() not implemented."
         << endl;
    return;
  }

  int vtShift = _vtShiftMatch;
  int vtMatch = _vtStepMatch;  
  compareStandard ( newVt, vtShift, vtMatch, vtError, vtMatchId );
}


/************************/
/*** compare Standard ***/
void
RecodedLV::compareStandard( VisualTemplate *newVt,
                            int shiftMatch, int stepMatch,
                            double &vtError, int &vtMatchId )
{
  double epsilon   = 0.005;
  double threshold = _vtThreshold + epsilon;
  double newMean   = newVt->mean();

  // Stores current mininum difference.
  double minDiff = vtError = DBL_MAX;

  // Run over all stored VT, in reverse order.
  //
  VisualTemplateSet::iterator it = _vtSet.begin();
  while ( it != _vtSet.end() )
  {
    VisualTemplate *vt = *it++;

    if ( ::abs( newMean - vt->mean() ) > threshold )
      continue;

    // Note: 'minDiff' is internally used by 'VisualTemplate::compare()'.
    if ( newVt->compare( vt, shiftMatch, stepMatch, &minDiff, &vtError ) )
      vtMatchId = vt->getId();
  }

  _vtRelativeRad = 0;
}
