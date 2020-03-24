# H2
cmake_minimum_required(VERSION 3.0)
# Build in release mode 
SET(CMAKE_BUILD_TYPE "Release")
project(H2)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
FIND_PACKAGE(AnyWave REQUIRED)
# Use the AnyWave cmake files to find other packages (MKL)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${AW_ROOT_DIR}/cmake)
SET(CMAKE_BUILD_TYPE "Release") # build in release mode
SET(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNEBUG")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing -w -fPIC -O4 -fpermissive")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing -w -fPIC -O3 -fpermissive -fopenmp")
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Svg REQUIRED)
# search for MKL
find_package(MKL)
if(NOT MKL_FOUND)
    find_package(OpenBLAS REQUIRED)
    SET(USE_MKL FALSE CACHE BOOL "Using MKL libraries")
    SET(BLAS_LIBRARIES ${OpenBLAS_LIB})
else()
    message(STATUS "MKL Found.")
    SET(USE_MKL TRUE CACHE BOOL "Using MKL libraries")
    INCLUDE_DIRECTORIES(${MKL_INCLUDE_DIRS})
    SET(BLAS_LIBRARIES ${MKL_LIBRARIES})
endif()
SET(CMAKE_INSTALL_RPATH ${AW_LIB_DIR})

SET(SRCS
${CMAKE_SOURCE_DIR}/h2.cpp
${CMAKE_SOURCE_DIR}/h2ui.cpp
${CMAKE_SOURCE_DIR}/H2ChannelPairItem.cpp
${CMAKE_SOURCE_DIR}/H2ChannelPair.cpp) 

SET(MOCS
${CMAKE_SOURCE_DIR}/h2.h
${CMAKE_SOURCE_DIR}/h2ui.h
${CMAKE_SOURCE_DIR}/H2ChannelPairItem.h)

SET(UIS
${CMAKE_SOURCE_DIR}/h2ui.ui)

qt5_wrap_cpp(H2_MOCS ${MOCS})
QT5_WRAP_UI(H2_UIS ${UIS})

add_library(H2 SHARED ${SRCS} ${H2_MOCS} ${H2_UIS})
qt5_use_modules(H2 Core Gui Widgets Concurrent Svg)

IF(APPLE)
	target_link_libraries(H2 ${AW_LIBRARIES} ${ACCELERATE_LIBRARY} z Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Concurrent)
ELSE()
    target_link_libraries(H2 ${AW_LIBRARIES} ${BLAS_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Concurrent)
ENDIF()
INSTALL(TARGETS H2 DESTINATION ${AW_PLUGINS_DIR})





