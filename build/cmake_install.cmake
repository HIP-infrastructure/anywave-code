# Install script for directory: /home/bruno/dev/aw-gitlab

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/bruno/dev/aw-gitlab/build/include/")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/bruno/dev/aw-gitlab/build/include" TYPE DIRECTORY FILES "/home/bruno/dev/aw-gitlab/include/")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/bruno/dev/aw-gitlab/build/src/Core/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/Filtering/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/Utilities/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/Graphics/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/Process/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/Widgets/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/ReadWrite/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/HDF5/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/Layouts/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/Math/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/MappingLib/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/MatlabLib/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/MatlabSupport/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/PythonSupport/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/AnyWave/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/4DNI_Reader/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/CTF_Reader/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/Micromed_Reader/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/FIFF_Reader/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/ICA/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/FileConverter/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/ANT_Reader/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/MergeFiles/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/BrainVisionIO/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/AppendFiles/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/SST/cmake_install.cmake")
  include("/home/bruno/dev/aw-gitlab/build/src/plugins/Correlation/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/bruno/dev/aw-gitlab/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
