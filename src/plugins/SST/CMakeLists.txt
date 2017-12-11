# SEEG Spatio Temporal
project(SST)
add_definitions("-DQT_PLUGIN -DARMA_DONT_USE_WRAPPER")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS  SEEGPlot.cpp Settings.cpp SST.cpp SSTWidget.cpp)

SET(MOCS SEEGPlot.h Settings.h SST.h SSTWidget.h)

SET(UIS Settings.ui SSTWidget.ui)

QT5_WRAP_CPP(SST_MOCS ${MOCS})
QT5_WRAP_UI(SST_UIS ${UIS})


add_library(SST SHARED ${SRCS} ${SST_MOCS} ${SST_UIS})
qt5_use_modules(SST Core Gui Widgets)
target_link_libraries(SST AwCoreLib AwProcessLib AwMathLib AwWidgetLib AwGraphicsLib Qt5::Core Qt5::Gui Qt5::Widgets openblas ${FFTW3_LIBRARY} ${QWT_LIBRARY})









