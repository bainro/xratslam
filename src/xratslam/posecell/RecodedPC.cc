/*
 * Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 * All rights reserved.
 * 
 * Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
 *
 * Date: 08/04/2019 (dd/mm/yyyy)
 *
 */
#include "xratslam/posecell/RecodedPC.hh"
#include "xratslam/Odometry.hh"
#include "xratslam/Template.hh"
#include "xratslam/Utils.hh"

#include <iostream>
#include <fstream>

#include <unistd.h>  // Debug: exit().


using namespace xratslam;
using namespace std;


/*************************************************************************/
/******************************* RecodedPC *******************************/

/*******************/
/*** constructor ***/

RecodedPC::RecodedPC( string configFile )
  : PoseCell( "RecodedPC" )
{
  // Read ratslam settings from 'configFile'.
  Utils::PTree rts;
  Utils::getChildFromFile( rts, configFile, "ratslam" );

  Utils::getFromPtree( _dimXY, rts, "pc_dim_xy" , 21 );
  Utils::getFromPtree( _dimZ , rts, "pc_dim_th" , 36 );
  Utils::getFromPtree( _dimWE, rts, "pc_w_e_dim", 7  );
  Utils::getFromPtree( _dimWI, rts, "pc_w_i_dim", 5  );
  
  Utils::getFromPtree( _globalInhib, rts, "pc_global_inhib", 0.00002 );
  
  Utils::getFromPtree( _vtActiveDecay , rts, "vt_active_decay"    , 1.0  );
  Utils::getFromPtree( _vtInjectEnergy, rts, "pc_vt_inject_energy", 0.15 );
  Utils::getFromPtree( _vtRestore     , rts, "pc_vt_restore"      , 0.05 );

  Utils::getFromPtree( _pcDeltaThreshold, rts, "exp_delta_pc_threshold",
                       2.0 );

  Utils::getFromPtree( _varWE, rts, "pc_w_e_var", 1 );
  Utils::getFromPtree( _varWI, rts, "pc_w_i_var", 2 );

  
  // the starting position within the posecell network
  _bestX = floor( (double)_dimXY / 2.0 );
  _bestY = floor( (double)_dimXY / 2.0 );
  _bestZ = floor( (double)_dimZ  / 2.0 );

  _numExp = 0;
  _expId  = 0;
  _vtId   = 0;

  constructPC();

  _isUpdated = false;
}


/******************/
/*** destructor ***/

RecodedPC::~RecodedPC()
{
  destroyPC();
}


/************/
/*** save ***/
bool
RecodedPC::save( string conf, string data, int counter )
{
  return saveConfig( conf ) && saveData( data );
}


/************/
/*** load ***/
bool
RecodedPC::load( string conf, string data, int counter )
{
  return loadConfig( conf ) && loadData( data );
}


/***************/
/*** on Feed ***/
void
RecodedPC::onFeed( Odometry *odometry, Template *templ,
                   Action::Type *actionType, int *dstExpId,
                   double *relRad )
{
  //  cout <<"RecodedPC::onFeed() - not UPDATING PoseCell values!!!"<< endl;

  /*
  // First: network iteration related.
  //
  excite();
  inhibit();
  globalInhibit();
  normalise();
  */
  
  *actionType = getAction();

  // Second: template related.
  //

  // If it is a new VisualTemplate, created it.
  int newVtId = templ->id;
  if ( newVtId >= _pcTemplateSet.size() )
  {
    PcTemplate pcT;
    _pcTemplateSet.push_back( pcT );
  }
  
  _prevVtId = _vtId;
  _vtId     = newVtId;
  
  //  onViewTemplate( templ->id, templ->radius );
  
  *dstExpId = _expId;
  *relRad   = getRelativeRad();

  _isUpdated = true;
}


/*******************/
/*** save Config ***/
bool
RecodedPC::saveConfig( string configFile )
{
  ofstream file{ configFile };
  if ( ! file.is_open() ) return false;

  // Base class attributes.
  file
    << "ExpID = " << _expId << endl
    ;

  // Class attributes.
  file
    << "DimX     = " << _dimXY    << endl
    << "DimY     = " << _dimXY    << endl
    << "DimZ     = " << _dimZ     << endl
    << "VtId     = " << _vtId     << endl
    << "PrevVtId = " << _prevVtId << endl
    << "NumExp   = " << _numExp   << endl
    ;
  
  file.close();
  return true;
}


