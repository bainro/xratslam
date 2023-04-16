/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include "xratslam/posecell/OriginalPC.hh"
#include "xratslam/Odometry.hh"
#include "xratslam/Template.hh"
#include "xratslam/Utils.hh"

#include "ratslam/posecell_network.h"


using namespace xratslam;
using namespace std;


/*************************************************************************/
/****************************** OriginalPC *******************************/

/*******************/
/*** constructor ***/

OriginalPC::OriginalPC( string configFile )
  : PoseCell( "OriginalPC" )
{
  // Read ratslam settings from 'configFile'.
  Utils::PTree rss;
  Utils::getChildFromFile( rss, configFile, "ratslam" );

  _originalPC = new ratslam::PosecellNetwork( rss );
}


/******************/
/*** destructor ***/

OriginalPC::~OriginalPC()
{
  delete _originalPC;
}


/***************/
/*** on Feed ***/
void
OriginalPC::onFeed( Odometry *odo, Template *templ,
                    Action::Type *actionType, int *dstExpId,
                    double *relRad )
{
  // Network iteration related.
  //
  _originalPC->on_odo( odo->dPosition, odo->dAngle, odo->dTime );
  *actionType = (Action::Type) _originalPC->get_action();

  // Template related.
  //
  _originalPC->on_view_template( templ->id, templ->radius );  
  *dstExpId = _originalPC->get_current_exp_id();
  *relRad   = _originalPC->get_relative_rad();
}
