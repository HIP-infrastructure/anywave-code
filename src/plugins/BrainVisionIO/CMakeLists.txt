# BrainVision vhdr reader/writer
project(BrainVisionIO)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
IF(NOT APPLE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF()
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/BrainVisionIO/BrainVisionIO.cpp) 

SET(MOCS
   ${CMAKE_SOURCE_DIR}/src/plugins/BrainVisionIO/BrainVisionIO.h)


 qt5_wrap_cpp(BrainVision_MOCS ${MOCS})
 add_library(BrainVisionIO SHARED ${SRCS} ${BrainVision_MOCS})
 qt5_use_modules(BrainVisionIO Core)
 target_link_libraries(BrainVisionIO AwCore AwReadWrite Qt5::Core)
INSTALL(TARGETS BrainVisionIO DESTINATION ${PLUGIN_INSTALL_DIR})










