function [ markers ] = aw_gettriggers(cfg)
%aw_gettriggers open a file and get events on trigger channels.
%   returns markers which match the settings defined in cfg structure
%	cfg may contain the following fields:
%
%	cfg.file = 'filename';
%		specify the file to open.
%		This field is mandatory.
%	cfg.channels = { 'Trigger', 'Response' };
%		specify the names of the channels to scan.
%		This field is optional (if not specified, all trigger channels will be scanned.)