# EpochLib
project(AwEpochLib)
add_definitions("-DAW_BUILD_EPOCH_LIB")

SET(HEADERS ../../include/epoch/AwEpochAverageChannel.h 
    ../../include/epoch/AwEpoch.h  ../../include/epoch/AwAvgEpoch.h 
    ../../include/epoch/AwEpochComputeSettings.h 
    ../../include/epoch/AwEpochDataBuffer.h
    ../../include/epoch/AwAvgEpoch.h ../../include/epoch/AwEpochSignalView.h  
    ../../include/epoch/AwEpochTree.h)

set(SRC AwEpochAverageChannel.cpp AwAvgEpoch.cpp
    ./AwEpoch.cpp 
    ./AwEpochComputeSettings.cpp 
    ./AwEpochDataBuffer.cpp 
    ./AwEpochTree.cpp ./AwEpochSignalView.cpp  )


QT5_WRAP_CPP(MOC ${HEADERS})

add_library(AwEpoch SHARED ${MOC} ${SRC})
qt5_use_modules(AwEpoch Core Widgets Gui)
TARGET_LINK_LIBRARIES(AwEpoch AwCore AwMath Qt5::Core Qt5::Widgets Qt5::Gui)

INSTALL(TARGETS AwEpoch DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)


