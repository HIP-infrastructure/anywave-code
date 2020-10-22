
from PyQt5 import QtCore, QtNetwork
import anywave
import json
"""
"""
# constants
STATUS_SUCCESS = 0
STATUS_FAILED  = 1
SIZE_INT = 4

class TCPRequest:
    def __init__(self, request):
        self.request = request
        self.socket = QtNetwork.QTcpSocket()
        self.size = QtCore.QByteArray()
        self.data = QtCore.QByteArray()
        self.streamData = QtCore.QDataStream(self.data, QtCore.QIODevice.WriteOnly)
        self.streamData.setVersion(QtCore.QDataStream.Version.Qt_4_4)
        self.streamSize = QtCore.QDataStream(self.size, QtCore.QIODevice.WriteOnly)
        self.streamSize.setVersion(QtCore.QDataStream.Version.Qt_4_4)
        self.streamResponse = QtCore.QDataStream(self.socket)
        self.streamResponse.setVersion(QtCore.QDataStream.Version.Qt_4_4)
        self.connect()

    def connect(self):
        self.socket.connectToHost(anywave.host, anywave.port)
        if not self.socket.waitForConnected():
           self.status = STATUS_FAILED
           error = "Unable to connect to AnyWave :" + self.socket.errorString()
           raise Exception(error)
        self.status = STATUS_SUCCESS

    def simpleRequest(self):
        # arg is a string that could be empty
        if not self.status == STATUS_SUCCESS:
            raise Exception('not connected to AnyWave')
            return False
        self.streamSize << anywave.pid << int(SIZE_INT) << self.request
        self.socket.write(self.size)
        self.socket.flush()
        if not self.socket.waitForBytesWritten():
            raise Exception('Error while sending request to AnyWave')
            return False
        return True

    # sendRequest() send a request with parameters a json string.
    # the string can be none or empty
    def sendRequest(self, args):
        # args must be a dict
        if self.status is not STATUS_SUCCESS:
            raise Exception('Sending request while not connected to AnyWave')
            return False
        str = json_dumps(args)
        self.streamSize << anywave.pid << str.size() + SIZE_INT << self.request
        self.socket.write(self.size)
        self.streamData << json
        self.socket.write(self.data)
        self.socket.flush()
        if not self.socket.waitForBytesWritten():
            raise Exception('Error while sending request to AnyWave')
            return False
        return True

    def sendData(self, data):
        # data must be a bytes, not a json string
        if self.status is not STATUS_SUCCESS:
            raise Exception('Sending request while not connected to AnyWave')
            return False
        if not isinstance(data, bytes):
            raise Exception('argument must be of type bytes')
            return False
        # use a qbyte array to send the data
        self.streamSize.writeInt32(anywave.pid)
        self.streamSize.writeInt32(int(len(data) + SIZE_INT))
        self.streamSize.writeInt32(self.request)
        self.streamData.writeBytes(data)
        self.socket.write(self.size)
        self.socket.write(self.data)
        #self.socket.flush()
        if not self.socket.waitForBytesWritten():
            raise Exception('Error while sending request to AnyWave')
            return False
        return True