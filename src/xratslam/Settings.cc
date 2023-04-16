/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 31/05/2019 (dd/mm/yyyy)
 *
 */

#include "xratslam/Settings.hh"
#include "xratslam/Utils.hh"

#include <boost/property_tree/xml_parser.hpp>


using namespace std;
using namespace xratslam;


/*************************************************************************/
/******************************* Settings ********************************/

Settings *Settings::_myself = 0;


/***********/
/*** get ***/
Settings *
Settings::get()
{
  return (_myself ? _myself : _myself = new Settings());
}


/******************/
/*** destructor ***/

Settings::~Settings()
{
}


/******************/
/*** set Source ***/
void
Settings::setSource( string fileName )
{
  _configFile = fileName;
  load();
}


/************/
/*** load ***/
void
Settings::load()
{
  // Read ratslam settings from 'configFile'.
  Utils::PTree ptree;
  Utils::getChildFromFile( ptree, _configFile, "logger" );
  //  Utils::getChildFromFile( ptree, _configFile, "" );

  string dBase = "./";
  string dLV   = "LocalView.log";
  string dPC   = "PoseCell.log";
  string dEM   = "ExperienceMap.log";
  string dExec = "Execution.log";
  
  // Read ratslam local view settings.
  Utils::getFromPtree( logBaseDir  , ptree, "log_base_dir"      , dBase );
  Utils::getFromPtree( logLV       , ptree, "log_local_view"    , dLV   );
  Utils::getFromPtree( logPC       , ptree, "log_pose_cell"     , dPC   );
  Utils::getFromPtree( logEM       , ptree, "log_experience_map", dEM   );
  Utils::getFromPtree( logExecution, ptree, "log_execution"     , dExec );

  cout <<"Settings::load():"<< endl;
  cout
    << "  baseDir:  " << logBaseDir   << endl
    << "  lv:       " << logLV        << endl
    << "  pc:       " << logPC        << endl 
    << "  em:       " << logEM        << endl  
    << "  execution:" << logExecution << endl
    << endl
    << endl;
  
}


/************/
/*** save ***/
void
Settings::save()
{
  Utils::PTree ptree;

  ptree.put( "logger.log_base_dir"      , logBaseDir   );
  ptree.put( "logger.log_local_view"    , logLV        );
  ptree.put( "logger.log_pose_cell"     , logPC        );
  ptree.put( "logger.log_experience_map", logEM        );
  ptree.put( "logger.log_execution"     , logExecution );
  
  boost::property_tree::write_xml( _configFile, ptree );
}


/*******************/
/*** constructor ***/

Settings::Settings()
{
  // Todo: set configuration file default value to user home directory
  // for unix and windows.
  _configFile = "./xratslam.config";
}


