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
#include <AwDataClient.h>
#include <Qt3DCore/QEntity>

class Aw3DViewer : public AwDataClient
{
	Q_OBJECT
public:
	static Aw3DViewer* instance();
	static bool isInstanciated() { return m_instance != nullptr; }
	~Aw3DViewer();
public slots:
	int init();

protected:
	explicit Aw3DViewer(QObject* parent = nullptr);
	void start();
	Qt3DCore::QEntity* createScene();

	static Aw3DViewer* m_instance;
	QString m_freesurferDir;
	Qt3DCore::QEntity* m_rootEntity;
	QWidget* m_widget;
};
