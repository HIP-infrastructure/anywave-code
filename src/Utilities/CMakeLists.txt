# AwUtilities
project(AwUtilitiesLib)
add_definitions("-DAW_BUILD_UTILITIES_LIB")

add_library(AwUtilitiesLib SHARED AwUtilities.cpp)
QT5_USE_MODULES(AwUtilitiesLib Core Gui)
TARGET_LINK_LIBRARIES(AwUtilitiesLib AwCoreLib AwMathLib Qt5::Core Qt5::Gui ${MATIO_LIBRARY})
