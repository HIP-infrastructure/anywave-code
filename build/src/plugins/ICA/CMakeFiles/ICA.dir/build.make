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
include src/plugins/ICA/CMakeFiles/ICA.dir/depend.make

# Include the progress variables for this target.
include src/plugins/ICA/CMakeFiles/ICA.dir/progress.make

# Include the compile flags for this target's objects.
include src/plugins/ICA/CMakeFiles/ICA.dir/flags.make

src/plugins/ICA/moc_ICASettings.cpp: ../src/plugins/ICA/ICASettings.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating moc_ICASettings.cpp"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/lib/x86_64-linux-gnu/qt5/bin/moc @/home/bruno/dev/aw-gitlab/build/src/plugins/ICA/moc_ICASettings.cpp_parameters

src/plugins/ICA/moc_ica.cpp: ../src/plugins/ICA/ica.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating moc_ica.cpp"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/lib/x86_64-linux-gnu/qt5/bin/moc @/home/bruno/dev/aw-gitlab/build/src/plugins/ICA/moc_ica.cpp_parameters

src/plugins/ICA/ui_ICASettings.h: ../src/plugins/ICA/ICASettings.ui
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating ui_ICASettings.h"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/lib/x86_64-linux-gnu/qt5/bin/uic -o /home/bruno/dev/aw-gitlab/build/src/plugins/ICA/ui_ICASettings.h /home/bruno/dev/aw-gitlab/src/plugins/ICA/ICASettings.ui

src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o: src/plugins/ICA/CMakeFiles/ICA.dir/flags.make
src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o: ../src/plugins/ICA/ICASettings.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ICA.dir/ICASettings.cpp.o -c /home/bruno/dev/aw-gitlab/src/plugins/ICA/ICASettings.cpp

src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ICA.dir/ICASettings.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/src/plugins/ICA/ICASettings.cpp > CMakeFiles/ICA.dir/ICASettings.cpp.i

src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ICA.dir/ICASettings.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/src/plugins/ICA/ICASettings.cpp -o CMakeFiles/ICA.dir/ICASettings.cpp.s

src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o.requires:

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o.requires

src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o.provides: src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o.requires
	$(MAKE) -f src/plugins/ICA/CMakeFiles/ICA.dir/build.make src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o.provides.build
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o.provides

src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o.provides.build: src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o


src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o: src/plugins/ICA/CMakeFiles/ICA.dir/flags.make
src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o: ../src/plugins/ICA/infomax_algo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ICA.dir/infomax_algo.cpp.o -c /home/bruno/dev/aw-gitlab/src/plugins/ICA/infomax_algo.cpp

src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ICA.dir/infomax_algo.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/src/plugins/ICA/infomax_algo.cpp > CMakeFiles/ICA.dir/infomax_algo.cpp.i

src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ICA.dir/infomax_algo.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/src/plugins/ICA/infomax_algo.cpp -o CMakeFiles/ICA.dir/infomax_algo.cpp.s

src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o.requires:

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o.requires

src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o.provides: src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o.requires
	$(MAKE) -f src/plugins/ICA/CMakeFiles/ICA.dir/build.make src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o.provides.build
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o.provides

src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o.provides.build: src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o


src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o: src/plugins/ICA/CMakeFiles/ICA.dir/flags.make
src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o: ../src/plugins/ICA/ica.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ICA.dir/ica.cpp.o -c /home/bruno/dev/aw-gitlab/src/plugins/ICA/ica.cpp

src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ICA.dir/ica.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/src/plugins/ICA/ica.cpp > CMakeFiles/ICA.dir/ica.cpp.i

src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ICA.dir/ica.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/src/plugins/ICA/ica.cpp -o CMakeFiles/ICA.dir/ica.cpp.s

src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o.requires:

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o.requires

src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o.provides: src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o.requires
	$(MAKE) -f src/plugins/ICA/CMakeFiles/ICA.dir/build.make src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o.provides.build
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o.provides

