/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 * Wrapper class to access ratslam_ros original code without ROS.
 */
#ifndef _xratslam_XRatSlam_
#define _xratslam_XRatSlam_

// External xratslam accessed classes.
//
#include <xratslam/Image.hh>
#include <xratslam/Odometry.hh>
#include <xratslam/TimeLogger.hh>

#include <thread>
#include <mutex>
#include <vector>


/*************************************************************************/
/******************************* XRatSlam ********************************/

namespace xratslam
{
  template <class T> class Queue;
  class Action;
  class Template;
  class LocalView;
  class PoseCell;
  class ExperienceSet;
  class ExpLinkSet;
  class DataEM;
  class ExperienceMap;
  
  
  /************/
  class XRatSlam
  {
  public:

    XRatSlam( std::string configFile, int queueSize = 10 );
    ~XRatSlam();

    
    /** Read DataEM from file using default EM file format.
     */
    static DataEM *readEMFromFile( std::string fileName );

    // Change XRatSlam modules implementation.
    bool setLV( std::string name, std::string configFile );
    bool setPC( std::string name, std::string configFile );
    bool setEM( std::string name, std::string configFile );

    std::string getNameLV();
    std::string getNamePC();
    std::string getNameEM();
    
    bool start();
    bool stop();

    /** Return true if some message queue has some pending data.
     */
    bool hasData();
    
    /** Feed XRatSlam algorithm with an image and its related odometry.
     * These pointer are internally deallocated!
     */
    bool feed( Image *image, Odometry *odometry );

    /** Copy internal experience map data.
     * Used for drawing purposes.
     */
    DataEM *getDataEM();

    // Performance measurement related.
    // Loggers are external to this class, so they can live longer.
    //
    void setTimeLV( TimeLogger *logger )  { _loggerLV = logger; }
    void setTimePC( TimeLogger *logger )  { _loggerPC = logger; }
    void setTimeEM( TimeLogger *logger )  { _loggerEM = logger; }


    /** Set experiment directory.
     * All saveState() and loadState() calls will use this directory as
     * reference.
     * If 'experimentDir' does not exist it is created.
     * Defaults to "./".
     * Return false if it could not be created.
     */
    bool setExperimentDir( std::string experimentDir );
    
    /** Store current modules internal state structures.
     * States are stored to '_experimentDir' directory.
     * Return false in case of error.
     * TODO: before the state can be saved, all threads must to find
     * a steady state.
     */
    bool saveState( int iteration );

    /** Only save indicated modules.
     */
    bool saveState( int iteration, bool lv, bool pc, bool em );
    
    /** Load  modules state structures as they were at 'iteration' time.
     * States are read from '_experimentDir' directory.
     * Return false in case of error.
     * TODO: to load the state all communication queues must be considered.
     */
    bool loadState( int iteration );


  private:

    void loopLV();
    void loopPC();
    void loopEM();

    /** If 'logger' exists, start to count the time to log.
     */
    void startLog( TimeLogger *logger );

    /** If 'logger' exitss, stop to count the time and log it to the
     * logger specific file.
     */
    void stopLog( TimeLogger *logger );

    /** Save extra information to 'info.txt' file.
     */
    bool saveInfo( std::string dir );

    // Save modules state.
    // 'it' is the iteration (related time step).
    //
    bool saveStateLV( std::string expDir, std::string itDir, int it );
    bool saveStatePC( std::string expDir, std::string itDir, int it );
    bool saveStateEM( std::string expDir, std::string itDir, int it );
    
    // Load modules state.
    // 'it' is the iteration (related time step).
    //
    bool loadStateLV( std::string expDir, std::string itDir, int it );
    bool loadStatePC( std::string itDir, int it );
    bool loadStateEM( std::string itDir, int it );

    
    //========
    
    bool        _isRunning;
    std::string _experimentDir;

    // XRatSlam structures.
    xratslam::LocalView     *_moduleLV;
    xratslam::PoseCell      *_modulePC;
    xratslam::ExperienceMap *_moduleEM;

    // Mutex to access each module internal structure.
    std::mutex _mutexLV;
    std::mutex _mutexPC;
    std::mutex _mutexEM;

    // Threads for modules loop maintenance.
    std::thread _threadLV;
    std::thread _threadPC;
    std::thread _threadEM;

    // Queue to store external incoming images for LocalView.
    // Producer: external.
    // Consumer: loopLV().
    Queue<Image> *_queueImageLV;

    // Queue to store activated templates for PoseCell.
    // Producer: loopLV().
    // Consumer: loopPC().
    Queue<Template> *_queueTemplatePC;
    
    // Queue to store external incoming odometries for PoseCell.
    // Producer: external.
    // Consumer: loopPC().
    Queue<Odometry> *_queueOdometryPC;
    
    // Queue to store external incoming odometries for ExperienceMap.
    // Producer: external.
    // Consumer: loopEM().
    Queue<Odometry> *_queueOdometryEM;
    
    // Queue to store experience map actions for ExperienceMap.
    // Producer: loopPC().
    // Consumer: loopEM().
    Queue<Action> *_queueActionEM;

    // Performance measurement.
    TimeLogger *_loggerLV;
    TimeLogger *_loggerPC;
    TimeLogger *_loggerEM;
  };
}

#endif
