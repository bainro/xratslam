/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/ExpLink.hh>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************** ExpLink ********************************/

/************/
/*** save ***/
void
ExpLink::save( ostream &os )
{
  os
    << _expIdSrc << " "
    << _expIdDst << " "
    << _dPos     << " "
    << _heading  << " "
    << _facing   << " "
    << _dTime    << " "
    ;
}


/************/
/*** load ***/
ExpLink *
ExpLink::load( istream &is )
{
  ExpLink *link = new ExpLink();
  
  is >> link->_expIdSrc;
  is >> link->_expIdDst;
  is >> link->_dPos    ;
  is >> link->_heading ;
  is >> link->_facing  ;
  is >> link->_dTime   ;

  return link;
}
