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
#ifndef H2_H
#define H2_H

#include "h2_global.h"
#include <AwProcessInterface.h>
#include <QtCore>
#include "h2ui.h"
#include "H2Params.h"
#include <matlab/AwMATLAB.h>
class H2MainGui;
#include <armadillo>
using namespace arma;

namespace Global {
	namespace Method {
		enum Methods { h2, r2 };
	}
}

typedef struct {
	QString name;
	float hp, lp;
} FreqBand;

class H2_EXPORT H2 : public AwProcess
{
	Q_OBJECT

public:
	H2();
	~H2();	
	bool showUi() override;
	void run() override;
	void runFromCommandLine() override;
	//void init() override;
	int initialize();
	QList<H2_run *>& runs() { return m_runs; }
	
	float windowSize() { return m_winSize; }
	void setWindowSize(float size) { m_winSize = size; }
	float downSample() { return m_downSample; }
	void setDownSample(float f) { m_downSample = f; }
	QString markerIgnored() { return m_markerToSkip; }
	void setMarkerToSkip(QString label) { m_markerToSkip = label; }
	float step() { return m_step; }
	void setStep(float step) { m_step = step; }
	void setMethodName(QString method);
	QString methodName() { return m_methodName; }

	float maxLag() { return m_maxLag; }
	void setMaxLag(float ml) { m_maxLag = ml; }

	QString matlabFile() { return m_MatlabBaseFilePath; }
	void setMatlabFile(QString file) { m_MatlabBaseFilePath = file; }
	// Launch H2 on specific event on a channel.
	QString markerTrigger() { return m_markerTrigger; }
	void setTriggerMarker(QString trigger) { m_markerTrigger = trigger; }
	float preTriggerTime() { return m_preTrigger;  }
	void setPreTriggerTime(float time) { m_preTrigger = time; }
	float postTriggerTime() { return m_postTrigger; }
	void setPostTriggerTime(float time) { m_postTrigger = time; }

	int saveToMatlab(const QString& fileName);
public slots: // used to expose method to qml script
	void setFreqBand(QString name, float hp, float lp);
private slots:
	void setProgressNewRange(int, int);
	void setProgressValue(int);
private:
	void clean();
	/* Compute all the runs using a specified frequency band */
	int computeRuns();
    // debug purpose
	void saveSignals(const AwChannelList& channels);
	// ui
	H2UI *m_ui;
	float m_winSize;	// taille de fenetre en secondes
	float m_maxLag;		// max lag en secondes
	float m_downSample;	// down sampling freq. No downsampling if 0
	quint32 m_nCells;	// nombre de cellules pour calculer les moyennes
	float m_step;		// progression de la fenetre glissante en s
	float m_currentSamplingRate;
	QString m_MatlabBaseFilePath;	// Used in scripted mode to specify the matlab file.
	QString m_error;	// error string
	QString m_methodName;
	// parameters
	QList<H2_run *> m_runs; // number of H2 runs (number of markers selected to run H2 on)
	AwMarkerList m_markers, m_skippedMarkers;	// markers used to compute H2 and markers skipped because of insufficient data
	QString m_markerTrigger;	// marker to use to trigger H2 computation using pre and post timing.
	float m_preTrigger, m_postTrigger;
	// synchros Threads
	QFuture<void> m_future;
	// progress bar
	float m_scaleProgress;
	// output
	QString m_filePath;
	QString m_markerToSkip;	// among the markers set as input, the name of the one to skip (usually artefact)
	int m_method;			// Algorithm used to compute (H2, R2)
	FreqBand m_currentBand;		// index of frequency band used for compute.
	QStringList m_resultFiles;
};

class H2_EXPORT H2Plugin : public AwProcessPlugin
{
    Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
#endif
    Q_INTERFACES(AwProcessPlugin)
public:
	H2Plugin();
	~H2Plugin();
	AW_INSTANTIATE_PROCESS(H2)
};

// global functions

void ComputeBackground(h2_params *p);

double linvar(float *y, float *x, int nn);
//double rr2(vec x, vec y);
double r2(float *x, float *y, int n);	
double nonlinvar(float *x, float *y, int n, int nbCell, QVector<int>& indexes);
double computeH2(float *x, float *y, int n, int horizon, int nbCell, int max_lag, int step, QVector<int>& delay_array, QVector<double>& h2);
double computeR2(float *x, float *y, int n, int horizon, int nbCell, int max_lag, int step, QVector<int>& delay, QVector<double>& h2);

Q_DECLARE_METATYPE(H2 *)

#endif // H2_H
