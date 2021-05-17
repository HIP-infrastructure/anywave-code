#ifndef AWGTCMENU_H
#define AWGTCMENU_H

#include <QMenu>

/** AwGTCMenu: Build a menu with sub menus depending on labels of channels.
Build a letter sub menu grouping all channels starting with the same letter. **/

class AwGTCMenu : public QMenu
{
	Q_OBJECT

public:
	AwGTCMenu(const QString &title, QWidget *parent = 0);
	~AwGTCMenu();

	void updateLabels(const QStringList& labels);
private:
	
};

#endif // AWGTCMENU_H
