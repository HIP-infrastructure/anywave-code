# ICA Plugin
project(ICA)
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
IF (APPLE)
target_link_libraries(ICA AwCore AwFiltering AwProcess AwUtilities AwLayout AwHDF5 AwMATLAB Qt5::Core Qt5::Gui ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} "-framework Accelerate")
ELSE()
target_link_libraries(ICA AwCore AwFiltering AwProcess AwHDF5 AwUtilities AwLayout AwMATLAB Qt5::Core Qt5::Gui ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${BLAS_LIBRARIES})
ENDIF()
INSTALL(TARGETS ICA DESTINATION ${PLUGIN_INSTALL_DIR})