src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o.provides.build: src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o


src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o: src/plugins/ICA/CMakeFiles/ICA.dir/flags.make
src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o: ../src/plugins/ICA/r250.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ICA.dir/r250.c.o   -c /home/bruno/dev/aw-gitlab/src/plugins/ICA/r250.c

src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ICA.dir/r250.c.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bruno/dev/aw-gitlab/src/plugins/ICA/r250.c > CMakeFiles/ICA.dir/r250.c.i

src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ICA.dir/r250.c.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bruno/dev/aw-gitlab/src/plugins/ICA/r250.c -o CMakeFiles/ICA.dir/r250.c.s

src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o.requires:

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o.requires

src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o.provides: src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o.requires
	$(MAKE) -f src/plugins/ICA/CMakeFiles/ICA.dir/build.make src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o.provides.build
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o.provides

src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o.provides.build: src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o


src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o: src/plugins/ICA/CMakeFiles/ICA.dir/flags.make
src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o: ../src/plugins/ICA/randlcg.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ICA.dir/randlcg.c.o   -c /home/bruno/dev/aw-gitlab/src/plugins/ICA/randlcg.c

src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ICA.dir/randlcg.c.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bruno/dev/aw-gitlab/src/plugins/ICA/randlcg.c > CMakeFiles/ICA.dir/randlcg.c.i

src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ICA.dir/randlcg.c.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bruno/dev/aw-gitlab/src/plugins/ICA/randlcg.c -o CMakeFiles/ICA.dir/randlcg.c.s

src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o.requires:

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o.requires

src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o.provides: src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o.requires
	$(MAKE) -f src/plugins/ICA/CMakeFiles/ICA.dir/build.make src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o.provides.build
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o.provides

src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o.provides.build: src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o


src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o: src/plugins/ICA/CMakeFiles/ICA.dir/flags.make
src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o: src/plugins/ICA/moc_ICASettings.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ICA.dir/moc_ICASettings.cpp.o -c /home/bruno/dev/aw-gitlab/build/src/plugins/ICA/moc_ICASettings.cpp

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ICA.dir/moc_ICASettings.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/build/src/plugins/ICA/moc_ICASettings.cpp > CMakeFiles/ICA.dir/moc_ICASettings.cpp.i

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ICA.dir/moc_ICASettings.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/build/src/plugins/ICA/moc_ICASettings.cpp -o CMakeFiles/ICA.dir/moc_ICASettings.cpp.s

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o.requires:

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o.requires

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o.provides: src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o.requires
	$(MAKE) -f src/plugins/ICA/CMakeFiles/ICA.dir/build.make src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o.provides.build
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o.provides

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o.provides.build: src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o


src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o: src/plugins/ICA/CMakeFiles/ICA.dir/flags.make
src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o: src/plugins/ICA/moc_ica.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ICA.dir/moc_ica.cpp.o -c /home/bruno/dev/aw-gitlab/build/src/plugins/ICA/moc_ica.cpp

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ICA.dir/moc_ica.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/build/src/plugins/ICA/moc_ica.cpp > CMakeFiles/ICA.dir/moc_ica.cpp.i

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ICA.dir/moc_ica.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/build/src/plugins/ICA/moc_ica.cpp -o CMakeFiles/ICA.dir/moc_ica.cpp.s

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o.requires:

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o.requires

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o.provides: src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o.requires
	$(MAKE) -f src/plugins/ICA/CMakeFiles/ICA.dir/build.make src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o.provides.build
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o.provides

src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o.provides.build: src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o


# Object files for target ICA
ICA_OBJECTS = \
"CMakeFiles/ICA.dir/ICASettings.cpp.o" \
"CMakeFiles/ICA.dir/infomax_algo.cpp.o" \
"CMakeFiles/ICA.dir/ica.cpp.o" \
"CMakeFiles/ICA.dir/r250.c.o" \
"CMakeFiles/ICA.dir/randlcg.c.o" \
"CMakeFiles/ICA.dir/moc_ICASettings.cpp.o" \
"CMakeFiles/ICA.dir/moc_ica.cpp.o"

