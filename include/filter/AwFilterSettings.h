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
//    Authors: Bruno Colombet, Antoine Ducorps – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//    cenir - antoine.ducorps@umpc.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <AwGlobal.h>
#include <AwChannel.h>

class AwFilterGUI;
class AW_FILTER_EXPORT AwFilterSettings : public QObject
{
	Q_OBJECT
public:
	explicit AwFilterSettings();
	explicit AwFilterSettings(const AwFilterSettings& copy);
	~AwFilterSettings();
	AwFilterSettings& operator=(const AwFilterSettings& other);
	/** clean up all filter settings and bounds **/
	void clear();
	/** set filters for a specified type of channel **/
	void set(int type, const QVector<float>& values);
	/** convenience method to use a string instead of the channel type **/
	void set(const QString& name, const QVector<float>& values);
	/** set filters for a specified type of channel **/
	void set(int type, float hp, float lp, float notch);
	/** set bounds for a specified type of channel **/
	void setBounds(int type, const QVector<float>& values);
	void setBounds(int type, float hp, float lp);
	/** check for bounds **/
	QList<int> checkForBounds();
	/** apply the current settings to a channel **/
	void apply(AwChannel *channel) const;
	/** apply the current settings to a whole list of channels **/
	void apply(const AwChannelList& channels) const;
	/** init from previously saved json file for a specified data file **/
	bool initWithFile(const QString& filePath);
	/** init from a list of channels **/
	void initWithChannels(const AwChannelList& channels);
	/** reset all filters settings to zero **/
	void zero();
	/** save settings to json file **/
	void save(const QString& file);
	void load(const QString& file);
	inline int count() const { return m_hash.count(); }
	inline bool isEmpty() const { return m_hash.count() == 0; }
	QList<int> currentTypes() const { return m_hash.keys(); }
	QVector<float> filters(int type) const { return m_hash[type]; }

	QWidget *ui();
signals:
	void settingsChanged(const AwFilterSettings& settings);
	/** DebugLog complience **/
	void log(const QString& message);
public slots:
	void updateGUI();
protected slots:
	void setNewSettings(const AwFilterSettings& settings);
protected:
	QHash<int, QVector<float>> m_hash;
	QHash<int, QVector<float>> m_bounds;	// bounds holds the frequency boundaries for virtual channels like ICA/Source.
	AwFilterGUI *m_ui;
};

