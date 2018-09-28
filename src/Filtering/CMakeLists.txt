# FitleringLib
project(AwFiltering)
add_definitions("-DAW_BUILD_FILTERING_LIB")
INCLUDE_DIRECTORIES(./DspFilters)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

SET(SRC  AwButterWorth.cpp 
    AwFiltering.cpp 
    DspFilters/Bessel.cpp 
    DspFilters/Biquad.cpp 
    DspFilters/Butterworth.cpp 
    DspFilters/Cascade.cpp 
    DspFilters/ChebyshevI.cpp 
    DspFilters/ChebyshevII.cpp 
    DspFilters/Custom.cpp 
    DspFilters/Design.cpp 
    DspFilters/Documentation.cpp 
    DspFilters/Elliptic.cpp 
    DspFilters/Filter.cpp 
    DspFilters/Legendre.cpp 
    DspFilters/Param.cpp 
    DspFilters/PoleFilter.cpp 
    DspFilters/RBJ.cpp 
    DspFilters/RootFinder.cpp 
    DspFilters/State.cpp
    AwFilterSettings.cpp
    AwFilterGUI.cpp
    AwFilterModel.cpp
    AwFilterTableView.cpp
    AwFilterAddButton.cpp)


SET(HEADERS  ../../include/filter/AwFiltering.h ../../include/filter/AwFilterSettings.h ../../include/filter/AwFilterTableView.h
 AwFilterGUI.h AwFilterModel.h AwFilterAddButton.h)

SET(UI AwFilterGUI.ui)

QT5_WRAP_CPP(MOC ${HEADERS})
QT5_WRAP_UI(GUI ${UI})

add_library(AwFiltering SHARED ${SRC} ${MOC} ${GUI})
qt5_use_modules(AwFiltering Core Concurrent Widgets Gui)
TARGET_LINK_LIBRARIES(AwFiltering AwCore Qt5::Core Qt5::Concurrent Qt5::Widgets Qt5::Gui)




