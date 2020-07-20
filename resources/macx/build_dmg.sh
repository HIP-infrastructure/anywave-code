#!/bin/bash
export FRAMEWORK_DIR=/users/bruno/dev/anywave-mac/build/AnyWave.app/Contents/Frameworks
export APP_DIR=/users/bruno/dev/anywave-mac/build/AnyWave.app/Contents/MacOS
export APP_BUNDLE=/users/bruno/dev/anywave-mac/build/AnyWave.app
# cmake already build most of the lib with a correct rpath but some libs need to be modified 
# AwGraphics
#install_name_tool -change qwt.framework/Versions/6/qwt @rpath/qwt.framework/Versions/6/qwt $FRAMEWORK_DIR/libAwGraphics.dylib
# AwWidget
#install_name_tool -change qwt.framework/Versions/6/qwt @rpath/qwt.framework/Versions/6/qwt $FRAMEWORK_DIR/libAwWidget.dylib
# AwHDF5
#install_name_tool -change /users/bruno/dev/lib/hdf5/lib/libhdf5.10.dylib @rpath/libhdf5.10.dylib $FRAMEWORK_DIR/libAwHDF5.dylib
#install_name_tool -change /users/bruno/dev/lib/hdf5/lib/libhdf5_hl.10.dylib @rpath/libhdf5_hl.10.dylib $FRAMEWORK_DIR/libAwHDF5.dylib
# AwMATLAB
#install_name_tool -change /users/bruno/dev/lib/matio/lib/libmatio.4.dylib @rpath/libmatio.4.dylib $FRAMEWORK_DIR/libAwMATLAB.dylib

# AnyWave
#install_name_tool -change /users/bruno/dev/lib/hdf5/lib/libhdf5.10.dylib @rpath/libhdf5.10.dylib $APP_DIR/AnyWaveMac
#install_name_tool -change /users/bruno/dev/lib/hdf5/lib/libhdf5_hl.10.dylib @rpath/libhdf5_hl.10.dylib $APP_DIR/AnyWaveMac
#install_name_tool -change qwt.framework/Versions/6/qwt @rpath/qwt.framework/Versions/6/qwt $APP_DIR/AnyWaveMac
#install_name_tool -change /users/bruno/dev/lib/matio/lib/libmatio.4.dylib @rpath/libmatio.4.dylib $APP_DIR/AnyWaveMac

# create dmg
hdiutil create -volname AnyWave -srcfolder $APP_BUNDLE -ov -format UDZO AnyWave.dmg