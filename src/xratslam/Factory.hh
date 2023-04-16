/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Factory for LocalView, PoseCell and ExperienceMap implementations.
 * Note that any class derived from LV, PC or EM need to be coded
 * in this class to be used by the framework.
 * Todo: think an other way to do this.
 */
#ifndef _xratslam_Factory_
#define _xratslam_Factory_


#include <string>


/*************************************************************************/
/******************************** Factory ********************************/

namespace xratslam
{
  class LocalView;
  class PoseCell;
  class ExperienceMap;

  
  /***********/
  class Factory
  {
  public:

    Factory()  {}
    ~Factory() {}

    /** Instatiate and return a LocalView object associated with the
     * given 'name'.
     * If name == "", returns default implementation.
     * Return 0 if could not find refered implementation.
     */
    static LocalView *
    createLV( std::string name, std::string configFile );
    
    /** Instatiate and return a PoseCell object associated with the
     * given 'name'.
     * If name == "", returns default implementation.
     * Return 0 if could not find refered implementation.
     */
    static PoseCell *
    createPC( std::string name, std::string configFile );

    /** Instatiate and return a ExperienceMap object associated with the
     * given 'name'.
     * If name == "", returns default implementation.
     * Return 0 if could not find refered implementation.
     */
    static ExperienceMap *
    createEM( std::string name, std::string configFile );
  };
}

#endif
