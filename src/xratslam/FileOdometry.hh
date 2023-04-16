/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Read Odometry data from file.
 * 
 */
#ifndef _xratslam_FileOdometry_
#define _xratslam_FileOdometry_

#include <xratslam/OdometryReader.hh>
#include <xratslam/Odometry.hh>
#include <fstream>
#include <string>


/*************************************************************************/
/***************************** FileOdometry ******************************/

namespace xratslam
{
  /****************/
  class FileOdometry
    : public OdometryReader
  {
  public:

    FileOdometry( std::string fileName );
    ~FileOdometry();

    /** See base class.
     */
    bool isOpened()  { return _stream.good(); }

    /** See base class.
     */
    void reset();
    
    /** See base class.
     */
    Odometry *getNext( Image *image );

    
  private:

    std::string   _fileName;
    std::ifstream _stream;
  };
}

#endif
