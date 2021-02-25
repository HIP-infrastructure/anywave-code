SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Python)

MESSAGE("PYTHON HEADERS found in ${Python3_INCLUDE_DIRS}")
MESSAGE("numpy HEADERS found in ${Python3_NumPy_INCLUDE_DIR}")
MESSAGE("Python libraries found in ${Python3_LIBRARY}")
INCLUDE_DIRECTORIES(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS} ${PYTHON_NUMPY_INCLUDE_DIR})

SET(SOURCES  AddMarkers.cpp  Channel.cpp common.cpp GetData.cpp GetMarkers.cpp 
    GetPluginInfo.cpp Marker.cpp pythonmodule.cpp SendMessage.cpp GetDataEx.cpp)
IF(APPLE)
    SET(PYTHON_INSTALL_DIR "${PROJECT_BINARY_DIR}/${APP_BUNDLE}/Contents/Python")
    SET(CMAKE_INSTALL_RPATH "${PYTHON_INSTALL_DIR}/../Frameworks")
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
ENDIF()
add_library(anywave SHARED ${SOURCES})
target_link_libraries(anywave ${Python3_LIBRARY} Qt5::Core Qt5::Network AwCore)
IF(NOT APPLE)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
ENDIF()
SET_TARGET_PROPERTIES(anywave 
   PROPERTIES 
   SUFFIX ".pyd"
   PREFIX "")

qt5_use_modules(anywave Core Network)

INSTALL(TARGETS anywave DESTINATION ${PYTHON_INSTALL_DIR})

