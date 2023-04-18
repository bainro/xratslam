/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/XRatSlam.hh>

#include "xratslam/Template.hh"
#include "xratslam/Action.hh"
#include "xratslam/TimeLogger.hh"
#include "xratslam/Queue.hh"
#include "xratslam/LocalView.hh"
#include "xratslam/PoseCell.hh"
#include "xratslam/ExperienceMap.hh"
#include "xratslam/DataEM.hh"
#include "xratslam/Factory.hh"
#include "xratslam/Helper.hh"

// Default implementations.
#include "xratslam/localview/OriginalLV.hh"
#include "xratslam/posecell/OriginalPC.hh"
#include "xratslam/experiencemap/OriginalEM.hh"

// Used to read EM data from files: method RecodedEM::readEMFromFile()
// Todo: remove load/save DataEM from RecodedEM class.
#include "xratslam/experiencemap/RecodedEM.hh"

#include "ratslam/posecell_network.h"
#include "ratslam/experience_map.h"


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************* XRatSlam ********************************/

/*******************/
/*** constructor ***/

XRatSlam::XRatSlam( string configFile, int queueSize )
{
  _isRunning     = false;
  _experimentDir = "./";
  
  _queueImageLV    = new Queue<Image>   ( queueSize );
  _queueTemplatePC = new Queue<Template>( queueSize );
  _queueOdometryPC = new Queue<Odometry>( queueSize );
  _queueOdometryEM = new Queue<Odometry>( queueSize );
  _queueActionEM   = new Queue<Action>  ( queueSize );
  
  // Initialize ratslam internal structures.
  _moduleLV = new OriginalLV( configFile );
  _modulePC = new OriginalPC( configFile );
  _moduleEM = new OriginalEM( configFile );

  _loggerLV = 0;
  _loggerPC = 0;
  _loggerEM = 0;
}


/******************/
/*** destructor ***/

XRatSlam::~XRatSlam()
{
  stop();
  
  delete _moduleLV;
  delete _modulePC;
  delete _moduleEM;

  delete _queueImageLV   ;
  delete _queueTemplatePC;
  delete _queueOdometryPC;
  delete _queueOdometryEM;
  delete _queueActionEM  ;
}


/*************************/
/*** read EM From File ***/
DataEM *
XRatSlam::readEMFromFile( string fileName )
{
  return RecodedEM::readEMFromFile( fileName );
}


/**************/
/*** set LV ***/
bool
XRatSlam::setLV( string name, string configFile )
{
  LocalView *lv = Factory::createLV( name, configFile );
  if ( ! lv ) return false;
  
  lock_guard<mutex> lck( _mutexLV );
  delete _moduleLV;
  _moduleLV = lv;
  return true;
}


/**************/
/*** set PC ***/
bool
XRatSlam::setPC( string name, string configFile )
{
  PoseCell *pc = Factory::createPC( name, configFile );
  if ( ! pc ) return false;

  lock_guard<mutex> lck( _mutexPC );
  delete _modulePC;
  _modulePC = pc;
  return true;
}


/**************/
/*** set EM ***/
bool
XRatSlam::setEM( string name, string configFile )
{
  ExperienceMap *em = Factory::createEM( name, configFile );
  if ( ! em ) return false;

  lock_guard<mutex> lck( _mutexEM );
  delete _moduleEM;
  _moduleEM = em;
  return true;
}


/*******************/
/*** get Name LV ***/
string
XRatSlam::getNameLV()
{
  return _moduleLV->getName();
}


/*******************/
/*** get Name PC ***/
string
XRatSlam::getNamePC()
{
  return _modulePC->getName();
}


/*******************/
/*** get Name EM ***/
string
XRatSlam::getNameEM()
{
  return _moduleEM->getName();
}


