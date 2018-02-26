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
#include "Average.h"
#include <math/AwMath.h>
#include "AverageUi.h"


AveragePlugin::AveragePlugin()
{
	name = QString(tr("Average"));
	description = QString(tr("Average signals around markers."));
	type = AwProcessPlugin::Background;
}

Average::Average()
{
	pdi.addInputParameter(Aw::GetAllMarkers, QString("0-n"));
	pdi.addInputParameter(Aw::GetCurrentMontage|Aw::ProcessIgnoresChannelSelection, QString("0-n"));
	setFlags(Aw::ProcessHasInputUi|Aw::ProcessHasOutputUi);
	m_markerPosition = 0.;
}


bool Average::showUi()
{
	m_inputUi.setFlags(AwInputMarkerWidget::UniqueMarkerLabel | AwInputMarkerWidget::ShowLabel | AwInputMarkerWidget::ShowValue | AwInputMarkerWidget::ShowDuration);
	m_inputUi.setMarkers(pdi.input.markers);
	if (m_inputUi.exec() == QDialog::Accepted) {
		m_selectedMarkers = m_inputUi.markers();
		return true;
	}
	return false;
}

void Average::prepareOutputUi()
{
	AwMarker *marker = new AwMarker();
	marker->setLabel(m_markerLabel);
	marker->setStart(m_markerPosition);
	AverageUi *ui = new AverageUi(m_averageMatrix, pdi.input.channels, marker);
	
	pdi.output.widgets << ui;
}

void Average::run()
{
	if (m_selectedMarkers.isEmpty()) {
		sendMessage("No marker set as input. Done.");
		return;
	}
	AwMarkerList temp;
	AwMarker *sm = m_selectedMarkers.first();
	m_markerPosition = m_inputUi.preLatency();
	m_markerLabel = sm->label();
	foreach(AwMarker *m, pdi.input.markers) {
		if (m->label() == sm->label()) {
			AwMarker *n = new AwMarker(m);
			if (n->duration() == 0.) {
				n->setStart(n->start() - m_inputUi.preLatency());
				if (n->start() < 0.)
					n->setStart(0.0);
				n->setDuration(m_inputUi.postLatency() + m_inputUi.preLatency());
			}
			temp << n;
		}
	}
	arma::uword cols = (arma::uword)floor(temp.first()->duration() * pdi.input.channels.first()->samplingRate());

	int epoch = 0;
	mat z(pdi.input.channels.size(), cols);
	z.zeros();

	m_averageMatrix = conv_to<fmat>::from(z);

	foreach(AwMarker *m, temp) {
		sendMessage(QString("Getting data for epoch %1").arg(epoch + 1));
		requestData(&pdi.input.channels, m->start(), m->duration());
		m_averageMatrix += AwMath::channelsToFMat(pdi.input.channels);
		epoch++;
	}
	m_averageMatrix /= epoch;
	sendMessage(QString("Averaging is done."));
}
