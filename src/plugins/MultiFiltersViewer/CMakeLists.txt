# MFV
project(MFV)
add_definitions("-DQT_PLUGIN")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(SRCS
   MFV.cpp
   MFVGUI.cpp
   MFVTableModel.cpp)
 
SET(MOCS MFV.h  MFVGUI.h MFVTableModel.h)
SET(UIS MFVGUI.ui)

QT5_WRAP_CPP(COR_MOCS ${MOCS})
QT5_WRAP_UI(COR_UIS ${UIS})

add_library(MFV SHARED ${SRCS} ${COR_MOCS} ${COR_UIS})
qt5_use_modules(MFV Core Gui Widgets)
IF (APPLE)
    target_link_libraries(MFV AwCore AwProcess  AwWidgetLib Qt5::Core Qt5::Gui Qt5::Widgets)
ELSE()
    target_link_libraries(MFV AwCore AwProcess AwWidgetLib Qt5::Core Qt5::Gui)
INSTALL(TARGETS MFV DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)
ENDIF()








