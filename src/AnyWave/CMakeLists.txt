# AnyWave
# tell armadillo not to link with wrapper lib
ADD_DEFINITIONS("-DARMA_DONT_USE_WRAPPER -DARMA_USE_BLAS -DARMA_USE_LAPACK")

SET(AnyWave_SRCS
Carto/AwMappingWidget.cpp
Carto/AwDockMapping.cpp
Carto/AwMappingClient.cpp
Data/AwDataServer.cpp
Data/AwDataConnection.cpp
Debug/AwDebugLog.cpp 
Debug/AwDebugLogWidget.cpp 
Display/AwChannelSelectionDial.cpp 
Display/AwChannelSelectionModel.cpp 
Display/AwDisplay.cpp 
Display/AwDisplaySetup.cpp 
Display/AwDisplaySetupManager.cpp 
Display/AwSaveAsNewSetupDialog.cpp 
Display/AwScene.cpp 
Display/AwSignalView.cpp 
Display/AwDSMSaveSetupUI.cpp
Display/AwNewViewDial.cpp
Display/AwViewSetup.cpp
Display/AwCentralWidget.cpp
Filter/AwFilteringManager.cpp 
Filter/AwFilterSettings.cpp 
Filter/AwFilterSwitchButton.cpp
Filter/AwFilterToolBar.cpp
ICA/AwICAChannel.cpp
ICA/AwICAComponents.cpp
ICA/AwICAManager.cpp
ICA/AwICARejectButton.cpp
ICA/AwICASignalItem.cpp
ICA/AwMappingButton.cpp
ICA/AwICAPanel.cpp
ICA/AwPanelItem.cpp
Source/AwSourceChannel.cpp
Source/AwSourceManager.cpp
IO/ADES/ADESIO.cpp 
IO/HDF5/HDF5IO.cpp 
IO/HDF5/structs.cpp
IO/MATLAB/MATLABIO.cpp
Marker/AwMarkerFindReplaceUi.cpp 
Marker/AwMarkerManagerSettings.cpp 
Marker/AwMarkerRuleManageDial.cpp 
Marker/AwMarkerRuleWidget.cpp 
Marker/AwDockAddMarker.cpp
Marker/AwNewMarkerRuleDial.cpp 
Marker/AwMarkerListModel.cpp 
Marker/AwMarkerManager.cpp 
Marker/AwMarkerRule.cpp 
Marker/AwMarkerRuleElement.cpp 
Marker/AwMarkerTableView.cpp 
Marker/AwExportMarkersDial.cpp 
Marker/AwPickChannelsDial.cpp
Marker/AwExtractTriggers.cpp
Marker/AwMarkersExportWidget.cpp
MATPy/AwMATPyServer.cpp
MATPy/AwRequestServer.cpp
MATPy/AwResponse.cpp
MATPy/AddMarkers.cpp
MATPy/AwPidManager.cpp
MATPy/AwRequestServer.cpp
MATPy/GetPluginInfo.cpp
MATPy/SendCommand.cpp
MATPy/IsTerminated.cpp
MATPy/SendMessage.cpp
MATPy/GetData3.cpp
MATPy/GetMarkers2.cpp
MATPy/GetFileInfo.cpp
MATPy/GetScreenCapture.cpp
MATPy/GetICAPanelCapture.cpp
MATPy/SetBeamFormer.cpp
MATPy/GetTriggers.cpp
Montage/AwChannelListModel.cpp 
Montage/AwMontageDial.cpp 
Montage/AwMontageManager.cpp 
Montage/AwMontageTableView.cpp 
Montage/AwAVGChannel.cpp
Process/AwExporter.cpp
Process/AwExporterSelMarkers.cpp
Process/AwExportSelChannels.cpp
Process/AwExporterMarkerModel.cpp
Process/AwExporterSettings.cpp
Process/AwConverter.cpp 
Process/AwMatlabScriptPlugin.cpp 
Process/AwPythonScriptPlugin.cpp 
Process/AwScriptPlugin.cpp 
Process/AwTriggerParser.cpp 
Process/TPUi.cpp 
Process/AwProcessFromMarkersDial.cpp 
Process/AwProcessManager.cpp 
Process/AwProcessMarkerModel.cpp 
Process/AwProcessesWidget.cpp 
Process/AwProcessWidget.cpp
Process/AwProcessLogWindow.cpp
Process/AwProgressIndicator.cpp
Process/AwProcessLogManager.cpp
Process/AwProcessLog.cpp
Process/AwMarkerExporter.cpp
Widgets/AwAboutAnyWave.cpp 
Widgets/AwCursorMarkerToolBar.cpp 
Widgets/AwDisplaySetupToolBar.cpp 
Widgets/AwDisplayToolBar.cpp 
Widgets/AwDockFilter.cpp 
Widgets/AwFilePropertiesDialog.cpp 
Widgets/AwFilePropertiesModel.cpp 
Widgets/AwFileToolBar.cpp 
Widgets/AwMappingToolBar.cpp 
Widgets/AwMarkersBar.cpp 
Widgets/AwMontageRefWidget.cpp 
Widgets/AwMontageToolBar.cpp 
Widgets/AwPleaseWaitWidget.cpp 
Widgets/AwTriggerParsingDialog.cpp 
Plugin/AwPluginDial.cpp 
Plugin/AwPluginManager.cpp 
Prefs/AwPreferences.cpp 
Prefs/AwPrefsDial.cpp 
Prefs/AwSettings.cpp 
Script/AwScript.cpp
Script/AwScriptLog.cpp
Script/AwScriptManager.cpp
Script/AwScriptLogManager.cpp
Script/AwScriptProcessFileInput.cpp
Script/AwScriptWidget.cpp
Script/AwScriptsWidget.cpp
AwSelectTriggerChannelDialog.cpp
AwUpdater.cpp
AwExportDial.cpp 
AnyWave.cpp
main.cpp)

