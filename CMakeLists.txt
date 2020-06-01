cmake_minimum_required(VERSION 3.6) 
project(AnyWave)
# detecting compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
   MESSAGE(STATUS "Apple clang compiler detected")
   add_definitions("-w -fPIC -O4 -fpermissive") # clang flags
   SET(CLANG ON)
# apple clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
# gcc
   MESSAGE(STATUS "GCC compiler detected")
   add_definitions("-w -fPIC -O3 -fpermissive -fopenmp") # g++ flags
   SET(GCC ON)
endif()

SET(CMAKE_BUILD_TYPE "Release") # build in release mode
SET(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# default install path
SET(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/AnyWave)

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

IF(APPLE)
  SET(PLUGIN_OUT ${PROJECT_BINARY_DIR}/Anywave_Plugins)
ELSE()
   SET(PLUGIN_OUT ${PROJECT_BINARY_DIR}/Plugins)
ENDIF()

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
INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE})
find_package(Qwt REQUIRED)
INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIR})
LINK_DIRECTORIES(${QWT_LIBRARY})
message(STATUS "Qwt folders are ${QWT_INCLUDE_DIR} ${QWT_LIBRARY}")
include_directories(${ARMADILLO_INCLUDE_DIRS})

## use the static libs of HDF5
IF(USE_STATIC_HDF5)
   SET(HDF5_USE_STATIC_LIBRARIES TRUE)
ENDIF()
FIND_PACKAGE(HDF5 REQUIRED COMPONENTS C HL)
INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})
LINK_DIRECTORIES(${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})
message(STATUS "HDF5 are ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES}")


find_package(MATIO REQUIRED)
if(MATIO_FOUND)
    message(STATUS "Found Matio lib: ${MATIO_LIBRARIES}")
endif()

if(NOT APPLE)
# search for MKL only on Linux
find_package(MKL)
if(NOT MKL_FOUND)
    find_package(OpenBLAS REQUIRED)
    SET(BLAS_LIBRARIES ${OpenBLAS_LIB})
else()
    message(STATUS "MKL Found.")
    LINK_DIRECTORIES(${MKL_LIB_DIRS})
   # SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    SET(USE_MKL TRUE CACHE BOOL "Using MKL libraries")
    INCLUDE_DIRECTORIES(${MKL_INCLUDE_DIRS})
    SET(BLAS_LIBRARIES ${MKL_LIBRARIES})
    message(STATUS "MLK libs : ${MKL_LIBRARIES}")
endif()
endif()

add_definitions(-DNDEBUG)
# Add build flags - Armadillo will use openBLAS on Linux, Accelerate Framework on Mac OS and MKL on Windows.
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -DARMA_DONT_USE_WRAPPER -DARMA_USE_BLAS -DARMA_USE_LAPACK -DAW_DISABLE_EPOCHING")
if(USE_MKL)
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DMKL")
endif()
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

IF(NOT APPLE)
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ELSE()
   SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF()
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN/lib'")

# AnyWave SDK libraries
add_subdirectory(src/Core)
add_subdirectory(src/AwEpochLib)
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
add_dependencies(AwMontage AwCore)

# header files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${PROJECT_BINARY_DIR}/include)

find_package(Matlab COMPONENTS MX_LIBRARY ENG_LIBRARY)

# MATLAB SUPPORT
if (Matlab_FOUND)
  message(STATUS "MATLAB Support added. Root dir is ${Matlab_ROOT_DIR}")
  add_subdirectory(src/MatlabSupport)
# Build mex files
  add_subdirectory(src/Mex)
endif()
# END OF MATLAB SUPPORT

# PYTHON
find_package (Python COMPONENTS Interpreter Development)
if (Python_FOUND)
find_package(PythonLibs)
if (PYTHONLIBS_FOUND)
  find_package(NumPy)
  if (PYTHON_NUMPY_FOUND)
     message(STATUS "Python support added.")
     add_subdirectory(src/PythonSupport)
  endif()
endif()
endif()

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
add_subdirectory(src/plugins/ANT_Reader)
add_dependencies(ANTReader AwCore AwReadWrite)
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


