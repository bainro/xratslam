/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A RatSlam PoseCell with no pose cells nor pose state!
 *
 */
#ifndef _xratslam_NoCanPC_
#define _xratslam_NoCanPC_


#include <xratslam/PoseCell.hh>
#include <vector>
#include <string>


/*************************************************************************/
/******************************** NoCanPC ********************************/

namespace xratslam
{
  /***********/
  class NoCanPC
    : public xratslam::PoseCell
  {
  public:

    NoCanPC( std::string configFile );
    ~NoCanPC();


  private:

    /** See: xratslam::PoseCell base class.
     */
    void onFeed( Odometry *odometry, Template *vTemplate,
                 Action::Type *actionType, int *dstExpId, double *relRad
                 ) override;

    /** Find the action to be taken.
     * Set '_expId' with the ID of the activated experience.
     * If the current template has no associated experience, a new one
     * is created and activated (return Action::CREATE_NODE).
     * Otherwise its associated experience is activated
     * (return Action::CREATE_EDGE).
     */
    Action::Type getAction();
    

    // The i-th template is associated to experience '_templateExp[i]'.
    std::vector<int> _templateExp;
    int _vtId;      // Current  VT. 
    int _prevVtId;  // Previous VT.

    // Related to: ExperienceMap module:
    int _numExp;  // Number of created PcExperience's.
    int _expId;
  };
}

#endif

