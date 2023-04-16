/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A RatSlam ExperienceMap implementation.
 *
 * This implementation is based on:
 *   Code:
 *     David Ball (david.ball@qut.edu.au)         (1),
 *     Scott Heath (scott.heath@uqconnect.edu.au) (2)
 *   RatSLAM algorithm by:
 *     Michael Milford (1)
 *     Gordon Wyeth    (1) ([michael.milford, gordon.wyeth]@qut.edu.au)
 *
 *   1. Queensland University of Technology, Australia
 *   2. The University of Queensland, Australia
 *
 * The original code is used as an inspiration to try some personal ideas.
 * And also to understand the original code.
 *
 */
#include "xratslam/experiencemap/RecodedEM.hh"

#include "xratslam/Odometry.hh"
#include "xratslam/Action.hh"
#include "xratslam/Utils.hh"
#include <math.h>
#include <fstream>

#include <unistd.h>  // Debug: exit().


using namespace std;
using namespace xratslam;


/*************************************************************************/
/******************************* RecodedEM *******************************/

/*******************/
/*** constructor ***/

RecodedEM::RecodedEM( std::string configFile )
  : ExperienceMap( "RecodedEM" )
{
  // Read ratslam settings from 'configFile'.
  Utils::PTree rts;
  Utils::getChildFromFile( rts, configFile, "ratslam" );
  
  Utils::getFromPtree( _expCorrection  , rts, "exp_correction"    , 0.5  );
  Utils::getFromPtree( _expLoops       , rts, "exp_loops"         , 10   );
  Utils::getFromPtree( _expInitialEmDeg, rts, "exp_initial_em_deg", 90.0 );

  _expSet .reserve( 10000 );
  _linkSet.reserve( 10000 );

  _expId     = 0;
  _prevExpId = 0;
  
  _accumDAngle = _expInitialEmDeg * M_PI/180;
  _accumDX     = 0;
  _accumDY     = 0;
  _accumDTime  = 0;

  _relativeRad = 0;
}


/*******************/
/*** get Exp Set ***/
void
RecodedEM::getExpSet( ExperienceSet &expSet )
{
  expSet.copyFrom( _expSet );
}


/********************/
/*** get Link Set ***/
void
RecodedEM::getLinkSet( ExpLinkSet &linkSet )
{
  linkSet.copyFrom( _linkSet );
}


/******************/
/*** get Exp Id ***/
int
RecodedEM::getExpId()
{
  return _expId;
}


/************************/
/*** get Relative Rad ***/
double
RecodedEM::getRelativeRad()
{
  return _relativeRad;
}


/************/
/*** save ***/
bool
RecodedEM::save( string confName, string dataName, int counter )
{
  // Save configuration (attributes).
  ofstream confFile{ confName };
  if ( ! confFile.is_open() || ! saveConfig( confFile ) )
    return false;
  confFile.close();

  ofstream dataFile( dataName );
  if ( ! dataFile.is_open() || ! saveData( dataFile ) )
    return false;
  dataFile.close();

  return true;
}


/************/
/*** load ***/
bool
RecodedEM::load( string confName, string dataName, int counter )
{
  ifstream confFile{ confName };
  if ( ! confFile.is_open() || ! loadConfig( confFile ) )
    return false;
  confFile.close();

  ifstream dataFile( dataName );
  if ( ! dataFile.is_open() || ! loadData( dataFile ) )
    return false;
  dataFile.close();

  return true;
}


/*************/
/*** print ***/
void
RecodedEM::print( ostream &out )
{
  saveConfig( out );
  saveData  ( out );
}


/*************************/
/*** read EM From File ***/
DataEM *
RecodedEM::readEMFromFile( string fileName )
{
  // Open input data file.
  ifstream file( fileName );
  if ( ! file.is_open() ) return 0;

  // Read experiences and links.
  DataEM *dataEM = new DataEM;
  if ( ! dataEM->expSet.load( file ) || ! dataEM->linkSet.load( file ) )
  {
    delete dataEM;
    dataEM = 0;
  }

  file.close();
  return dataEM;
}


