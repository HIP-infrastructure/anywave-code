# AppendFiles
project(AppendFiles)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
#SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

SET(SRCS
appendfiles.cpp settings.cpp) 

SET(MOCS
appendfiles.h settings.h)

SET(UIS
settings.ui)

qt5_wrap_cpp(AF_MOCS ${MOCS})
QT5_WRAP_UI(AF_UIS ${UIS})

add_library(AppendFiles SHARED ${SRCS} ${AF_MOCS} ${AF_UIS})
qt5_use_modules(AppendFiles Core Gui Widgets)
target_link_libraries(AppendFiles AwCore AwProcess AwReadWrite AwFiltering Qt5::Core Qt5::Gui Qt5::Widgets)
INSTALL(TARGETS AppendFiles DESTINATION ${PLUGIN_INSTALL_DIR})







