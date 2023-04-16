/*
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Provide static functions for logging.
 */

#ifndef _xratslam_Log_
#define _xratslam_Log_

#include <string>


/*************************************************************************/
/********************************** Log **********************************/

namespace xratslam
{
  /*******/
  class Log
  {
  public:

    typedef unsigned char uchar;

    
  public:

    Log()  {}
    ~Log() {}

  
    static void setDir( std::string dir );
  
    static bool image( int seq, uchar *data, bool isGrey,
                       int width, int height );
  
    static bool odometry( int seq, double dTime, double vPos, double vAng );

    static bool message( std::string msg );
    static bool message( int id, std::string msg );
    
  
  private:

    static std::string fileName( int seq, std::string prefix );
  
    static std::string _dir;
  };
}

#endif
