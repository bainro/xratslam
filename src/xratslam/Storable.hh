/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 20/06/2019 (dd/mm/yyyy)
 *
 * Base class for structures capable to store its internal state.
 */
#ifndef _xratslam_Storable_
#define _xratslam_Storable_

#include <string>


/*************************************************************************/
/******************************* Storable ********************************/

namespace xratslam
{
  /************/
  class Storable
  {
  public:

    virtual ~Storable()  {}
    
    virtual bool save( std::string path ) =0;
    virtual bool load( std::string path ) =0;

    
  protected:

    Storable()  {}
  };
}

#endif
