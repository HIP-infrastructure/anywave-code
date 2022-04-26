# ICA Plugin
project(ICA)
set(LIBRARY_OUTPUT_PATH ${PLUGIN_OUT})

IF(APPLE)
  include(mac.cmake)
ELSE()
   include(linux.cmake)
ENDIF()









