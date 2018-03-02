# FitleringLib
project(AwFiltering)
add_definitions("-DAW_BUILD_FILTERING_LIB")
INCLUDE_DIRECTORIES(./DspFilters)

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
    DspFilters/State.cpp)

SET(HEADERS  ../../include/AwFiltering.h)

QT5_WRAP_CPP(MOC ${HEADERS})

add_library(AwFiltering SHARED ${SRC} ${MOC} )
qt5_use_modules(AwFiltering Core Concurrent)
TARGET_LINK_LIBRARIES(AwFiltering AwCore Qt5::Core Qt5::Concurrent)




