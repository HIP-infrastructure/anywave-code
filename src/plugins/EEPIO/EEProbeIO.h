/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "eepio_global.h"
#include <AwFileIO.h>
#include "cnt/cnt.h"

class EEPIO_EXPORT EEPIO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	EEPIO(const QString& fileName);
	~EEPIO();

	void cleanUpAndClose() override;
	qint64 readDataFromChannels(float start, float duration, AwChannelList &channelList) override;
	FileStatus openFile(const QString &path) override;
	FileStatus canRead(const QString &path) override;
protected:
	eeg_t *_libeep_cnt;	// pointer to eeprobe data structure
	float m_sampleRate;
	QVector<double> m_scales;
};


class EEPIO_EXPORT EEPIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
	Q_INTERFACES(AwFileIOPlugin)
public:
	EEPIOPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(EEPIO)
};
