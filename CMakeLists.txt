cmake_minimum_required(VERSION 3.16)
project(AnyWave)

SET(CMAKE_BUILD_TYPE Release) # build in release mode
SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_DIR})

SET(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if (APPLE) 
   include(mac.cmake)
else(APPLE)
   include(linux.cmake)
endif()

# PATH to resources
SET(AW_RES_DIR ${CMAKE_SOURCE_DIR}/resources)

SET(PLUGIN_OUT ${PROJECT_BINARY_DIR}/Plugins)

MESSAGE(STATUS "Qt version is ${Qt5Core_VERSION}")

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
IF(NOT VTK_FOUND)
   MESSAGE(STATUS "vtk required.")
   RETURN()
ELSE()
    MESSAGE(STATUS "VTK ${VTK_MAJOR_VERSION} detected.")
ENDIF()

## use the static libs of HDF5
FIND_PACKAGE(HDF5 REQUIRED COMPONENTS C HL)
message(STATUS "HDF5 are ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES}")

find_package(MATIO REQUIRED)
if(MATIO_FOUND)
    message(STATUS "Found Matio lib: ${MATIO_LIBRARIES}")
    INCLUDE_DIRECTORIES(MATIO_INCLUDE_DIRS)
endif()

# Add build flags - Armadillo will use openBLAS on Linux, Accelerate Framework on Mac OS and MKL on Windows.
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -DARMA_DONT_USE_WRAPPER -DARMA_USE_BLAS -DARMA_USE_LAPACK -DAW_DISABLE_EPOCHING")
IF(USE_MKL)
     MESSAGE(STATUS "Settings CXX flags for MKL")
     SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -m64 -DMKL")
ENDIF()

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

#IF(UNIX AND NOT APPLE)
#   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
#   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
#   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN/lib'")
#ENDIF()

#IF(APPLE)
#   SET(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
#   SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
#   SET(APPLE_ID "Apple Development: thechewie13@gmail.com (MY3J8AN9H6)")
#ENDIF()

#IF(APPLE)
#   SET(APP_BUNDLE "AnyWave.app")
#   SET(APP_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/MacOS")
#   SET(LIB_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Frameworks")
#   SET(PLUGIN_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins")
#   SET(MEX_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins/MATLAB/AnyWave")
#   SET(PLUGIN_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Plugins")
#ELSE()
#   SET(MEX_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Plugins/MATLAB/AnyWave")
#   SET(PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Plugins")
#ENDIF()

# third party libs
IF(BUILD_QWT)
SET(QWT_LIBRARY qwt)
add_subdirectory(thirdparty/qwt)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/thirdparty/qwt/src)
ELSE()
find_package(Qwt REQUIRED)
message(STATUS "Qwt folders are ${QWT_INCLUDE_DIR} ${QWT_LIBRARY}")
INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIR})
LINK_DIRECTORIES(${QWT_LIBRARY})
ENDIF()
# customplot
add_subdirectory(thirdparty/qcustomplot)


# AnyWave SDK libraries
add_subdirectory(src/Core)
add_subdirectory(src/AwMontageLib)
add_subdirectory(src/Filtering)
add_subdirectory(src/Utilities)
add_subdirectory(src/Process)
add_subdirectory(src/Widgets)
add_subdirectory(src/ReadWrite)
add_subdirectory(src/HDF5)
add_subdirectory(src/Layouts)
add_subdirectory(src/Math)
add_subdirectory(src/MappingLib)
add_subdirectory(src/MatlabLib)
# dependencies
add_dependencies(AwFiltering AwCore)
add_dependencies(AwProcess AwCore)
add_dependencies(AwReadWrite AwCore)
add_dependencies(AwWidget AwCore AwMath)
add_dependencies(AwLayout AwCore)
add_dependencies(AwMontage AwCore AwReadWrite)

# header files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${PROJECT_BINARY_DIR}/include)

find_package(Matlab COMPONENTS MX_LIBRARY ENG_LIBRARY)
# if matlab is detected, then build the shared lib to connect to MATLAB app
# However this required to build all the mexfiles of the MATLAB API
# This has to be done separatly, using the sub project in matlab sub folder.
if(Matlab_FOUND)
   add_subdirectory(src/MatlabSupport)
endif(Matlab_FOUND)

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
add_subdirectory(src/plugins/Micromed)
add_dependencies(MicromedReader AwCore AwReadWrite)
add_subdirectory(src/plugins/icav2)
add_dependencies(ICA AwCore AwProcess)
add_subdirectory(src/plugins/FileConverter)
add_dependencies(FileConverter AwCore AwProcess AwFiltering)
add_subdirectory(src/plugins/AutoMarking)
add_dependencies(AutoMarking AwCore AwProcess)
add_subdirectory(src/plugins/AppendFilesv2)
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
add_subdirectory(src/plugins/CompumedicsPF)
add_subdirectory(src/plugins/DeltamedBinTxtReader)
add_subdirectory(src/plugins/Spectral)