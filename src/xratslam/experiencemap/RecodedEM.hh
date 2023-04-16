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

#ifndef _xratslam_RecodedEM_
#define _xratslam_RecodedEM_

#include "xratslam/DataEM.hh"
#include <xratslam/ExperienceMap.hh>


/*************************************************************************/
/******************************* RecodedEM *******************************/

namespace xratslam
{
  class DataEM;

  
  /*************/
  class RecodedEM
    : public ExperienceMap
  {
  public:

    RecodedEM( std::string configFile );
    ~RecodedEM()  {}


    void   getExpSet     ( ExperienceSet &expSet  ) override;
    void   getLinkSet    ( ExpLinkSet    &linkSet ) override;
    int    getExpId      ()                         override;
    double getRelativeRad()                         override;
    int    getSize()  override { return _expSet.size(); }

    
    bool save( std::string conf, std::string data, int counter ) override;
    bool load( std::string conf, std::string data, int counter ) override;

    void print( std::ostream &out ) override;

    /** Read DataEM from file.
     */
    static DataEM *readEMFromFile( std::string fileName );

    
  private:
    
    // See: xratslam::ExperienceMap.
    void onFeed( Odometry *odometry, Action *action ) override;

    /** Save parameters.
     */
    bool saveConfig( std::ostream &file );
    
    /** Load parameters.
     */
    bool loadConfig( std::istream &file );

   /** Save internal data.
    */
    bool saveData( std::ostream &file );
    
    /** Load internal data.
     */
    bool loadData( std::istream &file );


    /** Create a new experience.
     * It is the current experience added by accumulated pose values.
     * Link current to created experience.
     * Return the ID of the new experience.
     */
    int onCreateExperience();

    /** Change the current experience.
     */
    bool onSetExperience( int newExpId, double relRad );

    /** Create a new link from experience 'expIdSrc' to 'expIdDst' if
     * there is no link between them.
     * The link content is calculated based on the accumulated pose values.
     */
    bool onCreateLink( int expIdSrc, int expIdDst, double relRad );

    /** Update the current position of the experience map since the
     * last experience, based on odometry.
     * Obs: vPos (m/s), vAngle (rad/s), dTime (s).
     */
    void onOdometry( double vPos, double vAngle, double dTime );
    
    /** Iterate the experience map.
     * Perform a graph relaxing algorithm to allow the map to
     * partially converge.
     */
    bool iterate();

    
    double _expCorrection;     // Used at "iterate()".
    int    _expLoops;          // Used at "iterate()".
    double _expInitialEmDeg;   // Just set "_accumDAngle".

    ExperienceSet _expSet;
    ExpLinkSet    _linkSet;

    int _expId;      // Current experience identifier.
    int _prevExpId;

    double _accumDX;      // X position variation, in metres.
    double _accumDY;      // Y position variation, in metres.
    double _accumDAngle;  // Angular variation, in radians.
    double _accumDTime;   // Time variation, in seconds.

    double _relativeRad;
  };

}

#endif
