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
#ifndef CHOOSEMARKER_H
#define CHOOSEMARKER_H

#include <QDialog>
#include "ui_ChooseMarker.h"
#include <AwMarker.h>

class ChooseMarkerDialog : public QDialog
{
	Q_OBJECT

public:
	ChooseMarkerDialog(AwMarkerList *markers, QWidget *parent = 0);
	~ChooseMarkerDialog();
	inline float value() { return m_value; }
	inline int before() { return m_beforems; }
	inline int after() { return m_afterms; }
	QStringList selectedMarkers() { return m_selectedMarkers; }
	QVector<int>& channelTypes() { return m_types; }
public slots:
	void accept();

private:
	Ui::ChooseMarkerUi m_ui;
	QStringList m_markersNames;
	QStringList m_selectedMarkers;
	float m_value;	// overwrite value in pT
	int m_beforems;
	int m_afterms;
	QVector<int> m_types;	// channel types on which to apply overwriting.
};

#endif // CHOOSEMARKER_H
