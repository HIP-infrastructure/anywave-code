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
#ifndef H2UI_H
#define H2UI_H

#include <QDialog>
#include "ui_h2ui.h"
#include <AwMarker.h>

using namespace Ui;

typedef QPair<float, float> band;
typedef QPair<QDoubleSpinBox *, QDoubleSpinBox *> spinBox;

class H2UI : public QDialog, public H2UIClass
{
	Q_OBJECT

public:
	H2UI(QWidget *parent = 0);
	~H2UI();
	QString dataFolder;
	QMap<QString, QPair<float, float> > bands;
	int method;
	int downSampling;
	float samplingRate;
	QString directory;
	AwMarkerList markers;
	QStringList usedLabels, skippedLabels;
public slots:
	void accept() override;
	int exec() override;
protected:
	QStringList m_bandNames;
	QList<spinBox> m_spinBoxes;
	QList<QCheckBox *> m_checkBoxes;
};

#endif // H2UI_H
