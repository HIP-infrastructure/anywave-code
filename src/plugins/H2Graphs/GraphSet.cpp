#include "GraphSet.h"
#include <matlab/AwMATLAB.h>
#include "GraphWindow.h"

GraphSet::GraphSet()
{
	m_meanGraphSet = nullptr;
	m_graphWindow = nullptr;
	m_isEmpty = true;
	m_isMean = false;
}

GraphSet::~GraphSet()
{
	if (m_graphWindow) {
		m_graphWindow->close();
		delete m_graphWindow;
	}
	if (m_meanGraphSet)
		delete m_meanGraphSet;
}

int GraphSet::load(const QString& fileName)
{
	AwMATLABFile file;
	QVector<double> filters;
	try {
		file.open(fileName);
		file.readScalar("aw_windowSize", &timeWindow);
		file.readScalar("aw_step", &step);
		file.readScalar("aw_maxLag", &maxLag);
		file.readStrings("electrode_names", labels);
		file.readMatrix("aw_h2", m_corrMatrix);
		file.readMatrix("aw_lag", m_lagMatrix);
		file.readVec("section", m_sections);
		file.readScalar("nb_section", &m_nSections);
		file.readVec("section_iterations", m_iterations);
		file.readVec("time_s", m_timePositions);
		file.readString("band", band);
		file.readString("method", method);
		file.readVec("filters", filters);
		file.readScalar("samplingRate", &samplingRate);
	}
	catch (const AwException& e) {
		m_error = QString("Error loading .mat file: %1").arg(e.errorString());
		return -1;
	}
	m_isEmpty = false;
	lp = (float)filters[1];
	hp = (float)filters[0];
	return 0;
}

void GraphSet::newMeanGraphSet()
{
	if (m_meanGraphSet) {
		m_meanGraphSet->visualize();
		return;
	}
	
	m_meanGraphSet = new GraphSet();
	m_meanGraphSet->m_isMean = true;
	m_meanGraphSet->m_corrMatrix = arma::cube(m_corrMatrix.n_rows, m_corrMatrix.n_cols, 1);
	m_meanGraphSet->m_lagMatrix = arma::cube(m_lagMatrix.n_rows, m_lagMatrix.n_cols, 1);
	m_meanGraphSet->m_corrMatrix.fill(0.);
	m_meanGraphSet->m_lagMatrix.fill(0.);

	auto nIterations = totalIterations();
	for (uword i = 0; i < m_corrMatrix.n_rows; i++) {  // X
		for (uword j = i + 1; j < m_corrMatrix.n_cols; j++) { // Y
			double mean_value = 0.;
			double mean_lag = 0.;
			// mean X/Y pair through all iterations. keep the maximum value and corresponding lag of a pair.
			for (auto iteration = 0; iteration < nIterations; iteration++) {
				double xy_value = m_corrMatrix(i, j, iteration);
				double yx_value = m_corrMatrix(j, i, iteration);
				double xy_lag = m_lagMatrix(i, j, iteration);
				double yx_lag = m_lagMatrix(j, i, iteration);
				double lag = xy_value >= yx_value ? xy_lag : yx_lag;
				mean_value += std::max(xy_value, yx_value);
				mean_lag += lag;
			}
			m_meanGraphSet->m_corrMatrix(i, j, 0) = mean_value / nIterations;
			m_meanGraphSet->m_lagMatrix(i, j, 0) = mean_lag / nIterations;
		}
	}
	//m_meanGraphSet->setCorrelationMatrix(matrix);
	//m_meanGraphSet->setLagMatrix(lagMatrix);
	m_meanGraphSet->band = band;
	m_meanGraphSet->labels = labels;
	m_meanGraphSet->maxLag = maxLag;
	m_meanGraphSet->m_nSections = 1;
	m_meanGraphSet->hp = hp;
	m_meanGraphSet->lp = lp;
	QVector<int> sections = { 0 };
	QVector<int> iterations = { 1 };
	m_meanGraphSet->m_iterations = iterations;
	m_meanGraphSet->m_sections = sections;
	QVector<double> timePositions = { 0. };
	m_meanGraphSet->m_timePositions = timePositions;
	m_meanGraphSet->m_isEmpty = false;
	m_meanGraphSet->samplingRate = this->samplingRate;

	m_meanGraphSet->visualize();
	return;
}


void GraphSet::visualize()
{
	if (m_isEmpty)
		return;
	if (m_graphWindow == Q_NULLPTR) 
		m_graphWindow = new GraphWindow(this);
	m_graphWindow->show();
}

///
/// Get the number of iterations for a section.
/// Returns 0 if section index is out of bounds.
int GraphSet::sectionIterations(int section) 
{
	if (section < 0 || section > m_nSections)
		return 0;
	return m_iterations.at(section);
}


int GraphSet::totalIterations()
{
	int count = 0;
	for (auto i : m_iterations)
		count += i;
	return count;
}