/*************/
/*** start ***/
bool
XRatSlam::start()
{
  if ( _isRunning ) return false;

  _queueImageLV   ->restoreBlock();
  _queueTemplatePC->restoreBlock();
  _queueOdometryPC->restoreBlock();
  _queueOdometryEM->restoreBlock();
  _queueActionEM  ->restoreBlock();
  
  _isRunning = true;
  
  _threadLV = thread( &XRatSlam::loopLV, ref(*this) );
  _threadPC = thread( &XRatSlam::loopPC, ref(*this) );
  _threadEM = thread( &XRatSlam::loopEM, ref(*this) );
  
  return true;
}


/************/
/*** stop ***/
bool
XRatSlam::stop()
{
  // Note that it is assumed that:
  // 1) LV depends on input data only;
  // 2) PC depends on input data and LV ouput;
  // 3) EM depends on input data, LV and PC outputs;
  //
  
  if ( ! _isRunning ) return false;

  _isRunning = false;

  // Wait for LocalView thread loop to consume all queued data
  // and the thread to finish.
  _queueImageLV->unblock();
  _threadLV.join();

  // Wait for PoseCell thread loop to consume all queued data
  // and the thread to finish.
  _queueTemplatePC->unblock();
  _queueOdometryPC->unblock();
  _threadPC.join();

  // Wait for Experience thread loop to consume all queued data
  // and the thread to finish.
  _queueOdometryEM->unblock();
  _queueActionEM  ->unblock();
  _threadEM.join();
  
  return true;
}


/****************/
/*** has Data ***/
bool
XRatSlam::hasData()
{
  return !
    (
     _queueActionEM  ->empty() &&
     _queueTemplatePC->empty() &&
     _queueOdometryPC->empty() &&
     _queueOdometryEM->empty() &&
     _queueImageLV   ->empty()
     );
}


/************/
/*** feed ***/
bool
XRatSlam::feed( Image *image, Odometry *odometry )
{
  // Does not feed if not running.
  if ( ! _isRunning )
  {
    delete image;
    delete odometry;
    return false;
  }
  
  _queueImageLV   ->push( image    );
  _queueOdometryPC->push( new Odometry( *odometry ) );
  _queueOdometryEM->push( odometry );
  
  return true;
}


/*******************/
/*** get Data EM ***/
DataEM *
XRatSlam::getDataEM()
{
  DataEM *dataEM = new DataEM;

  lock_guard<mutex> lck( _mutexEM );
  _moduleEM->getExpSet ( dataEM->expSet  );
  _moduleEM->getLinkSet( dataEM->linkSet );
  dataEM->expId  = _moduleEM->getExpId();
  dataEM->relRad = _moduleEM->getRelativeRad();

  return dataEM;
}


/**************************/
/*** set Experiment Dir ***/
bool
XRatSlam::setExperimentDir( std::string experimentDir )
{
  // Create experiment output directory.
  // Note: continue even if there is some error as the directory already
  // exists.
  Helper::mkDir( _experimentDir = experimentDir );  
  return true;
}


/******************/
/*** save State ***/
bool
XRatSlam::saveState( int iteration )
{
  return saveState( iteration, true, true, true );
}


/******************/
/*** save State ***/
bool
XRatSlam::saveState( int iteration, bool lv, bool pc, bool em )
{
  // If there is nothing to save.
  if ( ! (lv || pc || em) )
    return true;
  
  // Create iteration output directory.
  char itDir[2048];
  if ( iteration >= 0 )
    sprintf( itDir, "%s/t-%06d/", _experimentDir.c_str(), iteration );
  else
    sprintf( itDir, "%s", _experimentDir.c_str() );    
  string iterationDir( itDir );
  Helper::mkDir( iterationDir );

  // cout <<"XRatSlam::saveState() - saving modules to "
  //      << iterationDir << endl;

  // Stop all modules threads.
  stop();

  // Save some extra information in a "info.txt" file.
  saveInfo( iterationDir );
  
  // Save modules state.
  if ( lv ) saveStateLV( _experimentDir, iterationDir, iteration );
  if ( pc ) saveStatePC( _experimentDir, iterationDir, iteration );
  if ( em ) saveStateEM( _experimentDir, iterationDir, iteration );

  // Restart all threads.
  start();

  return true;
}


