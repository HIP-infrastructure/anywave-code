# ICA Plugin
project(ICA)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

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
IF (APPLE)
	target_link_libraries(ICA AwCoreLib AwFilteringLib AwProcessLib AwHDF5Lib AwMATLABLib /Users/bruno/HDF5/lib/libhdf5.a /Users/bruno/HDF5/lib/libhdf5_hl.a  Qt5::Core Qt5::Gui ${ACCELERATE_LIBRARY})
ELSE()
      target_link_libraries(ICA AwCore AwFiltering AwProcess AwHDF5 AwMATLAB Qt5::Core Qt5::Gui ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${OpenBLAS_LIB})
ENDIF()








