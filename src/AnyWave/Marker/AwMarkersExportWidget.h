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

#include <QDialog>
#include "ui_AwMarkersExportWidget.h"
#include <AwMarker.h>
#include <AwChannel.h>

class AwMarkersExportWidget : public QDialog
{
	Q_OBJECT

public:
	AwMarkersExportWidget(const AwMarkerList& selection, const AwMarkerList& markers, QWidget *parent = Q_NULLPTR);
	~AwMarkersExportWidget();
	inline bool concatenate() { return m_concatenate;  }
	AwChannelList& channels() { return m_channels; }
	inline QString& file() { return m_file; }
	AwMarkerList& markers() { return m_markers; }
	inline bool isMatlabFormat() { return m_ui.radioMATLAB->isChecked(); }
public slots:
	void selectFile();
	void accept() override;
private:
	bool checkFile();
	Ui::AwMarkersExportWidgetUi m_ui;
	AwMarkerList m_selected, m_all, m_markers;
	AwChannelList m_channels;
	bool m_concatenate;
	QString m_file;
};
