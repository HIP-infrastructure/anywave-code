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
#include <qwizard.h>
#include "AwOutputFileWizardPage.h"
#include "AwChannelsExportWizardPage.h"
#include "AwMarkersExportWizardPage.h"
#include "AwFilterExportWizardPage.h"

class AwExportWizardDial : public QWizard
{
	Q_OBJECT
public:
	AwExportWizardDial(const QVariantMap& settings = QVariantMap(), QWidget* parent = nullptr);

	AwChannelList channels;
	QStringList useMarkers, skipMarkers;
	AwFilterSettings& filterSettings() { return m_filterPage->filterSettings; }
	QString writerPluginName;
	QString filePath;
public slots:
	void accept() override;
protected:
	QVariantMap m_settings;
	AwChannelsExportWizardPage* m_channelsPage;
	AwMarkersExportWizardPage* m_markersPage;
	AwFilterExportWizardPage* m_filterPage;
	AwOutputFileWizardPage* m_outputPage;
	bool m_isBids;
};