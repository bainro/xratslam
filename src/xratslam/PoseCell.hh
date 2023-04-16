/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Base class for all PoseCell implementations.
 */
#ifndef _xratslam_PoseCell_
#define _xratslam_PoseCell_


#include <xratslam/Action.hh>
#include <string>


/*************************************************************************/
/******************************* PoseCell ********************************/

namespace xratslam
{
  class Odometry;;
  class Template;


  /************/
  class PoseCell
  {
  public:

    PoseCell( std::string name )
    {
      _expId = -1;
      _name  = name;
    };

    virtual ~PoseCell()  {}

    
    std::string getName()  { return _name; }
    
    /** Feeds PoseCell with odometry and visual template.
     * Fills 'actionType', 'dstExpId' and 'relativeRadius'.
     */
    void feed( Odometry *odometry, Template *vTemplate,
               Action::Type *actionType, int *dstExpId, double *relRad )
    {
      onFeed( odometry, vTemplate, actionType, dstExpId, relRad );
      _expId = *dstExpId;
    }

    /** Return the ID of the last activated experience.
     */
    int getExpId()
    { return _expId; }


    /** Save current state.
     * @param confF   : configuration file.
     * @param dataF   : data file.
     * @param counter: current iteration counter.
     * Note that '_expId' must be saved.
     */
    virtual bool save( std::string confF, std::string dataF, int counter )
    { return false; }
    
    /** Load state from filesystem.
     * @param confF   : configuration file.
     * @param dataF   : data file.
     * @param counter: current iteration counter.
     * Note that '_expId' must be loaded.
     */
    virtual bool load( std::string confF, std::string dataF, int counter )
    { return false; }
    

  protected:

   int _expId; // Current experience ID.

    
  private:

    /** Compute template influence and set current experience and
     * relative radius.
     */
    virtual void onFeed( Odometry *odometry, Template *vTemplate,
                         Action::Type *actionType, int *dstExpId,
                         double *relRad ) =0;


    std::string _name;  // PC implementation name.
  };
}

#endif
