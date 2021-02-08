# AwProcessLib
project(AwProcess)
add_definitions("-DAW_BUILD_PROCESS_LIB")

SET(SRC AwProcessIO.cpp AwProcessLib.cpp AwProcessGUIWidget.cpp AwProcessOutputWidget.cpp)
SET(HEADERS ../../include/AwProcess.h
../../include/AwProcessInterface.h
../../include/AwProcessIO.h
../../include/AwProcessLib.h
../../include/process/AwProcessGUIWidget.h
../../include/process/AwProcessOutputWidget.h)

QT5_WRAP_CPP(MOC ${HEADERS})

ADD_LIBRARY(AwProcess SHARED ${SRC} ${MOC})
QT5_USE_MODULES(AwProcess Core Widgets)
TARGET_LINK_LIBRARIES(AwProcess AwCore AwReadWrite AwFiltering Qt5::Core Qt5::Widgets)

IF (APPLE)
INSTALL(TARGETS AwProcess DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwProcess DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()





