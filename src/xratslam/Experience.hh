/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * The Experience structure describes a node in the Experience Map.
 *
 */
#ifndef _xratslam_Experience_
#define _xratslam_Experience_

#include <vector>
#include <iostream>


/*************************************************************************/
/****************************** Experience *******************************/

namespace xratslam
{
  /**************/
  class Experience
  {
  public:

    Experience()  {}
    Experience( Experience &exp );
    ~Experience() {}

    
    /** Save this object to 'os'.
     */
    void save( std::ostream &os );

    /** Load and create a new object from 'ifs'.
     */
    static Experience *load( std::istream &ifs );
    

    int    id;  // its own id
    double x;   // Experience x position, in metres.
    double y;   // Experience y position, in metres.
    double th;  // Experience angular position (theta), in radians.

    std::vector<int> links_from; // links from others to this experience

    // Todo: check if this is really used.
    std::vector<int> links_to; // links to this experience


  private:

    /** Save 'linkSet' elements. First number is the linkSet size.
     */
    void saveLinkSet( std::ostream &os, std::vector<int> &linkSet );

    /** Loaded elements are appended to 'lingSet'.
     */
    void loadLinkSet( std::istream &is, std::vector<int> &linkSet );
  };
}

#endif

    
