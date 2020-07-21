#!/bin/bash
# change rpath names in the libraries
#first arg must be the path to AnyWave bundle contents (path to AnyWave.app/Contents/Frameworks)
# Filtering
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwFilter.dylib
install_name_tool -change libAwMath.dylib @rpath/libAwMath.dylib $1/libAwFilter.dylib
# Epoch
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwEpoch.dylib
install_name_tool -change libAwMath.dylib @rpath/libAwMath.dylib $1/libAwEpoch.dylib
# Montage
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwMontage.dylib
# Graphics
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwGraphics.dylib
install_name_tool -change libAwUtilities.dylib @rpath/libAwUtilities.dylib $1/libAwGraphics.dylib
# Layout
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwLayout.dylib
# Mapping
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwMapping.dylib
install_name_tool -change libAwLayout.dylib @rpath/libAwLayout.dylib $1/libAwMapping.dylib
# Math
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwMath.dylib
# Process
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwProcess.dylib
# RW
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwRW.dylib
# Utilities
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwUtilities.dylib
# MAtlab
install_name_tool -change /Users/bruno/matio/lib/libmatio.4.dylib @rpath/libmatio.4.dylib $1/libAwMATLAB.dylib
# Widgets
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/libAwWidgets.dylib
install_name_tool -change qwt.framework/Versions/6/qwt @rpath/qwt.framework/Versions/6/qwt $1/libAwWidgets.dylib
install_name_tool -change libAwLayout.dylib @rpath/libAwLayout.dylib $1/libAwWidgets.dylib
install_name_tool -change libAwMapping.dylib @rpath/libAwMapping.dylib $1/libAwWidgets.dylib
install_name_tool -change libAwGraphics.dylib @rpath/libAwGraphics.dylib $1/libAwWidgets.dylib
install_name_tool -change libAwMath.dylib @rpath/libAwMath.dylib $1/libAwWidgets.dylib
install_name_tool -change libAwUtilities.dylib @rpath/libAwUtilities.dylib $1/libAwWidgets.dylib
# AnyWave app
install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $1/../MacOS/AnyWaveMac
install_name_tool -change qwt.framework/Versions/6/qwt @rpath/qwt.framework/Versions/6/qwt $1/../MacOS/AnyWaveMac
