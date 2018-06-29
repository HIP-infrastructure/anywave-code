# AwReadWriteLib
project(AwReadWrite)
add_definitions("-DAW_BUILD_RW_LIB")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
add_library(AwReadWrite SHARED AwReadWriteLib.cpp AwFileInfo.cpp)
QT5_USE_MODULES(AwReadWrite Core)
TARGET_LINK_LIBRARIES(AwReadWrite AwCore Qt5::Core)
