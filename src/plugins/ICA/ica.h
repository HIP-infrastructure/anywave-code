/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Marmaduke Woodman � Laboratoire UMR INS INSERM 1106 - michael.woodman@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef ICA_H
#define ICA_H
#include <AwProcessInterface.h>
#include "ica_global.h"
#include <armadillo>
using namespace arma;

class ICA_EXPORT ICA : public AwProcess
{
	Q_OBJECT
public:
	ICA();
	~ICA();
	enum Algos { Infomax, FASTICA };
	void run();
	bool showUi();
private:
	void infomax(int m, int n, int nc);
	void runica(double *data, double *weights, int chans, int samples, double *bias, int *signs);
	int runica_matlab(int nc);
	void saveToFile();
	/* Prepare matlab input file for MATLAB ica code. */
	void createInputFile();
	/* Launch matlab plugin to compute ICA. */
	void launchMatlabPlugin();

	int m_modality;
	QString m_ignoredMarkerLabel;
	bool m_ignoreMarkers;
	bool m_ignoreBadChannels;
	int m_nComp;
	int m_decimateFactor;
	int m_algo;
	QString m_fileName;
	float m_lpf, m_hpf, m_samplingRate;
	AwChannelList m_channels;
	QStringList m_algoNames;
	arma::mat m_unmixing;
	arma::mat m_mixing;
};

class ICA_EXPORT ICAPlugin : public AwProcessPlugin
{
    Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
#endif
    Q_INTERFACES(AwProcessPlugin)
public:
    ICAPlugin();
    ICA *newInstance() { return new ICA; }
};


#endif // INFOMAXICA_H
