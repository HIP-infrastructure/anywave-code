# BrainVision vhdr reader/writer
project(EDFIO)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
  EDFIO.cpp) 

SET(MOCS
   EDFIO.h)


 qt5_wrap_cpp(EDFIO_MOCS ${MOCS})
 add_library(EDFIO SHARED ${SRCS} ${EDFIO_MOCS})
 qt5_use_modules(EDFIO Core)
 target_link_libraries(EDFIO AwCoreLib AwReadWriteLib Qt5::Core)










