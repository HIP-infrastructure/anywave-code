# ANT Reader
add_definitions(-DQT_PLUGIN)
project(ANTReader)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/ANT_Reader/ant_reader.cpp
${CMAKE_SOURCE_DIR}/src/plugins/ANT_Reader/misc.cpp
${CMAKE_SOURCE_DIR}/src/plugins/ANT_Reader/raw3.cpp
${CMAKE_SOURCE_DIR}/src/plugins/ANT_Reader/trg.cpp
${CMAKE_SOURCE_DIR}/src/plugins/ANT_Reader/var_string.cpp
)

 SET(MOCS
     ${CMAKE_SOURCE_DIR}/src/plugins/ANT_Reader/ant_reader.h)


qt5_wrap_cpp(ANT_MOCS ${MOCS})

add_library(ANTReader SHARED ${SRCS} ${ANT_MOCS})
qt5_use_modules(ANTReader Core)

IF(APPLE)
	target_link_libraries(ANTReader AwCoreLib AwReadWriteLib Qt5::Core)
ELSE() # link with openmp lib on Linux
	target_link_libraries(ANTReader AwCoreLib AwReadWriteLib Qt5::Core gomp)
ENDIF()







