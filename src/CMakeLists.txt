PROJECT(WarpToMeanSpace)
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE Release)

option(USE_TPS "Warp using Thin Plate Splines" ON)
add_definitions(-DUSE_TPS)
set(CMAKE_CXX_LINKER_FLAGS "-lboost_system -lboost_thread -fopenmp")
set(CMAKE_C_LINKER_FLAGS "-lboost_system -lboost_thread -fopenmp")
#SET(RUN_ON_SERVER 0)


# Configure ITK -- REQUIRED
find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
else(ITK_FOUND)
  message(SEND_ERROR "ITK (The Insight Toolkit) is required, but could not be found.")
endif(ITK_FOUND)

# Configure VTK -- REQUIRED
FIND_PACKAGE(VTK COMPONENTS
    vtkCommonCore
	vtkImagingCore
    vtkFiltersCore
    vtkIOCore
    vtkRenderingFreeTypeOpenGL
	vtkFiltersModeling
    vtkIOLegacy
    vtkIOPLY
    REQUIRED)
IF (VTK_FOUND)
    INCLUDE (${VTK_USE_FILE})
ELSE(VTK_FOUND)
    MESSAGE(FATAL_ERROR "VTK (required) could not be found.")
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

TARGET_LINK_LIBRARIES(WarpToMeanSpace ${ITK_LIBRARIES} ${VTK_LIBRARIES} tinyxml)

INSTALL(TARGETS WarpToMeanSpace  RUNTIME DESTINATION .)

