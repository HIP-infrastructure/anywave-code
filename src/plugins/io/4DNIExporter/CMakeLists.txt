project(4DNIExporter)

qt5_wrap_cpp(MOCS ./NI4DExporter.h)
add_library(4DNIExporter SHARED ${MOCS} ./NI4DExporter.cpp)
qt5_use_modules(4DNIExporter Core Widgets)
target_link_libraries(4DNIExporter AwCore AwReadWrite Qt5::Core Qt5::Widgets)

INSTALL(TARGETS 4DNIExporter DESTINATION ${PLUGIN_INSTALL_DIR})