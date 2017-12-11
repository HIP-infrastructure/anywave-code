# AwLayoutLib
project(AwLayoutLib)
add_definitions("-DAW_BUILD_LAYOUT_LIB")
QT5_ADD_RESOURCES(res ${AW_RES_DIR}/layouts.qrc)
ADD_LIBRARY(AwLayoutLib SHARED AwLayout.cpp AwLayoutManager.cpp ${res})
QT5_USE_MODULES(AwLayoutLib Core)
TARGET_LINK_LIBRARIES(AwLayoutLib AwCoreLib AwReadWriteLib Qt5::Core)


