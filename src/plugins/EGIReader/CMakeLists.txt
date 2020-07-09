# EGI reader
project(EGIReader)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS EGIReader.cpp MIFFFile.cpp SignalFile.cpp SignalUtil.cpp) 

SET(MOCS EGIReader.h )

qt5_wrap_cpp(EGI_MOCS ${MOCS})
add_library(EGIReader SHARED ${SRCS} ${EGI_MOCS})
qt5_use_modules(EGIReader Core Xml)
target_link_libraries(EGIReader AwCore AwReadWrite Qt5::Core Qt5::Xml)
INSTALL(TARGETS EGIReader DESTINATION ${PLUGIN_INSTALL_DIR})








