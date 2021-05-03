#pragma once

#include "mex.hpp"
#include "mexAdapter.hpp"



class MexFunction : public matlab::mex::Function {
public:
    MexFunction();
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs); 
    // functions
    void debug_connect(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void get_data(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
};