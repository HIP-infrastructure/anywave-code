# MFV
project(MFV)
add_definitions("-DQT_PLUGIN")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})
INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE})

SET(SRCS
   MFV.cpp
   MFVGUI.cpp
   MFVTableModel.cpp)
 
SET(MOCS MFV.h  MFVGUI.h MFVTableModel.h)
SET(UIS MFVGUI.ui)

QT5_WRAP_CPP(COR_MOCS ${MOCS})
QT5_WRAP_UI(COR_UIS ${UIS})

add_library(MFV SHARED ${SRCS} ${COR_MOCS} ${COR_UIS})
qt5_use_modules(MFV Core Gui Widgets)
target_link_libraries(MFV AwCore AwProcess AwWidget Qt5::Core Qt5::Gui)
INSTALL(TARGETS MFV DESTINATION ${PLUGIN_INSTALL_DIR})









