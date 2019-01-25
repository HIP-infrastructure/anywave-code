# SPMReader 
project(SPMReader)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS SPMReader.cpp) 

SET(MOCS SPMReader.h)

qt5_wrap_cpp(SPMReader ${MOCS})
add_library(SPMReader SHARED ${SRCS} ${SPMReader_MOCS})
qt5_use_modules(SPMReader Core)
target_link_libraries(SPMReader AwCore AwReadWrite AwMATLAB Qt5::Core)
INSTALL(TARGETS SPMReader DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)

