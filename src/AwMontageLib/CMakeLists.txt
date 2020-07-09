# AwMontageLib
project(AwMontage)
SET(AUTOMOC ON)
add_definitions("-DAW_BUILD_MONTAGE_LIB")
SET(HEADERS ${CMAKE_SOURCE_DIR}/include/montage/AwMontage.h ${CMAKE_SOURCE_DIR}/include/montage/AwAVGChannel.h)
ADD_LIBRARY(AwMontage SHARED ${AwMontageMOC} AwMontage.cpp AwAVGChannel.cpp)
QT5_USE_MODULES(AwMontage Core Xml)
TARGET_LINK_LIBRARIES(AwMontage Qt5::Core Qt5::Xml AwCore AwReadWrite)

IF (APPLE)
INSTALL(TARGETS AwMontage DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwMontage DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()




