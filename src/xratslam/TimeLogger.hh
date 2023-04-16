/*
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Log time intervals.
 *
 */
#ifndef _xratslam_TimeLogger_
#define _xratslam_TimeLogger_

#include <xratslam/Chronometer.hh>
#include <fstream>
#include <string>


/*************************************************************************/
/****************************** TimeLogger *******************************/

namespace xratslam
{
  /**************/
  class TimeLogger
  {
  public:

    TimeLogger( std::string logFile );
    ~TimeLogger();

    /** Set output directory for new instances.
     * The default directory is ".".
     */
    static bool setDir( std::string dir );
    
    /** Start timer.
     */
    void start();

    /** Stop timer and log result to file.
     */
    void stop();

    /** Log last time measure to destination file.
     * Log: [count, time].
     */
    void log();

    /** Log current time average to destination file.
     * Log: [count, average-time].
     */
    void logAverage();

    
    /** Return the time average of all 'start/stop' calls.
     */
    double average();
    
  
  private:

    std::ofstream _log;
    Chronometer   _chrono;
    
    double _time;       // Time of the last 'start/stop' event.
    int    _count;      // Number of calls to 'stop()'.
    double _totalTime;  // Total accumulated time.

    static std::string _dir;
  };
}

#endif
