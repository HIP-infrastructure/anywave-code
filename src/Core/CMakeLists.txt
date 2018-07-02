# CoreLib
project(AwCore)
add_definitions("-DAW_BUILD_CORE_LIB")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(HEADERS     ../../include/AwChannel.h  
    ../../include/AwDataClient.h 
    ../../include/AwFilteringOptions.h 
    ../../include/AwMarker.h 
    ../../include/AwMEGSensorManager.h 
    ../../include/AwSensor.h 
    ../../include/AwDataBuffer.h 
    ../../include/AwVirtualChannel.h)
SET(SRC     AwChannel.cpp 
    AwDataClient.cpp 
    AwFilteringOptions.cpp 
    AwMarker.cpp 
    AwMEGSensorManager.cpp 
    AwSensor.cpp 
    AwDataBuffer.cpp)

QT5_WRAP_CPP(MOC ${HEADERS})

add_library(AwCore SHARED ${MOC} ${SRC})
qt5_use_modules(AwCore Core)
TARGET_LINK_LIBRARIES(AwCore Qt5::Core)




