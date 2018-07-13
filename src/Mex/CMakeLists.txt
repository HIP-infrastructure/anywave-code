
include_directories(${Matlab_INCLUDE_DIRS})
# common mex lib
add_library(mexcommon common.cpp)
qt5_use_modules(mexcommon Core Network)
TARGET_LINK_LIBRARIES(mexcommon Qt5::Core Qt5::Network)



matlab_add_mex(NAME aw_getdata SRC aw_getdata.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_getdata LINK_TO mexcommon AwCore)
matlab_add_mex(NAME aw_getmarkers SRC aw_getmarkers.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_getmarkers LINK_TO mexcommon AwCore)
matlab_add_mex(NAME aw_addmarkers SRC aw_addmarkers.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_addmarkers LINK_TO mexcommon AwCore)
matlab_add_mex(NAME aw_getplugininfo SRC aw_getplugininfo.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_getplugininfo LINK_TO mexcommon AwCore)
matlab_add_mex(NAME aw_getfileinfo SRC aw_getfileinfo.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_getfileinfo LINK_TO mexcommon AwCore)
matlab_add_mex(NAME aw_sendcommand SRC aw_sendcommand.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_sendcommand LINK_TO mexcommon AwCore)
matlab_add_mex(NAME aw_gettriggers SRC aw_gettriggers.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_gettriggers LINK_TO mexcommon AwCore)
matlab_add_mex(NAME aw_getcapture SRC aw_getcapture.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_getcapture LINK_TO mexcommon AwCore)
matlab_add_mex(NAME aw_captureicapanel SRC aw_captureicapanel.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/aw_captureicapanel LINK_TO mexcommon AwCore)
matlab_add_mex(NAME AwSendMessage SRC AwSendMessage.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/AwSendMessage LINK_TO mexcommon AwCore)
matlab_add_mex(NAME AwIsProcessTerminated SRC AwIsProcessTerminated.cpp OUTPUT_NAME ../Plugins/MATLAB/AnyWave/AwIsProcessTerminated LINK_TO mexcommon AwCore)
