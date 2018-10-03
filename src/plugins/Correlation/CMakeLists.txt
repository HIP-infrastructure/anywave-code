# Correlation Plugin
project(Correlation)
add_definitions("-DQT_PLUGIN")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(SRCS
   settings.cpp
   correlation.cpp
   outputwidget.cpp
   maingui.cpp)
   
SET(MOCS settings.h correlation.h outputwidget.h maingui.h)
SET(UIS settings.ui outputwidget.ui maingui.ui)

QT5_WRAP_CPP(COR_MOCS ${MOCS})
QT5_WRAP_UI(COR_UIS ${UIS})

add_library(Correlation SHARED ${SRCS} ${COR_MOCS} ${COR_UIS})
qt5_use_modules(Correlation Core Gui Widgets)
IF (APPLE)
    target_link_libraries(Correlation AwCore AwGraphics AwProcess AwMath AwReadWrite
     AwWidgetLib Qt5::Core Qt5::Gui Qt5::Widgets ${ACCELERATE_LIBRARY} ${QWT_LIB}  ${MATIO_LIBRARIES})
ELSE()
      target_link_libraries(Correlation AwCore AwProcess AwReadWrite Qt5::Core Qt5::Gui ${OpenBLAS_LIB} 
${QWT_LIBRARY} ${MATIO_LIBRARIES})
INSTALL(TARGETS Correlation DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)
ENDIF()








