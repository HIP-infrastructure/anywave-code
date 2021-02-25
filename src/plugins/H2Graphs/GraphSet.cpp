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
	if (m_graphWindow) 
		delete m_graphWindow;
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
			double xy = 0., yx = 0.;
			double lagxy = 0., lagyx = 0.;
			// mean X/Y pair through all iterations. keep the maximum value and corresponding lag of a pair.
			for (auto iteration = 0; iteration < nIterations; iteration++) {
				xy += m_corrMatrix(i, j, iteration);
				yx += m_corrMatrix(j, i, iteration);
				lagxy += m_lagMatrix(i, j, iteration);
				lagyx += m_lagMatrix(j, i, iteration);
			}
			m_meanGraphSet->m_corrMatrix(i, j, 0) = xy / nIterations;
			m_meanGraphSet->m_corrMatrix(j, i, 0) = yx / nIterations;
			m_meanGraphSet->m_lagMatrix(i, j, 0) = lagxy / nIterations;
			m_meanGraphSet->m_lagMatrix(j, i, 0) = lagyx / nIterations;

		}
	}
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