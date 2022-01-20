# AwMappingLib
project(AwMapping)
add_definitions("-DAW_BUILD_MAPPING_LIB")

find_package(VTK
 COMPONENTS
    CommonCore
    CommonDataModel
    CommonTransforms
    FiltersCore
    FiltersGeneral
    FiltersGeometry
    FiltersModeling
    FiltersSources
    IOXML
    # These modules are suggested since they implement an existing module.
    # Uncomment those you need.
    # FiltersParallel # implements VTK::FiltersCore
)

SET(MOCS ${CMAKE_SOURCE_DIR}/include/mapping/AwMap.h)
QT5_WRAP_CPP(AwMappingMOC ${MOCS})
ADD_LIBRARY(AwMapping SHARED ${AwMappingMOC} AwMap.cpp AwMeshManager.cpp)
QT5_USE_MODULES(AwMapping Core)
#TARGET_LINK_LIBRARIES(AwMapping Qt5::Core AwCore AwLayout ${VTK_LIBRARIES})
TARGET_LINK_LIBRARIES(AwMapping Qt5::Core AwCore AwLayout VTK::CommonCore VTK::CommonDataModel VTK::CommonTransforms VTK::FiltersCore 
        VTK::FiltersGeneral VTK::FiltersGeometry VTK::FiltersModeling VTK::FiltersSources VTK::IOXML)
vtk_module_autoinit(TARGETS AwMapping MODULES VTK::RenderingOpenGL2 VTK::InteractionStyle VTK::RenderingFreeType)

IF (APPLE)
INSTALL(TARGETS AwMapping DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwMapping DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()



