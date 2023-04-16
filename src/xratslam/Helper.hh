/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * General helper functions.
 * 
 */
#ifndef _xratslam_Helper_
#define _xratslam_Helper_


#include <string>


/*************************************************************************/
/******************************** Helper *********************************/

namespace xratslam
{
  /**********/
  class Helper
  {
  public:

    /** Create a directory including any non existing directory in the
     * path.
     * Similar to "mkdir -p" in bash.
     */
    static bool mkDir( std::string path );

    /** Remove directory and its contents.
     * Similar to "rmdir -r" in bash.
     */
    static bool rmDir( std::string path );
  };
}

#endif

