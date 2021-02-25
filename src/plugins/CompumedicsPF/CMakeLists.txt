# compumedic
project(Compumedic)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})


SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/CompumedicsPF/CompumedicsPF.cpp) 

SET(MOCS
   ${CMAKE_SOURCE_DIR}/src/plugins/CompumedicsPF/CompumedicsPF.h)

qt5_wrap_cpp(COMP_MOCS ${MOCS})
add_library(Compumedic SHARED ${SRCS} ${COMP_MOCS})
qt5_use_modules(Compumedic Core Sql Xml) 
target_link_libraries(Compumedic AwCore AwReadWrite AwUtilities Qt5::Core Qt5::Sql Qt5::Xml) 
INSTALL(TARGETS Compumedic DESTINATION ${PLUGIN_INSTALL_DIR})








