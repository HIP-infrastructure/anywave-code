cmake_minimum_required(VERSION 3.13)
project(matlab_support)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE Release)

SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

# We assume that this project is within the AnyWave projects
# So AnyWave modules are located in ../src 

find_package(Matlab COMPONENTS MX_LIBRARY ENG_LIBRARY)
if(Matlab_FOUND)
message(STATUS "MATLAB Support added. Root dir is ${Matlab_ROOT_DIR}")
else(Matlab_FOUND)
message(STATUS "MATLAB not found. Skipping MATLAB Support.")
return()
endif(Matlab_FOUND)
MESSAGE(STATUS "Building old mex files...")

include_directories(${CMAKE_SOURCE_DIR}/../include)
include_directories(${Matlab_INCLUDE_DIRS})

if (APPLE)
    include(mac.cmake)
ELSE(APPLE)
    include(linux.cmake)
ENDIF(APPLE)

#if(NOT APPLE)
#    set(MEX_INSTALL_DIR /usr/local/AnyWave/Plugins/MATLAB/AnyWave)
#else(NOT APPLE)
#    set(MEX_INSTALL_DIR ${CMAKE_SOURCE_DIR}../build/AnyWave.app/Contents/Plugins/MATLAB/AnyWave)
#endif(NOT APPLE)

#if(QT_STATIC_DIR) 
# build AwCore lib static
#add_definitions("-DAW_USE_CORE_STATIC -fPIC -O2")
add_definitions("-fPIC -O3")
find_package(Qt5 COMPONENTS Core REQUIRED)

SET(HEADERS     
    ${CMAKE_SOURCE_DIR}/../include/AwDataClient.h 
    ${CMAKE_SOURCE_DIR}/../include/AwMEGSensorManager.h 
    ${CMAKE_SOURCE_DIR}/../include/AwSensor.h 
   ${CMAKE_SOURCE_DIR}/../include/AwDataBuffer.h)

SET(SRC     ${CMAKE_SOURCE_DIR}/../src/Core/AwChannel.cpp 
     ${CMAKE_SOURCE_DIR}/../src/Core/AwDataClient.cpp 
     ${CMAKE_SOURCE_DIR}/../src/Core/AwMarker.cpp 
     ${CMAKE_SOURCE_DIR}/../src/Core/AwMEGSensorManager.cpp 
     ${CMAKE_SOURCE_DIR}/../src/Core/AwSensor.cpp 
     ${CMAKE_SOURCE_DIR}/../src/Core/AwDataBuffer.cpp)

QT5_WRAP_CPP(MOC ${HEADERS})

add_library(AwCoreStatic  ${MOC} ${SRC})
qt5_use_modules(AwCoreStatic Core)

#build mex common
add_library(mexcommon STATIC ${CMAKE_SOURCE_DIR}/../src/Mex/common.cpp)
qt5_use_modules(mexcommon Core Network)
if(APPLE)
    set(T_LIBS ${LIBS} "-framework IOKit -framework AppKit -framework SystemConfiguration -framework Security -lz -framework GSS")
else(APPLE)  # Linux
# core : -licui18n -licuuc -licudata -lgthread-2.0 -lglib-2.0 -lpcre2-16 -ldl
endif(APPLE)

#target_link_libraries(AwCoreStatic ${T_LIBS})
#target_link_libraries(mexcommon ${T_LIBS} mx)

matlab_add_mex(NAME aw_getdata SRC  ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getdata.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getdata_ex SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getdataex.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getmarkers SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getmarkers.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getmarkers_ex ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getmarkers_ex.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_addmarkers SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_addmarkers.cpp LINK_TO mexcommon AwCoreStatic)
matlab_add_mex(NAME aw_getplugininfo SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getplugininfo.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getpluginio SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getpluginio.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getfileinfo SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getfileinfo.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_sendcommand SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_sendcommand.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_gettriggers SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_gettriggers.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_getcapture SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getcapture.cpp LINK_TO mexcommon)
matlab_add_mex(NAME aw_captureicapanel SRC ${CMAKE_SOURCE_DIR}/../src/Mex/aw_captureicapanel.cpp LINK_TO mexcommon) 
matlab_add_mex(NAME AwSendMessage SRC ${CMAKE_SOURCE_DIR}/../src/Mex/AwSendMessage.cpp LINK_TO mexcommon)
matlab_add_mex(NAME AwIsProcessTerminated SRC ${CMAKE_SOURCE_DIR}/../src/Mex/AwIsProcessTerminated.cpp LINK_TO mexcommon)
INSTALL(TARGETS aw_getdata DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getdata_ex DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getmarkers DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getmarkers_ex DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getplugininfo DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getpluginio DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getfileinfo DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_sendcommand DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_gettriggers DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_getcapture DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS aw_captureicapanel DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS AwSendMessage DESTINATION  ${MEX_INSTALL_DIR})
INSTALL(TARGETS AwIsProcessTerminated DESTINATION  ${MEX_INSTALL_DIR})

else(QT_STATIC_DIR)
find_package(Qt5 COMPONENTS Core Network REQUIRED)
if(NOT Qt5Core_FOUND OR NOT Qt5Network_FOUND)
    message(FATAL_ERROR "Qt5 Static modules not found")
endif()
MESSAGE(STATUS "Qt version is ${Qt5Core_VERSION}")
add_definitions("-fPIC -O2")
add_library(mexcommon STATIC ${CMAKE_SOURCE_DIR}/../src/Mex/common.cpp)
qt5_use_modules(mexcommon Core Network)

SET(LIB_DIR ${Matlab_ROOT_DIR}/bin/glnxa64)
message(STATUS "MATLAB dir: ${Matlab_ROOT_DIR}")
LINK_DIRECTORIES($LIB_DIR})
#target_link_libraries(mexcommon Qt5::Core Qt5::Network)
SET(T_LINK "-L${LIB_DIR} -lQt5Core.so.5.5.1 -lQt5Network.so.5.5.1")
target_link_libraries(mexcommon "-L${LIB_DIR} -lQt5Core.so.5.5.1 -lQt5Network.so.5.5.1")
matlab_add_mex(NAME aw_getdata SRC  ${CMAKE_SOURCE_DIR}/../src/Mex/aw_getdata.cpp LINK_TO mexcommon "-l${LIBS_DIR}/libQt5Core")

#SET_TARGET_PROPERTIES(aw_getdata PROPERTIES INSTALL_RPATH /usr/lib/x86_64-linux-gnu:/usr/local/AnyWave/lib)
INSTALL(TARGETS aw_getdata DESTINATION  ${MEX_INSTALL_DIR})

endif(QT_STATIC_DIR)

# Shared lib module to connect to Matlab
set(AwMatlabSupport_MOC_H ${CMAKE_SOURCE_DIR}/../src/MatlabSupport/AwMatlabSupport.h)
QT5_WRAP_CPP(AwMatlabSupport_MOC_SRCS ${AwMatlabSupport_MOC_H})

add_library(AwMatlabSupport SHARED ${CMAKE_SOURCE_DIR}/../src/MatlabSupportAwMatlabSupport.cpp ${AwMatlabSupport_MOC_SRCS})
qt5_use_modules(AwMatlabSupport Core)
target_link_libraries(AwMatlabSupport ${Matlab_MX_LIBRARY}  ${Matlab_ENG_LIBRARY} Qt5::Core)

IF (APPLE)
INSTALL(TARGETS AwMatlabSupport DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwMatlabSupport DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()