SET(AnyWave_MOC_HEADERS
   Carto/AwMappingWidget.h
   Carto/AwDockMapping.h
   Carto/AwMappingClient.h
   Data/AwDataServer.h 
   Data/AwDataConnection.h 
   Debug/AwDebugLog.h 
   Debug/AwDebugLogWidget.h   
   Display/AwChannelSelectionDial.h 
   Display/AwChannelSelectionModel.h 
   Display/AwDisplay.h 
   Display/AwDisplaySetup.h 
   Display/AwDisplaySetupManager.h 
   Display/AwDSMSaveSetupUI.h
   Display/AwSaveAsNewSetupDialog.h 
   Display/AwScene.h 
   Display/AwSignalView.h 
   Display/AwDSMSaveSetupUI.h
   Display/AwNewViewDial.h
   Display/AwViewSetup.h
   Display/AwCentralWidget.h
   Filter/AwFilteringManager.h 
   Filter/AwFilterSettings.h 
   Filter/AwFilterSwitchButton.h
   Filter/AwFilterToolBar.h
   ICA/AwICAComponents.h
   ICA/AwICAManager.h
   ICA/AwICARejectButton.h
   ICA/AwICASignalItem.h
   ICA/AwMappingButton.h
   ICA/AwPanelItem.h
   ICA/AwICAPanel.h
   Source/AwSourceManager.h
   IO/ADES/ADESIO.h 
   IO/HDF5/HDF5IO.h 
   IO/MATLAB/MATLABIO.h
   MATPy/AwMATPyServer.h
   MATPy/AwPidManager.h
   MATPy/AwRequestServer.h
   Marker/AwDockAddMarker.h
   Marker/AwNewMarkerRuleDial.h 
   Marker/AwMarkerFindReplaceUi.h 
   Marker/AwMarkerManagerSettings.h 
   Marker/AwMarkerRuleManageDial.h 
   Marker/AwMarkerRuleWidget.h 
   Marker/AwExportMarkersDial.h  
   Marker/AwMarkerListModel.h 
   Marker/AwMarkerManager.h 
   Marker/AwMarkerRule.h 
   Marker/AwMarkerRuleElement.h 
   Marker/AwMarkerTableView.h 
   Marker/AwPickChannelsDial.h
   Marker/AwMarkersExportWidget.h
   Montage/AwChannelListModel.h 
   Montage/AwMontageDial.h 
   Montage/AwMontageManager.h 
   Montage/AwMontageTableView.h 
   Plugin/AwPluginDial.h 
   Plugin/AwPluginManager.h 
   Prefs/AwSettings.h 
   Prefs/AwPrefsDial.h
   Process/AwConverter.h
   Process/AwExporter.h
   Process/AwExportSelChannels.h
   Process/AwExporterMarkerModel.h
   Process/AwExporterSelMarkers.h
   Process/AwExporterSettings.h
   Process/AwMatlabScriptPlugin.h 
   Process/AwPythonScriptPlugin.h 
   Process/AwScriptPlugin.h 
   Process/AwTriggerParser.h 
   Process/TPUi.h 
   Process/AwProcessFromMarkersDial.h 
   Process/AwProcessManager.h 
   Process/AwProcessMarkerModel.h 
   Process/AwProcessesWidget.h 
   Process/AwProcessWidget.h
   Process/AwProgressIndicator.h 
   Process/AwProcessLogWindow.h
   Process/AwProcessLog.h
   Process/AwProcessLogManager.h
   Process/AwMarkerExporter.h
   Script/AwScript.h
   Script/AwScriptLog.h
   Script/AwScriptManager.h
   Script/AwScriptLogManager.h
   Script/AwScriptProcessFileInput.h
   Script/AwScriptWidget.h
   Script/AwScriptsWidget.h
   Widgets/AwAboutAnyWave.h 
   Widgets/AwCursorMarkerToolBar.h 
   Widgets/AwDisplaySetupToolBar.h 
   Widgets/AwDisplayToolBar.h 
   Widgets/AwDockFilter.h 
   Widgets/AwFilePropertiesDialog.h 
   Widgets/AwFilePropertiesModel.h 
   Widgets/AwFileToolBar.h 
   Widgets/AwMappingToolBar.h 
   Widgets/AwMontageRefWidget.h 
   Widgets/AwMontageToolBar.h 
   Widgets/AwPleaseWaitWidget.h 
   Widgets/AwTriggerParsingDialog.h 
   Widgets/AwMarkersBar.h
   AwExportDial.h 
   AwUpdater.h
   AwSelectTriggerChannelDialog.h 
   AnyWave.h)

