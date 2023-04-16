/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Base class for all LocalView implementations.
 */
#ifndef _xratslam_LocalView_
#define _xratslam_LocalView_


#include <string>


/*************************************************************************/
/******************************* LocalView *******************************/

namespace xratslam
{
  class Image;

  
  /*************/
  class LocalView
  {
  public:

    typedef unsigned int uint;

    
  public:

    LocalView( std::string name )  { _name = name; };
    
    virtual ~LocalView()  {}


    /** LocalView implementation name.
     */
    std::string getName()  { return _name; }
    
    /** Return the current visual template identifier.
     */
    uint getVisualTemplateId()  { return _templId; }
    
    /** Return the current relative radius.
     */
    double getRelativeRadius()  { return _relRadius; }

    /** Feeds LocalView with given image.
     */
    void feed( Image *image )
    { onFeed( image, &_templId, &_relRadius ); }
    
    /** Save current state.
     * @param dir    : base directory.
     * @param conf   : configuration file.
     * @param counter: current iteration counter.
     * Note that '_templId' and '_relRadius' must be saved.
     */
    virtual bool save( std::string dir, std::string conf, int counter )
    { return false; }
    
    /** Load state from filesystem.
     * @param dir    : base directory.
     * @param conf   : configuration file.
     * @param counter: current iteration counter.
     * Note that '_templId' and '_relRadius' must be loaded.
     */
    virtual bool load( std::string dir, std::string conf, int counter )
    { return false; }

    /** Return the number of visual elements.
     */
    virtual int getSize()  { return 0; }


  protected:

    uint   _templId;    // Store data from last onFee() call.
    double _relRadius;  // Store data from last onFee() call.

    
  private:

    /** Should find 'visualTemplateId' and 'relativeRadius' based
     * on given 'image'.
     */
    virtual void onFeed( Image  *image,
                         uint   *visualTemplateId,
                         double *relativeRadius ) =0;

    
    // /** Should save templates to the given directory.
    //  * Note that '_lastSavedId' can be used to not save templates
    //  * already stored.
    //  * This method should return the ID of the last saved template,
    //  * so it can be used by future calls.
    //  */
    // virtual int saveTemplateSet( std::string dir, int iteration ) =0;

    // /** Should read templates from 'dir' directory with all templates up
    //  * to 'iteration'.
    //  */
    // virtual int loadTemplateSet( std::string dir, int iteration ) =0;

    
    std::string _name;  // LV implementation name.    
  };
}

#endif

