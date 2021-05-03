#pragma once

#include <QTcpSocket>
#include <QDataStream>
#include <QString>
constexpr auto WAIT_TIME_OUT = 3000000;
class TCPRequest
{
	friend class MexFunction;
public:
	TCPRequest(int request);
	~TCPRequest();
	enum Status  { Success, Failed };
	inline QString& errorString() {
		return m_errorString;
	}

	QDataStream& response() {
		return *m_streamResponse;
	}
	int waitForResponse();
	void simpleRequest();
	void sendData(const QByteArray& data);
	void sendString(const QString&);
protected:
	void connect();
	void clear();

	QTcpSocket m_socket;
	QByteArray m_data;
	QByteArray m_size;
	QDataStream *m_streamData;
	QDataStream *m_streamSize;
	QDataStream* m_streamResponse;
	QString m_errorString;
	int m_status, m_request;
};