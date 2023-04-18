/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/DataReaderVideo.hh>

#include "xratslam/Utils.hh"
#include <xratslam/Image.hh>
#include <xratslam/Odometry.hh>

#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <string.h>


using namespace xratslam;
using namespace std;
using namespace cv;


/*************************************************************************/
/**************************** DataReaderVideo ****************************/

/*******************/
/*** constructor ***/

DataReaderVideo::DataReaderVideo( string videoFile )
  : DataReader()
{
  _frameCounter = 0;
  _videoFile    = videoFile;

  // Open video file.
  _videoCapture = new cv::VideoCapture( _videoFile );

  // Debug.
  if ( ! _videoCapture->isOpened() )
  {
    cerr << "DataReaderVideo::DataReaderVideo() ERROR: ";
    cerr << "could not open video file: "<< _videoFile << endl;
  }
  //  cout <<"DataReaderVideo::setVideo() - videoFile: "<< _videoFile << endl;
}


/******************/
/*** destructor ***/

DataReaderVideo::~DataReaderVideo()
{
  if ( _videoCapture )
  {
    _videoCapture->release();
    delete _videoCapture;
  }
}


/***********************/
/*** read Next Image ***/
Image *
DataReaderVideo::readNextImage( int )
{
  if ( ! _videoCapture->isOpened() )
  {
    cerr << "DataReaderVideo::readNext() - NOT opened." << endl;
    return 0;
  }

  cv::Mat frame;
  *_videoCapture >> frame;
  if (frame.empty()) {
    cout << "end of video!" << endl;
    return 0;
  }

  int  width  = frame.cols;
  int  height = frame.rows;
  bool isGrey = false;      // BGR is the OpenCV default format (!?).

  Image *image = new Image( _frameCounter++, isGrey, width, height );

  // Time in seconds.
  image->time = 0.001 * _videoCapture->get( CAP_PROP_POS_MSEC );
    
  // Copy OpenCV read image.
  // This is needed because OpenCV does not its image data let go.
  // It destroys image data with cv::Mat object, always!
  // Even when we use: "frame.data = 0"!
  // So, we need to make a copy of the image data :(
  //
  int imgSize = 3 * width * height;
  image->data = new uchar[imgSize];
  memcpy( image->data, frame.data, imgSize );

  return image;
}
