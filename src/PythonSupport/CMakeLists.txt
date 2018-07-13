SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Python)

MESSAGE("PYTHON HEADERS found in ${PYTHON_INCLUDE_DIRS}")
MESSAGE("numpy HEADERS found in ${PYTHON_NUMPY_INCLUDE_DIR}")
MESSAGE("Python libraries found in ${PYTHON_LIBRARIES}")
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS} ${PYTHON_NUMPY_INCLUDE_DIR})
INCLUDE_DIRECTORIES(/usr/include/python2.7)
SET(SOURCES  AddMarkers.cpp  Channel.cpp common.cpp GetData.cpp GetMarkers.cpp 
    GetPluginInfo.cpp Marker.cpp pythonmodule.cpp SendMessage.cpp)

add_library(anywave SHARED ${SOURCES})
qt5_use_modules(anywave Core Network)
target_link_libraries(anywave ${PYTHON_LIBRARIES} Qt5::Core Qt5::Network AwCore)


