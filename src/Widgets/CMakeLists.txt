# AwWidgetLib
project(AwWidget)
add_definitions("-DAW_BUILD_WIDGETS_LIB")

QT5_ADD_RESOURCES(WIDGET_QRC ${AW_RES_DIR}/AnyWave.qrc ${AW_RES_DIR}/amplitudes.qrc)

SET(HEADERS ../../include/widget/AwMessageBox.h  
    ../../include/widget/AwSelectInputDataWidget.h
    ../../include/widget/AwWait.h  
	../../include/widget/AwGetValueDialog.h  
    ../../include/AwGainLevels.h  
    ./Aw2DGrid.h  
    ../../include/widget/AwWaitWidget.h  
    ../../include/widget/AwComboCMap.h  
    ./AwTopoLabelImage.h  
    ../../include/widget/AwMarkAroundWidget.h  
    ../../include/widget/AwComboColor.h  
    ../../include/widget/AwDownSamplingWidget.h  
    ../../include/widget/AwComboMarker.h  
    ../../include/widget/AwComboDS.h  
    ../../include/widget/AwSpectroWidget.h  
    ../../include/widget/AwVTKWidget.h  
    ../../include/widget/AwMarkerInspector.h  
    ./AwAddPredefinedMarker.h  
    ./AwAddTargetChannels.h  
    ../../include/AwMarkingSettings.h  
    ../../include/widget/AwTopoBuilder.h  
    ./AwTopoWidgetSettings.h  
    ../../include/widget/AwAmplitudeWidget.h  
    ../../include/widget/AwMapWidget.h  
    ../../include/widget/AwTopoWidget.h  
    ./AwTimeScaleCombo.h  
    ./SignalView/AwGTCMenu.h  
    ../../include/widget/SignalView/AwBaseSignalView.h  
    ../../include/widget/SignalView/AwViewSettings.h  
    ../../include/widget/SignalView/AwBaseMarkerBar.h  
    ./SignalView/AwViewSettingsUi.h  
    ./SignalView/AwGoToLatencyDial.h  
    ./SignalView/AwPickMarkersDial.h 
    ../../include/widget/AwGainLevelsWidget.h 
    ../../include/widget/SignalView/AwGraphicsScene.h  
    ../../include/widget/SignalView/AwGraphicsView.h  
    ../../include/widget/SignalView/AwNavigationBar.h  
    ./InputMarker/InputMarkerModel.h  
    ../../include/widget/AwInputMarkerTableView.h  
    ../../include/widget/AwInputMarkerWidget.h
	../../include/widget/AwVideoPlayer.h
	../../include/widget/AwGraphicsObjects.h 
../../include/widget/AwGraphicInterface.h 
    MarkerItem/AwMarkerLabelButton.h 
    MarkerItem/AwMarkerValueButton.h 
    SignalItem/AwSIAmpButton.h 
    SignalItem/AwSignalItemSettings.h 
    ../../include/widget/AwColorMap.h 
   ../../include/widget/AwCursorItem.h 
   ../../include/widget/AwGraphicInterface.h 
   ../../include/widget/AwGraphicsDefines.h 
   ../../include/widget/AwGraphicsObjects.h 
   ../../include/widget/AwHighLightMarker.h 
   ../../include/widget/AwHighLightPositionMarker.h 
   ../../include/widget/AwLabelItem.h 
   ../../include/widget/AwMappingCursorItem.h 
   ../../include/widget/AwMarkerChannelItem.h 
   ../../include/widget/AwMarkerItem.h 
   ../../include/widget/AwQwtColorMap.h 
   ../../include/widget/AwSignalItem.h 
   ../../include/widget/AwSignalLabelItem.h)
	

