/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/Odometry.hh>
#include <fstream>
#include <iostream>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************* Odometry ********************************/

/**************/
/*** create ***/
Odometry *
Odometry::create( std::string fileName, int seq )
{
  ifstream file( fileName );
  if ( ! file.is_open() )
    return 0;

  Odometry *odo = new Odometry();
  
  string label;
  file >> label; file >> odo->seq;
  file >> label; file >> odo->dTime;
  file >> label; file >> odo->dPosition;
  file >> label; file >> odo->dAngle;

  file.close();

  // cout << "Odometry::create():" << endl
  //      << "  seq      : " << odo->seq       << endl
  //      << "  dTime    : " << odo->dTime     << endl
  //      << "  dPosition: " << odo->dPosition << endl
  //      << "  dAngle   : " << odo->dAngle    << endl
  //   ;
  
  return odo;
}


/*******************/
/*** constructor ***/

Odometry::Odometry()
{
  seq = 0;
}


/*************/
/*** write ***/
bool
Odometry::write( string fileName )
{
  ofstream file( fileName );
  if ( ! file.is_open() )
    return false;

  file << "seq       "<< seq       << endl;
  file << "dTime     "<< dTime     << endl;
  file << "dPosition "<< dPosition << endl;
  file << "dAngle    "<< dAngle    << endl;
  
  file.close();
  return true;
}


