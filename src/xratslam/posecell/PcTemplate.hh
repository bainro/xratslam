/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A Template stored in PoseCell class.
 *
 * This implementation is based on:
 *   Code:
 *     David Ball (david.ball@qut.edu.au)         (1),
 *     Scott Heath (scott.heath@uqconnect.edu.au) (2)
 *   RatSLAM algorithm by:
 *     Michael Milford (1)
 *     Gordon Wyeth    (1) ([michael.milford, gordon.wyeth]@qut.edu.au)
 *
 *   1. Queensland University of Technology, Australia
 *   2. The University of Queensland, Australia
 *
 * The original code is used as an inspiration to try some personal ideas.
 * And also to understand the original code.
 *
 */
#ifndef _xratslam_PcTemplate_
#define _xratslam_PcTemplate_


#include <vector>
#include <fstream>


/*************************************************************************/
/****************************** PcTemplate *******************************/

namespace xratslam
{
  /**************/
  class PcTemplate
  {
  public:

    PcTemplate() {}

    
    void clear()  { expSet.clear(); }
    
    bool save( std::ofstream &file );
    bool load( std::ifstream &file );
    
    
    std::vector<int> expSet;
  };
}

#endif

