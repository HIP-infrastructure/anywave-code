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
#include "AwGTCMenu.h"

AwGTCMenu::AwGTCMenu(const QString& title, QWidget *parent)
	: QMenu(title, parent)
{

}

AwGTCMenu::~AwGTCMenu()
{

}


void AwGTCMenu::updateLabels(const QStringList& labels)
{
	clear();
	QStringList sorted = labels;
	sorted.sort();
	QMenu *m_subMenu = NULL;
	QChar currentLetter = ' ';
	foreach (QString s, sorted) {
		if (s.isEmpty()) // skip any empty strings (should not happen in most cases...)
			continue;
		if (s.at(0) != currentLetter) {
			currentLetter = s.at(0);
			m_subMenu = addMenu(QString(currentLetter));
		}
		m_subMenu->addAction(s);
	}
}