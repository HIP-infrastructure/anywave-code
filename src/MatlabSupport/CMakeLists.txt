# MATLAB Support Module
include_directories(${Matlab_INCLUDE_DIRS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

set(AwMatlabSupport_MOC_H AwMatlabSupport.h)
QT5_WRAP_CPP(AwMatlabSupport_MOC_SRCS ${AwMatlabSupport_MOC_H})

add_library(AwMatlabSupport SHARED AwMatlabSupport.cpp ${AwMatlabSupport_MOC_SRCS})
qt5_use_modules(AwMatlabSupport Core)
target_link_libraries(AwMatlabSupport ${Matlab_MX_LIBRARY}  ${Matlab_ENG_LIBRARY} Qt5::Core)

IF (APPLE)
INSTALL(TARGETS AwMatlabSupport DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS AwMatlabSupport DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()

