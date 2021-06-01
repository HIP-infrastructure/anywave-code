import requests
import network.tcprequest as tcp
import json

def get_props():
     aw = tcp.TCPRequest(requests.GET_PROP_REQUEST)
     aw.simpleRequest()
     aw.waitForResponse()
     response = aw.response()
     str = response.readQString()
     return json.loads(str)
