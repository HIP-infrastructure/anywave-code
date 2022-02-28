SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -I/usr/include -L/usr/lib -w -fPIC -O3 -fpermissive -DNDEBUG -mmacosx-version-min=10.13")

MESSAGE(STATUS "Building macos intel64 with Qt5")
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

SET(APP_BUNDLE "AnyWave.app")
SET(APP_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/MacOS")
SET(LIB_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Frameworks")
SET(PLUGIN_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins")
SET(MEX_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins/MATLAB/AnyWave")


IF(USE_MKL) # Must be set if the system has oneapi mkl installed
 # We assume intel mkl is installed as part of Intel ONEAPI and the setvars.sh script has been executed before
   SET(MKL_ARCH intel64)
   SET(MKL_LINK dynamic)
   SET(MKL_THREADING intel_thread)
   SET(MKL_INTERFACE lp64)
   find_package(MKL REQUIRED)
   MESSAGE(STATUS "root: ${MKL_ROOT}")
   MESSAGE(STATUS "MKL Include: ${MKL_INCLUDE}")
   INCLUDE_DIRECTORIES(${MKL_INCLUDE})
   MESSAGE(STATUS "MKL core: ${MKL_CORE}")
   MESSAGE(STATUS "MKL link: ${MKL_THREAD_LIB} ${MKL_SUPP_LINK}")
   MESSAGE(STATUS "MKL link: ${MKL_LIBRARIES}")

   SET(BLAS_LIBRARIES "-L${MKL_ROOT}/lib  -Wl,-rpath,${MKL_ROOT}/lib ${MKL_THREAD_LIB} -lmkl_intel_lp64 -lmkl_core -lmkl_intel_thread -lpthread -lm -ldl")
   add_definitions("-DMKL")
ELSE()
    MESSAGE(STATUS "Using Accelerate Framework for BLAS")
    SET(BLAS_LIBRARIES "-framework Accelerate")
endif()

# Armadillo
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/armadillo-macos/include)
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/armadillo-macos/lib)
SET(ARMA_LIB armadillo)

SET(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
SET(APPLE_ID "Apple Development: thechewie13@gmail.com (MY3J8AN9H6)")