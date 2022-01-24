#SET(ARMADILLO_INCLUDE /users/bruno/armadillo-9.400.4/include)
#INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE} ./matlab_common)

INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE} ./matlab_common)

# dont build sobi algorthm on mac because Clang compiler does not support OpenMP
FILE(GLOB INFOMAX infomax/*.cpp)
FILE(GLOB MATLAB matlab_common/*.cpp)

SET(SRCS
   ICASettings.cpp
   ica.cpp
   ICAAlgorithm.cpp
   ${INFOMAX} ${MATLAB})

SET(MOCS ICASettings.h
   ica.h ICAAlgorithm.h)

SET(UIS ICASettings.ui)

QT5_WRAP_CPP(ICA_MOCS ${MOCS})
QT5_WRAP_UI(ICA_UIS ${UIS})
qt5_add_resources(RES Resource.qrc) 

add_library(ICA SHARED ${SRCS} ${ICA_MOCS} ${ICA_UIS} ${RES})
qt5_use_modules(ICA Core Gui)

target_link_libraries(ICA AwCore AwFiltering AwWidget AwProcess AwUtilities AwLayout AwHDF5 AwMATLAB Qt5::Core Qt5::Gui ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} "-framework Accelerate")

INSTALL(TARGETS ICA DESTINATION ${PLUGIN_INSTALL_DIR})