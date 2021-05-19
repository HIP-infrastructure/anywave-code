#include "AwGTCMenu.h"

AwGTCMenu::AwGTCMenu(const QString& title, QWidget *parent)
	: QMenu(title, parent)
{

}

AwGTCMenu::~AwGTCMenu()
{

}


void AwGTCMenu::updateLabels(const QStringList& labels)
{
	clear();
	QStringList sorted = labels;
	sorted.sort();
	QMenu *m_subMenu = NULL;
	QChar currentLetter = ' ';
	foreach (QString s, sorted) {
		if (s.isEmpty()) // skip any empty strings (should not happen in most cases...)
			continue;
		if (s.at(0) != currentLetter) {
			currentLetter = s.at(0);
			m_subMenu = addMenu(QString(currentLetter));
		}
		m_subMenu->addAction(s);
	}
}