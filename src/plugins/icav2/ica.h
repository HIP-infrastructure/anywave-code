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
#ifdef Q_OS_MAC
#define ARMA_USE_BLAS
#define ARMA_DONT_USE_WRAPPER
#define ARMA_USE_LAPACK
#include <armadillo>
#else
#include <aw_armadillo.h>
#endif
class ICAAlgorithm;

namespace keys {
	constexpr auto comp = "comp";
	constexpr auto use_seeg_electrode = "use_seeg_electrode";
	constexpr auto modality = "modality";
	constexpr auto downsampling = "downsampling";
	constexpr auto algorithm = "algorithm";
	constexpr auto save_comp_traces = "save_comp_traces";
}

class ICA_EXPORT ICA : public AwProcess
{
	Q_OBJECT
public:
	ICA();
	~ICA();

	void run() override;
	void runFromCommandLine() override;
	bool showUi() override;
	void init() override;

	bool batchParameterCheck(const QVariantMap& args) override;
	QList<ICAAlgorithm*>& algorithms() { return m_algorithms; }
	inline AwChannelList& rawChannels() { return m_rawChannels; }
private:
	int initParameters();
	void saveToFile();
	void exportComponents();

	void run_cca(int m, int n);
    int m_modality;
	QString m_ignoredMarkerLabel;
	bool m_ignoreMarkers;
	bool m_ignoreBadChannels;
	int m_nComp;
	bool m_isDownsamplingActive;
	bool m_SEEGElectrodeMode;
	QString m_SEEGElectrode;
	QString m_fileName, m_componentsEEGFileName;
	qint64 m, n;
	float m_lpf, m_hpf, m_samplingRate, m_notch;
	// the default input will be current montage channels but we also manage raw channels which were the default input before we added the possibility to compute on current montage..
	AwChannelList m_channels, m_rawChannels; // now we handle also the computation on current montage
	arma::mat m_unmixing;
	arma::mat m_mixing;

	QList<ICAAlgorithm *> m_algorithms;
	ICAAlgorithm * m_selectedAlgo;
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
