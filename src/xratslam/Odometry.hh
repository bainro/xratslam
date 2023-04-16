/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * RatSlam internal odometry class.
 */

#ifndef _xratslam_Odometry_
#define _xratslam_Odometry_

#include <string>


/*************************************************************************/
/******************************* Odometry ********************************/

namespace xratslam
{
  /************/
  class Odometry
  {
    typedef unsigned char uchar;

    
  public:
    
    /** Create Odometry from file.
     */
    static Odometry *create( std::string fileName, int seq = 0 );
    
    Odometry ();
    ~Odometry()  {}

    
    /** Write odometry to a file.
     */
    bool write( std::string fileName );

    
    int    seq;
    double dTime;
    double dPosition;
    double dAngle;
  };
}

#endif

