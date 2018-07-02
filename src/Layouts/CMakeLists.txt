# AwLayoutLib
project(AwLayout)
add_definitions("-DAW_BUILD_LAYOUT_LIB")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
QT5_ADD_RESOURCES(res ${AW_RES_DIR}/layouts.qrc)
ADD_LIBRARY(AwLayout SHARED AwLayout.cpp AwLayoutManager.cpp ${res})
QT5_USE_MODULES(AwLayout Core)
TARGET_LINK_LIBRARIES(AwLayout AwCore AwReadWrite Qt5::Core)


