# AwWidgetLib
project(WidgetsLib)
add_definitions("-DAW_BUILD_WIDGETS_LIB")

SET(AwWidgetLib_MOC_HEADERS
	${CMAKE_SOURCE_DIR}/include/widget/AwMarkAroundWidget.h
	${CMAKE_SOURCE_DIR}/include/widget/AwMapWidget.h 
	${CMAKE_SOURCE_DIR}/include/widget/AwAmplitudeButton.h
	${CMAKE_SOURCE_DIR}/include/widget/AwAmplitudeWidget.h
	${CMAKE_SOURCE_DIR}/include/widget/AwTopoWidget.h
	${CMAKE_SOURCE_DIR}/include/widget/AwTopoBuilder.h
	${CMAKE_SOURCE_DIR}/include/widget/AwMarkerInspector.h
    ${CMAKE_SOURCE_DIR}/include/widget/AwComboCMap.h
    ${CMAKE_SOURCE_DIR}/include/widget/AwComboMarker.h
	${CMAKE_SOURCE_DIR}/include/widget/AwComboColor.h
    ${CMAKE_SOURCE_DIR}/include/widget/AwWaitWidget.h
	${CMAKE_SOURCE_DIR}/include/widget/SignalView/AwBaseMarkerBar.h
	${CMAKE_SOURCE_DIR}/include/widget/SignalView/AwBaseSignalView.h
	${CMAKE_SOURCE_DIR}/include/widget/SignalView/AwNavigationBar.h
	${CMAKE_SOURCE_DIR}/include/widget/SignalView/AwGraphicsView.h
	${CMAKE_SOURCE_DIR}/include/widget/SignalView/AwGraphicsScene.h
	${CMAKE_SOURCE_DIR}/include/widget/SignalView/AwViewSettings.h
	${CMAKE_SOURCE_DIR}/include/widget/AwSEEGViewer.h
    ${CMAKE_SOURCE_DIR}/include/widget/AwDownSamplingWidget.h
	${CMAKE_SOURCE_DIR}/include/AwMarkingSettings.h
	SignalView/AwGoToLatencyDial.h
	SignalView/AwViewSettingsUi.h
	SignalView/AwGTCMenu.h
	AwAddTargetChannels.h
	AwAddPredefinedMarker.h
	AwTimeScaleCombo.h
    AwTopoLabelImage.h
	AwTopoWidgetSettings.h
	SEEGViewer/AwSEEGWidget.h)

QT5_WRAP_CPP(AwWidgetLib_MOC_SRCS ${AwWidgetLib_MOC_HEADERS})

# .ui files
SET(AwWidgetLib_UIS
	AwMarkAroundWidget.ui
	AwAddPredefinedMarker.ui
	AwAddTargetChannels.ui
	AwAmplitudeWidget.ui
	AwMarkerInspector.ui
	AwTopoWidgetSettings.ui
	AwTopoWidget.ui
	AwWaitWidget.ui
    AwDownSamplingWidget.ui
	SignalView/AwGoToLatencyDial.ui
	SignalView/AwNavigationBarv2.ui
	SignalView/AwViewSettingsUi.ui
	SEEGViewer/AwSEEGWidget.ui)
	
QT5_WRAP_UI(AwWidgetLib_UI_HEADERS ${AwWidgetLib_UIS})
QT5_ADD_RESOURCES(WIDGET_QRC ${AW_RES_DIR}/AnyWave.qrc ${AW_RES_DIR}/amplitudes.qrc)

SET(SRC
AwWaitWidget.cpp
AwMapWidget.cpp 
AwAmplitudeButton.cpp
AwAmplitudeWidget.cpp
AwTimeScaleCombo.cpp
AwTopoWidget.cpp
AwTopoBuilder.cpp
AwTopoWidgetSettings.cpp
Aw2DGrid.cpp
AwAddPredefinedMarker.cpp
AwAddTargetChannels.cpp
AwMarkerInspector.cpp
AwMarkingSettings.cpp
AwQwtColorMap.cpp
AwComboCMap.cpp
AwComboColor.cpp
AwComboMarker.cpp
AwTopoLabelImage.cpp
AwMarkAroundWidget.cpp
AwMessageBox.cpp
AwDownSamplingWidget.cpp
SEEGViewer/AwSEEGWidget.cpp
SEEGViewer/AwSEEGViewer.cpp
SignalView/AwBaseMarkerBar.cpp
SignalView/AwBaseSignalView.cpp
SignalView/AwGoToLatencyDial.cpp
SignalView/AwGraphicsScene.cpp
SignalView/AwGraphicsView.cpp
SignalView/AwNavigationBar.cpp
SignalView/AwViewSettings.cpp
SignalView/AwViewSettingsUi.cpp
SignalView/AwGTCMenu.cpp
SEEGViewer/AwSEEGViewer.cpp
SEEGViewer/AwSEEGWidget.cpp)


ADD_LIBRARY(AwWidgetLib SHARED ${SRC} ${AwWidgetLib_MOC_SRCS} ${AwWidgetLib_UI_HEADERS} ${WIDGET_QRC})
QT5_USE_MODULES(AwWidgetLib Core Widgets PrintSupport)
TARGET_LINK_LIBRARIES(AwWidgetLib AwCoreLib AwGraphicsLib AwMathLib AwMappingLib Qt5::Core Qt5::Widgets Qt5::PrintSupport ${VTK_LIBRARIES} ${QWT_LIBRARY})

