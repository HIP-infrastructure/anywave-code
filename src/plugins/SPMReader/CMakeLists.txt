# SPMReader 
project(SPMReader)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DSPMREADER_EXPORT)
INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE})
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS SPMReader.cpp) 

SET(MOCS SPMReader.h)

qt5_wrap_cpp(SPMReader_MOCS ${MOCS})
add_library(SPMReader SHARED ${SRCS} ${SPMReader_MOCS})
qt5_use_modules(SPMReader Core)
target_link_libraries(SPMReader AwCore AwReadWrite AwMATLAB Qt5::Core ${MATIO_LIBRARIES})
INSTALL(TARGETS SPMReader DESTINATION ${PLUGIN_INSTALL_DIR})


