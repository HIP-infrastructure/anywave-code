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
#include "AwTCPResponse.h"

AwTCPResponse::AwTCPResponse(QTcpSocket *socket)
{
	m_socket = socket;
	m_streamSize = new QDataStream(&m_size, QIODevice::WriteOnly);
	m_streamSize->setVersion(QDataStream::Qt_4_4);
	m_streamData = new QDataStream(&m_data, QIODevice::WriteOnly);
	m_streamData->setVersion(QDataStream::Qt_4_4);
}

AwTCPResponse::~AwTCPResponse()
{
	delete m_streamSize;
	delete m_streamData;
}

void AwTCPResponse::send(int status)
{
	*m_streamSize << status;
	*m_streamSize << m_data.size();
	m_socket->write(m_size);
	m_socket->write(m_data);
	m_socket->waitForBytesWritten();
	clear();
}

void AwTCPResponse::clear()
{
	m_data.clear();
	m_size.clear();
	m_streamData->device()->reset();
	m_streamSize->device()->reset();
}