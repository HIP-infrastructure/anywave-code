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
#include "correlation.h"
#include <math/AwMath.h>
#include "outputwidget.h"
#include "maingui.h"

CorrelationPlugin::CorrelationPlugin()
{
	type = AwProcessPlugin::Background;
	name = "Channels Correlation";
	description = tr("Compute the correlation matrix between channels.");
	category = "Process:Correlation:Channels Correlation";
	setFlags(Aw::ProcessFlags::PluginAcceptsTimeSelections);
}

Correlation::Correlation()
{
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
	pdi.addInputParameter(Aw::ProcessInput::AnyChannels, "2-n");
	pdi.addInputParameter(Aw::ProcessInput::GetAllMarkers, "1-n");
	m_ui = NULL;
}

Correlation::~Correlation()
{
	delete m_ui;
	while (!m_results.isEmpty())
		delete m_results.takeFirst();
}

corr_result *Correlation::computeCorrelation(float start, float duration)
{
	emit progressChanged(tr("Computing correlation..."));
	corr_result *res = new corr_result;
	if (m_ui->computeAllvsAll) {
		mat chan1 = AwMath::channelsToMat(pdi.input.channels);
		emit progressChanged(tr("Done."));
		res->corr = cor(chan1.t());
		res->xLabels = res->yLabels = AwChannel::getLabels(pdi.input.channels);
	}
	else {
		// 
		mat chan1 = AwMath::channelsToMat(m_ui->channels1).t();
		mat chan2 = AwMath::channelsToMat(m_ui->channels2).t();
		// compute the correlation for each line
		res->corr = mat(chan1.n_cols, chan2.n_cols);
		// check if matrices have the same number of lines
		int numRows = 0;
		if( chan1.n_rows != chan2.n_rows) {  // take the lowest dimension and set numRows value.
			numRows = qMin(chan1.n_rows, chan2.n_rows);
		}
		for (int i = 0; i < chan1.n_cols; i++) {
			for (int j = 0; j < chan2.n_cols; j++) {
				if (numRows == 0) {
					colvec r = cor(chan1.col(i), chan2.col(j));	
					res->corr(i,j) = r(0);
				}
				else {
					colvec col1 = colvec(numRows);
					colvec col2 = colvec(numRows);
					colvec c1 = chan1.col(i);
					colvec c2 = chan2.col(j);
					memcpy(col1.memptr(), c1.memptr(), numRows * sizeof(double));
					memcpy(col2.memptr(), c2.memptr(), numRows * sizeof(double));
					colvec r = cor(col1, col2);
					res->corr(i,j) = r(0);
				}
			}
		}
		emit progressChanged(tr("Done."));
		res->xLabels = AwChannel::getLabels(m_ui->channels2);
		res->yLabels = AwChannel::getLabels(m_ui->channels1);
	}
	res->position = start;
	res->duration = duration;
	return res;
}

void Correlation::prepareOutputUi()
{
	if (m_results.size() > 2)
		pdi.output.widgets << new maingui(m_results);

	if (m_results.size() < 3) {
		for (auto r : m_results) {
			pdi.output.widgets << new OutputWidget(r);
		}
	}
}

void Correlation::run()
{
	if (m_ui->marker.isEmpty()) {
		emit progressChanged(tr("Computing using all the data..."));
		emit progressChanged(tr("Loading data..."));
		requestData(&pdi.input.channels, (float)0, (float)-1);
		emit progressChanged(tr("OK."));
		m_results << computeCorrelation(0, pdi.input.reader()->infos.totalDuration());
	}
	else {
		foreach (AwMarker *m, pdi.input.markers) {
			if (m->label() == m_ui->marker) {
				emit progressChanged(tr("Loading data..."));
				requestData(&pdi.input.channels, m->start(), m->duration());
				emit progressChanged(tr("OK."));
				m_results << computeCorrelation(m->start(), m->duration());
			}
		}
	}
	if (!m_results.isEmpty()) {
		setFlags(flags() | Aw::ProcessFlags::ProcessHasOutputUi);
	}
}

bool Correlation::showUi()
{
	if (m_ui == NULL)
		m_ui = new settings(pdi.input.channels, pdi.input.markers);
	if (m_ui->exec() == QDialog::Accepted) 
		return true;
	return false;
}
