/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include "xratslam/posecell/PcTemplate.hh"

#include <iostream>


using namespace xratslam;
using namespace std;


/*************************************************************************/
/****************************** PcTemplate *******************************/

/************/
/*** save ***/
bool
PcTemplate::save( ofstream &file )
{
  // Number of associated experiences.
  file << expSet.size();

  // ID of each associated experience.
  for ( int expId : expSet )
    file << " " << expId;

  // One line for each experience set.
  file << endl;
  
  return true;
}


/************/
/*** load ***/
bool
PcTemplate::load( ifstream &file )
{
  expSet.clear();

  // Number of associated experiences.
  int numExp = 0;
  file >> numExp;
  if ( numExp <= 0 ) return false;

  // Read each experience ID.
  int expId;
  for ( int i = 0; i < numExp; i++ )
  {
    file >> expId;
    expSet.push_back( expId );
  }

  return true;
}
