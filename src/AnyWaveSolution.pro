#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T17:00:19
#
#-------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS +=  core filtering  utilities rw process math graphics matlab HDF5 layout mapping epoch widget MatlabSupport AnyWave \
  plugins Mex PythonSupport

core.subdir = Core
rw.subdir = ReadWrite
rw.depends = core
process.subdir = Process
process.depends = core
math.subdir = Math
math.depends = core utilities
filtering.subdir = Filtering
filtering.depends = core math
graphics.subdir = Graphics
graphics.depends = core
utilities.subdir = Utilities
utilities.depends = core
matlab.subdir = MatlabLib
HDF5.subdir = HDF5
layout.subdir = Layouts
layout.depends = core
mapping.subdir = MappingLib
mapping.depends = core
epoch.subdir = AwEpochLib
epoch.depends = core
widget.subdir =  Widgets
widget.depends = core graphics layout mapping math utilities

AnyWave.depends = core process rw math utilities filtering graphics matlab HDF5 layout mapping epoch widget
