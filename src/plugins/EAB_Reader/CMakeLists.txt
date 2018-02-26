# EAB reader
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

if (APPLE)
   set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/Anywave_Plugins)
else()
   set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/Plugins)
endif()

SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/EAB_Reader/eabreader.cpp) 

SET(MOCS
${CMAKE_SOURCE_DIR}/src/plugins/EAB_Reader/eabreader.h)
qt4_wrap_cpp(EAB_MOCS ${MOCS})
add_library(EABReader SHARED ${SRCS} ${EAB_MOCS})
target_link_libraries(EABReader AwCoreLib AwReadWriteLib ${QT_LIBRARIES})







