# TFWavelet
project(TFWavelet)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIR})
LINK_DIRECTORIES(${QWT_LIBRARY})

SET(SRCS display_settings.cpp settingsui.cpp TFPicker.cpp TFPlot.cpp tf_settings.cpp TFWavelet2.cpp TFWidget.cpp)

SET(MOCS display_settings.h settingsui.h TFPicker.h TFPlot.h tf_settings.h TFWavelet2.h TFWidget.h)

SET(UIS TFWidget.ui settingsui.ui)

qt5_wrap_cpp(TFW_MOCS ${MOCS})
QT5_WRAP_UI(TFW_UIS ${UIS})

add_library(TFWavelet SHARED ${SRCS} ${TFW_MOCS} ${TFW_UIS})
qt5_use_modules(TFWavelet Core Gui Widgets Concurrent)

IF(APPLE)
	target_link_libraries(TFWavelet ${AW_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Concurrent ${QWT_LIBRARIES})
ELSE()
    target_link_libraries(TFWavelet ${AW_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Concurrent ${QWT_LIBRARIES} ${BLAS_LIBRARIES})
ENDIF()
INSTALL(TARGETS TFWavelet DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)




