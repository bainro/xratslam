/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/FileOdometry.hh>

#include <xratslam/Image.hh>
#include <xratslam/Odometry.hh>

#include <iostream>
//#include <string.h>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/***************************** FileOdometry *****************************/

/*******************/
/*** constructor ***/

FileOdometry::FileOdometry( string fileName )
{
  _stream.open( _fileName = fileName );
}


/******************/
/*** destructor ***/

FileOdometry::~FileOdometry()
{
  _stream.close();
}


/*************/
/*** reset ***/
void
FileOdometry::reset()
{
  _stream.close();
  _stream.open( _fileName );
}


/****************/
/*** get Next ***/
Odometry *
FileOdometry::getNext( Image * )
{
  Odometry *odometry = new Odometry();
  odometry->seq = 0;
  try
  {
    _stream >> odometry->seq;
    _stream >> odometry->dTime;
    _stream >> odometry->dPosition;;
    _stream >> odometry->dAngle;
  } catch ( ifstream::failure ex )
  {
    cerr <<"FileOdometry::readNext() - error."<< endl;
    delete odometry;
    return 0;
  }

  return odometry;
}
