/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/TimeLogger.hh>

#include <xratslam/Helper.hh>
#include <string.h>
#include <iostream>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/****************************** TimeLogger *******************************/

string TimeLogger::_dir = ".";


/*******************/
/*** constructor ***/

TimeLogger::TimeLogger( std::string logFile )
{
  _count     = 0;
  _totalTime = 0.0;
  
  string file = _dir + "/" + logFile;
  _log.open( file );
}


/******************/
/*** destructor ***/

TimeLogger::~TimeLogger()
{
  _log.close();
}


/***************/
/*** set Dir ***/
bool
TimeLogger::setDir( string dir )
{
  return Helper::mkDir( _dir = dir ) == 0;
}


/*************/
/*** start ***/
void
TimeLogger::start()
{
  _chrono.reset();
}


/************/
/*** stop ***/
void
TimeLogger::stop()
{
  _time       = _chrono.elapsed();
  _totalTime += _time;
  _count++;
}


/***********/
/*** log ***/
void
TimeLogger::log()
{
  _log << _count <<"\t"<< _time << endl;
  _log.flush();
}


/*******************/
/*** log Average ***/
void
TimeLogger::logAverage()
{
  _log << _count <<"\t"<< average() << endl;
  _log.flush();
}


/***************/
/*** average ***/
double
TimeLogger::average()
{
  return _totalTime / _count;
}
