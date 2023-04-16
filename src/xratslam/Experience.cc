/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/Experience.hh>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/****************************** Experience *******************************/

/*******************/
/*** constructor ***/

Experience::Experience( Experience &exp )
  : links_from( exp.links_from ),
    links_to  ( exp.links_to   )
{
  id = exp.id;
  x  = exp.x;
  y  = exp.y;
  th = exp.th;
}


/************/
/*** save ***/
void
Experience::save( ostream &os )
{
  os
    << id << " "
    << x  << " "
    << y  << " "
    << th << " "
    ;

  saveLinkSet( os, links_from );
  saveLinkSet( os, links_to   );
}


/************/
/*** load ***/
Experience *
Experience::load( istream &is )
{
  Experience *exp = new Experience();

  try
  {
    is >> exp->id;
    is >> exp->x;
    is >> exp->y;
    is >> exp->th;

    exp->loadLinkSet( is, exp->links_from );
    exp->loadLinkSet( is, exp->links_to   );
  }
  catch( std::exception ex )
  {
    delete exp;
    exp = 0;
  }

  return exp;
}


/*********************/
/*** save Link Set ***/
void
Experience::saveLinkSet( ostream &os, vector<int> &linkSet )
{
  os << linkSet.size() << " ";
  for ( int linkId : linkSet )
    os << linkId << " ";
}


/*********************/
/*** load Link Set ***/
void
Experience::loadLinkSet( istream &is, vector<int> &linkSet )
{
  int numLink;
  is >> numLink;

  int linkId = 0;
  for ( int i = 0; i < numLink; i++ )
  {
    is >> linkId;
    linkSet.push_back( linkId );
  }
}


