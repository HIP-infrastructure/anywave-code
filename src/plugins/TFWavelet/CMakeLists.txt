# TFWavelet
project(TFWavelet)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

IF(USE_MKL)
IF(APPLE)
INCLUDE_DIRECTORIES(${MKL_INCLUDE_DIRS})
ELSE()
INCLUDE_DIRECTORIES(${MKL_ROOT}/include)
ENDIF()
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/armadillo/include)

#MESSAGE("MKL INC: ${MKL_INCLUDE_DIRS}")
SET(SRCS display_settings.cpp settingsui.cpp TFPicker.cpp 
  TFPlot.cpp tf_settings.cpp TFWavelet2.cpp TFWidget.cpp
  TFColorMapWidget.cpp)

SET(MOCS display_settings.h settingsui.h TFPicker.h TFPlot.h tf_settings.h TFWavelet2.h 
   TFWidget.h TFColorMapWidget.h)

SET(UIS TFWidget.ui settingsui.ui TFColorMapWidget.ui)

qt5_wrap_cpp(TFW_MOCS ${MOCS})
QT5_WRAP_UI(TFW_UIS ${UIS})

add_library(TFWavelet SHARED ${SRCS} ${TFW_MOCS} ${TFW_UIS})
qt5_use_modules(TFWavelet Core Gui Widgets Concurrent)

target_link_libraries(TFWavelet AwProcess AwCore AwWidget Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Concurrent ${QWT_LIBRARY} ${BLAS_LIBRARIES})

INSTALL(TARGETS TFWavelet DESTINATION ${PLUGIN_INSTALL_DIR})





