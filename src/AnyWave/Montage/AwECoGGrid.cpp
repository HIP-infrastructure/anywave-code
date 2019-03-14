#include "AwECoGGrid.h"

AwECoGGrid::AwECoGGrid(int rows, int cols, const QStringList& labels)
{
	m_rows = rows;
	m_cols = cols;
	m_orientation = AwECoGGrid::Horizontal;
	m_electrodes = labels;
}

void AwECoGGrid::setOrientation(int o)
{
	if (o == AwECoGGrid::Vertical || o == AwECoGGrid::Horizontal)
		m_orientation = o;
}