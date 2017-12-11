/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwResponse.h"
#include <QDataStream>

AwResponse::AwResponse(QTcpSocket *client)
{
	m_client = client;
	m_data = NULL;
	m_buffer = NULL;
}

AwResponse::~AwResponse()
{
	if (m_data)
		delete m_data;
	if (m_buffer) {
		m_buffer->close();
		delete m_buffer;
	}
}

void AwResponse::begin()
{
	if (m_data)
		delete m_data;
	m_data = new QByteArray;
	if (m_buffer) {
		m_buffer->close();
		m_buffer->setBuffer(m_data);
		m_buffer->open(QIODevice::WriteOnly);
	}
	else {
		m_buffer = new QBuffer(m_data);
		m_buffer->open(QIODevice::WriteOnly);
	}
}

void AwResponse::send()
{
	QByteArray size;
	QDataStream stream_size(&size, QIODevice::WriteOnly);
	stream_size.setVersion(QDataStream::Qt_4_4);
	int status = 0; //send status to client   0 = OK
	stream_size << status;
	stream_size << m_data->size();
	m_client->write(size);
	m_client->write(*m_data);
	m_client->waitForBytesWritten();
}