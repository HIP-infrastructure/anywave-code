import requests
import network.tcprequest as tcp

def send_message(message):
    request = tcp.TCPRequest(requests.SEND_MESSAGE_REQUEST)
    request.sendString(message)
