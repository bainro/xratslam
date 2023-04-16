/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include "xratslam/Helper.hh"

#include <boost/filesystem.hpp>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************** Helper *********************************/

/**************/
/*** mk Dir ***/
bool
Helper::mkDir( string path )
{
  return boost::filesystem::create_directories( path );
}


/**************/
/*** rm Dir ***/
bool
Helper::rmDir( string path )
{
  return boost::filesystem::remove_all( path );
}
