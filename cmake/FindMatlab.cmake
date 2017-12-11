# - this module looks for Matlab
# Defines:
#  MATLAB_INCLUDE_DIR: include path for mex.h
#  MATLAB_LIBRARIES:   required libraries: libmex, libmx
#  MATLAB_MEX_LIBRARY: path to libmex
#  MATLAB_MX_LIBRARY:  path to libmx

SET(MATLAB_FOUND 0)
IF( "$ENV{MATLAB_ROOT}" STREQUAL "" )
    MESSAGE(STATUS "MATLAB_ROOT environment variable not set." )
    MESSAGE(STATUS "In Linux this can be done in your user .bashrc file by appending the corresponding line, e.g:" )
    MESSAGE(STATUS "export MATLAB_ROOT=/usr/local/MATLAB/R2012b" )
    MESSAGE(STATUS "In Windows this can be done by adding system variable, e.g:" )
    MESSAGE(STATUS "MATLAB_ROOT=D:\\Program Files\\MATLAB\\R2011a" )
ELSE("$ENV{MATLAB_ROOT}" STREQUAL "" )

        FIND_PATH(MATLAB_INCLUDE_DIR mex.h
                  $ENV{MATLAB_ROOT}/extern/include)

        FIND_LIBRARY( MATLAB_MEX_LIBRARY
                      NAMES libmex mex
                      PATHS $ENV{MATLAB_ROOT}/bin $ENV{MATLAB_ROOT}/extern/lib 
                      PATH_SUFFIXES maci64 glnxa64 glnx86)

        FIND_LIBRARY( MATLAB_MX_LIBRARY
                      NAMES libmx mx
                      PATHS $ENV{MATLAB_ROOT}/bin $ENV{MATLAB_ROOT}/extern/lib 
                      PATH_SUFFIXES maci64 glnxa64 glnx86)

        FIND_LIBRARY( MATLAB_MAT_LIBRARY
                      NAMES libmat mat
                      PATHS $ENV{MATLAB_ROOT}/bin $ENV{MATLAB_ROOT}/extern/lib 
                      PATH_SUFFIXES maci64 glnxa64 glnx86)

        FIND_LIBRARY( MATLAB_ENG_LIBRARY
                      NAMES libeng eng
                      PATHS $ENV{MATLAB_ROOT}/bin $ENV{MATLAB_ROOT}/extern/lib 
                      PATH_SUFFIXES maci64 glnxa64 glnx86)
              
    # This is common to UNIX and Win32:
   SET(MATLAB_LIBRARIES
    ${MATLAB_MEX_LIBRARY}
    ${MATLAB_MX_LIBRARY}
    ${MATLAB_MAT_LIBRARY}
    ${MATLAB_ENG_LIBRARY})
    
    SET(MATLAB_ROOT $ENV{MATLAB_ROOT})
ENDIF("$ENV{MATLAB_ROOT}" STREQUAL "" )

IF(MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)
  SET(MATLAB_FOUND 1)
  MESSAGE(STATUS "Matlab libraries were found.")
  MARK_AS_ADVANCED(MATLAB_LIBRARIES
  MATLAB_MEX_LIBRARY
  MATLAB_MX_LIBRARY
  MATLAB_INCLUDE_DIR
  MATLAB_FOUND
  MATLAB_ROOT)
ELSE()
  MESSAGE(STATUS "Matlab not found.")
ENDIF(MATLAB_INCLUDE_DIR AND MATLAB_LIBRARIES)

# Find MCR
#SET(MATLAB_MCR_FOUND 0)
#IF( "$ENV{MCR_ROOT}" STREQUAL "" )
#    MESSAGE(STATUS "MCR_ROOT environment variable not set." )
#    MESSAGE(STATUS "In Linux this can be done in your user .bashrc file by appending the corresponding line, e.g:" )
#    MESSAGE(STATUS "export MCR_ROOT=/usr/local/MATLAB/MATLAB_Compiler_Runtime" )
#    MESSAGE(STATUS "In Windows this can be done by adding system variable, e.g:" )
#    MESSAGE(STATUS "MCR_ROOT=D:\\Program Files\\MATLAB\\MATLAB_Compiler_Runtime" )
#ELSE()
#   FIND_PATH(MATLAB_MCR_INCLUDE_DIR mclmcrrt.h $ENV{MCR_ROOT}/extern/include)
#   INCLUDE_DIRECTORIES(${MATLAB_MCR_INCLUDE_DIR})
#
#   FIND_LIBRARY(MATLAB_MCR_LIBRARY
#                  NAMES libmwmclmcrrt mwmclmcrrt
#                  PATHS $ENV{MCR_ROOT}/runtime  
#                  PATH_SUFFIXES glnxa64 glnx86 maci64 win64/microsoft win32/microsoft)
#          
#   SET(MCR_ROOT $ENV{MCR_ROOT})
#ENDIF()

#IF(MATLAB_MCR_INCLUDE_DIR AND MATLAB_MCR_LIBRARY)
#  SET(MATLAB_MCR_FOUND 1)
#  MARK_AS_ADVANCED(MATLAB_MCR_LIBRARY
#  MATLAB_MCR_INCLUDE_DIR
#  MATLAB_MCR_FOUND
#  MCR_ROOT)
#  MESSAGE(STATUS "Matlab MCR libraries were found.")
#ENDIF(MATLAB_MCR_INCLUDE_DIR AND MATLAB_MCR_LIBRARY)

