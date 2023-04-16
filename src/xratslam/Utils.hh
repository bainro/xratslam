/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * A RatSlam ExperienceMap implementation.
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

#ifndef _xratslam_Utils_
#define _xratslam_Utils_

#include <boost/property_tree/ptree.hpp>

#include <iostream>
#include <string>


/*************************************************************************/
/********************************* Utils *********************************/

namespace xratslam
{
  /*********/
  class Utils
  {
  public:
    
    typedef boost::property_tree::ptree PTree;

    
  public:
    
    /** Helper to read child boost::property_tree::ptree from a file.
     */
    static bool getChildFromFile( PTree &child, std::string file,
                                  std::string name );

    /** Helper to read child PTree from a ptree.
     * Old: get_setting_child()
     */
    static bool getPtreeChild( PTree &child, PTree &settings,
                               std::string name );

    /** Helper to read data from PTree.
     * Old: get_setting_from_ptree()
     */
    template<typename T>
    static void getFromPtree( T &var, PTree &settings, std::string name,
                              T defaultValue )
    {
      try
      {
        var = settings.get<T>( name );
      }
      catch ( boost::property_tree::ptree_bad_path pbp )
      {
        var = defaultValue;
        /*
        std::cout << "SETTINGS(warning): "<< name
             <<" not found so default ("<< var << ") used."
             << std::endl;
        */
      }
    }

    /** Clip input angle to the range [0, 2PI].
     * Old: clip_rad_360()
     */
    static double clipTo360( double angle );

    /** Clip the input angle to the range: [-pi, +pi], in radians
     * Old: clip_rad_180()
     */
    static double clipTo180( double angle );
  
    /** Get the signed delta angle from angle1 to angle2 handling the
     * wrap from 2pi to 0.
     * Do: (angle1 - angle2).
     *
     * Old: get_signed_delta_rad()
     */
    static double angleDiff( double angle1, double angle2 );
  };
}

#endif
