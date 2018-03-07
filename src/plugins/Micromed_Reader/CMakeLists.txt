# Micromed reader
project(MicromedReader)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/Micromed_Reader/micromedreader.cpp) 

 SET(MOCS
   ${CMAKE_SOURCE_DIR}/src/plugins/Micromed_Reader/micromedreader.h)

qt5_wrap_cpp(Micromed_MOCS ${MOCS})
add_library(MicromedReader SHARED ${SRCS} ${Micromed_MOCS})
qt5_use_modules(MicromedReader Core)
target_link_libraries(MicromedReader AwCoreLib AwReadWriteLib Qt5::Core)








