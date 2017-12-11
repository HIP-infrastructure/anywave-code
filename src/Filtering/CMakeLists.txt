# FitleringLib
project(AwFilteringLib)
add_definitions("-DAW_BUILD_FILTERING_LIB")
INCLUDE_DIRECTORIES(./DspFilters)
SET(SRC ${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Bessel.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Biquad.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Butterworth.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Cascade.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/ChebyshevI.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/ChebyshevII.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Custom.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Design.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Documentation.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Elliptic.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Filter.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Legendre.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/Param.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/PoleFilter.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/RBJ.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/RootFinder.cpp
${CMAKE_CURRENT_SOURCE_DIR}/DspFilters/State.cpp
AwFiltering.cpp)

add_library(AwFilteringLib SHARED ${SRC})
qt5_use_modules(AwFilteringLib Core Concurrent)
TARGET_LINK_LIBRARIES(AwFilteringLib AwCoreLib Qt5::Core Qt5::Concurrent)




