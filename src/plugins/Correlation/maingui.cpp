#include "maingui.h"
#include <qfiledialog.h>
#include <AwMATLAB.h>
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