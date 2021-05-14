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

#ifndef AW_PREFS_DIAL_H
#define AW_PREFS_DIAL_H

#include <QDialog>
#include "ui_AwPrefsDial.h"

using namespace Ui;


class AwPrefsDial : public QDialog, public DialPrefsClass
{
	Q_OBJECT

public:
	enum Tabs { General = 0, ScreenCalibration = 1 };
	AwPrefsDial(int startingTab = AwPrefsDial::General, QWidget *parent = 0);
protected:
	void changeEvent(QEvent *);
private:
	void changeMarkerColor(const QString& color);
	void changeCursorColor(const QString& color);
	void changeMappingCursorColor(const QString& color);
	void changeCursorFontText(const QFont& font);
	void changeMappingCursorFontText(const QFont& font);
	void changeMarkerFontText(const QFont& font);
	QString fontToLabel(const QFont& font);
	bool detectPython(const QString& path);

	QRect screen;
	QStringList m_markerTypes;
	QStringList m_markerColors;
	qint32 m_markerIndex;
	bool m_colorsChanged;
public slots:
	void accept();
	void restoreDefaultMarkersColors();
	void restoreDefaultCursorColors();
	void restoreDefaultMappingColors();
	void selectCursorColor();
	void selectMappingColor();
	void editCursorFont();
	void editMappingFont();
	void editMarkerFont();
	void changeMarkerType(int index);
	void chooseCustomColor();
	void pickMatlabFolder();
	void pickMCRFolder();
	void pickGARDEL();
	void pickITK();
	void pickPython();
signals:
	void screenCalibrationChanged(float xPixPerCm, float yPixPerCm);
	void markersColorChanged(const QStringList& colors);
};

#endif // DIALPREFS_H
