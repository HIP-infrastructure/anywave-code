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
#include "AwDataSet.h"
#include <AwFileIO.h>
#include "Data/AwDataServer.h"
//#include "Montage/AwMontageManager.h"
#include <montage/AwMontage.h>


AwDataSet::AwDataSet(AwFileIO *reader, QObject *parent)
	: QObject(parent)
{
	m_reader = reader;
	m_ds = AwDataServer::getInstance()->duplicate(m_reader);
	m_ds->setParent(this);
	m_montage = new AwMontage(reader);
}

AwChannelList& AwDataSet::montage()
{
	return m_montage->channels();
}

AwDataSet::~AwDataSet()
{
	delete m_montage;
}

void AwDataSet::connect(AwDataClient *client)
{
	m_ds->openConnection(client);
}
