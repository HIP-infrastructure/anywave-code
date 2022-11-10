project(4DNIExporter)

qt5_wrap_cpp(MOCS ./NI4DExporter.h ./Aw4DNIExportDialog.h)
QT5_WRAP_UI(UI ./DialogButtonBottom.ui)
add_library(4DNIExporter SHARED ${MOCS} ${UI} ./NI4DExporter.cpp ./Aw4DNIExportDialog.cpp)
qt5_use_modules(4DNIExporter Core Widgets)
target_link_libraries(4DNIExporter AwCore AwReadWrite AwProcess AwUtilities AwWidget Qt5::Core Qt5::Widgets)

INSTALL(TARGETS 4DNIExporter DESTINATION ${PLUGIN_INSTALL_DIR})