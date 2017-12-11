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
include src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/depend.make

# Include the progress variables for this target.
include src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/progress.make

# Include the compile flags for this target's objects.
include src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/flags.make

src/plugins/AutoMarking/moc_automarking.cpp: ../src/plugins/AutoMarking/automarking.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating moc_automarking.cpp"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/lib/x86_64-linux-gnu/qt5/bin/moc @/home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/moc_automarking.cpp_parameters

src/plugins/AutoMarking/qrc_automarking.cpp: ../src/plugins/AutoMarking/images/marker_pre_post.png
src/plugins/AutoMarking/qrc_automarking.cpp: ../src/plugins/AutoMarking/images/marker_around.png
src/plugins/AutoMarking/qrc_automarking.cpp: ../src/plugins/AutoMarking/automarking.qrc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating qrc_automarking.cpp"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/lib/x86_64-linux-gnu/qt5/bin/rcc --name automarking --output /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/qrc_automarking.cpp /home/bruno/dev/aw-gitlab/src/plugins/AutoMarking/automarking.qrc

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/flags.make
src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o: ../src/plugins/AutoMarking/automarking.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AutoMarking.dir/automarking.cpp.o -c /home/bruno/dev/aw-gitlab/src/plugins/AutoMarking/automarking.cpp

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AutoMarking.dir/automarking.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/src/plugins/AutoMarking/automarking.cpp > CMakeFiles/AutoMarking.dir/automarking.cpp.i

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AutoMarking.dir/automarking.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/src/plugins/AutoMarking/automarking.cpp -o CMakeFiles/AutoMarking.dir/automarking.cpp.s

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o.requires:

.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o.requires

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o.provides: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o.requires
	$(MAKE) -f src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/build.make src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o.provides.build
.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o.provides

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o.provides.build: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o


src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/flags.make
src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o: src/plugins/AutoMarking/moc_automarking.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o -c /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/moc_automarking.cpp

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AutoMarking.dir/moc_automarking.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/moc_automarking.cpp > CMakeFiles/AutoMarking.dir/moc_automarking.cpp.i

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AutoMarking.dir/moc_automarking.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/moc_automarking.cpp -o CMakeFiles/AutoMarking.dir/moc_automarking.cpp.s

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o.requires:

.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o.requires

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o.provides: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o.requires
	$(MAKE) -f src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/build.make src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o.provides.build
.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o.provides

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o.provides.build: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o


src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/flags.make
src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o: src/plugins/AutoMarking/qrc_automarking.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o -c /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/qrc_automarking.cpp

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.i"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/qrc_automarking.cpp > CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.i

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.s"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/qrc_automarking.cpp -o CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.s

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o.requires:

.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o.requires

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o.provides: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o.requires
	$(MAKE) -f src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/build.make src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o.provides.build
.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o.provides

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o.provides.build: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o


# Object files for target AutoMarking
AutoMarking_OBJECTS = \
"CMakeFiles/AutoMarking.dir/automarking.cpp.o" \
"CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o" \
"CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o"

# External object files for target AutoMarking
AutoMarking_EXTERNAL_OBJECTS =

