
import requests
import network.tcprequest as tcp
import json
import core.channel as channel
import numpy

def get_data(args = None):
    aw = tcp.TCPRequest(requests.GET_DATA_EX_REQUEST)
    str = ''
    res = []
    if not args is None:
        str = json.dumps(args)
    aw.sendString(str)
    aw.waitForResponse()
    stream = aw.stream()
    nChannels = stream.readInt32()
    if nChannels is 0:
        return res
    for i in range(0, nChannels):
         aw.waitForResponse()
         chan = channel.Channel()
         chan.name = stream.readQString()
         chan.type = stream.readQString()
         chan.ref = stream.readQString()
         chan.sr = stream.readFloat()
         chan.hp = stream.readFloat()
         chan.lp = stream.readFloat()
         chan.notch = stream.readFloat()
         nSamples = stream.readInt64()
         if nSamples > 0:
             chan.data = numpy.zeros(nSamples, numpy.float32)
             finished = False
             nSamplesRead = 0
             chunkSize = 0
             while not finished:
                 aw.waitForResponse()
                 chunkSize = stream.readInt64()
                 if chunkSize is 0:
                     finished = True
                 else:
                     for j in range(0, chunkSize):
                         chan.data[j + nSamplesRead] = stream.readFloat()
                     nSamplesRead += chunkSize
         res.append(chan)
    return res



