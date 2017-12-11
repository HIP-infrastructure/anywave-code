/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWMAPPINGWIDGET2_H
#define AWMAPPINGWIDGET2_H

#include <QWidget>
#include "ui_AwMappingWidget.h"
#include <AwChannel.h>
//class AwFilteringManager;
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
	Ui::AwMappingWidgetUi2 ui;
	AwLayout *m_l2D, *m_l3D;
	QStringList m_selectedLabels;
	int m_type;	// type of Mapping (EEG or MEG)
};




#endif // AWMAPPINGWIDGET_H
