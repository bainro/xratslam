/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include <xratslam/ExperienceSet.hh>
#include <xratslam/Experience.hh>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/***************************** ExperienceSet *****************************/

/*******************/
/*** constructor ***/

ExperienceSet::ExperienceSet( ExperienceSet &expSet )
{
  for ( Experience *exp : expSet )
    push_back( new Experience( *exp ) );
}


/******************/
/*** destructor ***/

ExperienceSet::~ExperienceSet()
{
  ExperienceSet::clear();
}


/*****************/
/*** copy From ***/
void
ExperienceSet::copyFrom( ExperienceSet &expSet )
{
  ExperienceSet::clear();
  
  for ( Experience *exp : expSet )
    push_back( new Experience( *exp ) );
}


/*************/
/*** clear ***/
void
ExperienceSet::clear()
{
  destroy();
  vector<Experience*>::clear();
}


/***************/
/*** destroy ***/
void
ExperienceSet::destroy()
{
  for ( Experience *exp : *this )
    delete exp;
}


/************/
/*** save ***/
bool
ExperienceSet::save( ostream &os )
{
  int numElem = size();
  os << "NumberOfExperiences = "<< numElem << endl;
  for ( Experience *exp : *this )
  {
    exp->save( os );
    os << endl;
  }
  return true;
}


/************/
/*** load ***/
bool
ExperienceSet::load( istream &is )
{
  // Number of elements, with label and '=' signal.
  string str;
  is >> str >> str;
  int numElem;
  is >> numElem;
  if ( numElem < 0 )
    return false;

  clear();

  Experience *exp;
  for ( int i = 0; i < numElem; i++ )
  {
    if ( (exp = Experience::load( is )) != 0 )
      push_back( exp );
  }

  return true;
}

