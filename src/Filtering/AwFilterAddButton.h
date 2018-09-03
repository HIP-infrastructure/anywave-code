#pragma once

#include <qpushbutton.h>
#include <qmenu.h>
class AwFilterAddButton : public QPushButton
{
	Q_OBJECT
public:
	AwFilterAddButton(QWidget *parent = Q_NULLPTR);
protected slots:
	void addNewFilter();
signals:
	void addFilterClicked(int type);
protected:
	QMenu *m_menu;
};