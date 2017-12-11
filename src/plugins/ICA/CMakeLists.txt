# ICA Plugin
project(ICA)
add_definitions("-DQT_PLUGIN")
ADD_DEFINITIONS("-DARMA_DONT_USE_WRAPPER -DARMA_USE_BLAS -DARMA_USE_LAPACK")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
   ICASettings.cpp
   infomax_algo.cpp
   ica.cpp
   r250.c
   randlcg.c)
   
 SET(MOCS ICASettings.h
    ica.h)

SET(UIS ICASettings.ui)

QT5_WRAP_CPP(ICA_MOCS ${MOCS})
QT5_WRAP_UI(ICA_UIS ${UIS})

add_library(ICA SHARED ${SRCS} ${ICA_MOCS} ${ICA_UIS})
qt5_use_modules(ICA Core Gui)
target_link_libraries(ICA AwCoreLib AwFilteringLib AwProcessLib AwHDF5Lib AwMATLABLib Qt5::Core Qt5::Gui ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${OpenBLAS_LIB})








