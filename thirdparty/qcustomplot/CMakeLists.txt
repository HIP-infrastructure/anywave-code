cmake_minimum_required(VERSION 3.16)
project(qcustomplot LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_definitions("-DQCUSTOMPLOT_COMPILE_LIBRARY")
QT5_WRAP_CPP(MOC qcustomplot.h)
add_library(qcustomplot SHARED qcustomplot.cpp ${MOC}) 
qt5_use_modules(qcustomplot Core Widgets)

target_link_libraries(qcustomplot Qt5::Core Qt5::Widgets)
IF (APPLE)
INSTALL(TARGETS qcustomplot DESTINATION  ${LIB_INSTALL_DIR})
ELSE()
INSTALL(TARGETS qcustomplot DESTINATION  ${CMAKE_INSTALL_PREFIX}/lib)
ENDIF()
