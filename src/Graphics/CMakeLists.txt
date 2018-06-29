# AwGraphicsLib
project(AwGraphics)
add_definitions("-DAW_BUILD_GRAPHICS_LIB")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

set (AwGraphicsLib_SRCS
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
    AwAmplitudeManager.cpp 
    AwColorMap.cpp 
    AwGraphicsObjects.cpp 
    AwQwtColorMap.cpp)
	
set(HEADERS  ../../include/graphics/AwGraphicsObjects.h 
../../include/graphics/AwGraphicInterface.h 
	../../include/AwAmplitudeManager.h 
    MarkerItem/AwMarkerLabelButton.h 
    MarkerItem/AwMarkerValueButton.h 
    SignalItem/AwSIAmpButton.h 
    SignalItem/AwSignalItemSettings.h 
    ../../include/graphics/AwColorMap.h 
   ../../include/graphics/AwCursorItem.h 
   ../../include/graphics/AwGraphicInterface.h 
   ../../include/graphics/AwGraphicsDefines.h 
   ../../include/graphics/AwGraphicsObjects.h 
   ../../include/graphics/AwHighLightMarker.h 
   ../../include/graphics/AwHighLightPositionMarker.h 
   ../../include/graphics/AwLabelItem.h 
   ../../include/graphics/AwMappingCursorItem.h 
   ../../include/graphics/AwMarkerChannelItem.h 
   ../../include/graphics/AwMarkerItem.h 
   ../../include/graphics/AwQwtColorMap.h 
   ../../include/graphics/AwSignalItem.h 
   ../../include/graphics/AwSignalLabelItem.h)

QT5_WRAP_CPP(MOC ${HEADERS})

SET(FORMS SignalItem/AwSignalItemSettings.ui)
QT5_WRAP_UI(UI ${FORMS})

add_library(AwGraphics SHARED ${AwGraphicsLib_SRCS} ${MOC} ${UI})
qt5_use_modules(AwGraphics Core Gui Widgets)
TARGET_LINK_LIBRARIES(AwGraphics AwCore AwUtilities Qt5::Core Qt5::Gui Qt5::Widgets ${QWT_LIBRARIES})
