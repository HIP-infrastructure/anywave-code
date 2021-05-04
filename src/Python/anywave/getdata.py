import requests
import network.tcprequest as tcp
import json
import core.channel as channel
import numpy


def get_data(args=None):
    aw = tcp.TCPRequest(requests.GET_DATA_EX_REQUEST)
    tmp = ''
    res = []
    if args is None:
        pass
    else:
        tmp = json.dumps(args)
    aw.sendString(tmp)
    aw.waitForResponse()
    response = aw.response()
    nChannels = response.readInt32()
    if nChannels == 0:
        return res
    for i in range(0, nChannels):
        aw.waitForResponse()
        chan = channel.Channel()
        chan.name = response.readQString()
        chan.type = response.readQString()
        chan.ref = response.readQString()
        chan.sr = response.readFloat()
        chan.hp = response.readFloat()
        chan.lp = response.readFloat()
        chan.notch = response.readFloat()
        nSamples = response.readInt64()
        if nSamples > 0:
            chan.data = numpy.zeros(nSamples, numpy.float32)
            finished = False
            nSamplesRead = 0
            chunkSize = 0
            while not finished:
                aw.waitForResponse()
                chunkSize = response.readInt64()
                if chunkSize == 0:
                    finished = True
                else:
                    for j in range(0, chunkSize):
                        chan.data[j + nSamplesRead] = response.readFloat()
                    nSamplesRead += chunkSize
        res.append(chan)
    return res
