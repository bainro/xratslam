/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/DataReader.hh>
#include <xratslam/Image.hh>
#include <xratslam/Odometry.hh>
#include <xratslam/XVisualOdometry.hh>
#include <xratslam/FileOdometry.hh>
#include <xratslam/Helper.hh>

#include <iostream>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/****************************** DataReader *******************************/

/*******************/
/*** constructor ***/

DataReader::DataReader()
{
  _counter    = 0;
  _totalRead  = 0;
  
 // Default: does not store. Should call "setStorageDir".
  _storageDir = "";

  // Must set odometry reader.
  _odometryReader = 0;
}


/******************/
/*** destructor ***/

DataReader::~DataReader()
{
  if ( _odometryReader ) delete _odometryReader;
}


/***************************/
/*** set Odometry Visual ***/
bool
DataReader::setOdometryVisual( string configFile )
{
  _imageSeq = 1;
  _isVisual = true;
  if ( _odometryReader )
    delete _odometryReader;
  
  _odometryReader = new xratslam::XVisualOdometry( configFile );

  return isOpened();
}


/*************************/
/*** set Odometry File ***/
bool
DataReader::setOdometryFile( string odometryFile )
{
  _isVisual = false;
  if ( _odometryReader )
    delete _odometryReader;
  _odometryReader = new xratslam::FileOdometry( odometryFile );
  return isOpened();
}


/*****************/
/*** is Opened ***/
bool
DataReader::isOpened()
{
  return _odometryReader ? _odometryReader->isOpened() : false;
}


/***********************/
/*** set Storage Dir ***/
bool
DataReader::setStorageDir( string storageDir )
{
  return Helper::mkDir( _storageDir = storageDir );
}


/*****************/
/*** read Next ***/
bool
DataReader::readNext( Image **image, Odometry **odometry )
{
  cout << "HI1" << endl;
  if ( ! _odometryReader )
  {
    cerr <<"DataReader::readNext() - ERROR: OdometryReader not set."
         << endl;
    return false;
  }
  
  cout << "HI2" << endl;
  // If data to be read had already been stored, read stored data.
  //
  if ( _counter < _totalRead && ! loadData( image, odometry ) )
    return false;

  // Read data from input channel.
  //
  else
  {
    if ( ! isOpened() )
      return false;

    // When visual odometry is used images are used to find odometry.
    // Thus, images should be read before odometry creation.
    // On the other hand, when robot generated odometries are used and
    // images are stored as a sequence of images, the odometry count
    // indicates what image should be read.
    //

    *odometry = 0;
    *image    = 0;

    cout << "HI3" << endl;
    
    try
    {
      // Read image before odometry.
      if ( _isVisual )
      {
        cout << "HI4" << endl;
        if ( ! (*image = readNextImage( _imageSeq++ )) )
          throw string( "image ERROR-1" );
        if ( ! (*odometry = _odometryReader->getNext( *image )) )
          throw string( "odometry ERROR-1" );
      }
      // Read odometry before image.
      else
      {
        cout << "HI4.5" << endl;
        if ( ! (*odometry = _odometryReader->getNext()) )
          throw string( "odometry ERROR-2" );
        if ( ! (*image = readNextImage( (*odometry)->seq )) )
          throw string( "image ERROR-2" );
      }
    } catch ( std::string &msg )
    {
      if ( *odometry ) delete *odometry; *odometry = 0;
      if ( *image    ) delete *image   ; *image    = 0;
      return false;
    }
    
    cout << "HI5" << endl;
    
    // Debug: all read data is saved in a storage inner directory.
    // This must be controled by library users!
    saveData( *image, *odometry );
    
    _totalRead++;
  }

  _counter++;

  return true;
}


/**********************/
/*** change Counter ***/
bool
DataReader::changeCounter( int newCounter )
{
  if ( _storageDir.empty() || newCounter >= _totalRead )
    return false;

  _counter = newCounter;
  return true;
}


/*****************/
/*** save Data ***/
bool
DataReader::saveData( Image *image, Odometry *odometry )
{
  // If should not store data or current data has already been stored,
  // return.
  if ( _storageDir.empty() || _counter < _totalRead )
  {
    /*
    cerr <<"DataReader::saveData() - no data stored."
         <<", _storageDir: ["<< _storageDir <<"]"
         <<", _counter: "    << _counter   
         <<", _totalRead: "  << _totalRead
         << endl;
    */
    return false;
  }
  
  // Write image.
  string imageFileName = getImageFileName();
  if ( ! image->write( imageFileName ) )
  {
    cerr <<"DataReader::saveData() - ERROR writing image: "
         << imageFileName << endl;
    return false;
  }

  // Write odometry.
  string odometryFileName = getOdometryFileName();
  if ( ! odometry->write( odometryFileName ) )
  {
    cerr <<"DataReader::saveData() - ERROR writing odometry: "
         << odometryFileName << endl;
    return false;
  }

  return true;
}


/*****************/
/*** load Data ***/
bool
DataReader::loadData( Image **image, Odometry **odometry )
{
  // Read image.
  string imageFileName = getImageFileName();
  if ( ! (*image = Image::create( imageFileName )) )
  {
    cerr <<"DataReader::readNext() - ERROR reading image: "
         << imageFileName << endl;
    return false;
  }
  
  // Read odometry.
  string odometryFileName = getOdometryFileName();
  if ( ! (*odometry = Odometry::create( odometryFileName )) )
  {
    cerr <<"DataReader::readNext() - ERROR reading odometry: "
         << odometryFileName << endl;
    
    delete *image; *image = 0;
    return false;
  }

  return true;
}


/***************************/
/*** get Image File Name ***/
string
DataReader::getImageFileName()
{
  static char buf[2048];
  
  sprintf( buf, "%s/image-%06d.png", _storageDir.c_str(), _counter );
  string fileName( buf );
  return fileName;
}


/******************************/
/*** get Odometry File Name ***/
string
DataReader::getOdometryFileName()
{
  static char buf[2048];

  sprintf( buf, "%s/odometry-%06d.png", _storageDir.c_str(), _counter );
  string fileName( buf );
  return fileName;
}

