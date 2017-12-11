# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bruno/dev/aw-gitlab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bruno/dev/aw-gitlab/build

# Include any dependencies generated for this target.
include src/HDF5/CMakeFiles/AwHDF5Lib.dir/depend.make

# Include the progress variables for this target.
include src/HDF5/CMakeFiles/AwHDF5Lib.dir/progress.make

# Include the compile flags for this target's objects.
include src/HDF5/CMakeFiles/AwHDF5Lib.dir/flags.make

src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o: src/HDF5/CMakeFiles/AwHDF5Lib.dir/flags.make
src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o: ../src/HDF5/AwHDF5.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/HDF5 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o -c /home/bruno/dev/aw-gitlab/src/HDF5/AwHDF5.cpp

src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/HDF5 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/src/HDF5/AwHDF5.cpp > CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.i

src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/HDF5 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/src/HDF5/AwHDF5.cpp -o CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.s

src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o.requires:

.PHONY : src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o.requires

src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o.provides: src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o.requires
	$(MAKE) -f src/HDF5/CMakeFiles/AwHDF5Lib.dir/build.make src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o.provides.build
.PHONY : src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o.provides

src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o.provides.build: src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o


# Object files for target AwHDF5Lib
AwHDF5Lib_OBJECTS = \
"CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o"

# External object files for target AwHDF5Lib
AwHDF5Lib_EXTERNAL_OBJECTS =

lib/libAwHDF5Lib.so: src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o
lib/libAwHDF5Lib.so: src/HDF5/CMakeFiles/AwHDF5Lib.dir/build.make
lib/libAwHDF5Lib.so: lib/libAwCoreLib.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.7.1
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/libhdf5_cpp.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/libpthread.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/libsz.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/libz.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/libdl.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/libm.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/libhdf5_hl_cpp.so
lib/libAwHDF5Lib.so: /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5_hl.so
lib/libAwHDF5Lib.so: src/HDF5/CMakeFiles/AwHDF5Lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../lib/libAwHDF5Lib.so"
	cd /home/bruno/dev/aw-gitlab/build/src/HDF5 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AwHDF5Lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/HDF5/CMakeFiles/AwHDF5Lib.dir/build: lib/libAwHDF5Lib.so

.PHONY : src/HDF5/CMakeFiles/AwHDF5Lib.dir/build

src/HDF5/CMakeFiles/AwHDF5Lib.dir/requires: src/HDF5/CMakeFiles/AwHDF5Lib.dir/AwHDF5.cpp.o.requires

.PHONY : src/HDF5/CMakeFiles/AwHDF5Lib.dir/requires

src/HDF5/CMakeFiles/AwHDF5Lib.dir/clean:
	cd /home/bruno/dev/aw-gitlab/build/src/HDF5 && $(CMAKE_COMMAND) -P CMakeFiles/AwHDF5Lib.dir/cmake_clean.cmake
.PHONY : src/HDF5/CMakeFiles/AwHDF5Lib.dir/clean

src/HDF5/CMakeFiles/AwHDF5Lib.dir/depend:
	cd /home/bruno/dev/aw-gitlab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bruno/dev/aw-gitlab /home/bruno/dev/aw-gitlab/src/HDF5 /home/bruno/dev/aw-gitlab/build /home/bruno/dev/aw-gitlab/build/src/HDF5 /home/bruno/dev/aw-gitlab/build/src/HDF5/CMakeFiles/AwHDF5Lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/HDF5/CMakeFiles/AwHDF5Lib.dir/depend

