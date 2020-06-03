#pragma once
#include <QStringList>
#include <QVector>
//#include <armadillo>
//using namespace arma;
#include <aw_armadillo.h>
class GraphWindow;

class GraphSet
{
public:
	// construct using a matlab file.
	GraphSet();
	~GraphSet();
	// load a matlab file containing results.
	int load(const QString& file);
	// computation settings used.
	float timeWindow;
	float maxLag;
	float step;
	QString band;	// frequency band name
	float hp, lp;
	QString method;	// algorithm used
	QStringList labels;	// channel labels
	float samplingRate; // Sampling rate of channels. Used to convert lag values in seconds.

	inline int sectionsCount() { return m_nSections; }
	inline QString& errorString() { return m_error; }
	inline QVector<double>& positions() { return m_timePositions; }
	inline QVector<int>& sections() { return m_sections; }
	/* Get the number of iteration in a section */
	int sectionIterations(int section);
	/* Get the total number of iterations through all sections */
	int totalIterations();
	/* Get the correlation tensor */
	cube& correlationMatrix() { return m_corrMatrix; }
	/* Get the lag tensor */
	cube& lagMatrix() { return m_lagMatrix; }
	/* Visualize the graph */
	void visualize();
	void newMeanGraphSet();
	inline bool isMean() { return m_isMean; }
protected:
	cube m_corrMatrix;
	cube m_lagMatrix;
	int m_nSections;	// total number of sections in the set.
	QVector<int> m_iterations; // contains the number of iterations per sections.
	QVector<int> m_sections;	// the vector of section number for each iterations.
	QVector<double> m_timePositions;	// contains the position in second for each iteration.
	QString m_error;
	GraphWindow *m_graphWindow;	
	GraphSet *m_meanGraphSet;
	bool m_isMean;	// true if the graph set is a mean graph set.
	bool m_isEmpty;	// true until load is called and successful.
};