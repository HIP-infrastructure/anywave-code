# Rewrite 4D
project(Rewrite4D)
add_definitions("-DQT_PLUGIN")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS ChooseMarker.cpp rewrite4d.cpp rewriteui.cpp)

SET(MOCS ChooseMarker.h rewrite4d.h rewriteui.h)

SET(UIS ChooseMarker.ui rewriteui.ui)

qt5_wrap_cpp(Rewrite_MOCS ${MOCS})
qt5_wrap_ui(Rewrite_UIS ${UIS})

add_library(Rewrite4D SHARED ${SRCS} ${Rewrite_MOCS} ${Rewrite_UIS})
target_link_libraries(Rewrite4D AwCore AwProcess AwReadWrite Qt5::Core Qt5::Gui Qt5::Widgets)
INSTALL(TARGETS Rewrite4D DESTINATION ${PLUGIN_INSTALL_DIR})





