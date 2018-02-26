function aw_sendcommand( cfg )
%aw_sendcommand Send command to AnyWave.
%   aw_sendcommand(cfg);
%   
%   cfg is a structure that must contain all the parameters relative to the
%   command.
%
%   cfg.command;
%       a string containing the command to send.
%       The current recognized commands are:
%       - 'center position'
%           Centers the views of AnyWave on a particular position (in
%           seconds).
%           This command requires the field cfg.position to be set to the
%           desired position.
%       - 'update markers'
%           Tells AnyWave to refresh the display of markers in all the
%           views.
%           This command should be user after a plugin added several
%           markers to manually refresh the display.
%
end

