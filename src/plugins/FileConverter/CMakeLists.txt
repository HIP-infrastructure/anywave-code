# FileConverter
project(FileConverter)

set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

SET(SRCS
fileconverter.cpp settings.cpp) 

SET(MOCS
fileconverter.h settings.h)

SET(UIS
settings.ui)

qt5_wrap_cpp(FC_MOCS ${MOCS})
QT5_WRAP_UI(FC_UIS ${UIS})

add_library(FileConverter SHARED ${SRCS} ${FC_MOCS} ${FC_UIS})
qt5_use_modules(FileConverter Core Gui Widgets)
target_link_libraries(FileConverter AwCore AwProcess AwReadWrite AwFiltering Qt5::Core Qt5::Gui Qt5::Widgets)
INSTALL(TARGETS FileConverter DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)







