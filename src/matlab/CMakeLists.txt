cmake_minimum_required(VERSION 3.13)
project(matlab_support)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE Release)

SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

# We assume that this project is within the AnyWave projects
# So AnyWave modules are located in ../src 

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)


# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

IF(APPLE)
    SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)
    add_definitions("-mmacosx-version-min=10.13") # clang flags
    SET(MEX_INSTALL_DIR "/users/bruno/dev/anywave-mac/build/AnyWave.app/Contents/Plugins/MATLAB/AnyWave")
ELSE()
    SET(MEX_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/AnyWave/Plugins/MATLAB/AnyWave")
    add_definitions("-std=c++20")
ENDIF()

find_package(Matlab COMPONENTS MX_LIBRARY)
IF(NOT Matlab_FOUND)
   MESSAGE(STATUS "MATLAB not found. Aborted.")
   RETURN()
ENDIF()

include_directories(${Matlab_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/../../include)

# build MATLAB_Interface (the main mex file using C++ API)
SET(SRC MATLAB_Interface/TCPRequest.cpp MATLAB_Interface/mexMain.cpp  
    MATLAB_Interface/MATLAB_Interface.cpp)
add_library(anywave SHARED ${SRC})
qt5_use_modules(anywave Core Network)
target_link_libraries(anywave ${Matlab_DATAARRAY_LIBRARY}  ${Matlab_MEX_LIBRARY} 
    Qt5::Core Qt5::Network)
set_target_properties(anywave PROPERTIES PREFIX "")
IF(APPLE)
set_target_properties(anywave PROPERTIES SUFFIX ".mexmaci64")
ELSE()
set_target_properties(anywave PROPERTIES SUFFIX ".mexa64")
ENDIF()
INSTALL(TARGETS anywave DESTINATION ${MEX_INSTALL_DIR})

# OLD mex files compatibility
# build AnyWave core lib as static
SET(CORE_SRC ../Core/AwAmplitudeSettings.cpp 
../Core/AwChannel.cpp
../Core/AwMarker.cpp
../Core/AwDataBuffer.cpp
../Core/AwMEGSensorManager.cpp
../Core/AwDataClient.cpp
../Core/AwSensor.cpp)
SET(CORE_H AwMarker.h AwChannel.h AwSensor.h AwDataClient.h 
AwDataBuffer.h AwAmpliduteSettings.cpp)


add_library(CoreStatic STATIC ${CORE_SRC})
qt5_use_modules(CoreStatic Core)
target_link_libraries(CoreStatic  Qt5::Core)

# build mex common static lib
add_library(mexcommon STATIC ../Mex/common.cpp)
qt5_use_modules(mexcommon Core Network)
target_link_libraries(mexcommon  Qt5::Core Qt5::Network ${Matlab_MEX_LIBRARY})

matlab_add_mex(NAME aw_getdata SRC ../Mex/aw_getdata.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getdata_ex SRC ../Mex/aw_getdataex.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getmarkers SRC ../Mex/aw_getmarkers.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getmarkers_ex SRC ../Mex/aw_getmarkers_ex.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_addmarkers SRC ../Mex/aw_addmarkers.cpp LINK_TO mexcommon CoreStatic)
matlab_add_mex(NAME aw_getplugininfo SRC ../Mex/aw_getplugininfo.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getpluginio SRC ../Mex/aw_getpluginio.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getfileinfo SRC ../Mex/aw_getfileinfo.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_sendcommand SRC ../Mex/aw_sendcommand.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_gettriggers SRC ../Mex/aw_gettriggers.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getcapture SRC ../Mex/aw_getcapture.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_captureicapanel SRC ../Mex/aw_captureicapanel.cpp LINK_TO mexcommon) 
matlab_add_mex(NAME AwSendMessage SRC ../Mex/AwSendMessage.cpp LINK_TO mexcommon)
matlab_add_mex(NAME AwIsProcessTerminated SRC ../Mex/AwIsProcessTerminated.cpp LINK_TO mexcommon)


INSTALL(TARGETS aw_getdata DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getdata_ex DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getmarkers DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getmarkers_ex DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_addmarkers DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getplugininfo DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getfileinfo DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getpluginio DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_sendcommand DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_gettriggers DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getcapture DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_captureicapanel DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS AwSendMessage DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS AwIsProcessTerminated DESTINATION  ${MEX_INSTALL_DIR})
