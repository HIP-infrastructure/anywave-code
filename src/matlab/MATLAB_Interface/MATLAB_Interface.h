#pragma once

#include "matlab_interface_global.h"
#include <QString>

class  MATLAB_Interface
{
public:
    MATLAB_Interface();

    static QString host; 
    static int port;
    static int pid;
};


