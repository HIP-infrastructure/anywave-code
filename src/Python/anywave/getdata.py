
import requests
import network.tcprequest as tcp

def get_data():
    request = tcp.TCPRequest(requests.GET_DATA_EX_REQUEST)
    