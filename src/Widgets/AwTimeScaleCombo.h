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
