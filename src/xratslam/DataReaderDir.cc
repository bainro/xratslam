/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/DataReaderDir.hh>

#include <xratslam/Image.hh>
#include <xratslam/Odometry.hh>

#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string.h>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/***************************** DataReaderDir *****************************/

/*******************/
/*** constructor ***/

DataReaderDir::DataReaderDir( string dir )
  : DataReader()
{
  _dir = dir;
}


/******************/
/*** destructor ***/

DataReaderDir::~DataReaderDir()
{
  _odometryReader = 0;
}


/***********************/
/*** read Next Image ***/
Image *
DataReaderDir::readNextImage( int seq )
{
  // Image is found by a given sequencial number.
  char fileName[2048];
  sprintf( fileName, "%s/%05d-image.png", _dir.c_str(), seq );
  return Image::create( fileName, seq );
}
