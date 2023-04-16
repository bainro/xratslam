/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Read slam data from a directory in a predefined format:
 * (format)
 * 
 */
#ifndef _xratslam_DataReaderVideo_
#define _xratslam_DataReaderVideo_

#include <xratslam/DataReader.hh>
#include <fstream>
#include <string>

namespace cv      { class VideoCapture;   }
namespace ratslam { class VisualOdometry; }


/*************************************************************************/
/**************************** DataReaderVideo ****************************/

namespace xratslam
{
  class Image;
  class Odometry;

  
  /*******************/
  class DataReaderVideo
    : public DataReader
  {
  public:

    /** Input data are read from a video file.
     * Images are taken from the video, frame by frame.
     * Odometries are built by visual odometry algorithm.
     * @param configFile : ratslam configuration file.
     * @param videoFile  : video file.
     */
    DataReaderVideo( std::string videoFile );
    ~DataReaderVideo();


    /** Read next image.
     * Return null if could not read.
     */
    Image *readNextImage( int seq );

    
  protected:

    int               _frameCounter;
    std::string       _videoFile;
    cv::VideoCapture *_videoCapture;

    ratslam::VisualOdometry *_visualOdometry;
  };
}

#endif
