/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */

#include "xratslam/localview/VisualTemplateSet.hh"
#include "xratslam/localview/VisualTemplate.hh"

#include <fstream>
#include <iostream>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/*************************** VisualTemplateSet ***************************/

/*******************/
/*** constructor ***/

VisualTemplateSet::VisualTemplateSet( int reserveSize )
{
  _maxVtId = -1;
  _set.reserve( reserveSize );
}


/******************/
/*** destructor ***/

VisualTemplateSet::~VisualTemplateSet()
{
  clear();
}


/*************/
/*** begin ***/
VisualTemplateSet::iterator
VisualTemplateSet::begin()
{
  iterator it = _set.rbegin();
  while ( (*it)-> getId() > _maxVtId )
    it++;
  return it;
}


/***********/
/*** add ***/
void
VisualTemplateSet::add( VisualTemplate *vt )
{
  _set.push_back( vt );
  _maxVtId = vt->getId();
}


/************/
/*** save ***/
int
VisualTemplateSet::save( string dir, int lastSavedId )
{
  // Note that template ID reflects its position on '_set' vector.
  //
  char vtName[2048];
  for ( int i = lastSavedId + 1; i < _set.size(); i++ )
  {
    VisualTemplate *vt = _set[i];
    sprintf( vtName, "%s/vt-%06d.txt", dir.c_str(), vt->getId() );
    vt->save( vtName );
  }

  return _set.size() - 1;
}


/************/
/*** load ***/
int
VisualTemplateSet::load( string dir, int maxVtId )
{
  // Find position of 'maxVtId' element in '_set'.
  int vtId = -1;
  int i = 0;
  for ( ; i < _set.size(); i++ )
  {
    vtId = _set[i]->getId();
    if ( vtId > maxVtId )
      break;
  }

  // Resize '_set' to 'i' elements.
  if ( i < _set.size() )
  {
    for ( int j = i; j < _set.size(); j++ )
      delete _set[j];
    _set.resize( i );

    return _maxVtId = maxVtId;
  }

  // This code make sense only when reading stored experiments,
  // not the current one!
  char vtName[2048];
  VisualTemplate *vt = 0;
  while ( vtId < maxVtId )
  {
    // If could not load VT.
    sprintf( vtName, "%s/vt-%06d.txt", dir.c_str(), ++vtId );
    if ( ! (vt = VisualTemplate::load( vtName )) )
    {
      vtId--;
      break;
    }
    _set.push_back( vt );
  }

  return _maxVtId = (vt ? vt->getId() : -1);
}


/*************/
/*** clear ***/
void
VisualTemplateSet::clear()
{
  for ( VisualTemplate *vt : _set )
    delete vt;
}
