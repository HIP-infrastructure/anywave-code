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
	if (status == -1) { // if status is incorrect ends here.
		mexPrintf("Bad status returned by AnyWave.\n");
		return status;
	}
    // getting size of data (in bytes)
    while (socket->bytesAvailable() < sizeof(int))
		if (!socket->waitForReadyRead(WAIT_TIME_OUT)) {
			mexPrintf("Nothing to read from socket.\n");
			return -1;
		}

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

mxArray *floatToMat(float value)
{
	mxArray *tmp = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
	float *s = (float *)mxGetData(tmp);
	s[0] = value;
	return tmp;
}

mxArray *doubleToMat(double value)
{
	mxArray *tmp = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);
	double *s = (double *)mxGetData(tmp);
	*s = value;
	return tmp;
}

mxArray *int32ToMat(qint32 value)
{
	mxArray *tmp = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	qint32 *s = (qint32 *)mxGetData(tmp);
	s[0] = value;
	return tmp;
}

mxArray *int16ToMat(qint16 value)
{
	mxArray *tmp = mxCreateNumericMatrix(1, 1, mxINT16_CLASS, mxREAL);
	qint16 *s = (qint16 *)mxGetData(tmp);
	s[0] = value;
	return tmp;
}

mxArray *boolToLogical(bool value)
{
	mxArray *tmp = mxCreateLogicalMatrix(1, 1);
	*mxGetLogicals(tmp) = value;
	return tmp;
}

QString toJson(const mxArray *struc)
{
	mxArray *out[1];
	mxArray *in[1] = { (mxArray *)struc };
	int status = mexCallMATLAB(1, out, 1, in, "jsonencode");
	if (status != 0) {
		return QString();
	}

	return QString(mxArrayToString(out[0]));
}

mxArray *floatVectorToMat(const QVector<float>& vector)
{
	mxArray *tmp = mxCreateNumericMatrix(1, vector.size(), mxSINGLE_CLASS, mxREAL);
	float *data = (float *)mxGetData(tmp);
	for (auto v : vector)
		*data++ = v;
	return tmp;
}

mxArray *doubleVectorToMat(const QVector<double>& vector)
{
	mxArray *tmp = mxCreateNumericMatrix(1, vector.size(), mxDOUBLE_CLASS, mxREAL);
	double *data = (double *)mxGetData(tmp);
	for (auto v : vector)
		*data++ = v;
	return tmp;
}

mxArray *intVectorToMat(const QVector<int>& vector)
{
	mxArray *tmp = mxCreateNumericMatrix(1, vector.size(), mxINT32_CLASS, mxREAL);
	int *data = (int *)mxGetData(tmp);
	for (auto v : vector)
		*data++ = v;
	return tmp;
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
	m_streamSize = new QDataStream(&m_size, QIODevice::WriteOnly);
	m_streamSize->setVersion(QDataStream::Qt_4_4);
	m_streamData = new QDataStream(&m_data, QIODevice::WriteOnly);
	m_streamData->setVersion(QDataStream::Qt_4_4);
}

TCPRequest::~TCPRequest()
{
	if (m_socket)
		delete m_socket;
	delete m_streamSize;
	delete m_streamData;
}

void TCPRequest::clear()
{
	m_size.clear();
	m_data.clear();
}

bool TCPRequest::sendRequest()
{
	int dataSize = m_data.size() + sizeof(int); // data size + request ID size
	// always send the pid first then size and data.
	*m_streamSize << m_pid;
	*m_streamSize << dataSize << m_requestID;
	m_socket->write(m_size);
	m_socket->write(m_data);
	return m_socket->waitForBytesWritten(-1);
}

bool TCPRequest::send(const QByteArray& data)
{
	QByteArray header;
	QDataStream stream(&header, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_4_4);
	stream << m_pid << data.size() << m_requestID;
	m_socket->write(header);
	m_socket->waitForBytesWritten();
	m_socket->write(data);
	return m_socket->waitForBytesWritten();
}

int TCPRequest::getResponse()
{
	return waitForResponse(m_socket);
}