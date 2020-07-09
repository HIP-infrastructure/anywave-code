# AwReadWriteLib
project(AwReadWrite)
add_definitions("-DAW_BUILD_RW_LIB")
add_library(AwReadWrite SHARED AwReadWriteLib.cpp AwFileInfo.cpp)
QT5_USE_MODULES(AwReadWrite Core)
TARGET_LINK_LIBRARIES(AwReadWrite AwCore Qt5::Core)

IF (APPLE)
INSTALL(TARGETS AwReadWrite DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwReadWrite DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()


