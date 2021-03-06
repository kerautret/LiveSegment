PROJECT(ssExtraction)

#Required in DGtal
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET (SEG_DETECTOR ../Detector/directionalscan.cpp	../Detector/linetool.cpp		../Detector/segmentdetector.cpp
../Detector/imagedata.cpp		../Detector/pixel.cpp)

SET (SEG_BS ../BlurredSegments/algorithm.cpp			../BlurredSegments/graphic.cpp
../BlurredSegments/antipodalh.cpp			../BlurredSegments/line.cpp
../BlurredSegments/antipodalv.cpp			../BlurredSegments/listpoint2d.cpp
../BlurredSegments/blurredsegment.cpp		../BlurredSegments/point2d.cpp
../BlurredSegments/boiterecouvrementcst.cpp	../BlurredSegments/point2dincv.cpp
../BlurredSegments/circulator.cpp			../BlurredSegments/point3d.cpp
../BlurredSegments/color.cpp			../BlurredSegments/segment.cpp
../BlurredSegments/convexhull.cpp			../BlurredSegments/segmentmax.cpp)


SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_FOUND FALSE)
FIND_PACKAGE(Boost 1.46.0 REQUIRED COMPONENTS program_options)
if ( Boost_FOUND )
  message(STATUS "Boost and boost_program_options found.")
  include_directories( ${Boost_INCLUDE_DIRS} )
  SET(DGtalToolsLibDependencies ${DGtalToolsLibDependencies} 
     ${Boost_LIBRAIRIES}  
     ${Boost_PROGRAM_OPTIONS_LIBRARY})
   SET(DGtalLibInc ${Boost_INCLUDE_DIRS})
endif( Boost_FOUND) 

FIND_PACKAGE(DGtal REQUIRED)
INCLUDE_DIRECTORIES(${DGTAL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(../Detector ../BlurredSegments )
LINK_DIRECTORIES(${DGTAL_LIBRARY_DIRS} )


ADD_EXECUTABLE(ssExtraction ssExtraction ${SEG_DETECTOR} ${SEG_BS} )
TARGET_LINK_LIBRARIES(ssExtraction ${DGTAL_LIBRARIES} ${DGtalToolsLibDependencies} )




