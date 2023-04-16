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
#ifndef _xratslam_DataReader_
#define _xratslam_DataReader_

#include <string>


/*************************************************************************/
/****************************** DataReader *******************************/

namespace xratslam
{
  class Image;
  class Odometry;
  class OdometryReader;
  
  
  /**************/
  class DataReader
  {
  public:

    /** Odometry should be set by setOdometryVisual() or setOdometryFile()
     * methods.
     */
    DataReader();
    virtual ~DataReader();

    /** Extract odometry based on incoming images.
     * Default behaviour.
     */
    bool setOdometryVisual( std::string configFile );

    /** Odometries are read from file.
     */
    bool setOdometryFile( std::string odometryFile );

    /** Return true if data source is connected.
     * Should be call obly after calling setOdometry<XX>() methods.
     */
    bool isOpened();

    /** Enable input data storage.
     * If enabled each time a new data (image, odometry) is read it is
     * stored at 'storageDir'.
     */
    bool setStorageDir( std::string storageDir );
    
    /** Read next couple of (image, odometry).
     * Return false if there is no more records or in case of error.
     */
    bool readNext( Image **image, Odometry **odometry );

    /** DataReader reads new input data.
     * If storage is set, DataReader stores all data read.
     * If so, one can set DataReader to go back in time calling this
     * method with a counter smaller than the current DataReader counter.
     * Note that 'counter' refers to the number of input data read.
     * Return true if 'newCounter' is smaller than the number of data
     * already read.
     */
    bool changeCounter( int newCounter );
    
    
  protected:

    /** Should read next input 'image'.
     * If needed, can use 'seq' as the image sequence number.
     * Return null if can not.
     */
    virtual Image *readNextImage( int seq=0 ) =0;


    OdometryReader *_odometryReader;

    
  private:
    
    /** Write a backup of input data to '_storageDir' using '_counter'.
     */
    bool saveData( Image *image, Odometry *odometry );

    /** Read data from '_storageDir' using '_counter'.
     */
    bool loadData( Image **image, Odometry **odometry );

    
    /** Return current image file name using '_storageDir' and '_counter'.
     */
    std::string getImageFileName();
    
    /** Return current odometry file name using '_storageDir' and
     * '_counter'.
     */
    std::string getOdometryFileName();
    
    
    std::string _storageDir;
    
    int  _counter;    // Current input counter.
    int  _totalRead;  // Number of input already read.
    bool _isVisual;   // True if visual odometry is been used.

    // Todo: review.
    int _imageSeq;  // Image sequency number for visual odometry.
  };
}

#endif
