# Correlation Plugin
project(Correlation)
add_definitions("-DQT_PLUGIN")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
ADD_DEFINITIONS("-DARMA_DONT_USE_WRAPPER -DARMA_USE_BLAS -DARMA_USE_LAPACK")
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
target_link_libraries(Correlation AwCoreLib AwProcessLib AwReadWriteLib Qt5::Core Qt5::Gui ${OpenBLAS_LIB} ${QWT_LIBRARY})









