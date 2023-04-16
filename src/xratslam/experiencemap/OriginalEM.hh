/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Wrapper class to plug original ExperienceMap ratslam implementation
 * into xRatSlam framework.
 */
#ifndef _xratslam_OriginalEM_
#define _xratslam_OriginalEM_


#include <xratslam/ExperienceMap.hh>

namespace ratslam { class ExperienceMap; }


/*************************************************************************/
/****************************** OriginalEM *******************************/

namespace xratslam
{
  /**************/
  class OriginalEM
    : public xratslam::ExperienceMap
  {
  public:

    OriginalEM( std::string configFile );
    ~OriginalEM();


    // See: xratslam::ExperienceMap.
    void   getExpSet     ( ExperienceSet &expSet  ) override;
    void   getLinkSet    ( ExpLinkSet    &linkSet ) override;
    int    getExpId      ()                         override;
    double getRelativeRad()                         override;

    
  private:

    // See: xratslam::ExperienceMap.
    void onFeed( Odometry *odometry, Action *action ) override;


    ratslam::ExperienceMap *_originalEM;
  };
}

#endif

