# Spectral
project(Spectral)
add_definitions(-DQT_PLUGIN -DMKL)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

IF(USE_MKL)
IF(APPLE)
INCLUDE_DIRECTORIES(${MKL_INCLUDE_DIRS})
ELSE()
INCLUDE_DIRECTORIES(${MKL_ROOT}/include)
ENDIF()
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/sigpack)

SET(SRCS
FFTIterations.cpp InputMarkersDial.cpp PlotWidget.cpp SIWidget.cpp Spectral.cpp)

SET(MOCS
Spectral.h InputMarkersDial.h PlotWidget.h SIWidget.h) 

SET(UIS PlotWidget.ui InputMarkersDial.ui SIWidget.ui)

qt5_add_resources(RES Resource.qrc)

qt5_wrap_cpp(Spectral_MOCS ${MOCS})
QT5_WRAP_UI(Spectral_UIS ${UIS})

add_definitions(-DQCUSTOMPLOT_USE_LIBRARY)

add_library(Spectral SHARED ${SRCS} ${Spectral_MOCS} ${Spectral_UIS} ${RES})
qt5_use_modules(Spectral Core Gui Widgets Concurrent)

IF(UNIX AND NOT APPLE)
target_link_libraries(Spectral AwCore AwProcess AwWidget AwMATLAB qcustomplot ${BLAS_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Concurrent)
ELSE()
target_link_libraries(Spectral AwCore AwProcess AwWidget AwMATLAB qcustomplot ${ARMA_LIB} ${FFTW3_LIBS} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Concurrent)
ENDIF()

INSTALL(TARGETS Spectral DESTINATION ${PLUGIN_INSTALL_DIR})





