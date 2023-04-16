/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * RatSlam internal action class.
 */

#ifndef _xratslam_Action_
#define _xratslam_Action_


/*************************************************************************/
/******************************** Action *********************************/

namespace xratslam
{
  /************/
  class Action
  {
  public:

    typedef enum {
      NO_ACTION = 0,
      CREATE_NODE,
      CREATE_EDGE,
      SET_NODE       // It seems this is not used!
    } Type;
    

    Action()  { seq = 0; type = NO_ACTION; }
    
    Type   type;
    int    seq;
    int    srcId;
    int    dstId;
    double relativeRadius;
    double timeStamp;
  };
}

#endif