/*******************/
/*** load Config ***/
bool
RecodedPC::loadConfig( string configFile )
{
  ifstream file{ configFile };
  if ( ! file.is_open() ) return false;

  // Used to read labels an 'equal' signs.
  string str;
  
  // Base class attributes.
  file >> str >> str; file >> _expId;

  // Class attributes.
  file >> str >> str; file >> _dimXY   ;
  file >> str >> str; file >> _dimXY   ;
  file >> str >> str; file >> _dimZ    ;
  file >> str >> str; file >> _vtId    ;
  file >> str >> str; file >> _prevVtId;
  file >> str >> str; file >> _numExp  ;

  file.close();
  return true;
}


/*****************/
/*** save Data ***/
bool
RecodedPC::saveData( string dataFile )
{
  ofstream file( dataFile );
  if ( ! file.is_open() ) return false;

  // Save '_pcTemplateSet' size and values.
  int pctSize = _pcTemplateSet.size();
  file << pctSize << endl;
  for ( int i = 0; i < pctSize; i++ )
  {
    _pcTemplateSet[i].save( file );
  }
  file << endl;

  // Save PoseCell dimensions and cell values.
  //
  file << _dimXY <<" "<< _dimXY <<" "<< _dimZ << endl;
  for ( int x = 0; x < _dimXY; x++ )
  {
    for ( int y = 0; y < _dimXY; y++ )
    {
      for ( int z = 0; z < _dimZ; z++ )
      {
        file << _cell[z][y][x] << " ";
      }
      file << endl;
    }
    file << endl;
  }

  file.close();
  return true;
}


/*****************/
/*** load Data ***/
bool
RecodedPC::loadData( string dataFile )
{
  ifstream file{ dataFile };
  if ( ! file.is_open() ) return false;

  // Load '_pcTemplateSet' size and values.
  //
  int pctSize = 0;
  file >> pctSize;
  _pcTemplateSet.resize( pctSize );
  for ( int i = 0; i < pctSize; i++ )
  {
    _pcTemplateSet[i].load( file );
  }

  // Posecell dimensions.
  //
  int dimXY = -1;
  int dimZ  = -1;
  file >> dimXY;
  file >> dimXY;
  file >> dimZ;

  // If new posecell has different dimensions than the current one,
  // reallocate.
  if ( _dimXY != dimXY || _dimZ != dimZ )
  {
    // NOTE: this code is executed only when a new "project" with
    // different PC dimensions is presented!!!
    // Todo: test.
    destroyPC();
    
    _dimXY = dimXY;
    _dimZ  = dimZ;
    constructPC();
  }

  // Loop in all three dimensions
  for ( int x = 0; x < _dimXY; x++ )
  {
    for ( int y = 0; y < _dimXY; y++ )
    {
      for ( int z = 0; z < _dimZ; z++ )
      {
        file >> _cell[z][y][x];
      }
    }
  }

  file.close();
  return true;
}


/******************/
/*** destroy PC ***/
void
RecodedPC::destroyPC()
{
  delete _memory;
  delete _pcaNewMemory;

  for ( int i = 0; i < _dimZ; i++ )
  {
    delete _cell  [i];
    delete _pcaNew[i];
  }
  delete _cell;
  delete _pcaNew;

  delete _wExcite;
  delete _wInhibit;

  delete _wrapEXY;
  delete _wrapIXY;
  delete _wrapEZ;
  delete _wrapIZ;

  delete _wrapAvgXY;
  delete _wrapAvgZ;

  delete _sumLookupSinXY;
  delete _sumLookupCosXY;
  delete _sumLookupSinZ;
  delete _sumLookupCosZ;

  _dimXY = _dimZ = -1;
}


