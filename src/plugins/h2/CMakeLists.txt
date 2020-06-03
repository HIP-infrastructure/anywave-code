# H2
project(H2)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(SRCS
${CMAKE_SOURCE_DIR}/h2.cpp
${CMAKE_SOURCE_DIR}/h2ui.cpp
${CMAKE_SOURCE_DIR}/H2ChannelPairItem.cpp
${CMAKE_SOURCE_DIR}/H2ChannelPair.cpp) 

SET(MOCS
${CMAKE_SOURCE_DIR}/h2.h
${CMAKE_SOURCE_DIR}/h2ui.h
${CMAKE_SOURCE_DIR}/H2ChannelPairItem.h)

SET(UIS
${CMAKE_SOURCE_DIR}/h2ui.ui)

qt5_add_resources(RES ${CMAKE_SOURCE_DIR}/Resource.qrc)

qt5_wrap_cpp(H2_MOCS ${MOCS})
QT5_WRAP_UI(H2_UIS ${UIS})

add_library(H2 SHARED ${SRCS} ${H2_MOCS} ${H2_UIS})
qt5_use_modules(H2 Core Gui Widgets Concurrent)

target_link_libraries(H2 ${AW_LIBRARIES} ${BLAS_LIBRARIES} ${RES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Concurrent)
INSTALL(TARGETS H2 DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)





