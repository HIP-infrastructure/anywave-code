project(FIFFReader)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff_reader.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_compat.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_dir_tree.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_def_dir.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_dir.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_id.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_io.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_insert.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_open.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_pack.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_writes.cpp
${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff/fiff_trans.cpp)

SET(MOCS
   ${CMAKE_SOURCE_DIR}/src/plugins/FIFF_Reader/fiff_reader.h)

qt5_wrap_cpp(FR_MOCS ${MOCS})
add_library(FIFFReader SHARED ${SRCS} ${FR_MOCS})
qt5_use_modules(FIFFReader Core)
target_link_libraries(FIFFReader AwCoreLib AwReadWriteLib Qt5::Core)
















