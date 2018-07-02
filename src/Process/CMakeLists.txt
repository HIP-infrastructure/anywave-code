# AwProcessLib
project(AwProcess)
add_definitions("-DAW_BUILD_PROCESS_LIB")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")


SET(SRC AwProcessIO.cpp AwProcessLib.cpp AwProcessGUIWidget.cpp)
SET(HEADERS ../../include/AwProcess.h
../../include/AwProcessInterface.h
../../include/AwProcessIO.h
../../include/AwProcessLib.h
../../include/process/AwProcessGUIWidget.h)

QT5_WRAP_CPP(MOC ${HEADERS})

ADD_LIBRARY(AwProcess SHARED ${SRC} ${MOC})
QT5_USE_MODULES(AwProcess Core Widgets)
TARGET_LINK_LIBRARIES(AwProcess AwCore AwReadWrite Qt5::Core Qt5::Widgets)


