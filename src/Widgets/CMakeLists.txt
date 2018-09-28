# AwWidgetLib
project(AwWidget)
add_definitions("-DAW_BUILD_WIDGETS_LIB")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
QT5_ADD_RESOURCES(WIDGET_QRC ${AW_RES_DIR}/AnyWave.qrc ${AW_RES_DIR}/amplitudes.qrc)

SET(HEADERS ../../include/widget/AwMessageBox.h  
    ../../include/widget/AwWait.h  
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
    ../../include/widget/AwAmplitudeButton.h  
    ../../include/widget/AwMapWidget.h  
    ../../include/widget/AwTopoWidget.h  
    ./AwTimeScaleCombo.h  
    ./SignalView/AwGTCMenu.h  
    ../../include/widget/SignalView/AwBaseSignalView.h  
    ../../include/widget/SignalView/AwViewSettings.h  
    ../../include/widget/SignalView/AwBaseMarkerBar.h  
    ./SignalView/AwViewSettingsUi.h  
    ./SignalView/AwGoToLatencyDial.h  
    ../../include/widget/SignalView/AwGraphicsScene.h  
    ../../include/widget/SignalView/AwGraphicsView.h  
    ../../include/widget/SignalView/AwNavigationBar.h  
    ./SEEGViewer/AwSEEGInteractor.h  
    ../../include/widget/AwSEEGViewer.h  
    ../../include/widget/AwSEEGWidget.h  
    ./InputMarker/InputMarkerModel.h  
    ../../include/widget/AwInputMarkerTableView.h  
    ../../include/widget/AwInputMarkerWidget.h)

SET(SRC ./Aw2DGrid.cpp  
    ./AwAddPredefinedMarker.cpp  
    ./AwAddTargetChannels.cpp  
    ./AwComboCMap.cpp  
    ./AwComboColor.cpp  
    ./AwComboDS.cpp  
    ./AwComboMarker.cpp  
    ./AwDownSamplingWidget.cpp  
    ./AwMarkAroundWidget.cpp  
    ./AwMarkerInspector.cpp  
    ./AwAmplitudeButton.cpp  
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
    ./SignalView/AwBaseMarkerBar.cpp  
    ./SignalView/AwBaseSignalView.cpp  
    ./SignalView/AwGoToLatencyDial.cpp  
    ./SignalView/AwGraphicsScene.cpp  
    ./SignalView/AwGraphicsView.cpp  
    ./SignalView/AwGTCMenu.cpp  
    ./SignalView/AwNavigationBar.cpp  
    ./SignalView/AwViewSettings.cpp  
    ./SignalView/AwViewSettingsUi.cpp  
    ./SEEGViewer/AwSEEGInteractor.cpp  
    ./SEEGViewer/AwSEEGViewer.cpp  
    ./SEEGViewer/AwSEEGWidget.cpp  
    ./InputMarker/AwInputMarkerTableView.cpp  
    ./InputMarker/AwInputMarkerWidget.cpp  
    ./InputMarker/InputMarkerModel.cpp)

QT5_WRAP_CPP(MOC ${HEADERS})


SET(FORMS ./AwAmplitudeWidget.ui  
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
    ./SEEGViewer/AwSEEGWidget.ui  
    ./InputMarker/AwInputMarkerWidget.ui)

QT5_WRAP_UI(UI ${FORMS})

ADD_LIBRARY(AwWidget SHARED ${SRC} ${MOC} ${UI} ${WIDGET_QRC})
QT5_USE_MODULES(AwWidget Core Widgets PrintSupport)
TARGET_LINK_LIBRARIES(AwWidget AwCore AwGraphics AwMath AwMapping AwFiltering Qt5::Core Qt5::Widgets Qt5::PrintSupport ${VTK_LIBRARIES} ${QWT_LIBRARY}) 

