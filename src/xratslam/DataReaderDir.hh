/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Read slam data [Odometry, Image] from a directory in a predefined
 * format.
 * 
 */
#ifndef _xratslam_DataReaderDir_
#define _xratslam_DataReaderDir_

#include <xratslam/DataReader.hh>
#include <string>


/*************************************************************************/
/***************************** DataReaderDir *****************************/

namespace xratslam
{
  class OdometryReader;

  
  /*****************/
  class DataReaderDir
    : public DataReader
  {
  public:

    DataReaderDir( std::string dir );
    ~DataReaderDir();


  private:

    /** Read next couple of (image, odometry).
     * Return false if there is no more records or in case of error.
     */
    Image *readNextImage( int seq );

    
    std::string _dir;
  };
}

#endif
