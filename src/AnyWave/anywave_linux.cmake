

 ADD_EXECUTABLE(AnyWaveLinux ${AnyWave_SRCS} ${MOC} ${UI} ${AnyWave_QRC})
 
 QT5_USE_MODULES(AnyWaveLinux Core Gui Network Widgets Xml Concurrent Svg PrintSupport Qml X11Extras)
 TARGET_LINK_LIBRARIES(AnyWaveLinux ${AW_LIBS} ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${QT_LIBS} ${VTK_LIBRARIES} ${QWT_LIBRARIES} 
 gomp ${MATIO_LIBRARY} ${BLAS_LIBRARIES} z qcustomplot Qt5::X11Extras)

 add_dependencies(AnyWaveLinux ${AW_LIBS})
 INSTALL(TARGETS AnyWaveLinux PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE DESTINATION ${CMAKE_INSTALL_PREFIX}) 