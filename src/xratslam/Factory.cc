/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/Factory.hh>

#include "xratslam/LocalView.hh"
#include "xratslam/PoseCell.hh"
#include "xratslam/ExperienceMap.hh"

// Default implementations.
#include "xratslam/localview/OriginalLV.hh"
#include "xratslam/posecell/OriginalPC.hh"
#include "xratslam/experiencemap/OriginalEM.hh"

// Other implementations.
#include "xratslam/localview/RecodedLV.hh"
#include "xratslam/posecell/RecodedPC.hh"
#include "xratslam/experiencemap/RecodedEM.hh"

#include "xratslam/posecell/NoCanPC.hh"


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************** Factory ********************************/

/*****************/
/*** create LV ***/
LocalView *
Factory::createLV( string lvName, string configFile )
{
  if ( lvName.empty() || lvName == "OriginalLV" )
    return new xratslam::OriginalLV( configFile );

  if ( lvName == "RecodedLV" )
    return new xratslam::RecodedLV( configFile );

  return 0;
}


/*****************/
/*** create PC ***/
PoseCell *
Factory::createPC( string pcName, string configFile )
{
  if ( pcName.empty() || pcName == "OriginalPC" )
    return new xratslam::OriginalPC( configFile );

  if ( pcName == "RecodedPC" )
    return new xratslam::RecodedPC( configFile );

  if ( pcName == "NoCanPC" )
    return new xratslam::NoCanPC( configFile );

  return 0;
}


/*****************/
/*** create EM ***/
ExperienceMap *
Factory::createEM( string emName, string configFile )
{
  if ( emName.empty() || emName == "OriginalEM" )
    return new xratslam::OriginalEM( configFile );

  if ( emName == "RecodedEM" )
    return new xratslam::RecodedEM( configFile );
  
  return 0;
}
