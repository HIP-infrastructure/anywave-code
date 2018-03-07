# CTF MEG reader
project(CTFReader)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/CTF_Reader/ctfreader.cpp) 

SET(MOCS
   ${CMAKE_SOURCE_DIR}/src/plugins/CTF_Reader/ctfreader.h)

qt5_wrap_cpp(CTF_MOCS ${MOCS})
add_library(CTFReader SHARED ${SRCS} ${CTF_MOCS})
qt5_use_modules(CTFReader Core)
target_link_libraries(CTFReader AwCoreLib AwReadWriteLib Qt5::Core)








