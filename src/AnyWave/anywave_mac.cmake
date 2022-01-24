ADD_EXECUTABLE(AnyWaveMac ${AnyWave_SRCS} ${MOC} ${UI} ${AnyWave_QRC})
QT5_USE_MODULES(AnyWaveMac Core Gui Network Widgets Xml Concurrent Svg PrintSupport Qml)
MESSAGE(STATUS "Mac qwt: ${QWT_LIBRARIES}")
TARGET_LINK_LIBRARIES(AnyWaveMac ${AW_LIBS} ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${QT_LIBS} ${VTK_LIBRARIES} ${QWT_LIBRARIES} ${MATIO_LIBRARY} 
   ${BLAS_LIBRARIES} z qcustomplot)
add_dependencies(AnyWaveMac ${AW_LIBS})
set_target_properties(AnyWaveMac PROPERTIES OUTPUT_NAME "anywavemac")
INSTALL(TARGETS AnyWaveMac DESTINATION ${APP_INSTALL_DIR})