# External object files for target ICA
ICA_EXTERNAL_OBJECTS =

bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o
bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o
bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o
bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o
bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o
bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o
bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o
bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/build.make
bin/Plugins/libICA.so: lib/libAwFilteringLib.so
bin/Plugins/libICA.so: lib/libAwProcessLib.so
bin/Plugins/libICA.so: lib/libAwHDF5Lib.so
bin/Plugins/libICA.so: lib/libAwMATLABLib.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libhdf5_cpp.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libpthread.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libsz.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libz.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libdl.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libm.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libhdf5_hl_cpp.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5_hl.so
bin/Plugins/libICA.so: /usr/lib/libopenblas.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libQt5Concurrent.so.5.7.1
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.7.1
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.7.1
bin/Plugins/libICA.so: lib/libAwReadWriteLib.so
bin/Plugins/libICA.so: lib/libAwCoreLib.so
bin/Plugins/libICA.so: lib/libAwMathLib.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.7.1
bin/Plugins/libICA.so: /usr/lib/libopenblas.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libfftw3.so
bin/Plugins/libICA.so: /usr/lib/x86_64-linux-gnu/libmatio.so
bin/Plugins/libICA.so: src/plugins/ICA/CMakeFiles/ICA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX shared library ../../../bin/Plugins/libICA.so"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ICA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/plugins/ICA/CMakeFiles/ICA.dir/build: bin/Plugins/libICA.so

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/build

src/plugins/ICA/CMakeFiles/ICA.dir/requires: src/plugins/ICA/CMakeFiles/ICA.dir/ICASettings.cpp.o.requires
src/plugins/ICA/CMakeFiles/ICA.dir/requires: src/plugins/ICA/CMakeFiles/ICA.dir/infomax_algo.cpp.o.requires
src/plugins/ICA/CMakeFiles/ICA.dir/requires: src/plugins/ICA/CMakeFiles/ICA.dir/ica.cpp.o.requires
src/plugins/ICA/CMakeFiles/ICA.dir/requires: src/plugins/ICA/CMakeFiles/ICA.dir/r250.c.o.requires
src/plugins/ICA/CMakeFiles/ICA.dir/requires: src/plugins/ICA/CMakeFiles/ICA.dir/randlcg.c.o.requires
src/plugins/ICA/CMakeFiles/ICA.dir/requires: src/plugins/ICA/CMakeFiles/ICA.dir/moc_ICASettings.cpp.o.requires
src/plugins/ICA/CMakeFiles/ICA.dir/requires: src/plugins/ICA/CMakeFiles/ICA.dir/moc_ica.cpp.o.requires

.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/requires

src/plugins/ICA/CMakeFiles/ICA.dir/clean:
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/ICA && $(CMAKE_COMMAND) -P CMakeFiles/ICA.dir/cmake_clean.cmake
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/clean

src/plugins/ICA/CMakeFiles/ICA.dir/depend: src/plugins/ICA/moc_ICASettings.cpp
src/plugins/ICA/CMakeFiles/ICA.dir/depend: src/plugins/ICA/moc_ica.cpp
src/plugins/ICA/CMakeFiles/ICA.dir/depend: src/plugins/ICA/ui_ICASettings.h
	cd /home/bruno/dev/aw-gitlab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bruno/dev/aw-gitlab /home/bruno/dev/aw-gitlab/src/plugins/ICA /home/bruno/dev/aw-gitlab/build /home/bruno/dev/aw-gitlab/build/src/plugins/ICA /home/bruno/dev/aw-gitlab/build/src/plugins/ICA/CMakeFiles/ICA.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/plugins/ICA/CMakeFiles/ICA.dir/depend

