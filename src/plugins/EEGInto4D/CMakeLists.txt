# EEGInto4D
cmake_minimum_required(VERSION 3.0)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

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
target_link_libraries(EEGInto4D AwCore AwReadWrite AwUtilities Qt5::Core Qt5::Gui Qt5::Widgets)
INSTALL(TARGETS EEGInto4D DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)









