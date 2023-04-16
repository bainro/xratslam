/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * The Link structure describes a link between two experiences.
 *
 */
#ifndef _xratslam_ExpLink_
#define _xratslam_ExpLink_

#include <iostream>


/*************************************************************************/
/******************************** ExpLink ********************************/

namespace xratslam
{
  /***********/
  class ExpLink
  {
  public:

    typedef unsigned int uint;
    

  public:

    ExpLink()  {}
    ~ExpLink() {}

    
    /** Save this object to 'os'.
     */
    void save( std::ostream &os );

    /** Load and create a new object from 'is'.
     */
    static ExpLink *load( std::istream &is );
    
    
    int    _expIdSrc;  // Source experience ID.      (exp_from_id).
    int    _expIdDst;  // Destination experience ID. (exp_to_id)
    double _dPos;      // Position variation, in metres.
    double _heading;   // In radians.
    double _facing;    // In radians.
    double _dTime;     // Time variation, in seconds.
  };
}

#endif


