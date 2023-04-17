/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/Log.hh>

#include <fstream>
#include <iostream>
#include <sstream>
#include <opencv2/highgui/highgui.hpp>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/********************************* Log ***********************************/

string Log::_dir = "./slamData/irat";


/***************/
/*** set Dir ***/
void
Log::setDir( string dir )
{
  _dir = dir;
}


/*************/
/*** image ***/
bool
Log::image( int seq, uchar *data, bool isGrey, int width, int height )
{
  string name = fileName( seq, "image.png" );

  cv::Mat mat( height, width, CV_8UC3, data );
  if ( ! cv::imwrite( name, mat ) )
  {
    stringstream msg;
    msg << "ERROR: failed to save the image: " << name;
    message( msg.str() );
    
    return false;
  }

  return true;
}


/****************/
/*** odometry ***/
bool
Log::odometry( int seq, double dTime, double vPos, double vAng )
{
  static char msg [2048];

  ofstream file( _dir + "/odometry.txt", std::ofstream::app );
  sprintf( msg, "%5d %f %+f %+f\n", seq, dTime, vPos, vAng );
  file << msg;

  file.close();
  return true;
}


/***************/
/*** message ***/
bool
Log::message( string msg )
{
  static char name[2048];
  
  sprintf( name, "%s/log.txt", _dir.c_str() );
  ofstream file( name, std::ofstream::app );
  file << msg << endl;
  
  file.close();
  return true;
}


/***************/
/*** message ***/
bool
Log::message( int id, string msg )
{
  static char name[2048];
  static char str [2048];

  sprintf( name, "%s/log.txt", _dir.c_str() );
  ofstream file( name, std::ofstream::app );
  sprintf( str, "%5d: %s\n", id, msg.c_str() );
  file << str << endl;
  
  file.close();
  return true;
}


/*****************/
/*** file Name ***/
string
Log::fileName( int seq, string suffix )
{
  static char str[2048];
  
  sprintf( str, "%s/%05d-%s", _dir.c_str(), seq, suffix.c_str() );
  return str;
}


