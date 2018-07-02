# AutoMarking
project(AutoMarking)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(SRCS
automarking.cpp) 

SET(MOCS
automarking.h)
QT5_ADD_RESOURCES(AM_QRC automarking.qrc)
qt5_wrap_cpp(AM_MOCS ${MOCS})

add_library(AutoMarking SHARED ${SRCS} ${AM_MOCS} ${AM_QRC})
qt5_use_modules(AutoMarking Core Gui Widgets)
target_link_libraries(AutoMarking AwCore AwProcess AwReadWrite AwFiltering AwWidget Qt5::Core Qt5::Gui Qt5::Widgets)







