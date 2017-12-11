# AwProcessLib
project(AwProcessLib)
add_definitions("-DAW_BUILD_PROCESS_LIB")
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)

SET(AwProcessLib_MOC_HEADERS 
     ${CMAKE_SOURCE_DIR}/include/AwProcessInterface.h
	 ${CMAKE_SOURCE_DIR}/include/process/AwProcessGUIWidget.h)


 QT5_WRAP_CPP(AwProcessLib_MOC_SRCS ${AwProcessLib_MOC_HEADERS})
 ADD_LIBRARY(AwProcessLib SHARED AwProcessIO.cpp AwProcessLib.cpp AwProcessGUIWidget.cpp ${AwProcessLib_MOC_SRCS})
 QT5_USE_MODULES(AwProcessLib Core Widgets)
 TARGET_LINK_LIBRARIES(AwProcessLib AwCoreLib AwReadWriteLib Qt5::Core Qt5::Widgets)


