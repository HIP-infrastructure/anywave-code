# - Find Intel MKL
# Find the MKL libraries
#
# Options:
#
#   MKL_STATAIC       :   use static linking
#   MKL_MULTI_THREADED:   use multi-threading
#   MKL_SDL           :   Single Dynamic Library interface
#
# This module defines the following variables:
#
#   MKL_FOUND            : True if MKL_INCLUDE_DIR are found
#   MKL_INCLUDE_DIR      : where to find mkl.h, etc.
#   MKL_INCLUDE_DIRS     : set when MKL_INCLUDE_DIR found
#   MKL_LIBRARIES        : the library to link against.


include(FindPackageHandleStandardArgs)
# Use environment variables from Intel build scripts, if available
if (NOT MKL_ROOT AND NOT $ENV{MKLROOT} STREQUAL "")
    set(MKL_ROOT $ENV{MKLROOT})
endif()

if (NOT INTEL_ROOT AND NOT $ENV{INTEL_ROOT} STREQUAL "")
   set(INTEL_ROOT $ENV{INTEL_ROOT})
endif()
if (NOT INTEL_ROOT)
   set(INTEL_ROOT /opt/intel)
endif()

if (NOT MKL_ROOT)
   message("MKL not found ! Define MKL_ROOT variable.")
endif()

# Find include dir
find_path(MKL_INCLUDE_DIR mkl.h
    PATHS ${MKL_ROOT}/include)

set(CMAKE_FIND_LIBRARY_SUFFIXES .so)


# MKL is composed by four layers: Interface, Threading, Computational and RTL

find_library(MKL_INTERFACE_LIBRARY mkl_intel_lp64
       PATHS ${MKL_ROOT}/lib/intel64/)

if (MKL_INTERFACE_LIBRARY)
   message("Found mkl Interface library ${MKL_INTERFACE_LIBRARY}")
endif()


find_library(MKL_THREADING_LIBRARY mkl_intel_thread PATHS ${MKL_ROOT}/lib/intel64/)

if (MKL_THREADING_LIBRARY)
   message("Found mkl threading library ${MKL_THREADING_LIBRARY}")
endif()


    ####################### Computational layer #####################
find_library(MKL_CORE_LIBRARY mkl_core
       PATHS ${MKL_ROOT}/lib/intel64/)

if (MKL_CORE_LIBRARY)
   message("Found mkl core library: ${MKL_CORE_LIBRARY}")
endif()

    ############################ RTL layer ##########################
find_library(MKL_RTL_LIBRARY iomp5 PATHS ${INTEL_ROOT}/lib/intel64/)

if (MKL_RTL_LIBRARY)
   message("Found mkl rtl library: ${MKL_RTL_LIBRARY}")
endif()

set(MKL_LIBRARY ${MKL_INTERFACE_LIBRARY} ${MKL_THREADING_LIBRARY} ${MKL_CORE_LIBRARY} ${MKL_FFT_LIBRARY} ${MKL_SCALAPACK_LIBRARY})

set(MKL_MINIMAL_LIBRARY ${MKL_INTERFACE_LIBRARY} ${MKL_THREADING_LIBRARY} ${MKL_CORE_LIBRARY})


find_package_handle_standard_args(MKL REQUIRED_VARS MKL_INCLUDE_DIR MKL_LIBRARY MKL_MINIMAL_LIBRARY)

if(MKL_FOUND)
    set(MKL_INCLUDE_DIRS ${MKL_INCLUDE_DIR})
    set(MKL_LIBRARIES ${MKL_LIBRARY})
    set(MKL_MINIMAL_LIBRARIES ${MKL_LIBRARY})
endif()
