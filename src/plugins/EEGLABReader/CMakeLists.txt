# EEGLABReader 
project(EEGLABReader)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(SRCS EEGLABReader.cpp) 

SET(MOCS EEGLABReader.h)

qt5_wrap_cpp(EEEGLABReader_MOCS ${MOCS})
add_library(EEGLABReader SHARED ${SRCS} ${EEGLABReader_MOCS})
qt5_use_modules(EEGLABReader Core)
target_link_libraries(EEGLABReader AwCore AwReadWrite AwMATLAB Qt5::Core )
INSTALL(TARGETS EEGLABReader DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)








