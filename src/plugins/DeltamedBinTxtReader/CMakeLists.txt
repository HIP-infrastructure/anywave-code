# compumedic
project(DeltamedBinTxtReader)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})


SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/DeltamedBinTxtReader/DeltamedBTReader.cpp) 

SET(MOCS
   ${CMAKE_SOURCE_DIR}/src/plugins/DeltamedBinTxtReader/DeltamedBTReader.h)

qt5_wrap_cpp(COMP_MOCS ${MOCS})
add_library(DeltamedBinTxtReader SHARED ${SRCS} ${COMP_MOCS})
qt5_use_modules(DeltamedBinTxtReader Core) 
target_link_libraries(DeltamedBinTxtReader AwCore AwReadWrite AwUtilities Qt5::Core) 
INSTALL(TARGETS DeltamedBinTxtReader DESTINATION ${PLUGIN_INSTALL_DIR})








