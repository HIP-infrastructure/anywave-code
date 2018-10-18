# AwMathLib
project(AwMath)
add_definitions("-DAW_BUILD_MATH_LIB")
add_library(AwMath SHARED AwMath.cpp AwSpline2D.cpp)
QT5_USE_MODULES(AwMath Core)
IF(USE_MKL)
    TARGET_LINK_LIBRARIES(AwMath AwCore Qt5::Core gomp ${MKL_LIBRARY})
ELSE()
    TARGET_LINK_LIBRARIES(AwMath AwCore Qt5::Core gomp ${OpenBLAS_LIB})
ENDIF()
INSTALL(TARGETS AwMath DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)

