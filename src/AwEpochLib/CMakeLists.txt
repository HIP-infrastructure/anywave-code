# EpochLib
project(AwEpochLib)
add_definitions("-DAW_BUILD_EPOCH_LIB")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
SET(HEADERS ../../include/epoch/AwEpochAverageChannel.h 
    ../../include/epoch/AwEpoch.h 
    ../../include/epoch/AwEpochComputeSettings.h 
    ../../include/epoch/AwEpochDataBuffer.h 
    ../../include/epoch/AwEpochTree.h)

set(SRC AwEpochAverageChannel.cpp 
    ./AwEpoch.cpp 
    ./AwEpochComputeSettings.cpp 
    ./AwEpochDataBuffer.cpp 
    ./AwEpochTree.cpp)


QT5_WRAP_CPP(MOC ${HEADERS})

add_library(AwEpoch SHARED ${MOC} ${SRC})
qt5_use_modules(AwEpoch Core Widgets Gui)
TARGET_LINK_LIBRARIES(AwEpoch AwCore AwMath Qt5::Core Qt5::Widgets Qt5::Gui)




