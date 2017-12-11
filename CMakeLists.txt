cmake_minimum_required(VERSION 2.8)
project(AnyWave)

# Flags for gcc/g++ compiler
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-c++11-narrowing -w -fPIC -O3 -fpermissive -fopenmp")
SET(CMAKE_BUILD_TYPE "Release") # build in release mode

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
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
# PATH to SDK ROOT
SET(AW_ROOT ${CMAKE_SOURCE_DIR})
SET(PLUGIN_OUT ${PROJECT_BINARY_DIR}/bin/Plugins)
# Packages required
# HDF5
FIND_PACKAGE(HDF5 REQUIRED COMPONENTS HL)
INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})
LINK_DIRECTORIES(${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})
#Qt 5
MESSAGE(STATUS "Building with Qt5")
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Script REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Svg REQUIRED)
MESSAGE(STATUS "Qt version is ${Qt5Core_VERSION}")

# VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

IF(NOT VTK_FOUND)
   MESSAGE(STATUS "VTK NOT FOUND. Please SET VTK_DIR variable to specify the VTK location.")
ENDIF()

# Armadillo
# Armadillo is included on the source tree
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/armadillo/include)

# Qwt
find_package(Qwt REQUIRED)
INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIR})
LINK_DIRECTORIES(${QWT_LIBRARY})
# matio
find_package(MATIO REQUIRED)
INCLUDE_DIRECTORIES(${MATIO_INCLUDE_DIR})
LINK_DIRECTORIES(${MATIO_LIBRARY})
# FFTW and OpenBlas (for AwMathlib and Armadillo linking)
FIND_PACKAGE(FFTW REQUIRED)
FIND_PACKAGE(OpenBLAS REQUIRED)

# If building in Release, define NDEBUG symbol
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")

#turn off epoching (under development)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DAW_DISABLE_EPOCHING")

# Search for MATLAB and enable MATLAB support if found
find_package(Matlab REQUIRED)

# AnyWave SDK libraries
add_subdirectory(src/Core)
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
add_dependencies(AwGraphicsLib AwCoreLib)
add_dependencies(AwFilteringLib AwCoreLib)
add_dependencies(AwProcessLib AwCoreLib)
add_dependencies(AwReadWriteLib AwCoreLib)
add_dependencies(AwWidgetLib AwGraphicsLib AwCoreLib AwMathLib)
add_dependencies(AwLayoutLib AwCoreLib)

# header files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${PROJECT_BINARY_DIR}/include)

# MATLAB SUPPORT
if (MATLAB_FOUND)
  message(STATUS "MATLAB Support added.")
  add_subdirectory(src/MatlabSupport)
endif()
# END OF MATLAB SUPPORT

# Try to build Python Module
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(NumPy)

IF (NOT PYTHONLIBS_FOUND)
   MESSAGE(STATUS "Python Module won't be built since Python libraries were not found.")
ELSEIF(NOT PYTHON_NUMPY_FOUND)
   MESSAGE(STATUS "Numpy package is required and was not found.")
ENDIF()

IF (PYTHONLIBS_FOUND)
   add_subdirectory(src/PythonSupport)
ENDIF()

# AnyWave
add_subdirectory(src/AnyWave)

# plugins
add_subdirectory(src/plugins/4DNI_Reader)
add_dependencies(4DNIReader AwCoreLib AwReadWriteLib)
add_subdirectory(src/plugins/BiosemiReader)
add_dependencies(EDFReader AwCoreLib AwReadWriteLib)
add_subdirectory(src/plugins/CTF_Reader)
add_dependencies(CTFReader AwCoreLib AwReadWriteLib)
add_subdirectory(src/plugins/Micromed_Reader)
add_dependencies(MicromedReader AwCoreLib AwReadWriteLib)
add_subdirectory(src/plugins/FIFF_Reader)
add_dependencies(FIFFReader AwCoreLib AwReadWriteLib)
add_subdirectory(src/plugins/ICA)
add_dependencies(ICA AwCoreLib AwProcessLib)
add_subdirectory(src/plugins/FileConverter)
add_dependencies(FileConverter AwCoreLib AwProcessLib AwFilteringLib)
add_subdirectory(src/plugins/ANT_Reader)
add_dependencies(ANTReader AwCoreLib AwReadWriteLib)
add_subdirectory(src/plugins/AutoMarking)
add_dependencies(AutoMarking AwCoreLib AwProcessLib)
add_subdirectory(src/plugins/MergeFiles)
add_subdirectory(src/plugins/BrainVisionIO)
add_dependencies(BrainVisionIO AwCoreLib AwReadWriteLib)
add_dependencies(MergeFiles AwCoreLib AwProcessLib AwReadWriteLib)
add_subdirectory(src/plugins/AppendFiles)
add_dependencies(AppendFiles AwCoreLib AwProcessLib AwReadWriteLib)
add_subdirectory(src/plugins/SST)
add_dependencies(SST AwCoreLib AwProcessLib AwMathLib)
add_subdirectory(src/plugins/Correlation)
add_dependencies(Correlation AwCoreLib AwProcessLib AwMathLib)

