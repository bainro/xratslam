/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Interface for classes responsible to generate odometry entries.
 * 
 */
#ifndef _xratslam_OdometryReader_
#define _xratslam_OdometryReader_

#include <xratslam/Odometry.hh>
#include <fstream>
#include <string>


/*************************************************************************/
/**************************** OdometryReader *****************************/

namespace xratslam
{
  class Odometry;
  class Image;
  
  
  /******************/
  class OdometryReader
  {
  public:

    OdometryReader()             {}
    virtual ~OdometryReader()    {}


    /** Return true if it is ready to read Odometry data.
     */
    virtual bool isOpened() =0;

    /** Reset odometry source.
     */
    virtual void reset() =0;
    
    /** Read next odometry value.
     * If 'image' is not null it can be used to find the odometry.
     * Return null if there is no more data or in case of error.
     */
    virtual Odometry *getNext( Image *image=0 ) =0;
  };
}

#endif
