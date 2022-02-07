# EEGLABReader 
project(EEGLABReader)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/armadillo/include)
SET(SRCS EEGLABReader.cpp) 

SET(MOCS EEGLABReader.h)

qt5_wrap_cpp(EEGLABReader_MOCS ${MOCS})
add_library(EEGLABReader SHARED ${SRCS} ${EEGLABReader_MOCS})
qt5_use_modules(EEGLABReader Core)
target_link_libraries(EEGLABReader AwCore AwReadWrite AwMATLAB Qt5::Core )
INSTALL(TARGETS EEGLABReader DESTINATION ${PLUGIN_INSTALL_DIR})








