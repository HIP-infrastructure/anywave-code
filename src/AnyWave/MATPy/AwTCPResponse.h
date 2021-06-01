// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#pragma once

#include <qtcpsocket.h>
#include <qdatastream.h>

class AwTCPResponse
{
public:
	explicit AwTCPResponse(QTcpSocket *socket);
	~AwTCPResponse();

	QDataStream *stream() { return m_streamData; }
	void send(int status = 0);
protected:
	void clear();

	QTcpSocket * m_socket;
	QByteArray m_size;
	QByteArray m_data;
	QDataStream *m_streamSize;
	QDataStream *m_streamData;

};