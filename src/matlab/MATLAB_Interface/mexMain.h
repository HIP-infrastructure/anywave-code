#pragma once

#include "mex.hpp"
#include "mexAdapter.hpp"
#include <QString>



class MexFunction : public matlab::mex::Function
{
public:
    MexFunction();
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs);
protected:
    // functions
    void init(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    //
    void debug_connect(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void get_data(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void get_markers(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void send_markers(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void send_message(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void get_properties(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    void run(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs);
    // 
    void error(const std::string&);
    void printf(std::ostringstream stream);
    void printf(const std::string& message);

    void getPidPort();

   std::shared_ptr<matlab::engine::MATLABEngine> m_matlabPtr;
};