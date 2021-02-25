# BrainVision vhdr reader/writer
project(EDFIO)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
  EDFIO.cpp edflib.c) 

SET(MOCS
   EDFIO.h)


 qt5_wrap_cpp(EDFIO_MOCS ${MOCS})
 add_library(EDFIO SHARED ${SRCS} ${EDFIO_MOCS})
 qt5_use_modules(EDFIO Core)
 target_link_libraries(EDFIO AwCore AwReadWrite Qt5::Core)
INSTALL(TARGETS EDFIO DESTINATION ${PLUGIN_INSTALL_DIR})










