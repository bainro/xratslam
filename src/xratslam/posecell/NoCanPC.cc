/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include "xratslam/posecell/NoCanPC.hh"
#include "xratslam/Odometry.hh"
#include "xratslam/Template.hh"
//#include "xratslam/Utils.hh"


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************** NoCanPC ********************************/

/*******************/
/*** constructor ***/

NoCanPC::NoCanPC( string configFile )
  : PoseCell( "NoCanPC" )
{
  _numExp = 0;
  _expId  = 0;
  _vtId   = 0;
}


/******************/
/*** destructor ***/

NoCanPC::~NoCanPC()
{
}


/***************/
/*** on Feed ***/
void
NoCanPC::onFeed( Odometry *odometry, Template *templ,
                 Action::Type *actionType, int *dstExpId,
                 double *relRad )
{
  // Fixed.
  *relRad = 0;

  // Find the action to be taken by ExperienceMap module based on the
  // last activated template (_vtId).
  // Also set '_expId' with the activated experience.
  *actionType = getAction();
  *dstExpId   = _expId;
  
  // If it is a new VisualTemplate, created it.
  int templateId = templ->id;
  if ( templateId >= _templateExp.size() )
  {
    _templateExp.push_back( -1 );
    templateId = _templateExp.size() - 1;
  }
  
  _prevVtId = _vtId;
  _vtId     = templateId;
}


/******************/
/*** get Action ***/
Action::Type
NoCanPC::getAction()
{
  // If there is no template, or
  // If current template is the same as the previous one,
  // Do nothing.
  //
  if ( _templateExp.empty() || (_vtId == _prevVtId) )
  {
    return Action::NO_ACTION;
  }

  // If the current VisualTemplate has no associated experience,
  // create a new experience.
  if ( _templateExp[_vtId] < 0 )
  {
    _templateExp[_vtId] = _expId = _numExp++;      
    return Action::CREATE_NODE;
  }

  // If activated template has some associated experience,
  // create a link to it.
  _expId = _templateExp[_vtId];
  return Action::CREATE_EDGE;
}
