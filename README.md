RatSlam C++ library.

The xRatSLam library implements the Simultaneous Location and Mapping (SLAM) algorithm
called RatSlam inspired by neural rodents knowing dynamics while performing self localization tasks.

Original code from Muñoz et al. (2022)

## Requirements
- Unix OS
- OpenCV
- Boost

## Building and installing

> $ cd libxratslam/build  
> $ cmake .. && make install  


## How to use

The library files libxratslam.so* are installed by default at "/usr/local/lib/".
Headers are installed at "/usr/local/include/{xratslam,ratslam}".
'pkg-config' can be used to find those locations.

If you are using cmake, add to your "CMakeLists.txt":  
> find_package( PkgConfig REQUIRED )  
> pkg_check_modules( XRATSLAM REQUIRED xratslam )  

and

> target_link_libraries(  
> ...  
> ${XRATSLAM_LIBRARIES}  
> ...  
> )   

See https://github.com/mesmu/xratslam-example for a practical example.


## Insertion of a new LocalView, PoseCell or ExperienceMap implementation
  
As an example, when creating the "MyCoolLV" LocalView class, follow these steps:

1. Create MyCoolLV.{cc,hh} and other dependency files at
"libxratslam/src/xratslam/localview/" directory.

2. The "MyCoolLV" class must extend "xratslam::LocalView" class and have
a constructor accepting a "std::string" with the name of a file
containing all "MyCoolLV" settings.  
This file is supposed to formated as "<key> = <value>" elements.
The default implementations use 'boost::property_tree::ptree' to read
these settings. For an example, see: "xratslam/localview/OriginalLV.cc".
  
3. Add all "*.cc" files to the "add_library( xratslam SHARED .."
clause in the "libxratslam/CMakeList.txt" file.
Doing that those files will be compiled within the 'libxratslam' library.

4. Modify the "libxratslam/src/xratslam/Factory.cc" file to:

* Include all necessary headers to instantiate an object of "MyCoolLV" class.
Ex:  
> #include "xratslam/localview/MyCoolLV.hh"

* Let Factor knows how to instatiate objects of class "MyCoolLV":  
Add to method "Factory::createLocalView(...)"  
> if ( lvName == "MyCoolLV" )  return new xratslam::MyCoolLV( configFile );  

Note that "MyCoolLV" class is supposed to be declared within "xratslam"
namespace!

5. The same steps should be taken for "PoseCell" and "ExperienceMap"
structures, modifying all "LocalView" references, of course.


## How to customized LocalView, PoseCell or ExperienceMap implementation

In the code using libxratslam library, after instantiate a
"xratslam::RatSlam" object, you should set your customized ratslam structure.
Example:  
> RatSlam slam( configFile );  
> ...  
> slam.setLocalView( "MyCoolLV", configFile );  
 
Note that "setLocalView()" should be called before "slam.start()"!
   
If you do not do that, the original structures coded by
David Ball  (david.ball@qut.edu.au) and
Scott Heath (scott.heath@uqconnect.edu.au)
will be used.

