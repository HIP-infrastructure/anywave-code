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

IF(APPLE)
   target_link_libraries(4DNIReader AwCore AwReadWriteLib Qt5::Core)
   INSTALL(TARGETS 4DNIReader DESTINATION ${CMAKE_INSTALL_PREFIX}/Anywave_Plugins)
ELSE() # link with openmp lib on Linux
   target_link_libraries(4DNIReader AwCore AwReadWrite Qt5::Core gomp)
   INSTALL(TARGETS 4DNIReader DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)
ENDIF()







