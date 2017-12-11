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
#ifndef AW_SOURCE_MAMANAGER_H
#define AW_SOURCE_MAMANAGER_H

#include <QObject>
#include <AwChannel.h>
#include <armadillo>
using namespace arma;
#include "AwSourceChannel.h"
#include <QMutex>

class AwSourceManager : public QObject
{
	Q_OBJECT

public:
	~AwSourceManager();

	static AwSourceManager *instance();
	enum SourceType { MEG, EEG };

	void closeFile();
	AwSourceChannelList& sources(int type);
	AwChannelList& realChannels(int type);
	float lp(int type);
	float hp(int type);
	QString autoSave(int type, mat& matrix, const QStringList& awLabels, const QStringList &labels, float lp, float hp);
	void computeSources(AwSourceChannelList& channels);
	
public slots:
	void setBeamformer(int type, mat& matrix, const QStringList& awLabels, const QStringList &labels, float lp, float hp);
	void load(QString path);
signals:
	void sourcesLoaded();
	void newSourcesCreated(int type);
protected:
	AwSourceManager(QObject *parent = 0);
	static AwSourceManager *m_instance;

	// only two source types (MEG,EEG)
	AwChannelList m_realChannels[2];	// Real channels used to create sources
	AwSourceChannelList m_channels[2];
	mat m_matrices[2];					// transfert matrix
	float m_lpf[2], m_hpf[2];
	QMutex m_mutex;
};

#endif // AWICAMANAGER_H
