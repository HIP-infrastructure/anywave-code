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
#ifndef AW_PREFS_DIAL_H
#define AW_PREFS_DIAL_H

#include <QDialog>
#include "ui_AwPrefsDial.h"
#include <QStandardItemModel>

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
	void initVenvTable();
	void addVenv(const QString& alias, const QString& dir);
	void setDefaultVenv(const QString& alias);

	QRect screen;
	QStringList m_markerTypes;
	QStringList m_markerColors;
	qint32 m_markerIndex;
	bool m_colorsChanged;
	QStandardItemModel* m_venvsModel;
	QMap<QString, QString> m_venvsMap;
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
	void pickGARDEL();
	void pickITK();
	void addPythonVenv();
	void setSelectedVenvAsDefault();
	void removeSelectedVenv();
protected slots:
	void changeDefaultRuntime();
	void changeDefaultMATLAB();

signals:
	void screenCalibrationChanged(float xPixPerCm, float yPixPerCm);
	void markersColorChanged(const QStringList& colors);
};

#endif // DIALPREFS_H
