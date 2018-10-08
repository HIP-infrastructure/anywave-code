# FindAnyWave
#
# Try to find AnyWave lib and include dirs (used for building plugins)
#
# The detection is based on the environment variable AWROOT.
# This variable can also be specified when running CMake with the -DAWROOT:PATH=/path
# MacOS : AWROOT must be the path to the Application Bundle, most often: /Applications/AnyWave.app
# Once done this will define:
#
#  AW_FOUND		- True if AnyWave was found.
#  AW_LIBRARIES         - All the libraries provided by AnyWave.
#  AW_LIB_DIR           - The path the libraries
#  AW_INCLUDE_DIR       - The path to the headers.
#  AW_PLUGINS_DIR	- The path where to put the plugins
# 
#=============================================================================
IF(DEFINED ENV{AWROOT})
  SET(AW_ROOT $ENV{AWROOT})
ELSE()
  IF(DEFINED AWROOT)
     SET(AW_ROOT ${AWROOT})
   ENDIF()
ENDIF()

IF(NOT DEFINED AW_ROOT)
    SET(AW_FOUND FALSE)
ELSE()
   IF(APPLE)
      SET(AW_INCLUDE_DIR ${AW_ROOT}/Contents/include)
      SET(AW_LIB_DIR ${AW_ROOT}/Contents/Frameworks)
   ELSE()
      SET(AW_INCLUDE_DIR ${AW_ROOT}/include)
      SET(AW_LIB_DIR ${AW_ROOT}/lib)
   ENDIF()
   MESSAGE(STATUS "Found AnyWave at ${AW_ROOT}")
   SET(AW_FOUND TRUE)
ENDIF()

mark_as_advanced(AW_INCLUDE_DIR AW_LIB_DIR)

# handle the QUIETLY and REQUIRED arguments and set AW_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(AnyWave REQUIRED_VARS AW_INCLUDE_DIR AW_LIB_DIR)

if(AW_FOUND)
   IF(APPLE)
     set(AW_LIBRARIES -framework AwCore AwProcess AwReadWrite AwUtilities AwMATLAB 
          AwMath AwFiltering AwGraphics AwLayout AwMapping AwHDF5 AwEpoch AwWidget)
     set(AW_PLUGINS_DIR ${AW_ROOT}/../Anywave_Plugins)
   ELSE()
     set(AW_LIBRARIES AwCore AwProcess AwReadWrite AwUtilities AwMATLAB AwMath AwFiltering AwGraphics 
            AwLayout AwMapping AwHDF5 AwEpoch AwWidget)
     set(AW_PLUGINS_DIR ${AW_ROOT}/Plugins)
   ENDIF()
   LINK_DIRECTORIES(${AW_LIB_DIR})
   INCLUDE_DIRECTORIES(${AW_INCLUDE_DIR})
else(AW_FOUND)
  set(AW_INCLUDE_DIR)
  set(AW_LIB_DIR)
  set(AW_LIBRARIES)
  set(AW_PLUGINS_DIR)
endif(AW_FOUND)

