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
#ifndef AWICAMANAGER_H
#define AWICAMANAGER_H

#include <QObject>
#include <AwChannel.h>
class AwICAComponents;

class AwICAManager : public QObject
{
	Q_OBJECT

public:
	AwICAManager(QObject *parent = 0);
	~AwICAManager();

	static AwICAManager *instance();

	void closeFile();
	int loadComponents(const QString& icaFile);
	AwICAComponents *getComponents(int type);
	AwICAComponents **getAllComponents();
	int numberOfComponents();
	bool reject(int type);
	void rejectComponents(int type, const AwChannelList& channels);
	bool containsComponents(int type);

	inline bool isFilteringActive() { return m_isFilteringOn; }
	void turnICAOff();
public slots:
	void setICAFiletring(bool on) { m_isFilteringOn = on;  emit filteringSwitched(on); }
signals:
	void componentsLoaded();
	void filteringSwitched(bool active);
	void icaComponentsUnloaded();
	void componentsFiltersLoaded(float lp, float hp);
private:
	QString convertToMatlab(const QString& fileName);

	AwICAComponents *m_comps[AW_CHANNEL_TYPES];  
	static AwICAManager *m_instance;

	bool m_isFilteringOn;
};

#endif // AWICAMANAGER_H
