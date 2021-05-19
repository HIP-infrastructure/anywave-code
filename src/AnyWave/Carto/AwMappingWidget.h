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
#ifndef AWMAPPINGWIDGET2_H
#define AWMAPPINGWIDGET2_H

#include <QWidget>
#include "ui_AwMappingWidget.h"
#include <AwChannel.h>
class AwLayout;


class AwMappingWidget : public QWidget
{
	Q_OBJECT

public:
	AwMappingWidget(int type, AwLayout *l2D, AwLayout *l3D, QWidget *parent = 0);
	~AwMappingWidget();
protected:
	void changeEvent(QEvent *e);
public slots:
	// values is the data from sensors.
	void updateMaps(float latency, const QVector<float>& data, const QStringList& labels);
	void updateMapsPCA(float start, float end, const QVector<float>& data, const QStringList& labels);
	void to2D(bool f);
	void to3D(bool f);
	void showBoth(bool f);
	void selectLabels(const QStringList& labels);
	void updateSelection(const QStringList& labels);
	void updateBadSensors(const QStringList& labels);
	void changeColorMap(int index);
signals:
	void selectedLabelsChanged(const QStringList& labels);
private:
	Ui::AwMappingWidgetUi ui;
	AwLayout *m_l2D, *m_l3D;
	QStringList m_selectedLabels;
	int m_type;	// type of Mapping (EEG or MEG)
};




#endif // AWMAPPINGWIDGET_H
