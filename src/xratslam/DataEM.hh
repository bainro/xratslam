/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Necessary data to represent an experience map.
 *
 */
#ifndef _xratslam_DataEM_
#define _xratslam_DataEM_


#include <xratslam/ExperienceSet.hh>
#include <xratslam/ExpLinkSet.hh>


/*************************************************************************/
/******************************** DataEM *********************************/

namespace xratslam
{
  /**********/
  class DataEM
  {
  public:

    DataEM();
    ~DataEM();


    /** Find experience map limits.
     * Return false if all experiences in the map are null.
     */
    bool findLimits( float *minX, float *maxX, float *minY, float *maxY );


    ExperienceSet expSet;
    ExpLinkSet    linkSet;
    int           expId;    // ID of activated experience.
    double        relRad;
  };
}

#endif

    
