/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/Image.hh>
#include <xratslam/Log.hh>

#include <opencv2/highgui/highgui.hpp>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/********************************* Image *********************************/

/**************/
/*** create ***/
Image *
Image::create( string fileName, int seq )
{
  // Todo: review how image is being read!!
  //

  cv::Mat frame = cv::imread( fileName );
  if ( frame.empty()  )
    return 0;

  /*
  // Debug:
  cv::imshow( "RatSlam Image", frame );
  */
  
  int  width  = frame.cols;
  int  height = frame.rows;
  bool isGrey = false;       // BGR OpenCV default format (!?).

  Image *image = new Image( seq, isGrey, width, height );

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


/*******************/
/*** constructor ***/

Image::Image()
{
  seq  = 0;
  time = 0.0;
  data = 0;
}

Image::Image( Image &image )
{
  seq    = image.seq;
  time   = image.time;
  isGrey = image.isGrey;
  width  = image.width;
  height = image.height;
  
  // Copy image data.
  int size = height * width;
  if ( ! isGrey ) size *= 3;
  data = new uchar[size];
  memcpy( data, image.data, size );
}

Image::Image( int s, bool ig, int w, int h )
{
  seq    = s;
  time   = 0.0;
  isGrey = ig;
  width  = w;
  height = h;
  data   = 0;
}


/******************/
/*** destructor ***/

Image::~Image()
{
  if ( data ) delete data;
}


/****************/
/*** set Data ***/
void
Image::setData( uchar *d )
{
  if ( data ) delete data;
  data = d;
}


/*************/
/*** write ***/
bool
Image::write( string fileName )
{
  cv::Mat mat( height, width, CV_8UC3, data );
  
  if ( cv::imwrite( fileName, mat ) )
    return true;

  // Error:
  stringstream msg;
  msg << "Image::write() - ERROR: failed to save the image: "
      << fileName;
  Log::message( msg.str() );
  
  return false;
}


/************/
/*** show ***/
bool
Image::show( string cvWindow, int mSeconds )
{
  // TODO: review.
  
  static cv::Mat cvImage;

  cvImage = cv::Mat( height, width, CV_8UC3, data );
  
  cv::imshow( cvWindow, cvImage );

  // This is needed to show image using OpenCV framework.
  // Wait for 1ms for some key stroke!
  cv::waitKey( mSeconds );

  return true;
}


