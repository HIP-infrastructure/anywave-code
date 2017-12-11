# FileConverter
project(FileConverter)
add_definitions(-DQT_PLUGIN)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

SET(SRCS
fileconverter.cpp settings.cpp) 

SET(MOCS
fileconverter.h settings.h)

SET(UIS
settings.ui)

qt5_wrap_cpp(FC_MOCS ${MOCS})
QT5_WRAP_UI(FC_UIS ${UIS})

add_library(FileConverter SHARED ${SRCS} ${FC_MOCS} ${FC_UIS})
qt5_use_modules(FileConverter Core Gui Widgets)
target_link_libraries(FileConverter AwCoreLib AwProcessLib AwReadWriteLib AwFilteringLib Qt5::Core Qt5::Gui Qt5::Widgets)







