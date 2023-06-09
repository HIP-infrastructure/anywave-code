

 ADD_EXECUTABLE(AnyWaveLinux ${AnyWave_SRCS} ${MOC} ${UI} ${AnyWave_QRC})
 
 QT5_USE_MODULES(AnyWaveLinux Core Gui Network Widgets Xml Concurrent Svg PrintSupport Qml MultimediaWidgets)
 TARGET_LINK_LIBRARIES(AnyWaveLinux ${AW_LIBS} ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${VTK_LIBRARIES} ${QWT_LIBRARIES} 
     gomp ${ARMA_LIB} z Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml Qt5::Svg Qt5::Network Qt5::X11Extras qcustomplot)

 add_dependencies(AnyWaveLinux ${AW_LIBS})
 INSTALL(TARGETS AnyWaveLinux PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE DESTINATION ${CMAKE_INSTALL_PREFIX}) 