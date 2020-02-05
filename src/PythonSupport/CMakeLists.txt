SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Python)

MESSAGE("PYTHON HEADERS found in ${PYTHON_INCLUDE_DIRS}")
MESSAGE("numpy HEADERS found in ${PYTHON_NUMPY_INCLUDE_DIR}")
MESSAGE("Python libraries found in ${PYTHON_LIBRARIES}")
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS} ${PYTHON_NUMPY_INCLUDE_DIR})

SET(SOURCES  AddMarkers.cpp  Channel.cpp common.cpp GetData.cpp GetMarkers.cpp 
    GetPluginInfo.cpp Marker.cpp pythonmodule.cpp SendMessage.cpp GetDataEx.cpp)

add_library(anywave SHARED ${SOURCES})
target_link_libraries(anywave ${PYTHON_LIBRARIES} Qt5::Core Qt5::Network AwCore)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
SET_TARGET_PROPERTIES(anywave 
   PROPERTIES 
   SUFFIX ".pyd"
   PREFIX "")

qt5_use_modules(anywave Core Network)

INSTALL(TARGETS anywave DESTINATION ${CMAKE_INSTALL_PREFIX}/Python)

