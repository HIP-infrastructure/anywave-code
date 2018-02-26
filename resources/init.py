import sys

# unpack args into variables
_, anywave_mod_path, pid, data_path, script_path = sys.argv

# load the anywave module
sys.path.append(anywave_mod_path)
import anywave

# put variables into anywave scope
anywave.pid = pid
anywave.data_path = data_path
anywave.script_path = script_path

# build scope for script execution
script_globals = {
	'anywave' : anywave,
	'__file__' : script_path,
}

# run script
execfile(anywave.script_path, script_globals)