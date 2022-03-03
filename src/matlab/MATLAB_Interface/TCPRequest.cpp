#include "TCPRequest.h"
#include "MATLAB_Interface.h"

TCPRequest::TCPRequest(int request)
{
	m_request = request;
	m_status = TCPRequest::Success;
	m_streamData = new QDataStream(&m_data, QIODevice::WriteOnly);
	m_streamSize = new QDataStream(&m_size, QIODevice::WriteOnly);
	m_streamSize->setVersion(QDataStream::Qt_4_4);
	m_streamData->setVersion(QDataStream::Qt_4_4);
	m_streamResponse = new QDataStream(&m_socket);
	m_streamResponse->setVersion(QDataStream::Qt_4_4);
	connect();
}

TCPRequest::~TCPRequest()
{
	delete m_streamData;
	delete m_streamSize;
	delete m_streamResponse;
}

void TCPRequest::connect()
{
	m_socket.connectToHost(MATLAB_Interface::host, MATLAB_Interface::port);
	if (!m_socket.waitForConnected()) {
		m_status = TCPRequest::Failed;
		m_errorString = QString("Failed to connect to AnyWave: %1\n").arg(m_socket.errorString());
		m_errorString += QString("Host: %1 Port: %2").arg(MATLAB_Interface::host).arg(MATLAB_Interface::port);
		throw(m_errorString);
	}
	else
		m_status = TCPRequest::Success;
}

void TCPRequest::clear()
{
	m_data.clear();
	m_size.clear();
	m_streamData->device()->reset();
	m_streamSize->device()->reset();
}

int TCPRequest::waitForResponse()
{
	while (m_socket.bytesAvailable() < 4)
		if (!m_socket.waitForReadyRead(WAIT_TIME_OUT))
			return -1;
	QDataStream* response = new QDataStream(&m_socket);
	response->setVersion(QDataStream::Qt_4_4);
	int status;
	*response >> status;
	if (status == -1) {
		*response >> m_errorString;
		throw(m_errorString);
	}
	while (m_socket.bytesAvailable() < 4)
		if (!m_socket.waitForReadyRead(WAIT_TIME_OUT)) {
			m_errorString = "Nothing to read from socket.";
			throw(m_errorString);
		}
	int size;
	*response >> size;
	while (m_socket.bytesAvailable() < size)
		if (!m_socket.waitForReadyRead(WAIT_TIME_OUT)) 
			size = -1;
	if (size == -1) {
		m_errorString = "No data received.";
		throw(m_errorString);
	}	
	return size;
}

void TCPRequest::simpleRequest()
{
	if (m_status != TCPRequest::Success) {
		m_errorString = "Not connected to AnyWave.";
		throw(m_errorString);
	}
	clear();
	*m_streamSize << MATLAB_Interface::pid <<  int(sizeof(int)) << m_request;
	m_socket.write(m_size);
	if (!m_socket.waitForBytesWritten()) {
		m_errorString = "Error while sending data to AnyWave.";
		throw(m_errorString);
	}

}

void TCPRequest::sendString(const QString& s)
{
	clear();
	*m_streamData << s;
	// write the total size of data in size: total size is in fact  data size + size of request value (int)
	// first 4 bytes of size QBytesArray MUST BE the pid number of process.
	*m_streamSize << MATLAB_Interface::pid << int(m_data.size() + sizeof(int)) << m_request;
	m_socket.write(m_size);
	m_socket.write(m_data);
	if (!m_socket.waitForBytesWritten()) {
		m_errorString = "Error while sending data to AnyWave.";
		throw(m_errorString);
	}
}

void  TCPRequest::sendData(const QByteArray& data)
{
	clear();
	// write the total size of data in size: total size is in fact  data size + size of request value (int)
	// first 4 bytes of size QBytesArray MUST BE the pid number of process.
	*m_streamSize << MATLAB_Interface::pid << int(data.size() + sizeof(int)) << m_request;
	*m_streamData << data;
	m_socket.write(m_size);
	m_socket.write(m_data);
	
	if (!m_socket.waitForBytesWritten()) {
		m_errorString = "Error while sending data to AnyWave.";
		throw(m_errorString);
	}
}
