/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Create odometry based on two consecutive images.
 * This class is a wrapper to Ball's VisualOdometry class implementation.
 * 
 */
#ifndef _xratslam_XVisualOdometry_
#define _xratslam_XVisualOdometry_

#include <xratslam/OdometryReader.hh>
#include <xratslam/Odometry.hh>
//#include <xratslam/Utils.hh>
#include <fstream>
#include <string>

namespace ratslam { class VisualOdometry; }


/*************************************************************************/
/**************************** XVisualOdometry ****************************/

namespace xratslam
{
  /*******************/
  class XVisualOdometry
    : public OdometryReader
  {
  public:

    XVisualOdometry( std::string configFile );
    ~XVisualOdometry();

    /** See base class.
     */
    bool isOpened()  { return true; }

    /** See base class.
     */
    void reset();
    
    /** See base class.
     */
    Odometry *getNext( Image *image );

    
  private:

    std::string              _configFile;
    ratslam::VisualOdometry *_visualOdometry;

    double _lastTime;  // In seconds.
  };
}

#endif
