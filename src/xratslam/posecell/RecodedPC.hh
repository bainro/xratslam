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
#ifndef _xratslam_RecodedPC_
#define _xratslam_RecodedPC_

#include <xratslam/PoseCell.hh>
#include <xratslam/Action.hh>
#include <xratslam/posecell/PcTemplate.hh>
#include <vector>
#include <string>
#include <math.h>   // M_PI.


/*************************************************************************/
/******************************* RecodedPC *******************************/

namespace xratslam
{
  /*************/
  class RecodedPC
    : public xratslam::PoseCell
  {
    typedef unsigned int uint;
    
    typedef float    Element;  // Cell element type.
    typedef Element  Cell;     // Cell element type.
    typedef Cell    *CellP;    // Pointer to Cell.   (Cell *  )
    typedef CellP   *CellPP;   // Pointer to CellP.  (Cell ** )
    typedef CellPP  *CellPPP;  // Pointer to CellPP. (Cell ***)

    
  public:

    RecodedPC( std::string configFile );
    ~RecodedPC();

    
    bool save( std::string conf, std::string data, int counter ) override;
    bool load( std::string conf, std::string data, int counter ) override;

    
  private:

    /** See: xratslam::PoseCell base class.
     */
    void onFeed( Odometry *odometry, Template *vTemplate,
                 Action::Type *actionType, int *dstExpId, double *relRad
                 ) override;

    /** Save parameters.
     */
    bool saveConfig( std::string configFile );
    
    /** Load parameters.
     */
    bool loadConfig( std::string configFile );

   /** Save internal data.
    */
    bool saveData( std::string dataFile );
    
    /** Load internal data.
     */
    bool loadData( std::string dataFile );


    /** Destroy current PoseCell network structures.
     * Todo: separate permanent data from PC dimension related data.
     * Obs: set '_dimXY' and '_dimZ' to -1.
     */
    void destroyPC();

    /** Build PoseCell network structures.
     * Use '_dimXY' and '_dimZ' as PC structure dimensions.
     * Todo: separate permanent data from PC dimension related data.
     */
    void constructPC();
    
    
    /** Access to exported data.
     */
    double getRelativeRad()
    { return _vtDeltaPcTh * 2.0 * M_PI / _dimZ; }

    /** Find the action to be taken.
     */
    Action::Type getAction();
    
    /** Treat a new perceived template.
     */
    void onViewTemplate( uint vtId, double vtRad );


    // Helpers.
    //

    /** Locally excite a pose cell element.
     * Excite spreads energy.
     */
    void excite();

    /** Locally inhibit a pose cell element.
     * Inhibit compresses energy.
     */
    void inhibit();

    /** Global inhibition.
     */
    void globalInhibit();

    /** Normalise all system energies.
     */
    void normalise();

    /** (pose_cell_excite_helper)
     */
    void exciteCell( int x, int y, int z );

    /** (pose_cell_inhibit_helper)
     */
    void inhibitCell( int x, int y, int z );

    /** Set indexes at wrap such that:
     * wrap = [start1, .. end1, start2, .., end2, start3, .., end3].
     * 
     * generate_wrap().
     */
    void generateWrap( int *wrap, int start1, int end1,
                       int start2, int end2, int start3, int end3 );

    /** Function to create local excitation and inhibition to the CAN
     * elements. It is used a gaussian function (mexican hat).
     */
    double norm2d( double var, int x, int y, int z, int dimCentre );

    
    bool _isUpdated;  // 'odo_update' and 'vt_update'.
                      // True if 'onTemplate()' was already called.
    
    // Network structure.
    //
    
    int _dimXY;     // Param. PC_DIM_XY
    int _dimZ;      // Param. PC_DIM_TH
    int _dimWE;     // Param. PC_W_E_DIM
    int _dimWI;     // Param. PC_W_I_DIM
    int _dimWEHalf; // PC_W_E_DIM_HALF;    Local?
    int _dimWIHalf; // PC_W_I_DIM_HALF;    Local?

    int _varWE;  // PC_W_E_VAR;
    int _varWI;  // PC_W_I_VAR;
    
    //    int  _cellToAvg; // PC_CELLS_TO_AVG.  Local?
    int *_wrapAvgXY; // PC_AVG_XY_WRAP.   Need deallocate.
    int *_wrapAvgZ;  // PC_AVG_TH_WRAP.   Need deallocate.

    double *_sumLookupSinXY;  //  PC_XY_SUM_SIN_LOOKUP;
    double *_sumLookupCosXY;  //  PC_XY_SUM_COS_LOOKUP;
    double *_sumLookupSinZ;   //  PC_TH_SUM_SIN_LOOKUP;
    double *_sumLookupCosZ;   //  PC_TH_SUM_COS_LOOKUP;

    
    CellPPP _cell;        // posecells
    CellP   _memory;      // posecells_memory

    // Network size, in bytes.
    int _memorySize;  // posecells_memory_size

    //  Number of elements in the network.
    int _pcNetSize;  // posecells_elements.
    
    CellPPP _pcaNew;        // pca_new
    CellP   _pcaNewMemory;  // pca_new_memory
    
    // Wrapper related
    //
    int *_wrapEXY;  // PC_E_XY_WRAP;
    int *_wrapEZ;   // PC_E_TH_WRAP;
    int *_wrapIXY;  // PC_I_XY_WRAP;
    int *_wrapIZ;   // PC_I_TH_WRAP;


    // Related to: posecell topology:
    double _pcDeltaThreshold;  // Param. EXP_DELTA_PC_THRESHOLD.

    // Energy related. Excitation or inhibition.
    //
    Element *_wExcite;     // PC_W_EXCITE
    Element *_wInhibit;    // PC_W_INHIB;
    double   _globalInhib; // Param. PC_GLOBAL_INHIB;


    // Best
    double _bestX;  // best_x
    double _bestY;  // best_y
    double _bestZ;  // best_th

    
    // Related to: VisualTemplate module:
    std::vector<PcTemplate> _pcTemplateSet;   // visual_templates.
    int    _vtId;           // current_vt. Current  VT. 
    int    _prevVtId;       // prev_vt.    Previous VT.
    double _vtRestore;      // Param. PC_VT_RESTORE.
    double _vtActiveDecay;  // Param. VT_ACTIVE_DECAY.
    double _vtInjectEnergy; // Param. PC_VT_INJECT_ENERGY;

    // Related to: ExperienceMap module:
    int _numExp;  // Number of created PcExperience's.
    
    // int _expId; // Defined in PoseCell base class.

    
    // Other things to investigate.
    // TODO: find theirs meanings
    // 
    CellPP _pcaNewRotPtr;  // pca_new_rot_ptr
    CellPP _pcaNewRotPtr2; // pca_new_rot_ptr2
    CellP  _planeAngle;    // posecells_plane_th

    double *_sumSinLookupXY;  // PC_XY_SUM_SIN_LOOKUP;
    double *_sumCosLookupXY;  // PC_XY_SUM_COS_LOOKUP;
    double *_sumSinLookupZ;   // PC_TH_SUM_SIN_LOOKUP;
    double *_sumCosLookupZ;   // PC_TH_SUM_COS_LOOKUP;

    double _vtDeltaPcTh;  // vt_delta_pc_th.

  };
}

#endif

