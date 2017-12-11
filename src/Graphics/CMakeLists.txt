# AwGraphicsLib
project(AwGraphicsLib)
add_definitions("-DAW_BUILD_GRAPHICS_LIB")

set (AwGraphicsLib_SRCS
    AwAmplitudeManager.cpp
	AwColorMap.cpp
	AwQwtColorMap.cpp
	AwGraphicsObjects.cpp
	SignalItem/AwSIAmpButton.cpp
	SignalItem/AwSignalItem.cpp
	SignalItem/AwSignalItemSettings.cpp
	Mark/AwHighLightPositionMarker.cpp
	MarkerItem/AwCursorItem.cpp
	MarkerItem/AwHighLightMarker.cpp
	MarkerItem/AwMappingCursorItem.cpp
	MarkerItem/AwMarkerChannelItem.cpp
	MarkerItem/AwMarkerItem.cpp
	MarkerItem/AwMarkerLabelButton.cpp
	MarkerItem/AwMarkerValueButton.cpp
	SignalItem/AwLabelItem.cpp
    SignalItem/AwSignalLabelItem.cpp)
	
set(AwGraphicsLib_MOC_H 
	${CMAKE_SOURCE_DIR}/include/graphics/AwGraphicsObjects.h
	${CMAKE_SOURCE_DIR}/include/graphics/AwHighLightPositionMarker.h
	${CMAKE_SOURCE_DIR}/include/graphics/AwSignalItem.h
	${CMAKE_SOURCE_DIR}/include/AwAmplitudeManager.h
	SignalItem/AwSignalItemSettings.h
	SignalItem/AwSIAmpButton.h
	MarkerItem/AwMarkerLabelButton.h
	MarkerItem/AwMarkerValueButton.h)

# .ui files
SET(AwGraphicsLib_UI_HEADERS
	SignalItem/AwSignalItemSettings.ui)
	
qt5_wrap_cpp(AwGraphicsLib_MOC_SRCS ${AwGraphicsLib_MOC_H})
qt5_wrap_ui(AwGraphicsLib_UI ${AwGraphicsLib_UI_HEADERS})
add_library(AwGraphicsLib SHARED ${AwGraphicsLib_SRCS} ${AwGraphicsLib_MOC_SRCS} ${AwGraphicsLib_UI})
qt5_use_modules(AwGraphicsLib Core Gui Widgets)
TARGET_LINK_LIBRARIES(AwGraphicsLib AwCoreLib AwUtilitiesLib Qt5::Core Qt5::Gui Qt5::Widgets ${QWT_LIBRARIES})
