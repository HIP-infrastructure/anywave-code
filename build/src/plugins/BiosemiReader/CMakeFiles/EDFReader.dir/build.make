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
include src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/depend.make

# Include the progress variables for this target.
include src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/progress.make

# Include the compile flags for this target's objects.
include src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/flags.make

src/plugins/BiosemiReader/moc_biosemireader.cpp: ../src/plugins/BiosemiReader/biosemireader.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating moc_biosemireader.cpp"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/lib/x86_64-linux-gnu/qt5/bin/moc @/home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader/moc_biosemireader.cpp_parameters

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/flags.make
src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o: ../src/plugins/BiosemiReader/biosemireader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EDFReader.dir/biosemireader.cpp.o -c /home/bruno/dev/aw-gitlab/src/plugins/BiosemiReader/biosemireader.cpp

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EDFReader.dir/biosemireader.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/src/plugins/BiosemiReader/biosemireader.cpp > CMakeFiles/EDFReader.dir/biosemireader.cpp.i

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EDFReader.dir/biosemireader.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/src/plugins/BiosemiReader/biosemireader.cpp -o CMakeFiles/EDFReader.dir/biosemireader.cpp.s

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o.requires:

.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o.requires

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o.provides: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o.requires
	$(MAKE) -f src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/build.make src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o.provides.build
.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o.provides

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o.provides.build: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o


src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/flags.make
src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o: ../src/plugins/BiosemiReader/edflib.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/EDFReader.dir/edflib.c.o   -c /home/bruno/dev/aw-gitlab/src/plugins/BiosemiReader/edflib.c

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/EDFReader.dir/edflib.c.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bruno/dev/aw-gitlab/src/plugins/BiosemiReader/edflib.c > CMakeFiles/EDFReader.dir/edflib.c.i

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/EDFReader.dir/edflib.c.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bruno/dev/aw-gitlab/src/plugins/BiosemiReader/edflib.c -o CMakeFiles/EDFReader.dir/edflib.c.s

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o.requires:

.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o.requires

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o.provides: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o.requires
	$(MAKE) -f src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/build.make src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o.provides.build
.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o.provides

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o.provides.build: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o


src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/flags.make
src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o: src/plugins/BiosemiReader/moc_biosemireader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o -c /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader/moc_biosemireader.cpp

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader/moc_biosemireader.cpp > CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.i

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader/moc_biosemireader.cpp -o CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.s

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o.requires:

.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o.requires

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o.provides: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o.requires
	$(MAKE) -f src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/build.make src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o.provides.build
.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o.provides

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o.provides.build: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o


# Object files for target EDFReader
EDFReader_OBJECTS = \
"CMakeFiles/EDFReader.dir/biosemireader.cpp.o" \
"CMakeFiles/EDFReader.dir/edflib.c.o" \
"CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o"

# External object files for target EDFReader
EDFReader_EXTERNAL_OBJECTS =

bin/Plugins/libEDFReader.so: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o
bin/Plugins/libEDFReader.so: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o
bin/Plugins/libEDFReader.so: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o
bin/Plugins/libEDFReader.so: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/build.make
bin/Plugins/libEDFReader.so: lib/libAwReadWriteLib.so
bin/Plugins/libEDFReader.so: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.7.1
bin/Plugins/libEDFReader.so: lib/libAwCoreLib.so
bin/Plugins/libEDFReader.so: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.7.1
bin/Plugins/libEDFReader.so: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.7.1
bin/Plugins/libEDFReader.so: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library ../../../bin/Plugins/libEDFReader.so"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EDFReader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/build: bin/Plugins/libEDFReader.so

.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/build

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/requires: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/biosemireader.cpp.o.requires
src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/requires: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/edflib.c.o.requires
src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/requires: src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/moc_biosemireader.cpp.o.requires

.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/requires

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/clean:
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader && $(CMAKE_COMMAND) -P CMakeFiles/EDFReader.dir/cmake_clean.cmake
.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/clean

src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/depend: src/plugins/BiosemiReader/moc_biosemireader.cpp
	cd /home/bruno/dev/aw-gitlab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bruno/dev/aw-gitlab /home/bruno/dev/aw-gitlab/src/plugins/BiosemiReader /home/bruno/dev/aw-gitlab/build /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader /home/bruno/dev/aw-gitlab/build/src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/plugins/BiosemiReader/CMakeFiles/EDFReader.dir/depend