/********************/
/*** construct PC ***/
void
RecodedPC::constructPC()
{
  // set the sizes
  _pcNetSize  = _dimXY * _dimXY * _dimZ;
  _memorySize = _pcNetSize * sizeof(Cell);

  // Allocate memory for current PoseCell and for a PoseCell scratch.
  _memory       = new Cell[ _pcNetSize ];
  _pcaNewMemory = new Cell[ _pcNetSize ];

  // Zero all memories.
  memset( _memory      , 0, _memorySize );
  memset( _pcaNewMemory, 0, _memorySize );

  // Allocate first level pointers
  _cell   = new CellPP[ _dimZ ];
  _pcaNew = new CellPP[ _dimZ ];

  for ( int i = 0; i < _dimZ; i++ )
  {
    // Allocate second level pointers.
    _cell  [i] = new CellP[ _dimXY ];
    _pcaNew[i] = new CellP[ _dimXY ];

    for ( int j = 0; j < _dimXY; j++ )
    {
      // TRICKY! point second level pointers at already allocated memory
      int pos = (j + i * _dimXY) * _dimXY;
      _cell  [i][j] = &_memory      [ pos ];
      _pcaNew[i][j] = &_pcaNewMemory[ pos ];
    }
  }

  
  // Excitation and Inhibition patterns.
  //
  _wExcite  = new Element[ _dimWE * _dimWE * _dimWE ];
  _wInhibit = new Element[ _dimWI * _dimWI * _dimWI ];

  // Set network initial energy.
  int px = (int) _bestX;
  int py = (int) _bestY;
  int pz = (int) _bestZ;
  _cell [pz] [py] [px] = 1;

  // set up the wrap lookups
  _dimWEHalf = _dimWE / 2;
  _dimWIHalf = _dimWI / 2;

  _wrapEXY = new int[ _dimXY + _dimWE - 1 ];
  _wrapEZ  = new int[ _dimZ  + _dimWE - 1 ];
  _wrapIXY = new int[ _dimXY + _dimWI - 1 ];
  _wrapIZ  = new int[ _dimZ  + _dimWI - 1 ];

  int dEH = _dimWEHalf;
  int dIH = _dimWIHalf;
  generateWrap( _wrapEXY, _dimXY - dEH, _dimXY, 0, _dimXY, 0, dEH );
  generateWrap( _wrapEZ , _dimZ  - dEH, _dimZ , 0, _dimZ , 0, dEH );
  generateWrap( _wrapIXY, _dimXY - dIH, _dimXY, 0, _dimXY, 0, dIH );
  generateWrap( _wrapIZ , _dimZ  - dIH, _dimZ , 0, _dimZ , 0, dIH );

  int cToAvg = 3; // PC_CELLS_TO_AVG.
  _wrapAvgXY = new int[ _dimXY + 2 * cToAvg ];
  _wrapAvgZ  = new int[ _dimZ  + 2 * cToAvg ];

  generateWrap( _wrapAvgXY, _dimXY - cToAvg, _dimXY, 0, _dimXY, 0, cToAvg );
  generateWrap( _wrapAvgZ , _dimZ  - cToAvg, _dimZ , 0, _dimZ , 0, cToAvg );

  // sine and cosine lookups
  _sumLookupSinXY = new double[ _dimXY ];
  _sumLookupCosXY = new double[ _dimXY ];
  _sumLookupSinZ  = new double[ _dimZ  ];
  _sumLookupCosZ  = new double[ _dimZ  ];

  double factorXY = 2.0 * M_PI / _dimXY;
  for ( int i = 0; i < _dimXY; i++ )
  {
    double angle = (i + 1) * factorXY;
    _sumLookupSinXY[i] = sin( angle );
    _sumLookupCosXY[i] = cos( angle );
  }

  double factorZ = 2.0 * M_PI / _dimZ;
  for ( int i = 0; i < _dimZ; i++ )
  {
    double angle = (i + 1) * factorZ;
    _sumLookupSinZ[i] = sin( angle );
    _sumLookupCosZ[i] = cos( angle );
  }

  // Set '_wExcite' with normal gaussian values (mexican hat).
  // All values are summed for latter normalization.
  //
  int     centerE = _dimWE / 2;
  Element *excite = _wExcite;
  double     sumE = 0.0;
  for ( int k = 0; k < _dimWE; k++ )
  {
    for ( int j = 0; j < _dimWE; j++ )
    {
      for ( int i = 0; i < _dimWE; i++ )
      {
        sumE += *excite++ = norm2d( _varWE, i, j, k, centerE );
      }
    }
  }

  // Normalize '_wExcite' values.
  excite = _wExcite;
  Element *endE = excite + _dimWE * _dimWE * _dimWE;
  while ( excite < endE )
    *excite++ /= sumE;

  
  // Set '_wInhibit' with normal gaussian values (mexican hat).
  // All values are summed for latter normalization.
  //
  int      centerI = _dimWI / 2;
  Element *inhibit = _wInhibit;
  double      sumI = 0.0; 
  for ( int k = 0; k < _dimWI; k++ )
  {
    for ( int j = 0; j < _dimWI; j++ )
    {
      for ( int i = 0; i < _dimWI; i++ )
      {
        sumI += *inhibit++ = norm2d( _varWI, i, j, k, centerI );
      }
    }
  }

  // Normalize '_wInhibit' values.
  Element *inh  = _wInhibit;
  Element *endI = inh + _dimWI * _dimWI * _dimWI;
  while ( inh < endI )
    *inh++ /= sumI;
}


