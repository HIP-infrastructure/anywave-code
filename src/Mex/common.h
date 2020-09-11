#ifndef COMMON_H
#define COMMON_H
#define IS_2D_ARRAY(P) (!mxIsComplex(P) && mxGetNumberOfDimensions(P) == 2 && !mxIsSparse(P))
#define IS_SCALAR(P)    (IS_2D_ARRAY(P) && mxGetNumberOfElements(P) == 1)
#include <QTcpSocket>
#include "mex.h"
#include "mat.h"
#include <QtCore/QDataStream>

int waitForResponse(QTcpSocket *socket);
int getPid();
QTcpSocket *connect();
mxArray *createAnyWaveStruct();
void writeToHost(QTcpSocket *socket, int pid, const QByteArray& data);

// utilities functions

mxArray *floatToMat(float value);
mxArray *doubleToMat(double value);
mxArray *int32ToMat(qint32 value);
mxArray *int16ToMat(qint16 value);
mxArray *boolToLogical(bool value);
mxArray *floatVectorToMat(const QVector<float>& vector);
mxArray *doubleVectorToMat(const QVector<double>& vector);
mxArray *intVectorToMat(const QVector<int>& vector);


QString toJson(const mxArray *struc);

// Request class

class TCPRequest
{
public:
	explicit TCPRequest(int requestID);
	~TCPRequest();
	enum Status { connected, failed, undefined };

	inline int status() { return m_status; }
	inline QTcpSocket *socket() { return m_socket; }
	QDataStream *stream() { return m_streamData; }
	/** Send a request to the host - data can be empty if the request does not require parameters. */
	bool sendRequest();
	bool send(const QByteArray& data);
	int getResponse();
	void clear();

protected:
	QTcpSocket * m_socket;
	int m_status, m_pid, m_requestID;
	QByteArray m_size;
	QByteArray m_data;
	QDataStream *m_streamSize;
	QDataStream *m_streamData;
};

#endif

