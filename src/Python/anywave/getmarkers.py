import requests
import network.tcprequest as tcp
import core.marker as marker

def get_markers(args = None):
    aw = tcp.TCPRequest(requests.GET_MARKERS_EX_REQUEST)
    res = []
    if not args is None:
        str = json.dumps(args)
    aw.sendString(str)
    aw.waitForResponse()
    response = aw.response()
    nMarkers = response.readInt32()
