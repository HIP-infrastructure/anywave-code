import anywave

anywave.send_message('test')
channels = anywave.get_data()
print(channels[0].data)
args = { 'types' : [ 'eeg']}
channels = anywave.get_data(args)
print(channels[0].data)