/***************/
/*** on Feed ***/
void
RecodedEM::onFeed(  Odometry *odo, Action *action )
{
  // Todo: check if should continue or not when "odo->dTime <= 0".

  // Odometry related.
  //
  double dTime = odo->dTime;
  if ( dTime <= 0 ) return;
  
  double dPos   = odo->dPosition * dTime;
  double dAngle = odo->dAngle    * dTime;

  _accumDAngle  = Utils::clipTo180( _accumDAngle + dAngle );
  _accumDX      = _accumDX + dPos * cos( _accumDAngle );
  _accumDY      = _accumDY + dPos * sin( _accumDAngle );
  _accumDTime  += dTime;

  
  // Action related.
  //

  // Shortcuts.
  int    srcId  = action->srcId;
  int    dstId  = action->dstId;
  double radius = action->relativeRadius;

  /*
  cout <<"RecodedEM::onFeed() - p2."<< endl;
  cout <<"  action->type: "<< action->type << endl;
  cout <<"  srcId       : "<< srcId  << endl;
  cout <<"  dstId       : "<< dstId  << endl;
  cout <<"  radius      : "<< radius << endl;
  cout << endl;
  */

  // Debug.
  int numExp = _expSet.size();
  if ( srcId > numExp || dstId > numExp )
  {
    cerr<<"RecodedEM::onFeed() - ERROR. Probably PC has not been saved:"
        << endl
        <<"  srcId : "<< srcId  << endl
        <<"  dstId : "<< dstId  << endl
        <<"  numExp: "<< numExp << endl
      ;
    return;
  }

  
  switch ( action->type )
  {
  case Action::CREATE_NODE:
    onCreateExperience();
    onSetExperience( dstId, 0 );
    break;

  case Action::CREATE_EDGE:
    onCreateLink( srcId, dstId, radius );
    onSetExperience( dstId, radius );
    break;

  case Action::SET_NODE:
    onSetExperience( dstId, radius );
    break;

  case Action::NO_ACTION:
    break;
  }

  iterate();
}


/*******************/
/*** save Config ***/
bool
RecodedEM::saveConfig( ostream &file )
{
  file
    << "ExpCorrection   = " << _expCorrection   << endl
    << "ExpLoops        = " << _expLoops        << endl
    << "ExpInitialEmDeg = " << _expInitialEmDeg << endl
    << "ExpId           = " << _expId           << endl
    << "PrevExpId       = " << _prevExpId       << endl
    << "AccumDX         = " << _accumDX         << endl
    << "AccumDY         = " << _accumDY         << endl
    << "AccumDAngle     = " << _accumDAngle     << endl
    << "AccumDTime      = " << _accumDTime      << endl
    << "RelativeRad     = " << _relativeRad     << endl
    ;

  return true;
}


/*******************/
/*** load Config ***/
bool
RecodedEM::loadConfig( istream &file )
{
  // Used to read labels an 'equal' signs.
  string str;
  
  file >> str >> str; file >> _expCorrection  ;
  file >> str >> str; file >> _expLoops       ;
  file >> str >> str; file >> _expInitialEmDeg;
  file >> str >> str; file >> _expId          ;
  file >> str >> str; file >> _prevExpId      ;
  file >> str >> str; file >> _accumDX        ;
  file >> str >> str; file >> _accumDY        ;
  file >> str >> str; file >> _accumDAngle    ;
  file >> str >> str; file >> _accumDTime     ;
  file >> str >> str; file >> _relativeRad    ;

  return true;
}


/*****************/
/*** save Data ***/
bool
RecodedEM::saveData( ostream &file )
{
  return _expSet.save( file ) && _linkSet.save( file );
}


/*****************/
/*** load Data ***/
bool
RecodedEM::loadData( istream &file )
{
  return _expSet.load( file ) && _linkSet.load( file );
}


/****************************/
/*** on Create Experience ***/
int
RecodedEM::onCreateExperience()
{
  // Create new Experience.
  Experience *newExp = new Experience();
  _expSet.push_back( newExp );

  // Set new experience with current experience added by accumulated
  // pose values.
  newExp->id = _expSet.size() - 1;
  newExp->x  = _expSet[_expId]->x + _accumDX;
  newExp->y  = _expSet[_expId]->y + _accumDY;
  newExp->th = Utils::clipTo180( _accumDAngle );

  // Link new experience with the current one.
  if ( _expSet.size() != 1 )
    onCreateLink( _expId, newExp->id, 0 );

  return newExp->id;
}


/*************************/
/*** on Set Experience ***/
bool
RecodedEM::onSetExperience( int newExpId, double relRad )
{
  int numExp = _expSet.size();
  
  // If 'newExpId' does not exist.
  if ( newExpId >= numExp )
  {
    cerr <<"RecodedEM::onSetExperience() - ERROR: "
         <<"newExpId("<< newExpId <<") >= "
         <<"numExp("  << numExp   <<")" << endl;
    return false;
  }

  // If 'newExpId' is already set.
  if ( newExpId == _expId )
    return true;

  _prevExpId   = _expId;
  _expId       = newExpId;
  _accumDX     = 0;
  _accumDY     = 0;
  _accumDAngle = Utils::clipTo180( _expSet[_expId]->th + relRad );

  _relativeRad = relRad;

  return true;
}


