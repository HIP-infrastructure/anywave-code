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

#ifndef AWFILTERINGMANAGER_H
#define AWFILTERINGMANAGER_H
#include <AwChannel.h>
#include <AwFilteringOptions.h>
#include <QObject>
class AwFilteringManager : public QObject
{
	Q_OBJECT
public:
	static AwFilteringManager *instance();

	inline bool anyFilterActive() { return m_flags != 0; }
	inline float lowPass(int type) { return m_lp[type]; }
	inline float highPass(int type) { return m_hp[type]; }
	inline float notch(int type) { return m_notch[type]; }

	void closeFile();
	void quit();
	void setFilename(const QString& path);
	void load();
	void save();
	// get a filteringOptions object containing current filters settings
	AwFilteringOptions filteringOptions();
public slots:
	void setFilter(int type, float LP, float HP);
	/** Set the filters used when computing the ICA. the type specifies MEG, EEG or EMG **/
	void setICASettings(int type, float lp, float hp);
	void setSourceSettings(int type, float lp, float hp);
	void setNotch(int type, float notch);
	void reset();
	void switchFilters(bool On);
signals:
	void filtersChanged();
	void noActiveFilters();
	void newICASettings(int type, float lp, float hp);
	void newSourceSettings(int type, float lp, float hp);
	/** Send when the current file is closed. */
	void filtersReset();

protected:
	AwFilteringManager(QObject *parent = 0);
	static AwFilteringManager *m_instance;

	enum Filters { EEG_HP = 1, EEG_LP = 2, EMG_HP = 4, EMG_LP = 8,
		MEG_HP = 16, MEG_LP = 32, ICA_HP = 64, ICA_LP = 128, Source_LP = 256, Source_HP = 512 };

	float m_lp[AW_CHANNEL_TYPES];	// low pass filter for all channel types
	float m_hp[AW_CHANNEL_TYPES];	// high pass filter for all channel types
	float m_notch[AW_CHANNEL_TYPES];
	float m_savedLP[AW_CHANNEL_TYPES];
	float m_savedHP[AW_CHANNEL_TYPES];
	float m_savedNotch[AW_CHANNEL_TYPES];
	bool checkForActiveFilters();

	int m_flags;			// Filters flag
	bool m_activeFilters;	// true if any filters is set.

	QString m_filePath;
	QPair<float, float> m_ICAComputationSettings[3];
	QPair<float, float> m_SourceComputationSettings[2];
};

#endif // AWFILTERINGMANAGER_H
