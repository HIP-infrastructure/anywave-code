# dont build sobi algorthm on mac because Clang compiler does not support OpenMP
SET(INFOMAX  
   infomax/ICAInfomax.cpp
   infomax/infomax_algo.cpp
   infomax/r250.c 
   infomax/randlcg.c)
   
SET(SRCS
   ICASettings.cpp
   ica.cpp
   ICAAlgorithm.cpp)

SET(MOCS ICASettings.h
   ica.h ICAAlgorithm.h infomax/ICAInfomax.h)

SET(UIS ICASettings.ui)

QT5_WRAP_CPP(ICA_MOCS ${MOCS})
QT5_WRAP_UI(ICA_UIS ${UIS})
qt5_add_resources(RES Resource.qrc) 

add_library(ICA SHARED ${SRCS} ${INFOMAX} ${ICA_MOCS} ${ICA_UIS} ${RES})
qt5_use_modules(ICA Core Gui)

target_link_libraries(ICA AwCore AwFiltering AwWidget AwProcess AwUtilities AwLayout AwHDF5 AwMATLAB Qt5::Core Qt5::Gui ${ARMA_LIB} "-framework Accelerate")
INSTALL(TARGETS ICA DESTINATION ${PLUGIN_INSTALL_DIR})