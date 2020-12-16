# 4D Reader
add_definitions(-DQT_PLUGIN)
project(4DNIReader)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
${CMAKE_SOURCE_DIR}/src/plugins/4DNI_Reader/meg4dreader.cpp) 

SET(MOCS
    ${CMAKE_SOURCE_DIR}/src/plugins/4DNI_Reader/meg4dreader.h)


qt5_wrap_cpp(4DNI_MOCS ${MOCS})
# .qrc
QT5_ADD_RESOURCES(4DNI_QRC ${CMAKE_SOURCE_DIR}/src/plugins/4DNI_Reader/montages.qrc)

add_library(4DNIReader SHARED ${SRCS} ${4DNI_MOCS} ${4DNI_QRC})
qt5_use_modules(4DNIReader Core)

target_link_libraries(4DNIReader AwCore AwReadWrite AwUtilities Qt5::Core)
INSTALL(TARGETS 4DNIReader DESTINATION ${PLUGIN_INSTALL_DIR})





