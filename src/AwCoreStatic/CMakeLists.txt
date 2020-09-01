# CoreLib
project(AwCoreStatic)

add_definitions("-DAW_USE_CORE_STATIC")
SET(QT_DIR ${QT_STATIC_DIR})
set (QT_STATIC_SUFFIX "lib/cmake")
 message(STATUS "Found qt5 dir: ${QT_DIR}")
find_path(QT_DIR HINTS "${QT_DIR}" PATH_SUFFIXES "${QT_STATIC_SUFFIX}")
if (NOT QT_DIR)
   message(FATAL_ERROR "Could not find static Qt5 dir")
else()
   message(STATUS "Found static Qt5 dir: ${QT_DIR}")
endif()

set (CMAKE_PREFIX_PATH ${QT_DIR})

SET(QT_STATIC_LIBS ${QT_DIR}/lib/libqt5core.a  ${QT_DIR}/lib/libqt5network.a ${QT_DIR}/lib/libqtpcre2.a)
include_directories(${QT_DIR}/include ${QT_DIR}/include/QtCore ${QT_DIR}/include/QtNetwork)



SET(HEADERS     
    ../../include/AwDataClient.h 
    ../../include/AwMEGSensorManager.h 
    ../../include/AwSensor.h 
    ../../include/AwDataBuffer.h)

SET(SRC     ../Core/AwChannel.cpp 
    ../Core/AwDataClient.cpp 
    ../Core/AwMarker.cpp 
    ../Core/AwMEGSensorManager.cpp 
    ../Core/AwSensor.cpp 
    ../Core/AwDataBuffer.cpp)

QT5_WRAP_CPP(MOC ${HEADERS})
add_library(AwCoreStatic ${MOC} ${SRC})

#qt5_use_modules(AwCoreStatic Core)
#TARGET_LINK_LIBRARIES(AwCoreStatic ${QT_DIR}/lib/libQt5Core.a)

IF (APPLE)
TARGET_LINK_LIBRARIES(AwCoreStatic ${QT_STATIC_LIBS} "-framework IOKit -framework AppKit -framework SystemConfiguration -framework Security -lz"
    "-framework GSS") 
INSTALL(TARGETS AwCoreStatic DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwCoreStatic DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()






