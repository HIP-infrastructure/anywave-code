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

#include <AwGlobal.h>
#include <AwChannel.h>
class AwFileIO;

class AW_MONTAGE_EXPORT AwMontage
{
public:
	explicit AwMontage(AwFileIO *reader);
	~AwMontage();
	enum MontageTypes { bipolar, monopolar };
	static AwChannelList load(const QString& path, QMap<QString, int> *asRecordedChannels = nullptr);
	static void save(const QString& path, const AwChannelList& channels, const AwChannelList& asRecorded = AwChannelList());
	inline AwChannelList& channels() { return m_channels; }
	/** load bad channel file and return the labels **/
	static QStringList loadBadChannels(const QString& filePath);
	/** from a channel list, create a list containing bipolar channels **/
	static AwChannelList createSEEGBipolarMontage(const AwChannelList& channels, const QStringList& badLabels = QStringList());
	static void removeBadChannels(AwChannelList& channels, const QStringList& badLabels);
	static int getElectrodeLabelAndIndex(AwChannel* chan, QString& label);
protected:
	void loadBadChannels();
	bool loadMontage(const QString& mtgFile);

	AwChannelList m_asRecorded, m_channels;
	QStringList m_badChannelLabels;
	QHash<QString, AwChannel *> m_asRecordedHash;
	QString m_badChannelPath;
	AwFileIO *m_reader;
};