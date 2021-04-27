import requests
import network.tcprequest as tcp
import anywave
import json

def debug_connect(port=None):
   if not port is None:
        anywave.port = port
   aw = tcp.TCPRequest(requests.CONNECT_DEBUG_REQUEST)
   aw.simpleRequest()
   aw.waitForResponse()
   # we must have a response containing the new pid allocated by anywave
   response = aw.response()
   pid = response.readInt32()
   anywave.pid = pid
   str = response.readQString()
   anywave.properties = json.loads(str)