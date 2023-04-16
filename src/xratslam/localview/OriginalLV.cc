/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include "xratslam/localview/OriginalLV.hh"
#include "xratslam/Image.hh"
#include "xratslam/Utils.hh"
#include "ratslam/local_view_match.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <fstream>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/****************************** OriginalLV *******************************/

/*******************/
/*** constructor ***/

OriginalLV::OriginalLV( string configFile )
  : LocalView( "OriginalLV" )
{
  // Read ratslam settings from 'configFile'.
  Utils::PTree rss;
  Utils::getChildFromFile( rss, configFile, "ratslam" );

  _originalLV = new ratslam::LocalViewMatch( rss );
}


/******************/
/*** destructor ***/

OriginalLV::~OriginalLV()
{
  delete _originalLV;
}


/***************/
/*** on Feed ***/
void
OriginalLV::onFeed( Image *img, uint *vtId, double *relRad )
{
  _originalLV->on_image( img->data, img->isGrey, img->width, img->height);

  *vtId   = _originalLV->get_current_vt();
  *relRad = _originalLV->get_relative_rad();
}

