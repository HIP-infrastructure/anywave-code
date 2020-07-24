cmake_minimum_required(VERSION 3.6) 
project(AnyWave)
# detecting compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
   MESSAGE(STATUS "Apple clang compiler detected")
   add_definitions("-w -fPIC -O3 -fpermissive") # clang flags
   #SET(CLANG ON)
# apple clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
# gcc
   MESSAGE(STATUS "GCC compiler detected")
   add_definitions("-w -fPIC -O3 -fpermissive -fopenmp -DNDEBUG") # g++ flags
   SET(GCC ON)
endif()

SET(CMAKE_BUILD_TYPE "Release") # build in release mode
SET(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# default install path
IF(UNIX AND NOT APPLE)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/AnyWave)
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# PATH to resources
SET(AW_RES_DIR ${CMAKE_SOURCE_DIR}/resources)

SET(PLUGIN_OUT ${PROJECT_BINARY_DIR}/Plugins)

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
if(UNIX AND NOT APPLE)
find_package(Qt5X11Extras REQUIRED)
ENDIF()

MESSAGE(STATUS "Qt version is ${Qt5Core_VERSION}")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

SET(ARMADILLO_INCLUDE ${CMAKE_SOURCE_DIR}/armadillo/include)
#SET(ARMADILLO_INCLUDE /users/bruno/armadillo-9.400.4/include)

#INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE})
find_package(Qwt REQUIRED)
#INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIR})
#LINK_DIRECTORIES(${QWT_LIBRARY})
message(STATUS "Qwt folders are ${QWT_INCLUDE_DIR} ${QWT_LIBRARY}")
#include_directories(${ARMADILLO_INCLUDE_DIRS})

## use the static libs of HDF5
FIND_PACKAGE(HDF5 REQUIRED COMPONENTS C HL)
#INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})
#LINK_DIRECTORIES(${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})
message(STATUS "HDF5 are ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES}")


find_package(MATIO REQUIRED)
if(MATIO_FOUND)
    message(STATUS "Found Matio lib: ${MATIO_LIBRARIES}")
    INCLUDE_DIRECTORIES(MATIO_INCLUDE_DIRS)
endif()


# search for MKL 
find_package(MKL)
if(NOT MKL_FOUND AND APPLE)
    SET(BLAS_LIBRARIES "-framework Accelerate")
ENDIF()
IF(NOT MKL_FOUND AND NOT APPLE)
    find_package(OpenBLAS REQUIRED)
    SET(BLAS_LIBRARIES ${OpenBLAS_LIB})
ENDIF()

IF(MKL_FOUND)
   message(STATUS "MKL Found.")
   #LINK_DIRECTORIES(${MKL_LIB_DIRS})
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
   SET(USE_MKL TRUE CACHE BOOL "Using MKL libraries")
   #INCLUDE_DIRECTORIES(${MKL_INCLUDE_DIRS})
   SET(BLAS_LIBRARIES "-lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl")
   message(STATUS "MLK libs : ${MKL_LIBRARIES}")
   message(STATUS "link flags: ${BLAS_LIBRARIES}")
endif()

# Add build flags - Armadillo will use openBLAS on Linux, Accelerate Framework on Mac OS and MKL on Windows.
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -DARMA_DONT_USE_WRAPPER -DARMA_USE_BLAS -DARMA_USE_LAPACK -DAW_DISABLE_EPOCHING")
IF(USE_MKL)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMKL")
ENDIF()

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

IF(UNIX AND NOT APPLE)
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN/lib'")
ENDIF()

