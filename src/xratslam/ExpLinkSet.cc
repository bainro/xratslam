/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/ExpLinkSet.hh>
#include <xratslam/ExpLink.hh>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/****************************** ExpLinkSet *******************************/

/*******************/
/*** constructor ***/

ExpLinkSet::ExpLinkSet( ExpLinkSet &set )
{
  for ( ExpLink *exp : set )
    push_back( new ExpLink( *exp ) );
}


/******************/
/*** destructor ***/

ExpLinkSet::~ExpLinkSet()
{
  ExpLinkSet::clear();
}


/*****************/
/*** copy From ***/
void
ExpLinkSet::copyFrom( ExpLinkSet &linkSet )
{
  ExpLinkSet::clear();
  
  for ( ExpLink *link : linkSet )
    push_back( new ExpLink( *link ) );
}


/*************/
/*** clear ***/
void
ExpLinkSet::clear()
{
  destroy();
  vector<ExpLink*>::clear();
}


/***************/
/*** destroy ***/
void
ExpLinkSet::destroy()
{
  for ( ExpLink *exp : *this )
    delete exp;
}


/************/
/*** save ***/
bool
ExpLinkSet::save( ostream &os )
{
  os << "NumberOfLinks = "<< size() << endl;
  for ( ExpLink *link : *this )
  {
    link->save( os );
    os << endl;
  }
  return true;
}


/************/
/*** load ***/
bool
ExpLinkSet::load( istream &is )
{
  // Read label and signal.
  string str;
  is >> str;
  is >> str;

  // Number of elements.
  int numElem;
  is >> numElem;
  if ( numElem < 0 )
    return false;

  clear();

  ExpLink *link;
  for ( int i = 0; i < numElem; i++ )
  {
    if ( (link = ExpLink::load( is )) != 0 )
      push_back( link );
  }

  return true;
}

