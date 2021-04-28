import requests
import network.tcprequest as tcp
from PyQt5 import QtCore

def send_markers(markers):
    MAX_MARKERS_AT_ONCE = 1000
    if type(markers) is not list:
        raise Exception('arguments of send_markers must be a tuple of Markers')
        return
    aw = tcp.TCPRequest(requests.SEND_MARKERS_REQUEST)
    nMarkers = int(len(markers))
    data = QtCore.QByteArray()
    streamData = QtCore.QDataStream(data, QtCore.QIODevice.WriteOnly)
    streamData.setVersion(QtCore.QDataStream.Version.Qt_4_4)
    start = 0
    counter = 0
    while True:
        n = min(MAX_MARKERS_AT_ONCE, nMarkers)
        streamData.writeInt32(n)        
        for i in range(0, n):
            marker = markers[i + counter]
            streamData.writeQString(marker.label)
            streamData.writeQString(marker.colour)
            streamData.writeFloat(marker.position)
            streamData.writeFloat(marker.duration)
            streamData.writeFloat(marker.value)
            streamData.writeQStringList(marker.targets)
        aw.sendData(bytes(data))
        aw.waitForResponse() # wait for anywave to acknowledge
        nMarkers = nMarkers - n
        counter = counter + n
        if nMarkers == 0:
            break
   