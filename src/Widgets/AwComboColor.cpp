#include <widget/AwComboColor.h>


AwComboColor::AwComboColor(QWidget *parent) : QComboBox(parent)
{
	m_names = QColor::colorNames();
	
	for (int i = 0; i < m_names.size(); i++)	{
		insertItem(i, m_names.at(i), i);
		setItemData(i, QColor(m_names.at(i)), Qt::DecorationRole);
		m_codes << QColor(m_names.at(i)).name();
	}
}


void AwComboColor::initToColor(const QString& color)
{
	int index = m_names.indexOf(color);
	if (index == -1)
		index = m_codes.indexOf(color);
	if (index == -1)
		return;
	setCurrentIndex(index);
}

QString AwComboColor::selectedColor()
{
	return m_names.at(currentIndex());
}