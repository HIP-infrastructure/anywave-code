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
#ifndef AW_LAYOUT_MANAGER_H
#define AW_LAYOUT_MANAGER_H
#include <AwGlobal.h>
#include <QString>
#include <QHash>
#include <AwChannel.h>
class AwLayout;
#include <AwFileIO.h>


class AW_LAYOUT_EXPORT AwLayoutManager
{
public:
	static AwLayoutManager *instance();
	~AwLayoutManager();

	/** Get the layout matching the name and type **/
	AwLayout *layoutByName(const QString& name, int type);
	/** Get the layout referenced by the reader plug-in and matching flags. **/
	AwLayout *layoutFromFile(AwFileIO *reader, int flags);
	/** Try to find a suitable layout by browsing electrode names in file **/
	AwLayout *guessLayout(AwFileIO *reader, int flags);
	/** Get all loaded layouts **/
	QList<AwLayout *> layouts();
	/** Defines the folder path where to read/write mesh files **/
	void setWorkingDir(const QString& dir) { m_workingDir = dir; }
	inline QString workingDir() { return m_workingDir; }
protected:
	AwLayoutManager();

	void init(); // load layouts in memory
	void addLayout(AwLayout *layout);
	int electrodesMatch(const AwChannelList& channels, AwLayout *layout);

	QString m_workingDir;
	QHash<QString, AwLayout *> m_layouts;
	static AwLayoutManager *m_instance;
	bool m_isInit;
};

#endif