QT5_WRAP_CPP(AnyWave_MOC_SRCS ${AnyWave_MOC_HEADERS})


# .ui files
SET(AnyWave_UIS
AnyWave.ui 
AwSelectTriggerChannelDialog.ui 
AwExportDial.ui 
Widgets/AwAboutAnyWave.ui 
Widgets/AwCursorMarkerToolBar.ui 
Widgets/AwDisplaySetupToolBar.ui 
Widgets/AwFilePropertiesDialog.ui
Widgets/AwFileToolBar.ui  
Widgets/AwMappingToolBar.ui
Widgets/AwMontageToolBar.ui 
Widgets/AwPleaseWaitWidget.ui 
Widgets/AwDisplayToolBarv2.ui 
Widgets/AwTriggerParsingDialog.ui 
Filter/AwFilterToolBar.ui 
Filter/AwFilterSettings.ui 
Marker/AwExportMarkersWizard.ui
Marker/AwMarkerFindReplaceUi.ui 
Marker/AwMarkerManagerSettings.ui 
Marker/AwMarkerRuleManageDial.ui 
Marker/AwMarkerRuleWidget.ui 
Marker/AwNewMarkerRuleDial.ui 
Marker/AwPickChannelsDial.ui
Marker/AwMarkersExportWidget.ui
Montage/AwMontageDial.ui 
Plugin/AwPluginDial.ui 
Prefs/AwPrefsDial.ui
Process/AwExporterSettings.ui
Process/AwExporterSelMarkers.ui
Process/AwExportSelChannels.ui
Process/AwProcessesWidget.ui 
Process/AwProcessWidget.ui 
Process/AwProcessFromMarkersDial.ui 
Process/AwProcessLogWindow.ui
Process/TPUi.ui 
Process/AwStatusBarProcessIndicatorWidget.ui 
Carto/AwMappingWidget.ui 
Display/AwChannelSelectionDial.ui 
Display/AwSaveAsNewSetupDialog.ui 
Display/AwDSMSaveSetupUI.ui
Display/AwNewViewDial.ui
Debug/AwDebugLogWidget.ui
ICA/AwICAPanel.ui
ICA/AwPanelItem.ui
Script/AwScriptWidget.ui
Script/AwScriptsWidget.ui)

QT5_WRAP_UI(AnyWave_UI_HEADERS ${AnyWave_UIS})
QT5_ADD_RESOURCES(AnyWave_QRC ${AW_RES_DIR}/AnyWave.qrc)

# translations
SET(Languages ${AW_RES_DIR}/anywave_fr.ts
${AW_RES_DIR}/awreadwritelib_fr.ts
${AW_RES_DIR}/awwidgetlib_fr.ts)
qt5_create_translation(AnyWaveTS ${Languages})

SET(AW_LIBS AwCoreLib AwGraphicsLib AwWidgetLib AwHDF5Lib AwReadWriteLib AwProcessLib 
	AwUtilitiesLib AwLayoutLib AwMathLib AwMappingLib AwFilteringLib AwMATLABLib)

# Linux
FIND_PACKAGE(OpenBLAS REQUIRED) # Required by armadillo

ADD_EXECUTABLE(AnyWaveLinux ${AnyWave_SRCS} ${AnyWave_MOC_SRCS} ${AnyWave_UI_HEADERS} ${AnyWave_QRC} ${AnyWave_TS})
QT5_USE_MODULES(AnyWaveLinux Core Gui Network Widgets Xml Script Concurrent Svg PrintSupport)
SET(QT_LIBS Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml Qt5::Network Qt5::Svg Qt5::Concurrent Qt5::PrintSupport)
TARGET_LINK_LIBRARIES(AnyWaveLinux  ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${AW_LIBS} ${QT_LIBS} ${VTK_LIBRARIES} ${QWT_LIBRARIES} gomp ${MATIO_LIBRARY})
add_dependencies(AnyWaveLinux AwCoreLib AwGraphicsLib AwWidgetLib AwProcessLib AwUtilitiesLib AwMathLib AwLayoutLib)
INSTALL(FILES ${AW_RES_DIR}/linux/Anywave.sh PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_EXECUTE DESTINATION ${PROJECT_BINARY_DIR}/bin)
#License files
INSTALL(FILES ${AW_RES_DIR}/Licence_fr.html ${AW_RES_DIR}/License_en.html DESTINATION
	${PROJECT_BINARY_DIR}/bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/cmake DESTINATION ${PROJECT_BINARY_DIR})


