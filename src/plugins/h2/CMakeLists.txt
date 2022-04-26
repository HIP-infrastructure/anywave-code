# H2
project(H2)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
h2.cpp
h2ui.cpp)

SET(MOCS
h2.h
h2ui.h)

SET(UIS h2ui.ui)

qt5_add_resources(RES Resource.qrc)

qt5_wrap_cpp(H2_MOCS ${MOCS})
QT5_WRAP_UI(H2_UIS ${UIS})

add_library(H2 SHARED ${SRCS} ${H2_MOCS} ${H2_UIS} ${RES})
qt5_use_modules(H2 Core Gui Widgets Concurrent)

IF(APPLE)
target_link_libraries(H2 AwCore AwProcess AwWidget AwMATLAB ${ARMA_LIB} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Concurrent)
ELSE()
target_link_libraries(H2 AwCore AwProcess AwWidget AwMATLAB ${BLAS_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg Qt5::Concurrent)
ENDIF()
INSTALL(TARGETS H2 DESTINATION ${PLUGIN_INSTALL_DIR})





