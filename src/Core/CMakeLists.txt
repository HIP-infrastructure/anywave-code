# CoreLib
project(AwCoreLib)
add_definitions("-DAW_BUILD_CORE_LIB")
SET(MOCS ${CMAKE_SOURCE_DIR}/include/AwDataClient.h)
QT5_WRAP_CPP(AwCoreLib_MOC ${MOCS})
add_library(AwCoreLib SHARED AwChannel.cpp AwFilteringOptions.cpp AwMarker.cpp AwSensor.cpp AwMEGSensorManager.cpp AwDataClient.cpp ${AwCoreLib_MOC})
qt5_use_modules(AwCoreLib Core)
TARGET_LINK_LIBRARIES(AwCoreLib Qt5::Core)