IF(APPLE)
   SET(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
  # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='@executable_path/../Frameworks'")
  #set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDIF()

IF(APPLE)
   SET(APP_BUNDLE "AnyWave.app")
   SET(APP_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/MacOS")
   SET(LIB_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Frameworks")
   SET(PLUGIN_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins")
   SET(MEX_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins/MATLAB/AnyWave")
   SET(PLUGIN_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins")
ELSE()
   SET(MEX_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Plugins/MATLAB/AnyWave")
   SET(PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Plugins")
ENDIF()


# AnyWave SDK libraries
add_subdirectory(src/Core)
# add core static
if(BUILD_STATIC_CORE AND QT_STATIC_DIR)
add_subdirectory(src/AwCoreStatic)
endif()
#add_subdirectory(src/AwEpochLib)
add_subdirectory(src/AwMontageLib)
add_subdirectory(src/Filtering)
add_subdirectory(src/Utilities)
add_subdirectory(src/Graphics)
add_subdirectory(src/Process)
add_subdirectory(src/Widgets)
add_subdirectory(src/ReadWrite)
add_subdirectory(src/HDF5)
add_subdirectory(src/Layouts)
add_subdirectory(src/Math)
add_subdirectory(src/MappingLib)
add_subdirectory(src/MatlabLib)
# dependencies
add_dependencies(AwGraphics AwCore)
add_dependencies(AwFiltering AwCore)
add_dependencies(AwProcess AwCore)
add_dependencies(AwReadWrite AwCore)
add_dependencies(AwWidget AwGraphics AwCore AwMath)
add_dependencies(AwLayout AwCore)
add_dependencies(AwMontage AwCore AwReadWrite)

# header files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${PROJECT_BINARY_DIR}/include)

find_package(Matlab COMPONENTS MX_LIBRARY ENG_LIBRARY)

# MATLAB SUPPORT
if (Matlab_FOUND AND BUILD_STATIC_CORE AND QT_STATIC_DIR)
  message(STATUS "MATLAB Support added. Root dir is ${Matlab_ROOT_DIR}")
  add_subdirectory(src/MatlabSupport)
# Build mex files
  add_subdirectory(src/Mex)      # REACTIVATE ME AT THE END
endif()
# END OF MATLAB SUPPORT

# PYTHON
find_package(Python3 COMPONENTS Interpreter Development NumPy)
if (Python3_FOUND AND Python3_NumPy_FOUND)
   message(STATUS "Python 3 support added")
#   add_subdirectory(src/PythonSupport)   # REACTIVATE ME AT THE END
endif()
#if (Python_FOUND)
#find_package(PythonLibs)
#if (PYTHONLIBS_FOUND)
#  find_package(NumPy)
#  if (PYTHON_NUMPY_FOUND)
#     message(STATUS "Python support added.")
#     add_subdirectory(src/PythonSupport)
#  endif()
#endif()
#endif()

# AnyWave
add_subdirectory(src/AnyWave)


# plugins
add_subdirectory(src/plugins/4DNI_Reader)
add_dependencies(4DNIReader AwCore AwReadWrite)
add_subdirectory(src/plugins/EDFIO)
add_dependencies(EDFIO AwCore AwReadWrite)
add_subdirectory(src/plugins/BrainVisionIO)
add_dependencies(BrainVisionIO AwCore AwReadWrite)
add_subdirectory(src/plugins/CTF_Reader)
add_dependencies(CTFReader AwCore AwReadWrite)
add_subdirectory(src/plugins/Micromed_Reader)
add_dependencies(MicromedReader AwCore AwReadWrite)
add_subdirectory(src/plugins/icav2)
add_dependencies(ICA AwCore AwProcess)
add_subdirectory(src/plugins/FileConverter)
add_dependencies(FileConverter AwCore AwProcess AwFiltering)
add_subdirectory(src/plugins/AutoMarking)
add_dependencies(AutoMarking AwCore AwProcess)
add_subdirectory(src/plugins/AppendFiles)
add_dependencies(AppendFiles AwCore AwProcess AwReadWrite)
add_subdirectory(src/plugins/EGIReader)
add_dependencies(EGIReader AwCore AwReadWrite)
add_subdirectory(src/plugins/EEGLABReader)
add_dependencies(EEGLABReader AwCore AwReadWrite AwMATLAB)
add_subdirectory(src/plugins/SPMReader)
add_dependencies(SPMReader AwCore AwReadWrite AwMATLAB)
add_subdirectory(src/plugins/FIFIO)
add_dependencies(FIFFIO AwCore AwReadWrite)
add_subdirectory(src/plugins/EEGInto4D)
add_dependencies(EEGInto4D AwCore AwReadWrite AwUtilities)
add_subdirectory(src/plugins/TFWavelet)
add_subdirectory(src/plugins/Rewrite4D)
add_subdirectory(src/plugins/MultiFiltersViewer)
add_subdirectory(src/plugins/h2)
add_subdirectory(src/plugins/H2Graphs)
add_subdirectory(src/plugins/EEPIO)

# external plugins (must be locater in plugins sub folder)
#find_file(EXTERNAL_PLUGINS NAMES external_plugins.cmake PATHS  ${CMAKE_SOURCE_DIRECTORY} NO_DEFAULT_PATH)
IF(EXISTS ${CMAKE_SOURCE_DIR}/src/plugins/external_plugins.cmake)
include(${CMAKE_SOURCE_DIR}/src/plugins/external_plugins.cmake)
MESSAGE(STATUS "will also build external plugins")
ENDIF()

IF(NOT APPLE)
# Install
INSTALL(FILES ${CMAKE_SOURCE_DIR}/resources/linux/start_aw.sh PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE DESTINATION ${CMAKE_INSTALL_PREFIX})

#License files
INSTALL(FILES ${AW_RES_DIR}/Licence_fr.html ${AW_RES_DIR}/License_en.html DESTINATION
	${CMAKE_INSTALL_PREFIX})
# desktop
INSTALL(FILES ${AW_RES_DIR}/linux/anywave.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
# icon
INSTALL(FILES ${AW_RES_DIR}/images/AnyWave.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/apps)
# cmake
install(DIRECTORY ${CMAKE_SOURCE_DIR}/cmake DESTINATION ${CMAKE_INSTALL_PREFIX})
# include 
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})
ENDIF()

