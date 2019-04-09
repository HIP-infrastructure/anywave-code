# AwMontageLib
project(AwMontage)
add_definitions("-DAW_BUILD_MONTAGE_LIB")
SET(MOCS ${CMAKE_SOURCE_DIR}/include/AwMontage.h)
QT5_WRAP_CPP(AwMontageMOC ${MOCS})
ADD_LIBRARY(AwMontage SHARED ${AwMontageMOC} AwMontage.cpp)
QT5_USE_MODULES(AwMontage Core Xml)
TARGET_LINK_LIBRARIES(AwMontage Qt5::Core Qt5::Xml AwCore)
INSTALL(TARGETS AwMontage DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)


