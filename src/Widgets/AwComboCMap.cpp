#include <widget/AwComboCMap.h>


AwComboCMap::AwComboCMap(QWidget *parent) : QComboBox(parent)
{
	m_names_types = AwColorMap::namesAndTypes();
	QPair<QString, int> pair;
	for (int i = 0; i < m_names_types.size(); i++) {
		pair = m_names_types.at(i);
		addItem(pair.first);
	}
}


void AwComboCMap::initToColorMap(int cmap)
{
	QPair<QString, int> pair;
	for (int i = 0; i < m_names_types.size(); i++) {
		pair = m_names_types.at(i);
		if (pair.second == cmap)
			setCurrentIndex(i);
	}
}

int AwComboCMap::selectedMap()
{
	return m_names_types.at(currentIndex()).second;
}