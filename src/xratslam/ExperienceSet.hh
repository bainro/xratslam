/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A set of pointers to Experience objects.
 *
 */
#ifndef _xratslam_ExperienceSet_
#define _xratslam_ExperienceSet_

#include <vector>
#include <iostream>


/*************************************************************************/
/***************************** ExperienceSet *****************************/

namespace xratslam
{
  class Experience;

  
  /*****************/
  class ExperienceSet
    : public std::vector<Experience*>
  {
  public:

    ExperienceSet()   {}
    ExperienceSet( ExperienceSet &expSet );
    ~ExperienceSet();

    
    /** Clear this set and fill it with copies of 'expSet' elements.
     */
    void copyFrom( ExperienceSet &expSet );
    
    /** Removes all elements from the vector (which are destroyed),
     * leaving the container with a size of 0.
     */
    void clear();

    /** Deallocates all elements form the set, leaving the container
     * with the same size.
     */
    void destroy();

    /** Save all experiencies to 'os'.
     */
    bool save( std::ostream &os );

    /** Load experiencies from 'is'.
     */
    bool load( std::istream &is );
  };
}

#endif

    
