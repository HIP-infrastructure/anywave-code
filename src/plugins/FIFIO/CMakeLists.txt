# FIFFIO plugin (read/export)
project(FIFFIO)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(SRCS FIFFIO.cpp) 

SET(MOCS FIFFIO.h )

qt5_wrap_cpp(FIFFIO_MOCS ${MOCS})
add_library(FIFFIO SHARED ${SRCS} ${FIFFIO_MOCS})
qt5_use_modules(FIFFIO Core)
target_link_libraries(FIFFIO AwCore AwReadWrite AwUtilities Qt5::Core)
INSTALL(TARGETS FIFFIO DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)








