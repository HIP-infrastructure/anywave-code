# AwMathLib
project(AwMath)
add_definitions("-DAW_BUILD_MATH_LIB")

add_library(AwMath SHARED AwMath.cpp AwSpline2D.cpp)
QT5_USE_MODULES(AwMath Core)

IF (APPLE)
   TARGET_LINK_LIBRARIES(AwMath AwCore Qt5::Core ${ARMA_LIB} "-framework Accelerate")
   INSTALL(TARGETS AwMath DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
   TARGET_LINK_LIBRARIES(AwMath AwCore Qt5::Core ${BLAS_LIBRARIES} gomp)
   INSTALL(TARGETS AwMath DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()

