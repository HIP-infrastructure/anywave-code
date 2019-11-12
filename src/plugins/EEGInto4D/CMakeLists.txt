# EEGInto4D
cmake_minimum_required(VERSION 3.0)
# Build in release mode 
SET(CMAKE_BUILD_TYPE "Release")
project(EEGInto4D)
add_definitions("-fPIC -O3")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNEBUG")

set(CMAKE_MODULE_PATH ${SDK_ROOT}/cmake)

SET(SDK_FOUND 0)
IF( "$ENV{AWSDK}" STREQUAL "" )
    MESSAGE(STATUS "AWSDK environment variable not set." )
    MESSAGE(STATUS "On Linux or Mac OS X this can be done in your user .bashrc file by appending the corresponding line, e.g:" )
    MESSAGE(STATUS "export AWSDK=/home/user/dev/SDK" )
    return()
ELSE()
   SET(SDK_FOUND 1)
   SET(SDK_ROOT $ENV{AWSDK})
   MESSAGE(STATUS "AWSDK found: ${SDK_ROOT}")
ENDIF()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

LINK_DIRECTORIES(${SDK_ROOT}/lib)
INCLUDE_DIRECTORIES(${SDK_ROOT}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_DIR})

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

if (APPLE)
   set(LIBRARY_OUTPUT_PATH ${SDK_ROOT}/bin/Anywave_Plugins)
else()
   set(LIBRARY_OUTPUT_PATH ${SDK_ROOT}/bin/Plugins)
endif()

SET(SRCS
eeginto4d.cpp settings.cpp) 

SET(MOCS
eeginto4d.h settings.h)

SET(UIS
settings.ui)

qt5_wrap_cpp(EEG_MOCS ${MOCS})
QT5_WRAP_UI(EEG_UIS ${UIS})

add_library(EEGInto4D SHARED ${SRCS} ${EEG_MOCS} ${EEG_UIS})
qt5_use_modules(EEGInto4D Core Gui Widgets)
target_link_libraries(EEGInto4D AwCoreLib AwProcessLib AwReadWriteLib Qt5::Core Qt5::Gui Qt5::Widgets)







