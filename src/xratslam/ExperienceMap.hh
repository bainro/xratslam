/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Base class for all ExperienceMap implementations.
 */
#ifndef _xratslam_ExperienceMap_
#define _xratslam_ExperienceMap_


#include <xratslam/Experience.hh>
#include <xratslam/ExperienceSet.hh>
#include <xratslam/ExpLink.hh>
#include <xratslam/ExpLinkSet.hh>

#include <string>
#include <iostream>


/*************************************************************************/
/***************************** ExperienceMap *****************************/

namespace xratslam
{
  class Odometry;
  class Action;
  
  /*****************/
  class ExperienceMap
  {
  public:

    ExperienceMap( std::string name )  { _name = name; }
    virtual ~ExperienceMap()             {}

    
    /** Return the Experience Map implementation name.
     */
    std::string getName()  { return _name; }

    
    /** Feeds ExperienceMap with odometry and action.
     */
    void feed( Odometry *odometry, Action *action )
    { onFeed( odometry, action ); }


    // Methods used to draw the experience map.
    //

    /** Return the set of experiences.
     */
    virtual void getExpSet( ExperienceSet &expSet ) =0;

    /** Return the set of links.
     */
    virtual void getLinkSet( ExpLinkSet &linkSet ) =0;

    /** Return current experience identifier.
     */
    virtual int getExpId() =0;

    /** Return the relative angle, in radians.
     */
    virtual double getRelativeRad() =0;

    /** Return the number of visual elements.
     */
    virtual int getSize()  { return 0; }
    
    /** Save current state.
     * @param confF   : configuration file.
     * @param dataF   : data file.
     * @param counter: current iteration counter.
     */
    virtual bool save( std::string confF, std::string dataF, int counter )
    { return false; }
    
    /** Load state from filesystem.
     * @param confF   : configuration file.
     * @param dataF   : data file.
     * @param counter: current iteration counter.
     */
    virtual bool load( std::string confF, std::string dataF, int counter )
    { return false; }

    /** Print attributes and data.
     */
    virtual void print( std::ostream &ofs ) {}
    

  private:

    virtual void onFeed( Odometry *odometry, Action *action ) =0;

    
    std::string _name;  // EM implementation name.
  };
}

#endif
