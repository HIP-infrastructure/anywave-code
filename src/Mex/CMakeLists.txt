# MATLAB Mex Files for scripted plugins
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(QT_USE_QTNETWORK TRUE)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_DIR})
# define matlab macrod

set(MEX_SRC AwAddMarker.cpp
AwGetMarkers.cpp
AwGetData.cpp
AwGetInputChannels.cpp
AwSendMessage.cpp
AwIsProcessTerminated.cpp
AwGetWorkingDir.cpp
AwGetDataInfo.cpp)
 
if (MATLAB_FOUND)
   set(MATLAB_ROOT $ENV{MATLAB_ROOT})
   add_definitions(${QT_DEFINITIONS})
   include_directories(${CMAKE_CURRENT_DIR})
   include_directories(${CMAKE_SOURCE_DIR}/include)
   include_directories(${MATLAB_INCLUDE_DIR})
   link_directories(${MATLAB_LIBARIES})
   if(APPLE)
      set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/Anywave_Plugins/Matlab/AnyWave)
      if(CMAKE_SIZEOF_VOID_P MATCHES "8") # 64bit?
     	  set(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -O -DNDEBUG -DMATLAB_MEX_FILE")
          set(CMAKE_SHARED_LINKER_FLAGS "-pthread -shared -Wl,-rpath-link,${MATLAB_ROOT}/bin/maci64 -Wl,--version-script,${MATLAB_ROOT}/extern/lib/maci64/mexFunction.map -Wl,--no-undefined")
          foreach(mex_file ${MEX_SRC})
              get_filename_component(module "${mex_file}" NAME_WE)
              add_library(${module} MODULE ${mex_file})
              target_link_libraries(${module} ${MATLAB_LIBRARIES} ${QT_LIBRARIES} AwProcessLib AwCoreLib -lstdc++)
              set_target_properties(${module} PROPERTIES SUFFIX .mexa64 PREFIX "")
          endforeach()
      else(CMAKE_SIZEOF_VOID_P MATCHES "8")
        message(STATUS "32bit mex files are not supported on Mac OS X.")
        return()
      endif()    
   else(APPLE)
      set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/Plugins/Matlab/AnyWave)
      if(CMAKE_SIZEOF_VOID_P MATCHES "8") # 64bit?
     	  set(CMAKE_CXX_FLAGS "-ansi -D_GNU_SOURCE -fPIC -fno-omit-frame-pointer -pthread -O -DNDEBUG -DMATLAB_MEX_FILE")
          set(CMAKE_SHARED_LINKER_FLAGS "-pthread -shared -Wl,-rpath-link,${MATLAB_ROOT}/bin/glnxa64 -Wl,--version-script,${MATLAB_ROOT}/extern/lib/glnxa64/mexFunction.map -Wl,--no-undefined")
          foreach(mex_file ${MEX_SRC})
              get_filename_component(module "${mex_file}" NAME_WE)
              add_library(${module} MODULE ${mex_file})
              target_link_libraries(${module} ${MATLAB_LIBRARIES} ${QT_LIBRARIES} AwProcessLib AwCoreLib -lstdc++)
              set_target_properties(${module} PROPERTIES SUFFIX .mexa64 PREFIX "")
          endforeach()
      else(CMAKE_SIZEOF_VOID_P MATCHES "8") #32bit
     	  set(CMAKE_CXX_FLAGS "-fPIC -fno-omit-frame-pointer -pthread -O -DNDEBUG -std=c99 -DMATLAB_MEX_FILE")
          set(CMAKE_SHARED_LINKER_FLAGS "-pthread -shared -Wl,-rpath-link,${MATLAB_ROOT}/bin/glnx -Wl,--version-script,${MATLAB_ROOT}/extern/lib/glnx/mexFunction.map -Wl,--no-undefined")
          foreach(mex_file ${MEX_SRC})
              get_filename_component(module "${mex_file}" NAME_WE)
              add_library(${module} MODULE ${mex_file})
              target_link_libraries(${module} ${MATLAB_LIBRARIES} ${QT_LIBRARIES} AwProcessLib AwCoreLib -lstdc++)
              set_target_properties(${module} PROPERTIES SUFFIX .mexglx PREFIX "")
          endforeach()
      endif()    
   endif()
endif()


