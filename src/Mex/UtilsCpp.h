#pragma once

#include <QTcpSocket>


// Request class

class TCPRequest
{
public:
	explicit TCPRequest(int requestID);
	~TCPRequest();
	enum Status { connected, failed, undefined };

	inline int status() { return m_status; }
	inline QTcpSocket* socket() { return m_socket; }
	QDataStream* stream() { return m_streamData; }
	/** Send a request to the host - data can be empty if the request does not require parameters. */
	bool sendRequest();
	bool send(const QByteArray& data);
	int getResponse();
	void clear();

protected:
	void connect();

	QTcpSocket* m_socket;
	int m_status, m_pid, m_requestID;
	QByteArray m_size;
	QByteArray m_data;
	QDataStream* m_streamSize;
	QDataStream* m_streamData;
};
