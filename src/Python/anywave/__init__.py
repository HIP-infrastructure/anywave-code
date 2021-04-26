import sys
#import numpy
#unpack variables passed as arguments
#_, plugin_dir, pid, server_port, data_path, script_path = sys.argv

#sys.path.append(plugin_dir)
# this is a pointer to the module object instance itself.
host = '127.0.0.1'
pid = -1
port = 59000

from .getdata import get_data
from .sendmessage import send_message
from .core.channel import Channel
from .core.marker import Marker