
SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)
add_definitions("-w -fPIC -O3 -fpermissive -DNDEBUG -mmacosx-version-min=10.13") # clang flags
SET(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/AnyWave)

MESSAGE(STATUS "Building with Qt5")
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

find_package(MKL)
if(NOT MKL_FOUND)
    MESSAGE(STATUS "Using Accelerate Framework for BLAS")
    SET(BLAS_LIBRARIES "-framework Accelerate")
endif()

SET(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
SET(APPLE_ID "Apple Development: thechewie13@gmail.com (MY3J8AN9H6)")

SET(APP_BUNDLE "AnyWave.app")
SET(APP_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/MacOS")
SET(LIB_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Frameworks")
SET(PLUGIN_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins")
SET(MEX_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins/MATLAB/AnyWave")
SET(PLUGIN_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins")