# AwUtilities
project(AwUtilities)
add_definitions("-DAW_BUILD_UTILITIES_LIB")
add_library(AwUtilities SHARED AwUtilities.cpp json.cpp bids.cpp)
QT5_USE_MODULES(AwUtilities Core Gui)
TARGET_LINK_LIBRARIES(AwUtilities AwCore Qt5::Core Qt5::Gui)
IF (APPLE)
INSTALL(TARGETS AwUtilities DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwUtilities DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()