SET(SRC  ./AwSelectInputDataWidget.cpp
     ./Aw2DGrid.cpp  
    ./AwGetValueDialog.cpp
    ./AwAddPredefinedMarker.cpp  
    ./AwAddTargetChannels.cpp  
    ./AwComboCMap.cpp  
    ./AwComboColor.cpp  
    ./AwComboDS.cpp  
    ./AwComboMarker.cpp  
    ./AwDownSamplingWidget.cpp  
    ./AwMarkAroundWidget.cpp  
    ./AwMarkerInspector.cpp  
    ./AwAmplitudeWidget.cpp  
    ./AwMapWidget.cpp  
    ./AwMarkingSettings.cpp  
    ./AwMessageBox.cpp  
    ./AwSpectroWidget.cpp  
    ./AwTimeScaleCombo.cpp  
    ./AwTopoBuilder.cpp  
    ./AwTopoLabelImage.cpp  
    ./AwTopoWidget.cpp  
    ./AwTopoWidgetSettings.cpp  
    ./AwVTKWidget.cpp  
    ./AwWaitWidget.cpp  
	./AwVideoPlayer.cpp
    ./SignalView/AwBaseMarkerBar.cpp  
    ./SignalView/AwBaseSignalView.cpp  
    ./SignalView/AwGoToLatencyDial.cpp  
    ./SignalView/AwGraphicsScene.cpp  
    ./SignalView/AwGraphicsView.cpp  
    ./SignalView/AwGTCMenu.cpp  
    ./SignalView/AwNavigationBar.cpp  
    ./SignalView/AwViewSettings.cpp  
    ./SignalView/AwViewSettingsUi.cpp  
    ./SignalView/AwPickMarkersDial.cpp 
    ./SignalView/AwGainLevelsWidget.cpp
    ./InputMarker/AwInputMarkerTableView.cpp  
    ./InputMarker/AwInputMarkerWidget.cpp  
    ./InputMarker/InputMarkerModel.cpp
	Mark/AwHighLightPositionMarker.cpp 
    MarkerItem/AwCursorItem.cpp 
    MarkerItem/AwHighLightMarker.cpp 
    MarkerItem/AwMappingCursorItem.cpp 
    MarkerItem/AwMarkerChannelItem.cpp 
    MarkerItem/AwMarkerItem.cpp 
    MarkerItem/AwMarkerLabelButton.cpp 
    MarkerItem/AwMarkerValueButton.cpp 
    SignalItem/AwLabelItem.cpp 
    SignalItem/AwSIAmpButton.cpp 
    SignalItem/AwSignalItem.cpp 
    SignalItem/AwSignalItemSettings.cpp 
    SignalItem/AwSignalLabelItem.cpp 
    AwGainLevels.cpp
    AwColorMap.cpp 
    AwGraphicsObjects.cpp 
    AwQwtColorMap.cpp)

QT5_WRAP_CPP(MOC ${HEADERS})


SET(FORMS  ./AwSelectInputDataWidget.ui
     ./AwAmplitudeWidget.ui 
	./AwGetValueDialog.ui
    ./AwMarkerInspector.ui  
    ./AwAddPredefinedMarker.ui  
    ./AwAddTargetChannels.ui  
    ./AwTopoWidgetSettings.ui  
    ./AwWaitWidget.ui  
    ./AwTopoWidget.ui  
    ./AwMarkAroundWidget.ui  
    ./AwDownSamplingWidget.ui  
    ./SignalView/AwGoToLatencyDial.ui  
    ./SignalView/AwNavigationBarv2.ui  
    ./SignalView/AwViewSettingsUi.ui  
    ./SignalView/AwPickMarkersDial.ui 
	./AwVideoPlayer.ui
    ./InputMarker/AwInputMarkerWidget.ui
	SignalItem/AwSignalItemSettings.ui)

QT5_WRAP_UI(UI ${FORMS})

ADD_LIBRARY(AwWidget SHARED ${SRC} ${MOC} ${UI} ${WIDGET_QRC})


IF (APPLE)
QT5_USE_MODULES(AwWidget Core Widgets PrintSupport Multimedia MultimediaWidgets)
TARGET_LINK_LIBRARIES(AwWidget AwCore AwMath AwMapping AwUtilities AwFiltering Qt5::Core Qt5::Widgets Qt5::PrintSupport Qt5::Multimedia 
  Qt5::MultimediaWidgets ${VTK_LIBRARIES} ${QWT_LIBRARY}) 
INSTALL(TARGETS AwWidget DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
QT5_USE_MODULES(AwWidget Core Widgets PrintSupport Multimedia MultimediaWidgets X11Extras)
TARGET_LINK_LIBRARIES(AwWidget AwCore AwMath AwMapping AwUtilities AwFiltering Qt5::Core Qt5::Widgets Qt5::PrintSupport Qt5::Multimedia 
  Qt5::MultimediaWidgets ${VTK_LIBRARIES} ${QWT_LIBRARY} Qt5::X11Extras) 
INSTALL(TARGETS AwWidget DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()


