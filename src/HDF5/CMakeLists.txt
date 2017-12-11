# HDF5Lib
project(AwHDF5Lib)
add_definitions("-DAW_BUILD_HDF5_LIB")
add_library(AwHDF5Lib SHARED AwHDF5.cpp)
qt5_use_modules(AwHDF5Lib Core)
TARGET_LINK_LIBRARIES(AwHDF5Lib AwCoreLib Qt5::Core ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})
