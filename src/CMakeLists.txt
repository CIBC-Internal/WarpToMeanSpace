PROJECT(WarpToMeanSpace)
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE Debug)

SET(VTK_DIR "/home/sci/prateepm/Projects/packages/vtk-5.8")

option(USE_TPS "Warp using Thin Plate Splines" ON)
if(USE_TPS)
    SET(ITK_DIR "/home/sci/prateepm/Projects/packages/itk-4.5.1")
    add_definitions(-DUSE_TPS)
ELSE(USE_TPS)
    SET(ITK_DIR "/home/sci/prateepm/Projects/packages/itk-3.20.1")
ENDIF(USE_TPS)
set(CMAKE_CXX_FLAGS "-lboost_system -lboost_thread -fopenmp")
set(CMAKE_C_FLAGS "-lboost_system -lboost_thread -fopenmp")
#SET(RUN_ON_SERVER 0)

# Configure ITK -- REQUIRED
#SET(ITK_DIR /scratch/TOOLKITS/ITKv3-BUILD)
INCLUDE (${CMAKE_ROOT}/Modules/FindITK.cmake)
IF (USE_ITK_FILE)
  INCLUDE(${USE_ITK_FILE})
ELSE(USE_ITK_FILE)
  MESSAGE(SEND_ERROR "ITK (The Insight Toolkit) is required, but could not be found.")
ENDIF(USE_ITK_FILE)


# Configure VTK -- REQUIRED
#SET(VTK_DIR /scratch/TOOLKITS/VTK5.10.1-BUILD/)
FIND_PACKAGE(VTK)
IF (VTK_FOUND)
  INCLUDE (${VTK_USE_FILE})
ELSE(VTK_FOUND)
  MESSAGE(SEND_ERROR "VTK (The Visualization Toolkit) is required, but could not be found.")
ENDIF (VTK_FOUND)


# Configure tinyxml
INCLUDE_DIRECTORIES(${WarpToMeanSpace_SOURCE_DIR}/tinyxml)
FILE(GLOB tinyxml_srcs ./tinyxml/*.cpp)
ADD_LIBRARY (tinyxml STATIC ${tinyxml_srcs})

INCLUDE_DIRECTORIES(
  ${WarpToMeanSpace_SOURCE_DIR}/Components/FixedImagePyramids
  ${WarpToMeanSpace_SOURCE_DIR}/Components/ImageSamplers
  ${WarpToMeanSpace_SOURCE_DIR}/Components/Interpolators
  ${WarpToMeanSpace_SOURCE_DIR}/Components/Metrics
  ${WarpToMeanSpace_SOURCE_DIR}/Components/MovingImagePyramids
  ${WarpToMeanSpace_SOURCE_DIR}}/Components/Optimizers
  ${WarpToMeanSpace_SOURCE_DIR}/Components/Registrations
  ${WarpToMeanSpace_SOURCE_DIR}/Components/ResampleInterpolators
  ${WarpToMeanSpace_SOURCE_DIR}/Components/Resamplers
  ${WarpToMeanSpace_SOURCE_DIR}/Components/Transforms
 )

file(
	GLOB
	source_files
	*.h
	*.c
	*.cpp
        *.hxx
	*.cxx
        *.txx
)

 
ADD_EXECUTABLE(WarpToMeanSpace ${source_files})

TARGET_LINK_LIBRARIES(WarpToMeanSpace  tinyxml vtkIO vtkWidgets vtkRendering vtkImaging vtkFiltering vtkHybrid vtkGraphics vtkCommon tinyxml ${ITK_LIBRARIES})

INSTALL(TARGETS WarpToMeanSpace  RUNTIME DESTINATION .)

