/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A chronometer.
 * 
 */
#ifndef _xratslam_Chronometer_
#define _xratslam_Chronometer_

#include <chrono>


/*************************************************************************/
/****************************** Chronometer ******************************/

namespace xratslam
{
  /***************/
  class Chronometer
  {
  public:

    Chronometer() : _begin( Clock::now() )  {}

    void reset()  { _begin = Clock::now(); }

    double elapsed() const
    { 
      return
        std::chrono::duration_cast<Second>
        (Clock::now() - _begin).count();
    }


private:
    
    typedef std::chrono::high_resolution_clock           Clock;
    typedef std::chrono::duration<double, std::ratio<1>> Second;
    
    std::chrono::time_point<Clock> _begin;
  };
}

#endif
