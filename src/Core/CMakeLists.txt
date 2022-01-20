# CoreLib
project(AwCore)

add_definitions("-DAW_BUILD_CORE_LIB")

SET(HEADERS     
    ../../include/AwDataClient.h 
    ../../include/AwEvent.h 
    ../../include/AwEventManager.h 
    ../../include/AwMEGSensorManager.h 
    ../../include/AwSensor.h 
    ../../include/AwDataBuffer.h
    ../../include/AwVirtualChannel.h
    ../../include/AwGlobalMarkers.h)

SET(SRC  AwChannel.cpp 
    AwDataClient.cpp 
    AwMarker.cpp 
    AwMEGSensorManager.cpp 
    AwSensor.cpp 
    AwDataBuffer.cpp
    AwEvent.cpp
    AwEventManager.cpp AwGlobalMarkers.cpp)

QT5_WRAP_CPP(MOC ${HEADERS})
add_library(AwCore SHARED ${MOC} ${SRC})

qt5_use_modules(AwCore Core)

IF(APPLE)
TARGET_LINK_LIBRARIES(AwCore Qt5::Core)
ELSE()
TARGET_LINK_LIBRARIES(AwCore Qt5::Core)
ENDIF()
#target_compile_features(AwCore PRIVATE cxx_std_17)

IF (APPLE)
INSTALL(TARGETS AwCore DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwCore DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()






