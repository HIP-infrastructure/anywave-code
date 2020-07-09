# H2 GRAPHS
project(H2Graphs)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})


SET(SRCS GraphArrow.cpp GraphWindow.cpp GraphArrowBase.cpp GraphArrowDir.cpp GraphArrowLag.cpp GraphColorMap.cpp
GraphLegend.cpp GraphSet.cpp GraphSetModel.cpp H2Graphs.cpp GraphSensorItem.cpp GraphManagerWidget.cpp)

SET(MOCS GraphArrow.h GraphWindow.h GraphArrowBase.h GraphArrowDir.h GraphArrowLag.h GraphColorMap.h
GraphLegend.h GraphSet.h GraphSetModel.h H2Graphs.h GraphSensorItem.h GraphManagerWidget.h)

SET(UIS GraphManagerWidget.ui GraphWindow.ui)
SET(RES Graphs.qrc)

qt5_wrap_cpp(H2G_MOCS ${MOCS})
QT5_WRAP_UI(H2G_UIS ${UIS})
QT5_ADD_RESOURCES(${RES})

add_library(H2Graphs SHARED ${SRCS} ${H2G_MOCS} ${H2G_UIS})
qt5_use_modules(H2Graphs Core Gui Widgets Svg)

target_link_libraries(H2Graphs AwCore AwProcess AwWidget AwMATLAB Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg ${QWT_LIBRARIES})
INSTALL(TARGETS H2Graphs DESTINATION ${PLUGIN_INSTALL_DIR})






