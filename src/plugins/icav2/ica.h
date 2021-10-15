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
#ifndef ICA_H
#define ICA_H
#include <AwProcessInterface.h>
#include "ica_global.h"
#include <aw_armadillo.h>
#include "ICAAlgorithm.h"

class ICA_EXPORT ICA : public AwProcess
{
	Q_OBJECT
public:
	ICA();
	~ICA();
	enum Algos { Infomax, FASTICA, acsobiro };
	void run() override;
	void runFromCommandLine() override;
	bool showUi();

	bool batchParameterCheck(const QVariantMap& args) override;
	QList<QSharedPointer<ICAAlgorithm>>& algorithms() { return m_algorithms; }
private:
	int initParameters();
	void infomax(int m, int n, int nc);
	void run_cca(int m, int n);
	void run_sobi(int m, int n, int nc);
	void runica(double *data, double *weights, int chans, int samples, double *bias, int *signs);
	int runica_matlab(int nc);
	int run_acsobiro(int nc);
	void saveToFile();
    int m_modality;
	QString m_ignoredMarkerLabel;
	bool m_ignoreMarkers;
	bool m_ignoreBadChannels;
	int m_nComp;
	bool m_isDownsamplingActive;
	int m_algo;
	QString m_fileName;
	qint64 m, n;
	float m_lpf, m_hpf, m_samplingRate;
	AwChannelList m_channels;
	//QStringList m_algoNames;
	arma::mat m_unmixing;
	arma::mat m_mixing;

	QList<QSharedPointer<ICAAlgorithm>> m_algorithms;
};

class ICA_EXPORT ICAPlugin : public AwProcessPlugin
{
    Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
    Q_INTERFACES(AwProcessPlugin)
public:
    ICAPlugin();
	AW_INSTANTIATE_PROCESS(ICA)
};


#endif // INFOMAXICA_H
