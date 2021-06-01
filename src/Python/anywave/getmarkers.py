import requests
import network.tcprequest as tcp
import core.marker as marker
import json

def get_markers(args = None):
    aw = tcp.TCPRequest(requests.GET_MARKERS_EX_REQUEST)
    res = []
    str = ''
    if not args is None:
        str = json.dumps(args)
    aw.sendString(str)
    if aw.waitForResponse() == -1:
        return res
    response = aw.response()
    nMarkers = response.readInt32()
    if nMarkers == 0:
        return res
    aw.waitForResponse()
    for i in range(0, nMarkers):
        m = marker.Marker()
        m.label = response.readQString()
        m.position = response.readFloat()
        m.duration = response.readFloat()
        m.value = response.readFloat()
        m.targets = response.readQStringList()
        res.append(m)
    return res