/******************/
/*** get Action ***/
Action::Type
RecodedPC::getAction()
{
  // If have not updated, or
  // If there is no template, or
  // If current template is the same as the previous one,
  // Do nothing.
  //
  if ( ! _isUpdated || _pcTemplateSet.empty() || (_vtId == _prevVtId) )
  {
    return Action::NO_ACTION;
  }

  // Debug:
  if ( _vtId >= _pcTemplateSet.size() )
  {
    cerr <<"RecodedPC::getAction() - ERROR:"<< endl
         <<"  _vtId                : "<< _vtId                 << endl
         <<"  _pcTemplateSet.size(): "<< _pcTemplateSet.size() << endl
      ;
    exit( -1 );
  }
  
  // The experience set of the activated template (_vtId).
  vector<int> *tplExpSet = &_pcTemplateSet[ _vtId ].expSet;

  // If the current VisualTemplate has no associated experience,
  // create a new experience.
  if ( tplExpSet->empty() )
  {
    // Add a new experience to the experience set of the current template.
    // The new created experience is the activated one (_expId).
    tplExpSet->push_back( _expId = _numExp++ );
    return Action::CREATE_NODE;
  }

  // If activated template has some associated experience,
  // create a link to it.
  // Set this experience as the activated experience.
  //
  // Note: it seems that 'tplExpSet' never has more than one element!
  // Todo: check that.
  //
  _expId = tplExpSet->front();

  return Action::CREATE_EDGE;
}


/************************/
/*** on View Template ***/
void
RecodedPC::onViewTemplate( uint newVtId, double vtRad )
{
  // If it is a new VisualTemplate, created it.
  if ( newVtId >= _pcTemplateSet.size() )
  {
    PcTemplate pcT;
    _pcTemplateSet.push_back( pcT );
  }

  _prevVtId = _vtId;
  _vtId     = newVtId;
}


/**************/
/*** excite ***/
void
RecodedPC::excite()
{
  memset( _pcaNewMemory, 0, _memorySize );

  // loop in all three dimensions
  for ( int x = 0; x < _dimXY; x++ )
  {
    for ( int y = 0; y < _dimXY; y++ )
    {
      for ( int z = 0; z < _dimZ; z++ )
      {
        if ( _cell[z][y][x] != 0 )
        {
          // spread the pose cell energy
          exciteCell( x, y, z );
        }
      }
    }
  }

  memcpy( _memory, _pcaNewMemory, _memorySize );
}


/***************/
/*** inhibit ***/
void
RecodedPC::inhibit()
{
  // Set _pcaNew to 0.
  memset( _pcaNewMemory, 0, _memorySize );

  // Loop through all dimensions
  for ( int x = 0; x < _dimXY; x++ )
  {
    for ( int y = 0; y < _dimXY; y++ )
    {
      for ( int z = 0; z < _dimZ; z++ )
      {
        // If there is energy in the current posecell, spread the energy.
        if ( _cell[z][y][x] != 0 )
          inhibitCell( x, y, z );
      }
    }
  }

  // Subtract new calculated posecell network from current network.
  Cell *src = _pcaNewMemory;
  Cell *dst = _memory;
  Cell *end = dst + _pcNetSize;
  while ( dst < end )
    *dst++ -= *src++;
}


