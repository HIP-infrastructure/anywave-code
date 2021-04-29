function [prop] = aw_debug_connect(port)
%AW_DEBUG_CONNECT Summary of this function goes here
%   Detailed explanation goes here
pyPort = py.int(port);
py.anywave.debug_connect(pyPort);
prop = struct(py.anywave.properties);
end

