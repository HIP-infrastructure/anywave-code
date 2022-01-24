# ICA Plugin
project(ICA)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

IF(APPLE)
SET(ARMADILLO_INCLUDE /users/bruno/armadillo-9.400.4/include)
ENDIF()
INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE} ./matlab_common)

FILE(GLOB SOBI sobi/*.cpp)
FILE(GLOB INFOMAX infomax/*.cpp)
FILE(GLOB MATLAB matlab_common/*.cpp)

SET(SRCS
   ICASettings.cpp
   ica.cpp
   ICAAlgorithm.cpp
	${SOBI} ${INFOMAX} ${MATLAB})

 SET(MOCS ICASettings.h
    ica.h ICAAlgorithm.h)

SET(UIS ICASettings.ui)

QT5_WRAP_CPP(ICA_MOCS ${MOCS})
QT5_WRAP_UI(ICA_UIS ${UIS})
qt5_add_resources(RES Resource.qrc) 

add_library(ICA SHARED ${SRCS} ${ICA_MOCS} ${ICA_UIS} ${RES})
qt5_use_modules(ICA Core Gui)
IF (APPLE)
target_link_libraries(ICA AwCore AwFiltering AwWidget AwProcess AwUtilities AwLayout AwHDF5 AwMATLAB Qt5::Core Qt5::Gui ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} "-framework Accelerate")
ELSE()
target_link_libraries(ICA AwCore AwFiltering AwProcess AwHDF5 AwUtilities AwLayout AwMATLAB Qt5::Core Qt5::Gui ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${BLAS_LIBRARIES})
ENDIF()
INSTALL(TARGETS ICA DESTINATION ${PLUGIN_INSTALL_DIR})









