/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include "xratslam/Utils.hh"


#include <exception>
#include <math.h>
#include <iostream>

#include <boost/property_tree/ini_parser.hpp>
namespace pt = boost::property_tree;


using namespace xratslam;
using namespace std;



/*************************************************************************/
/********************************* Utils *********************************/

#define M_2PI  (2.0 * M_PI)   // A shortcut.


/***************************/
/*** get Child From File ***/
bool
Utils::getChildFromFile( pt::ptree &child, string file, string name )
{
  try
  {
    pt::ptree settings;
    read_ini( file, settings );
    return getPtreeChild( child, settings, name );
  }
  catch ( pt::ptree_bad_path pbp )
  {
    cout <<"SETTINGS(error): "<< name <<" child not found."<< endl;
    return false;
  }
  catch ( exception &ex ) { return false; }
  
  return true;
}


/***********************/
/*** get Ptree Child ***/
bool
Utils::getPtreeChild( pt::ptree &child, pt::ptree &settings, string name )
{
  try
  {
    child = settings.get_child( name );
  }
  catch ( pt::ptree_bad_path pbp )
  {
    cout <<"SETTINGS(error): "<< name <<" child not found."<< endl;
    return false;
  }
  return true;
}


/*******************/
/*** clip To 360 ***/
double
Utils::clipTo360( double angle )
{
  while ( angle <  0     )  angle += M_2PI;
  while ( angle >= M_2PI )  angle -= M_2PI;
  return angle;
}


/*******************/
/*** clip To 180 ***/
double
Utils::clipTo180( double angle )
{
  while ( angle >   M_PI )  angle -= M_2PI;
  while ( angle <= -M_PI )  angle += M_2PI;
  return angle;
}


/******************/
/*** angle Diff ***/
double
Utils::angleDiff( double angle1, double angle2 )
{
  double dir = clipTo180( angle2 - angle1 );

  double diffAngle = clipTo360( angle1 ) - clipTo360( angle2 );
  diffAngle = fabs( diffAngle );

  if ( diffAngle < M_2PI - diffAngle )
  {
    if ( dir > 0 ) return +diffAngle;
    else           return -diffAngle;
  }
  else
  {
    if ( dir > 0 ) return +(M_2PI - diffAngle);
    else           return -(M_2PI - diffAngle);
  }
}
