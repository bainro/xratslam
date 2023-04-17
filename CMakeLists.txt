##
## Copyright 2019 - Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
## All rights reserved.
##
## Author: Mauro Enrique de Souza Muñoz (mesmunoz@gmail.com)
## 
cmake_minimum_required( VERSION 3.7.2 )


## Project.
##
project( xratslam VERSION 1.0.0 )

#New

set (CMAKE_CXX_STANDARD 11)


## External dependencies.
##

# OpenCV
find_package( OpenCV REQUIRED )
include_directories( xratslam ${OpenCV_INCLUDE_DIRS} )

# Boost
find_package( Boost REQUIRED COMPONENTS system filesystem serialization)
include_directories( xratslam ${Boost_INCLUDE_DIRS} )

## Default install directories.
##
include( GNUInstallDirs )

## Library code files.
##
add_library( xratslam SHARED
  src/ratslam/visual_odometry.cpp
  src/ratslam/local_view_match.cpp
  src/ratslam/posecell_network.cpp
  src/ratslam/experience_map.cpp
  src/xratslam/Helper.cc
  src/xratslam/Settings.cc
  src/xratslam/Utils.cc
  src/xratslam/DataReader.cc
  src/xratslam/DataReaderDir.cc
  src/xratslam/DataReaderVideo.cc
  src/xratslam/Image.cc
  src/xratslam/Odometry.cc
  src/xratslam/FileOdometry.cc
  src/xratslam/XVisualOdometry.cc
  src/xratslam/Factory.cc
  src/xratslam/localview/VisualTemplate.cc
  src/xratslam/localview/VisualTemplateSet.cc
  src/xratslam/localview/OriginalLV.cc
  src/xratslam/localview/RecodedLV.cc
  src/xratslam/posecell/PcTemplate.cc
  src/xratslam/posecell/OriginalPC.cc
  src/xratslam/posecell/RecodedPC.cc
  src/xratslam/posecell/NoCanPC.cc
  src/xratslam/Experience.cc
  src/xratslam/ExperienceSet.cc
  src/xratslam/ExpLink.cc
  src/xratslam/ExpLinkSet.cc
  src/xratslam/DataEM.cc
  src/xratslam/experiencemap/OriginalEM.cc
  src/xratslam/experiencemap/RecodedEM.cc
  src/xratslam/Log.cc
  src/xratslam/TimeLogger.cc
  src/xratslam/XRatSlam.cc
  )

## Include directories.
## Uses only the root directory.
##
target_include_directories( xratslam PRIVATE src )


## Enable pkg-config to use this library.
##
configure_file( xratslam.pc.in xratslam.pc @ONLY )


##############
### Properties
##
set_target_properties( xratslam PROPERTIES VERSION ${PROJECT_VERSION} )
set_target_properties( xratslam PROPERTIES SOVERSION 1 )


#################
### Install rules

# core
install(
  TARGETS xratslam
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

# ratslam headers.
install(
  FILES
  src/ratslam/path_node.h
  src/ratslam/experience_map.h
  src/ratslam/utils.h
  src/ratslam/visual_odometry.h
  
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ratslam
  )

# xratslam headers.
install(
  FILES
  src/xratslam/Helper.hh
  src/xratslam/Settings.hh
  src/xratslam/Image.hh
  src/xratslam/Odometry.hh
  src/xratslam/OdometryReader.hh
  src/xratslam/DataReader.hh
  src/xratslam/DataReaderDir.hh
  src/xratslam/DataReaderVideo.hh
  src/xratslam/Chronometer.hh
  src/xratslam/TimeLogger.hh
  src/xratslam/Experience.hh
  src/xratslam/ExperienceSet.hh
  src/xratslam/ExpLink.hh
  src/xratslam/ExpLinkSet.hh
  src/xratslam/DataEM.hh
  src/xratslam/XRatSlam.hh
  src/xratslam/LocalView.hh
  src/xratslam/ExperienceMap.hh

  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xratslam
  )

# pkgconfig
install(
  FILES       ${CMAKE_BINARY_DIR}/xratslam.pc
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
  )

find_package( PkgConfig REQUIRED )
pkg_check_modules( XRATSLAM REQUIRED xratslam )

#add_executable(test
#  test.cc
#)

## External libraries.
#
target_link_libraries( xratslam ${OpenCV_LIBRARIES}   )
target_link_libraries( xratslam ${Boost_LIBRARIES}    )
target_link_libraries( test xratslam)
