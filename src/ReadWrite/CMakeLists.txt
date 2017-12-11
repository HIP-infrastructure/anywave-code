# AwReadWriteLib
project(AwReadWriteLib)
add_definitions("-DAW_BUILD_RW_LIB")
add_library(AwReadWriteLib SHARED AwReadWriteLib.cpp)
QT5_USE_MODULES(AwReadWriteLib Core)
TARGET_LINK_LIBRARIES(AwReadWriteLib AwCoreLib Qt5::Core)
