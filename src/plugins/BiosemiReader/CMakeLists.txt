# Biosemi EDF/BDF
project(EDFReader)
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/BiosemiReader/biosemireader.cpp
${CMAKE_SOURCE_DIR}/src/plugins/BiosemiReader/edflib.c) 

SET(MOCS ${CMAKE_SOURCE_DIR}/src/plugins/BiosemiReader/biosemireader.h)

qt5_wrap_cpp(EDF_MOCS ${MOCS})
add_library(EDFReader SHARED ${SRCS} ${EDF_MOCS})
qt5_use_modules(EDFReader Core)
target_link_libraries(EDFReader AwCoreLib AwReadWriteLib Qt5::Core Qt5::Widgets)








