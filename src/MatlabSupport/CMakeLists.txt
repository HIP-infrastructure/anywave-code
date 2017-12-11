# MATLAB Support Module
INCLUDE_DIRECTORIES(${MATLAB_INCLUDE_DIR})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
link_directories(${MATLAB_ROOT}/bin/glnxa64)

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

set(AwMatlabSupport_MOC_H AwMatlabSupport.h)
QT5_WRAP_CPP(AwMatlabSupport_MOC_SRCS ${AwMatlabSupport_MOC_H})

add_library(AwMatlabSupport SHARED AwMatlabSupport.cpp ${AwMatlabSupport_MOC_SRCS})
qt5_use_modules(AwMatlabSupport Core)
target_link_libraries(AwMatlabSupport ${MATLAB_MX_LIBRARY}
    ${MATLAB_MAT_LIBRARY}
    ${MATLAB_ENG_LIBRARY} Qt5::Core)

