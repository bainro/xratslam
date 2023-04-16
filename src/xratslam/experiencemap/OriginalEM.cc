/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include "xratslam/experiencemap/OriginalEM.hh"

#include "ratslam/experience_map.h"
#include "xratslam/Odometry.hh"
#include "xratslam/Action.hh"
#include "xratslam/Utils.hh"


using namespace xratslam;
using namespace std;


/*************************************************************************/
/****************************** OriginalEM *******************************/

/*******************/
/*** constructor ***/

OriginalEM::OriginalEM( string configFile )
  : ExperienceMap( "OriginalEM" )
{
  // Read ratslam settings from 'configFile'.
  Utils::PTree settings;
  Utils::getChildFromFile( settings, configFile, "ratslam" );
  
  _originalEM = new ratslam::ExperienceMap( settings );
}


/******************/
/*** destructor ***/

OriginalEM::~OriginalEM()
{
  delete _originalEM;
}


/******************/
/*** get ExpSet ***/
void
OriginalEM::getExpSet( ExperienceSet &expSet )
{
  int numExp = _originalEM->get_num_experiences();

  // Zero expSet.
  expSet.clear();

  // Recreate all experiences in new format.
  expSet.resize( numExp );
  for ( int i = 0; i < numExp; i++ )
  {
    ratslam::Experience *oExp = _originalEM->get_experience( i );
    
    Experience *nExp = new Experience();
    nExp->id         = oExp->id;
    nExp->x          = oExp->x_m;
    nExp->y          = oExp->y_m;
    nExp->th         = oExp->th_rad;

    //    nExp->links_from = oExp->links_from;
    for ( unsigned int link : oExp->links_from )
      nExp->links_from.push_back( (int) link );

    //    nExp->links_to = oExp->links_to;
    for ( unsigned int link : oExp->links_to )
      nExp->links_to.push_back( (int) link );
    
    expSet[i] = nExp;
  }
}


/********************/
/*** get Link Set ***/
void
OriginalEM::getLinkSet( ExpLinkSet &linkSet )
{
  int numLink = _originalEM->get_num_links();

  // Zero linkSet.
  linkSet.clear();

  // Recreate all experience links in the new format.
  linkSet.resize( numLink );
  for ( int i = 0; i < numLink; i++ )
  {
    ratslam::Link *oLink = _originalEM->get_link( i );
    
    ExpLink *nLink = new ExpLink();
    nLink->_dPos     = oLink->d;
    nLink->_heading  = oLink->heading_rad;
    nLink->_facing   = oLink->facing_rad;
    nLink->_expIdDst = oLink->exp_to_id;
    nLink->_expIdSrc = oLink->exp_from_id;
    nLink->_dTime    = oLink->delta_time_s;
    linkSet[i] = nLink;
  }
}


/******************/
/*** get Exp Id ***/
int
OriginalEM::getExpId()
{
  return _originalEM->get_current_id();
}


/************************/
/*** get Relative Rad ***/
double
OriginalEM::getRelativeRad()
{
  return _originalEM->get_relative_rad();
}


/***************/
/*** on Feed ***/
void
OriginalEM::onFeed( Odometry *odo, Action *action )
{
  // Todo: check if should continue or not when "odo->dTime <= 0".
  
  // Odometry related.
  //
  if ( odo->dTime > 0 )
    _originalEM->on_odo( odo->dPosition, odo->dAngle, odo->dTime );

  
  // Action related.
  //
  
  // Shortcuts.
  uint   srcId  = action->srcId;
  uint   dstId  = action->dstId;
  double radius = action->relativeRadius;

  switch ( action->type )
  {
  case Action::CREATE_NODE:
    _originalEM->on_create_experience( dstId );
    _originalEM->on_set_experience( dstId, 0 );
    break;

  case Action::CREATE_EDGE:
    _originalEM->on_create_link( srcId, dstId, radius );
    _originalEM->on_set_experience( dstId, radius );
    break;

  case Action::SET_NODE:
    _originalEM->on_set_experience( dstId, radius );
    break;

  case Action::NO_ACTION:
    break;
  }
  _originalEM->iterate();
}

