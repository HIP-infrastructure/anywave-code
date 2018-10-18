/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <QTcpSocket>
#include <Python.h>
#include <qdatastream.h>

QTcpSocket *connect();
void sendRequest(QTcpSocket *, const QByteArray& );
QByteArray initRequest(int request);
int waitForResponse(QTcpSocket *);

/** parse a whole dictionnary and convert it to JSON format. **/
QString dictToJson(PyObject *dict);

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
