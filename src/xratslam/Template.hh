/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Stores only template data needed for communication between RatSlam
 * modules.
 * Note that LocalView structure stores all templates.
 * Other ratslam modules use just references (ID) to those templates.
 */

#ifndef _xratslam_Template_
#define _xratslam_Template_


/*************************************************************************/
/******************************* Template ********************************/

namespace xratslam
{
  /************/
  class Template
  {
    typedef unsigned int uint;

    
  public:
    
    Template()                           { seq = 0; }
    Template( int s, uint i, double r )  { seq = s; id = i; radius = r; }
      
    int    seq;
    uint   id;
    double radius;
  };
}

#endif

