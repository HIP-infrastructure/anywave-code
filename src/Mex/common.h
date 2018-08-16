#ifndef COMMON_H
#define COMMON_H
#define IS_2D_ARRAY(P) (!mxIsComplex(P) && mxGetNumberOfDimensions(P) == 2 && !mxIsSparse(P))
#define IS_SCALAR(P)    (IS_2D_ARRAY(P) && mxGetNumberOfElements(P) == 1)
#include <QTcpSocket>
#include "mex.h"
#include "mat.h"
#include <QtCore/QDataStream>

int waitForResponse(QTcpSocket *socket);
//int waitForResponse2(QTcpSocket *socket, QByteArray& response);
int getPid();
QTcpSocket *connect();
mxArray *createAnyWaveStruct();
void writeToHost(QTcpSocket *socket, int pid, const QByteArray& data);


class TCPRequest
{
public:
	explicit TCPRequest(int requestID);
	~TCPRequest();
	enum Status { connected, failed, undefined };

	inline int status() { return m_status; }
	inline QTcpSocket *socket() { return m_socket; }
	/** Send a request to the host - data can be empty if the request does not require parameters. */
	bool writeToHost(const QByteArray& data = QByteArray());
	int getResponse();

protected:
	QTcpSocket * m_socket;
	int m_status, m_pid, m_requestID;
};

#endif

