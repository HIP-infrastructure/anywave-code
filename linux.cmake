
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   MESSAGE(STATUS "GCC compiler detected")
   SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -w -fPIC -O3 -D_REENTRANT -pipe -std=gnu++1z -DNDEBUG")
#   add_definitions("-w -fPIC -O3 -D_REENTRANT -pipe -std=gnu++1z -DNDEBUG") # g++ flags
ELSE()
    MESSAGE(STATUS "gcc compiler not found.")
    return()
ENDIF()

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lpthread")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")


MESSAGE(STATUS "Building linux amd64 using Qt5")
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Svg REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)
find_package(Qt5X11Extras REQUIRED)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

IF(USE_MKL) # Must be set if the system has oneapi mkl installed 
 # find_package(MKL_ REQUIRED)      
   # We assume intel mkl is installed as part of Intel ONEAPI and the setvars.sh script has been executed before
   SET(MKL_ARCH intel64)
   SET(MKL_LINK dynamic)
   SET(MKL_THREADING intel_thread)
   SET(MKL_INTERFACE lp64)
   find_package(MKL REQUIRED)
   
   MESSAGE(STATUS "MKL Include: ${MKL_INCLUDE}")
   MESSAGE(STATUS "MKL core: ${MKL_CORE}")
   MESSAGE(STATUS "MKL link: ${MKL_THREAD_LIB} ${MKL_SUPP_LINK}")
   MESSAGE(STATUS "MKL link: ${MKL_LIBRARIES}")

   #SET(BLAS_LIBRARIES "-L${MKL_ROOT}/lib/intel64 ${MKL_THREAD_LIB} mkl_intel_lp64 mkl_core mkl_intel_thread ")
   #MESSAGE(STATUS "${MKL_IMPORTED_TARGETS}") #Provides available list of targets based on input)
 #  IF(NOT DEFINED ENV{MKLROOT})
 #      MESSAGE(STATUS "MKLROOT variable not defined. please run vars.sh script to init mkl vars")
 #      RETURN()
 #  ENDIF()
 #   SET(MKLROOT $ENV{MKLROOT})
 #  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
   SET(USE_MKL TRUE CACHE BOOL "Using MKL libraries")
   #SET(BLAS_LIBRARIES "-L${MKL_ROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -lpthread -lm -ldl")
   SET(BLAS_LIBRARIES "-L${MKL_ROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lpthread -lgomp -lm -ldl")
#   SET(BLAS_LIBRARIES ${MKL_LIBRARIES} "-lpthread -lm -ldl")
#   SET(BLAS_LIBRARIES "${MKL_LINK_LINE} ${MKL_THREAD_LIB} ${MKL_SUPP_LINK}")
#    MESSAGE(STATUS "MKL libraires: ${BLAS_LIBRARIES}")
#    MESSAGE(STATUS "MKL Include dir: ${MKL_INCLUDE}")
 #   MESSAGE(STATUS "MKKL: ${MKLROOT}") # ${MKL_ARCH} ${MKL_INCLUDE} ${MKL_LINK} ${MKL_THREADING} ${MKL_INTERFACE_FULL}")
 #   INCLUDE_DIRECTORIES(${MKLROOT}/include)

    # Manual linking
    #SET(BLAS_LIBRARIES "-L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -lpthread -lm -ldl  ${MKL_THREAD_LIB}")

   # MESSAGE(STATUS "BLAS: ${BLAS_LIBRARIES}")
ELSE(USE_MKL)
    find_package(OpenBLAS REQUIRED)
    SET(BLAS_LIBRARIES ${OpenBLAS_LIB})
    SET(OPEN_BLAS_FOUND TRUE)
ENDIF()

SET(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/AnyWave)


SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN/lib'")

SET(MEX_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Plugins/MATLAB/AnyWave")
SET(PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Plugins")

# Install
INSTALL(FILES ${CMAKE_SOURCE_DIR}/resources/linux/start_aw.sh PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE DESTINATION ${CMAKE_INSTALL_PREFIX})

#License files
INSTALL(FILES ${AW_RES_DIR}/Licence_fr.html ${AW_RES_DIR}/License_en.html DESTINATION
	${CMAKE_INSTALL_PREFIX})
# desktop
INSTALL(FILES ${AW_RES_DIR}/linux/anywave.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
# icon
INSTALL(FILES ${AW_RES_DIR}/images/AnyWave_icon.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/apps)
# cmake
install(DIRECTORY ${CMAKE_SOURCE_DIR}/cmake DESTINATION ${CMAKE_INSTALL_PREFIX})
# include 
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})
# symlink
#INSTALL(EXECUTE_PROCESS(COMMAND ln -sf  ${CMAKE_INSTALL_PREFIX}/start_aw.sh /usr/bin/anywave))
#INSTALL(COMMAND ln -sf  ${CMAKE_INSTALL_PREFIX}/start_aw.sh /usr/bin/anywave)
INSTALL(CODE "execute_process(COMMAND ln -sf  ${CMAKE_INSTALL_PREFIX}/start_aw.sh /usr/bin/anywave)")