/**********************/
/*** on Create Link ***/
bool
RecodedEM::onCreateLink( int expIdSrc, int expIdDst, double relRad )
{
  int numExp = _expSet.size();
  
  if ( expIdSrc >= numExp )
  {
    cerr <<"RecodedEM::onCreateLink() - ERROR: "
         <<"expIdSrc("<< expIdSrc <<") >= "
         <<"numExp("  << numExp   <<")" << endl;
    exit( -1 );
    return false;
  }

  if ( expIdDst >= numExp )
  {
    cerr <<"RecodedEM::onCreateLink() - ERROR: "
         <<"expIdDst("<< expIdDst <<") >= "
         <<"numExp("  << numExp   <<")" << endl;
    exit( -2 );
    return false;
  }
  
  
  // Check if there already exists a link from 'expIdSrc' to 'expIdDst'.
  vector<int> *linksFrom = &( _expSet[ expIdSrc ]->links_from );
  for ( int linkId : *linksFrom )
  {
    if ( _linkSet[ linkId ]->_expIdDst == expIdDst )
      return false;
  }
  
  // Check if there already exists a link from 'expIdDst' to 'expIdSrc'.
  linksFrom = &( _expSet[ expIdDst ]->links_from );
  for ( int linkId : *linksFrom )
  {
    if ( _linkSet[ linkId ]->_expIdDst == expIdSrc )
      return false;
  }

  ExpLink *newLink = new ExpLink();
  _linkSet.push_back( newLink );
  int newLinkId = _linkSet.size() - 1;  
  /*
  _linkSet.resize( _linkSet.size() + 1 );
  ExpLink *newLink = new ExpLink();
  int    newLinkId = _linkSet.size() - 1;
  _linkSet[ newLinkId ] = newLink;
  */
  
  // Target (current) experience.
  Experience *exp = _expSet[ expIdSrc ];

  // Find link content using the accumulated pose values.
  newLink->_expIdSrc = expIdSrc;
  newLink->_expIdDst = expIdDst;
  newLink->_dPos     = sqrt(_accumDX * _accumDX + _accumDY * _accumDY);
  newLink->_heading  =
    Utils::angleDiff( exp->th, atan2(_accumDY, _accumDX) );
  newLink->_facing =
    Utils::angleDiff( exp->th, Utils::clipTo180( _accumDAngle + relRad ) );
  newLink->_dTime = _accumDTime;

  // Let source and destination experiences know about the link.
  _expSet[expIdSrc]->links_from.push_back( newLinkId );
  _expSet[expIdDst]->links_to  .push_back( newLinkId );

  return true;
}


/***************/
/*** iterate ***/
bool
RecodedEM::iterate()
{
  for ( int i = 0; i < _expLoops; i++ )
  {
    // For each Experience:
    for ( Experience *expSrc : _expSet )
    {
      // For each 'links_from' of the experience:
      for ( int linkId : expSrc->links_from )
      {
        ExpLink *link = _linkSet[ linkId ];
        
        // The experience connected to 'expSrc' by 'link'.
        Experience *expDst = _expSet[ link->_expIdDst ];

        // Work out where e0 thinks e1 (x,y) should be based on the
        // stored link information
        double angH = expSrc->th + link->_heading;
        double lX   = expSrc->x  + link->_dPos * cos( angH );
        double lY   = expSrc->y  + link->_dPos * sin( angH );

        // Correct e0 and e1 (x,y) by equal but opposite amounts
        // a 0.5 correction parameter means that e0 and e1 will be fully
        // corrected based on e0's link information

        double dX = (expDst->x - lX) * _expCorrection;
        double dY = (expDst->y - lY) * _expCorrection;

        expSrc->x += dX;
        expSrc->y += dY;
        expDst->x -= dX;
        expDst->y -= dY;

        // Determine the angle between where e0 thinks e1's facing
        // should be based on the link information
        double angF = expSrc->th + link->_facing;
        double dF   = Utils::angleDiff( angF, expDst->th );

        // Correct e0 and e1 facing by equal but opposite amounts
        // a 0.5 correction parameter means that e0 and e1 will be fully
        // corrected based on e0's link information
        dF *= _expCorrection;
        expSrc->th = Utils::clipTo180( expSrc->th + dF );
        expDst->th = Utils::clipTo180( expDst->th - dF );
      }
    }
  }

  return true;
}

