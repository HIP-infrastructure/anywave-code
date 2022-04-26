# AwMappingLib
project(AwMapping)
add_definitions("-DAW_BUILD_MAPPING_LIB")

SET(MOCS ${CMAKE_SOURCE_DIR}/include/mapping/AwMap.h)
QT5_WRAP_CPP(AwMappingMOC ${MOCS})
ADD_LIBRARY(AwMapping SHARED ${AwMappingMOC} AwMap.cpp AwMeshManager.cpp)
QT5_USE_MODULES(AwMapping Core)
TARGET_LINK_LIBRARIES(AwMapping Qt5::Core AwCore AwLayout ${VTK_LIBRARIES})


IF (APPLE)
INSTALL(TARGETS AwMapping DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwMapping DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()



