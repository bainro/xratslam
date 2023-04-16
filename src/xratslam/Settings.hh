/*
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 31/05/2019 (dd/mm/yyyy)
 *
 * Singleton class to manage library settings.
 * 
 */

#ifndef _Settings_
#define _Settings_

#include <string>


/*************************************************************************/
/******************************* Settings ********************************/

namespace xratslam
{
  /************/
  class Settings
  {
  public:

    /** Return the singleton object.
     */
    static Settings *get();
  
    ~Settings();

  
    void setSource( std::string fileName );
  
    void load();
    void save();


    // Logger data
    //
    std::string logBaseDir  ;
    std::string logLV       ;
    std::string logPC       ;
    std::string logEM       ;
    std::string logExecution;

    
  private:

    Settings();

    
    static Settings *_myself;

    std::string _configFile;
  };
}


#endif
