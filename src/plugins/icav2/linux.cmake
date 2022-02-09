
INCLUDE_DIRECTORIES(./matlab_common)

set(SOBI sobi/eigStandard.cpp
   sobi/ICASobi.cpp 
   sobi/mtimes.cpp
   sobi/pca.cpp
   sobi/schur.cpp
   sobi/sobi_coder_data.cpp
   sobi/sobi_coder_initialize.cpp
   sobi/sobi_coder_rtwutil.cpp
   sobi/sobi_coder_terminate.cpp
   sobi/sobi_coder.cpp
   sobi/sort.cpp
   sobi/sortAscendLE.cpp
   sobi/sqrt.cpp
   sobi/xdhseqr.cpp
   sobi/xdlanv2.cpp
   sobi/xnrm2.cpp
   sobi/xzggbal.cpp
   sobi/xzhgeqz.cpp
   sobi/xzlarf.cpp
   sobi/xzlarfg.cpp
   sobi/xzlartg.cpp
   sobi/xztgevc.cpp)

SET(SOBI_H sobi/ICASobi.h)

SET(INFOMAX infomax/ICAInfomax.cpp
 infomax/infomax_algo.cpp
  infomax/r250.c 
  infomax/randlcg.c)

SET(MATLAB matlab_common/pinv.cpp
matlab_common/rot90.cpp 
matlab_common/rt_nonfinite.cpp 
matlab_common/rtGetInf.cpp
matlab_common/rtGetNaN.cpp
matlab_common/svd.cpp)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/armadillo/include)

IF(USE_MKL)
INCLUDE_DIRECTORIES(${MKL_ROOT}/include)
ENDIF()

SET(SRCS
   ICASettings.cpp
   ica.cpp
   ICAAlgorithm.cpp
	${SOBI} ${INFOMAX} ${MATLAB})

 SET(MOCS ICASettings.h
    ica.h ICAAlgorithm.h)

SET(UIS ICASettings.ui)

QT5_WRAP_CPP(ICA_MOCS ${MOCS} ${SOBI_H})
QT5_WRAP_UI(ICA_UIS ${UIS})
qt5_add_resources(RES Resource.qrc) 

add_library(ICA SHARED ${SRCS} ${ICA_MOCS} ${ICA_UIS} ${RES})
qt5_use_modules(ICA Core Gui)
target_link_libraries(ICA AwCore AwFiltering AwProcess AwHDF5 AwUtilities AwLayout AwMATLAB Qt5::Core Qt5::Gui ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${BLAS_LIBRARIES} gomp)
INSTALL(TARGETS ICA DESTINATION ${PLUGIN_INSTALL_DIR})