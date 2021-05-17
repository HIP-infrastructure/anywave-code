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
#include "maingui.h"
#include <qfiledialog.h>
#include <matlab/AwMATLAB.h>
#include <widget/AwMessageBox.h>


maingui::maingui(const QList<corr_result *>& results, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_results = results;
	computeMean();
	m_outputMeanWidget = new OutputWidget(&m_mean);
	m_outputMeanWidget->setWindowTitle("Mean Correlation");
	m_outputMeanWidget->show();
}

maingui::~maingui()
{
	delete m_outputMeanWidget;
}


void maingui::computeMean() 
{
	// do the mean
	arma::uword row = m_results.first()->corr.n_rows;
	arma::uword col = m_results.first()->corr.n_cols;
	mat avg(row, col);
	avg.zeros();
	for (auto c : m_results)
		avg += c->corr;
	avg /= m_results.size();
	m_mean.corr = avg;
	m_mean.xLabels = m_results.first()->xLabels;
	m_mean.yLabels = m_results.first()->yLabels;

}

void maingui::saveToMatlab()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save to .mat", "/", "Matlab file (*.mat)");
	if (fileName.isEmpty())
		return;

	AwMATLABFile file;
	try {

		file.create(fileName);
		file.writeStringCellArray("xLabels", m_mean.xLabels);
		file.writeStringCellArray("yLabels", m_mean.yLabels);
		file.writeMatrix("data", m_mean.corr);
	}
	catch (const AwException& e) {
		AwMessageBox::critical(this, "File Operation", e.errorString());
		return;
	}
	AwMessageBox::information(this, "File Operation", "File saved successfully.");
}