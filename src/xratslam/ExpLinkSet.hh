/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A set of pointers to ExpLink objects.
 *
 */
#ifndef _xratslam_ExpLinkSet_
#define _xratslam_ExpLinkSet_

#include <vector>
#include <iostream>


/*************************************************************************/
/****************************** ExpLinkSet *******************************/

namespace xratslam
{
  class ExpLink;

  
  /**************/
  class ExpLinkSet
    : public std::vector<ExpLink*>
  {
  public:

    ExpLinkSet()   {}
    ExpLinkSet( ExpLinkSet &set );
    ~ExpLinkSet();

    
    /** Clear this set and fill it with copies of 'expSet' elements.
     */
    void copyFrom( ExpLinkSet &linkSet );
    
    /** Removes all elements from the set (which are destroyed),
     * leaving the container with a size of 0.
     */
    void clear();

    /** Deallocates all elements form the set, leaving the container
     * with the same size.
     */
    void destroy();

    /** Save all links to 'os'.
     */
    bool save( std::ostream &os );

    /** Load links from 'is'.
     */
    bool load( std::istream &is );
  };
}

#endif

    
