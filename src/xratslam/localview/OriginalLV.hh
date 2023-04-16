/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Wrapper class to plug original LocalView ratslam implementation into
 * xRatSlam framework.
 */
#ifndef _xratslam_OriginalLV_
#define _xratslam_OriginalLV_


#include <xratslam/LocalView.hh>
#include <string>


// Original LocalView implementation.
namespace ratslam { class LocalViewMatch; }


/*************************************************************************/
/****************************** OriginalLV *******************************/

namespace xratslam
{
  /**************/
  class OriginalLV
    : public LocalView
  {
  public:

    OriginalLV( std::string configFile );
    ~OriginalLV();


  private:

    /** See: xratslam::LocalView base class.
     */
    void onFeed( Image *image, uint *vtId, double *relRad );


    ratslam::LocalViewMatch *_originalLV;
  };
}

#endif

