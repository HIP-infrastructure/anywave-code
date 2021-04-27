import requests
import network.tcprequest as tcp
from PyQt5 import QtCore

def send_markers(markers):
    MAX_MARKERS_AT_ONCE = 1000
    if type(markers) is not tuple:
        raise Exception('arguments of send_markers must be a tuple of Markers')
        return
    aw = tcp.TCPRequest(requests.SEND_MARKERS_REQUEST)
    nMarkers = int(len(markers))
    data = QtCore.QByteArray()
    streamData = QtCore.QDataStream(data, QtCore.QIODevice.WriteOnly)
    start = 0
    n = max(MAX_MARKERS_AT_ONCE, nMarkers)
    streamData.writeInt32(n)
    stop = False
    while not stop:
        for i in range(start, start + n):
            marker = markers[i];
            streamData.writeQString(marker.label)
            streamData.writeFloat(marker.position)
            streamData.writeFloat(marker.duration)
            streamData.writeFloat(marker.value)
            streamData.writeQStringList(marker.targets)

        # send data
        aw.sendData(data)
        aw.waitForResponse() # wait for anywave to acknowledge
        start = start + n
        stop = start >= n
