/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Wrapper class to plug original PoseCell ratslam implementation
 * into xRatSlam framework.
 */
#ifndef _xratslam_OriginalPC_
#define _xratslam_OriginalPC_


#include <xratslam/PoseCell.hh>
#include <string>


namespace ratslam { class PosecellNetwork; }


/*************************************************************************/
/****************************** OriginalPC *******************************/

namespace xratslam
{
  /**************/
  class OriginalPC
    : public xratslam::PoseCell
  {
  public:

    OriginalPC( std::string configFile );
    ~OriginalPC();


  private:

    /** See: xratslam::PoseCell base class.
     */
    void onFeed( Odometry *odometry, Template *vTemplate,
                 Action::Type *actionType, int *dstExpId, double *relRad
                 ) override;

    
    ratslam::PosecellNetwork *_originalPC;
  };
}

#endif