bin/Plugins/libAutoMarking.so: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o
bin/Plugins/libAutoMarking.so: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o
bin/Plugins/libAutoMarking.so: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o
bin/Plugins/libAutoMarking.so: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/build.make
bin/Plugins/libAutoMarking.so: lib/libAwProcessLib.so
bin/Plugins/libAutoMarking.so: lib/libAwFilteringLib.so
bin/Plugins/libAutoMarking.so: lib/libAwWidgetLib.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libQt5Concurrent.so.5.7.1
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libQt5PrintSupport.so.5.7.1
bin/Plugins/libAutoMarking.so: lib/libAwGraphicsLib.so
bin/Plugins/libAutoMarking.so: lib/libAwUtilitiesLib.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libmatio.so
bin/Plugins/libAutoMarking.so: lib/libAwMathLib.so
bin/Plugins/libAutoMarking.so: /usr/lib/libopenblas.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libfftw3.so
bin/Plugins/libAutoMarking.so: lib/libAwMappingLib.so
bin/Plugins/libAutoMarking.so: lib/libAwLayoutLib.so
bin/Plugins/libAutoMarking.so: lib/libAwReadWriteLib.so
bin/Plugins/libAutoMarking.so: lib/libAwCoreLib.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkDomainsChemistry-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libexpat.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersGeneric-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersHyperTree-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersParallelFlowPaths-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersFlowPaths-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersParallelGeometry-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersParallelImaging-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersParallelMPI-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersParallelStatistics-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersProgrammable-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersPython-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/libvtkWrappingTools-6.3.a
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersReebGraph-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersSMP-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersSelection-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersVerdict-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkverdict-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libjpeg.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libpng.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libtiff.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkGUISupportQtOpenGL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkGUISupportQtSQL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkGUISupportQtWebkit-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkViewsQt-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOAMR-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOEnSight-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOExport-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingGL2PS-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingContextOpenGL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libgl2ps.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOFFMPEG-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOMovie-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libtheoraenc.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libtheoradec.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libogg.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOGDAL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOGeoJSON-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOImport-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOInfovis-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOMINC-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOMPIImage-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOMPIParallel-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOParallel-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIONetCDF-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libjsoncpp.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOMySQL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOODBC-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOPLY-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOParallelExodus-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOExodus-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkexoIIc-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libnetcdf_c++.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libnetcdf.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOParallelLSDyna-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOLSDyna-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOParallelNetCDF-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOParallelXML-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOPostgreSQL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOVPIC-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkVPIC-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOVideo-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOXdmf2-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkxdmf2-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/hdf5/openmpi/libhdf5.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libsz.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libdl.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libm.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libxml2.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingMath-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingMorphological-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingStatistics-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingStencil-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkInfovisBoostGraphAlgorithms-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkInteractionImage-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkLocalExample-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkParallelMPI4Py-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingExternal-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingFreeTypeFontConfig-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingImage-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingLOD-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingMatplotlib-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkWrappingPython27Core-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkPythonInterpreter-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libpython2.7.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingParallel-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersParallel-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingParallelLIC-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkParallelMPI-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingLIC-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingQt-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersTexture-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkGUISupportQt-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.7.1
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.7.1
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.7.1
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingVolumeAMR-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersAMR-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkParallelCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOLegacy-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingVolumeOpenGL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingOpenGL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libSM.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libICE.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libX11.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libXext.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libXt.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkTestingGenericBridge-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkTestingIOSQL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOSQL-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkTestingRendering-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkViewsContext2D-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkViewsGeovis-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkViewsInfovis-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkChartsCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingContext2D-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersImaging-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingLabel-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkGeovisCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOXML-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOGeometry-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOXMLParser-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkInfovisLayout-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkInfovisCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkViewsCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkInteractionWidgets-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersHybrid-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingGeneral-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingSources-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersModeling-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkInteractionStyle-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingHybrid-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOImage-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkDICOMParser-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkIOCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkmetaio-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libz.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingAnnotation-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingFreeType-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkftgl-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libfreetype.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libGL.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingColor-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingVolume-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkRenderingCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonColor-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersExtraction-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersStatistics-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingFourier-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkImagingCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkalglib-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersGeometry-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersSources-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersGeneral-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkFiltersCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonExecutionModel-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonComputationalGeometry-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonDataModel-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonMisc-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonTransforms-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonMath-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonSystem-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtksys-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libproj.so
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkWrappingJava-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/x86_64-linux-gnu/libvtkCommonCore-6.3.so.6.3.0
bin/Plugins/libAutoMarking.so: /usr/lib/libqwt-qt5.so
bin/Plugins/libAutoMarking.so: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bruno/dev/aw-gitlab/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library ../../../bin/Plugins/libAutoMarking.so"
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AutoMarking.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/build: bin/Plugins/libAutoMarking.so

.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/build

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/requires: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/automarking.cpp.o.requires
src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/requires: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/moc_automarking.cpp.o.requires
src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/requires: src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/qrc_automarking.cpp.o.requires

.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/requires

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/clean:
	cd /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking && $(CMAKE_COMMAND) -P CMakeFiles/AutoMarking.dir/cmake_clean.cmake
.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/clean

src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/depend: src/plugins/AutoMarking/moc_automarking.cpp
src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/depend: src/plugins/AutoMarking/qrc_automarking.cpp
	cd /home/bruno/dev/aw-gitlab/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bruno/dev/aw-gitlab /home/bruno/dev/aw-gitlab/src/plugins/AutoMarking /home/bruno/dev/aw-gitlab/build /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking /home/bruno/dev/aw-gitlab/build/src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/plugins/AutoMarking/CMakeFiles/AutoMarking.dir/depend

