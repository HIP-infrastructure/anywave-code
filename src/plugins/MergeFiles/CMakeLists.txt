# MergeFiles
project(MergeFiles)
add_definitions("-DQT_PLUGIN")
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
mergefiles.cpp settings.cpp) 

SET(MOCS
mergefiles.h settings.h)

SET(UIS
settings.ui)

qt5_wrap_cpp(MF_MOCS ${MOCS})
QT5_WRAP_UI(MF_UIS ${UIS})

add_library(MergeFiles SHARED ${SRCS} ${MF_MOCS} ${MF_UIS})
qt5_use_modules(MergeFiles Core Gui Widgets)
target_link_libraries(MergeFiles AwCoreLib AwProcessLib AwReadWriteLib AwFilteringLib Qt5::Core Qt5::Gui Qt5::Widgets)







