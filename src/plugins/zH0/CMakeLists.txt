# zH0
project(zH0)
add_definitions("-DQT_PLUGIN")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(SRCS
   zH0.cpp
   zH0GUI.cpp
   zH0TableModel.cpp)
 
SET(MOCS zH0.h  zH0GUI.h zH0TableModel.h)
SET(UIS zH0GUI.ui)

QT5_WRAP_CPP(COR_MOCS ${MOCS})
QT5_WRAP_UI(COR_UIS ${UIS})

add_library(zH0 SHARED ${SRCS} ${COR_MOCS} ${COR_UIS})
qt5_use_modules(zH0 Core Gui Widgets)
IF (APPLE)
    target_link_libraries(zH0 AwCore AwProcess  AwWidgetLib Qt5::Core Qt5::Gui Qt5::Widgets)
ELSE()
    target_link_libraries(zH0 AwCore AwProcess AwWidgetLib Qt5::Core Qt5::Gui)
INSTALL(TARGETS zH0 DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)
ENDIF()








