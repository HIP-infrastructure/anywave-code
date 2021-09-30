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
#ifndef AWTIMESCALECOMBO_H
#define AWTIMESCALECOMBO_H
#include <AwGlobal.h>
#include <QComboBox>

class AW_WIDGETS_EXPORT AwTimeScaleCombo : public QComboBox
{
	Q_OBJECT

public:
	AwTimeScaleCombo(QWidget *parent);
	~AwTimeScaleCombo();
signals:
	void valueChanged(float value);
private slots:
	void extractValue(int index);
private:

	QVector<float> m_values;
	QStringList m_items;
};

#endif // AWTIMESCALECOMBO_H
