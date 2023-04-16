/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * RatSlam internal image class.
 */

#ifndef _xratslam_Image_
#define _xratslam_Image_

#include <string>


/*************************************************************************/
/********************************* Image *********************************/

namespace xratslam
{
  /*********/
  class Image
  {
    typedef unsigned char uchar;
    typedef unsigned int  uint;

    
  public:

    /** Create image from file.
     * Todo: remove OpenCV dependency from implementation.
     */
    static Image *create( std::string fileName, int seq =0 );
    
    Image();
    Image( Image &image );
    Image( int seq, bool isGrey, int width, int height );
    ~Image();

    
    void setData( uchar *d );

    /** Write image to file.
     * Uses OpenCV to write image.
     * Image format is determined by 'fileName' extension.
     */
    bool write( std::string fileName );

    /** Show image in to openCV named window "cvWindow".
     */
    bool show( std::string cvWindow, int mSeconds =4000 );

    
    int    seq;
    double time;   // In seconds.
    uchar *data;
    bool   isGrey;
    uint   width;
    uint   height;
  };
}

#endif

