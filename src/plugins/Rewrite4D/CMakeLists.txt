# Rewrite 4D
cmake_minimum_required(VERSION 3.0)
# Build in release mode 
SET(CMAKE_BUILD_TYPE "Release")
add_definitions("-fPIC -O3")
project(Rewrite4D)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNEBUG")

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

if (APPLE)
   set(LIBRARY_OUTPUT_PATH ${SDK_ROOT}/bin/Anywave_Plugins)
else()
   set(LIBRARY_OUTPUT_PATH ${SDK_ROOT}/bin/Plugins)
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

LINK_DIRECTORIES(${SDK_ROOT}/lib)
INCLUDE_DIRECTORIES(${SDK_ROOT}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_DIR})

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

SET(SRCS ChooseMarker.cpp rewrite4d.cpp rewriteui.cpp)

SET(MOCS ChooseMarker.h rewrite4d.h rewriteui.h)

SET(UIS ChooseMarker.ui rewriteui.ui)

qt5_wrap_cpp(Rewrite_MOCS ${MOCS})
qt5_wrap_ui(Rewrite_UIS ${UIS})

add_library(Rewrite4D SHARED ${SRCS} ${Rewrite_MOCS} ${Rewrite_UIS})
target_link_libraries(Rewrite4D AwCoreLib AwProcessLib AwReadWriteLib Qt5::Core Qt5::Gui Qt5::Widgets)






