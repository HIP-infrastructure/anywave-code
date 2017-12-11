# Python Module

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Python)

SET (SRC common.cpp Channel.cpp Marker.cpp GetData.cpp AddMarkers.cpp GetPluginInfo.cpp
   pythonmodule.cpp GetMarkers.cpp SendMessage.cpp)

INCLUDE_DIRECTORIES(/users/bruno/anaconda/include/python2.7)
INCLUDE_DIRECTORIES(/users/bruno/anaconda/pkgs/numpy-1.12.1-py27_0/lib/python2.7/site-packages/numpy/core/include)
LINK_DIRECTORIES(/users/bruno/anaconda/include/lib)

add_library(anywave SHARED ${SRC})
qt5_use_modules(anywave Core Network)
target_link_libraries(anywave AwCoreLib Qt5::Core Qt5::Network)

