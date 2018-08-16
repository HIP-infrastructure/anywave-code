////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// communication functions

#include "common.h"
#define WAIT_TIME_OUT   3000000   // 300s socket time out
void writeToHost(QTcpSocket *socket, int pid, const QByteArray& data)
{
    QByteArray size;
    QDataStream stream_size(&size, QIODevice::WriteOnly);
    stream_size.setVersion(QDataStream::Qt_4_4);
    // always send the pid first then size and data.
    stream_size << pid;
    stream_size << data.size();
	socket->write(size);
	socket->write(data);
	socket->waitForBytesWritten();   
}

int waitForResponse(QTcpSocket *socket)
{
    while (socket->bytesAvailable() < sizeof(int))
       if (!socket->waitForReadyRead(WAIT_TIME_OUT))
			return -1;

    QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_4);
    int status, size;
    in >> status;
    if (status == -1)   // if status is incorrect ends here.
        return status;
    // getting size of data (in bytes)
    while (socket->bytesAvailable() < sizeof(int))
		if (!socket->waitForReadyRead(WAIT_TIME_OUT))
			return -1;

    in >> size;

	// update 3.7.2014   Wait for all data to be available
	while (socket->bytesAvailable() < size)
        if (!socket->waitForReadyRead(WAIT_TIME_OUT))
            return -1;
    return size;
}

int getPid() 
{
    // get pid
    mxArray *mxpid = mexGetVariable("base", "pid");
    if (mxpid == NULL)
        return -1;
    
    return (int)mxGetPr(mxpid)[0];
}

QTcpSocket *connect()
{
    // Getting host from workspace base
    mxArray *mxhost = mexGetVariable("base", "host");
    
   if (mxhost == NULL)
       return NULL;
    
    // get pid
    mxArray *mxpid = mexGetVariable("base", "pid");
    if (mxpid == NULL)
        return NULL;
    
    // get port
    mxArray *mxport = mexGetVariable("base", "port");
    if (mxpid == NULL)
        return NULL;
    
    QString host(mxArrayToString(mxhost));
    int pid = (int)mxGetPr(mxpid)[0];
    int port = (int)mxGetPr(mxport)[0];
    
    QTcpSocket* socket = new QTcpSocket;
    socket->connectToHost(host, port);
    if (!socket->waitForConnected())
    {
        mexErrMsgTxt("Unable to connect to host.");
        mexErrMsgTxt(socket->errorString().toLatin1().data());
        delete socket;
        return NULL;
    }
    return socket;
}


mxArray *createAnyWaveStruct()
{
    const char *fields[] = { "input_channels", "data_path", "working_dir", "total_duration" };
    int dim = 1;
    return mxCreateStructMatrix(1, 1, 4, fields);
}


//// Classes implementations

TCPRequest::TCPRequest(int requestID)
{
	m_status = TCPRequest::undefined;
	m_socket = connect();
	m_pid = -1;
	m_requestID = requestID;
	if (m_socket == NULL) {
		m_status = TCPRequest::failed;
	}
	else {
		m_status = TCPRequest::connected;
		m_pid = getPid();
	}
}

TCPRequest::~TCPRequest()
{
	if (m_socket)
		delete m_socket;
}

bool TCPRequest::writeToHost(const QByteArray& data)
{
	QByteArray size;
	QDataStream stream_size(&size, QIODevice::WriteOnly);
	stream_size.setVersion(QDataStream::Qt_4_4);
	int dataSize = data.size() + sizeof(int); // data size + request ID size
	// always send the pid first then size and data.
	stream_size << m_pid;
	stream_size << dataSize << m_requestID;
	m_socket->write(size);
	if (!data.isEmpty())
		m_socket->write(data);
	return m_socket->waitForBytesWritten();
}

int TCPRequest::getResponse()
{
	int size = waitForResponse(m_socket);
	if (size == -1) {
		mexErrMsgTxt("Bad status received from AnyWave.");
	}
	return size;
}