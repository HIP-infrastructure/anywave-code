/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2020 AMU, INSERM
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
#pragma once

#include <QObject>
class AwFileIO;
class AwMontageManager;
class AwDataServer;
class AwDataClient;
class AwMontage;
#include <AwChannel.h>


class AwDataSet : public QObject
{
	Q_OBJECT

public:
	// add a unique instance feature
	static AwDataSet *instance();
	explicit AwDataSet(AwFileIO *reader, QObject *parent);
	~AwDataSet();

	void connect(AwDataClient *client);
	AwChannelList& montage();
	inline AwFileIO *reader() { return m_reader; }
	//void loadMontage(const QString& filePath);
protected:
	static AwDataSet *m_instance;
	/** protected constructor dedicated to instance() feature **/
	AwDataSet();

	AwFileIO *m_reader;
	AwDataServer *m_ds;
	AwMontage *m_montage;
};
