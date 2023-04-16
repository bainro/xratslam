/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/DataEM.hh>

#include <xratslam/Experience.hh>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************** DataEM *********************************/

/*******************/
/*** constructor ***/

DataEM::DataEM()
{
  expId  = -1;
  relRad = 0.0;
}


/******************/
/*** destructor ***/

DataEM::~DataEM()
{
}


/*******************/
/*** find Limits ***/
bool
DataEM::findLimits( float *minX, float *maxX, float *minY, float *maxY )
{
  if ( expSet.empty() ) return false;

  Experience *expFirst = *expSet.begin();
  *minX = *maxX = expFirst->x;
  *minY = *maxY = expFirst->y;

  for ( Experience *exp : expSet )
  {
    float x = exp->x;
    float y = exp->y;

    if      ( *minX > x ) *minX = x;
    else if ( *maxX < x ) *maxX = x;
    
    if      ( *minY > y ) *minY = y;
    else if ( *maxY < y ) *maxY = y;
  }

  return true;
}