/**********************/
/*** global Inhibit ***/
void
RecodedPC::globalInhibit()
{
  Cell *cell = _memory;
  Cell *end  = cell + _pcNetSize;
  while ( cell < end )
  {
    if ( *cell >= _globalInhib )
      *cell++ -= _globalInhib;
    else
      *cell++  = 0.0;
  }
}


/*****************/
/*** normalise ***/
void
RecodedPC::normalise()
{
  // The sum of all cells.
  //
  double sum = 0.0;
  Cell *cell = _memory;
  Cell *end  = cell + _pcNetSize;
  while ( cell < end )
    sum += *cell++;

  if ( ! sum )
    return;

  // Normalise: divide all cells by 'sum'.
  cell = _memory;
  while ( cell < end )
    *cell++ /= sum;
}


/*******************/
/*** excite Cell ***/
void
RecodedPC::exciteCell( int x, int y, int z )
{
  // The source cell energy.
  Cell cellXYZ = _cell[z][y][x];
  
  // Excitation factors.
  Element *excite = _wExcite;

  // Dimensions range limits.
  int dimZ = z + _dimWE;
  int dimY = y + _dimWE;
  int dimX = x + _dimWE;
    
  // Loop through all dimensions.
  for ( int zl = z; zl < dimZ; zl++ )
  {
    for ( int yl = y; yl < dimY; yl++ )
    {
      for ( int xl = x; xl < dimX; xl++ )
      {
        // Generate indices by wrapping where necessary
        int xw = _wrapEXY[ xl ];
        int yw = _wrapEXY[ yl ];
        int zw = _wrapEZ [ zl ];

        // for every pose cell, multiply the current energy by
        // a pdf to spread the energy (_wExcite is a 3d pdf)
        _pcaNew[zw][yw][xw] += cellXYZ * (*excite++);
      }
    }
  }
}


/********************/
/*** inhibit Cell ***/
void
RecodedPC::inhibitCell( int x, int y, int z )
{
  // The source cell energy.
  Cell cellXYZ = _cell[z][y][x];
  
  // Inhibition factors.
  Element *inhibit = _wInhibit;

  // Dimensions range limits.
  int dimZ = z + _dimWI;
  int dimY = y + _dimWI;
  int dimX = x + _dimWI;
    
  // Loop through all dimensions.
  for ( int zl = z; zl < dimZ; zl++ )
  {
    for ( int yl = y; yl < dimY; yl++ )
    {
      for (int xl = x; xl < dimX; xl++ )
      {
        // generate indices by wrapping where necessary
        int xw = _wrapIXY[ xl ];
        int yw = _wrapIXY[ yl ];
        int zw = _wrapIZ [ zl ];

        // spread the energy using a pdf
        _pcaNew[zw][yw][xw] += cellXYZ * (*inhibit++);
      }
    }
  }
}


/*********************/
/*** generate Wrap ***/
void
RecodedPC::generateWrap( int *wrap, int start1, int end1, int start2,
                         int end2, int start3, int end3 )
{
  int i = 0;
  for ( int j = start1; j < end1; )  wrap[i++] = j++;
  for ( int j = start2; j < end2; )  wrap[i++] = j++;
  for ( int j = start3; j < end3; )  wrap[i++] = j++;
}


/***************/
/*** norm 2d ***/
double
RecodedPC::norm2d( double var, int x, int y, int z, int dimCentre )
{
  const double sqrt2PI = std::sqrt( 2.0 * M_PI );

  double dx   = x - dimCentre;
  double dy   = y - dimCentre;
  double dz   = z - dimCentre;
  double prod = dx*dx + dy*dy + dz*dz;
  
  return 1.0 / (var * sqrt2PI) * std::exp( -prod / (2*var*var) );
}
