#pragma once

#include <QStringList>



class AwECoGGrid
{
public:
	AwECoGGrid(int rows, int cols, const QStringList& labels);
	enum Orientation { Horizontal, Vertical };

	inline int rows() { return m_rows;  }
	inline int cols() { return m_cols; }
	inline int orientation() { return m_orientation; }
	inline QStringList& labels() { return m_electrodes; }
	void setOrientation(int orientation);
protected:
	int m_rows, m_cols;
	int m_orientation;
	QStringList m_electrodes;
};

using AwECoGGrids = QList<AwECoGGrid *>;