/******************/
/*** load State ***/
bool
XRatSlam::loadState( int iteration )
{
  // Iteration directory.
  char itDir[2048];
  sprintf( itDir, "%s/t-%06d/", _experimentDir.c_str(), iteration );
  string iterationDir( itDir );

  cout <<"XRatSlam::loadState() - loading modules from "
       << iterationDir << endl;

  // Stop all modules threads.
  stop();

  // Load modules state.
  loadStateLV( _experimentDir, iterationDir, iteration );
  loadStatePC( iterationDir, iteration );
  loadStateEM( iterationDir, iteration );

  // Restart all threads.
  start();
  
  return true;
}


/***************/
/*** loop LV ***/
void
XRatSlam::loopLV()
{
  // Consumes: ratslam::Image    from external input.
  // Produces: ratslam::Template to   loopTemplatePC().
  //

  Image *image;
  while ( _isRunning || ! _queueImageLV->empty() )
  {
    // Does not read image while it is being copied by 'getImage()'.
    if ( ! (image = _queueImageLV->pop()) ) break;

    _mutexLV.lock();
    startLog( _loggerLV );

    // LocalView image processing.
    _moduleLV->feed( image );
    uint   vtId   = _moduleLV->getVisualTemplateId();
    double relRad = _moduleLV->getRelativeRadius();

    stopLog( _loggerLV );
    _mutexLV.unlock();

    // Construct template object with visual template ID and feed
    // its queue (for PoseCell).
    _queueTemplatePC->push( new Template( image->seq, vtId, relRad ) );

    image->data = 0;  // '_moduleLV' deals with allocated image data.
    delete image;
  }
}


/***************/
/*** loop PC ***/
void
XRatSlam::loopPC()
{
  // Consumes: ratslam::Odometry from external input.
  // Produces: ratslam::Action   to   loopActionEM().
  //

  Odometry *odometry;
  Template *vTemplate;
  while ( _isRunning ||
          ( ! _queueOdometryPC->empty() && ! _queueTemplatePC->empty() )
          )
  {
    if ( ! (odometry  = _queueOdometryPC->pop()) ) break;
    if ( ! (vTemplate = _queueTemplatePC->pop()) )
    {
      delete odometry;
      break;
    }
    
    _mutexPC.lock();
    startLog( _loggerPC );

    // Action resulting from PoseCell template processing.
    Action *action = new Action();
    action->srcId  = _modulePC->getExpId();
    action->seq    = vTemplate->seq;

    _modulePC->feed( odometry, vTemplate, &action->type,
                     &action->dstId, &action->relativeRadius );

    stopLog( _loggerPC );
    _mutexPC.unlock();

    // Send 'action' to ExperienceMap.
    _queueActionEM->push( action );

    delete odometry;
    delete vTemplate;
  }
}


/***************/
/*** loop EM ***/
void
XRatSlam::loopEM()
{
  // Consumes: ratslam::Odometry from external input.
  // Produces: Nothing.
  //

  Odometry *odometry;
  Action   *action;
  while ( _isRunning ||
          ( ! _queueOdometryEM->empty() && ! _queueActionEM->empty() )
          )
  {
    if ( ! (odometry = _queueOdometryEM->pop()) )  break;
    if ( ! (action   = _queueActionEM  ->pop()) )
    {
      delete odometry;
      break;
    }

    _mutexEM.lock();
    startLog( _loggerEM );

    // ExperienceMap odometry processing.
    _moduleEM->feed( odometry, action );

    stopLog( _loggerEM );
    _mutexEM.unlock();

    delete odometry;
    delete action;
  }
}


