/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/XVisualOdometry.hh>

#include <xratslam/Image.hh>
#include <xratslam/Odometry.hh>
#include "xratslam/Utils.hh"

#include <ratslam/visual_odometry.h>

#include <iostream>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/**************************** XVisualOdometry ****************************/

/*******************/
/*** constructor ***/

XVisualOdometry::XVisualOdometry( string configFile )
  : _configFile( configFile )
{
  _lastTime       = -1.0;
  _visualOdometry = 0;     // Needed to call reset().

  reset();
}


/******************/
/*** destructor ***/

XVisualOdometry::~XVisualOdometry()
{
  delete _visualOdometry;
}


/*************/
/*** reset ***/
void
XVisualOdometry::reset()
{
  // Read ratslam settings from 'configFile' to create VisualOdometry.
  Utils::PTree settings;
  Utils::getChildFromFile( settings, _configFile, "visual_odometry" );

  if ( _visualOdometry ) delete _visualOdometry;
  _visualOdometry = new ratslam::VisualOdometry( settings );
}


/****************/
/*** get Next ***/
Odometry *
XVisualOdometry::getNext( Image *image )
{
  if ( ! image ) return 0;
  
  double vPos = 0.01;  // Default positional velocity (m/s).
  double vAng = 0.01;  // Default angular    velocity (rad/s).
  _visualOdometry->on_image( image->data , image->isGrey,
                             image->width, image->height,
                             &vPos, &vAng
                             );

  // Construct xratslam::Odometry object.
  Odometry *odometry  = new Odometry();
  odometry->seq       = image->seq;
  odometry->dPosition = vPos;
  odometry->dAngle    = vAng;
  odometry->dTime     = (_lastTime > 0 ? image->time - _lastTime : 0.0);

  // Gambiarra
  if ( image->time < 1e-8 )
    odometry->dTime = 1.0 / _visualOdometry->getCameraHz();
    
  _lastTime = image->time;

  return odometry;
}
