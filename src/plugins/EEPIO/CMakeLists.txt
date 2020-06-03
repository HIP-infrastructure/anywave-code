# EEProbe plugin (read/export)
project(EEPIO)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(SRCS EEProbeIO.cpp libeep.cpp  
libcnt/cnt.cpp  
libcnt/cntutils.cpp
libcnt/evt.cpp
libcnt/raw3.cpp
libcnt/rej.cpp
libcnt/riff.cpp
libcnt/riff64.cpp
libcnt/seg.cpp
libcnt/trg.cpp)


SET(MOCS EEProbeIO.h )

qt5_wrap_cpp(EEPIO_MOCS ${MOCS})
add_library(EEPIO SHARED ${SRCS} ${EEPIO_MOCS})
qt5_use_modules(EEPIO Core)
target_link_libraries(EEPIO AwCore AwReadWrite AwUtilities Qt5::Core)
INSTALL(TARGETS EEPIO DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)