/*****************/
/*** start Log ***/
void
XRatSlam::startLog( TimeLogger *logger )
{
  if ( logger )
    logger->start();
}


/****************/
/*** stop Log ***/
void
XRatSlam::stopLog( TimeLogger *logger )
{
  if ( logger )
  {
    logger->stop();
    logger->logAverage();
  }
}

/*
bool
XRatSlam::saveInfo( string dir )
{
  // Get LocalView number of elements.
  _mutexLV.lock();
  int sizeLV = _moduleLV->getSize();
  _mutexLV.unlock();
  
  // Get ExperienceMap number of elements.
  _mutexEM.lock();
  int sizeEM = _moduleEM->getSize();
  _mutexEM.unlock();
  

  // Open output file.
  ofstream file{ dir + "info.txt" };
  if ( ! file.is_open() )
    return false;

  // Extra infos to be saved.
  file
    << "LocalViewSize     = " << sizeLV << endl
    << "ExperienceMapSize = " << sizeEM << endl
    ;
  file.close();

  return true;
}
*/

/*********************/
/*** save State LV ***/
bool
XRatSlam::saveStateLV( string expDir, string itDir, int it )
{
  string moduleName = getNameLV();
  
  // Create data directory.
  string dataDir = expDir + "/" + moduleName + "/";
  Helper::mkDir( dataDir );

  // Configuration file.
  string configFile = itDir + "lv-config.txt";

  // Save module state.
  lock_guard<mutex> lck( _mutexLV );
  _moduleLV->save( dataDir, configFile, it );

  return true;
}


/*********************/
/*** save State PC ***/
bool
XRatSlam::saveStatePC( string expDir, string itDir, int it )
{
  // Configuration and data files.
  string moduleName = getNamePC();
  string configFile = itDir + "pc-config.txt";
  string dataFile   = itDir + "pc-data.txt";

  // Save module state.
  lock_guard<mutex> lck( _mutexPC );
  _modulePC->save( configFile, dataFile, it );

  return true;
}


/*********************/
/*** save State EM ***/
bool
XRatSlam::saveStateEM( string expDir, string itDir, int it )
{
  // ExperienceMap configuration file.
  string moduleName = getNameEM();
  string configFile = itDir + "em-config.txt";
  string dataFile   = itDir + "em-data.txt";

  // cout <<"XRatSlam::saveStateEM(): configFile: "<< configFile << endl;
  // cout <<"XRatSlam::saveStateEM(): dataFile  : "<< dataFile   << endl;

  // Save EM state.
  lock_guard<mutex> lck( _mutexEM );
  _moduleEM->save( configFile, dataFile, it );

  return true;
}


/*********************/
/*** load State LV ***/
bool
XRatSlam::loadStateLV( string expDir, string itDir, int it )
{
  // Configuration file and data directory.
  string configFile = itDir + "lv-config.txt";
  string dataDir    = expDir + "/" + getNameLV() + "/";

  // Load LV object.
  lock_guard<mutex> lck( _mutexLV );
  _moduleLV->load( dataDir, configFile, it );

  return true;
}


/*********************/
/*** load State PC ***/
bool
XRatSlam::loadStatePC( string itDir, int it )
{
  // Configuration and data files.
  string moduleName = getNamePC();
  string configFile = itDir + "pc-config.txt";
  string dataFile   = itDir + "pc-data.txt";

  // Load PC object.
  lock_guard<mutex> lck( _mutexPC );
  _modulePC->load( configFile, dataFile, it );

  return true;
}


/*********************/
/*** load State EM ***/
bool
XRatSlam::loadStateEM( string itDir, int it )
{
  // Configuration and data files.
  string moduleName = getNameEM();
  string configFile = itDir + "em-config.txt";
  string dataFile   = itDir + "em-data.txt";

  // Load EM object.
  lock_guard<mutex> lck( _mutexEM );
  _moduleEM->load( configFile, dataFile, it );

  return true;
}
