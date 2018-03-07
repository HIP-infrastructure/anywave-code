# AwMappingLib
project(AwMappingLib)
add_definitions("-DAW_BUILD_MAPPING_LIB")
SET(MOCS ${CMAKE_SOURCE_DIR}/include/mapping/AwMap.h)
QT5_WRAP_CPP(AwMappingMOC ${MOCS})
ADD_LIBRARY(AwMappingLib SHARED ${AwMappingMOC} AwMap.cpp AwMeshManager.cpp)
QT5_USE_MODULES(AwMappingLib Core)
TARGET_LINK_LIBRARIES(AwMappingLib Qt5::Core AwCoreLib AwLayoutLib ${VTK_LIBRARIES})


