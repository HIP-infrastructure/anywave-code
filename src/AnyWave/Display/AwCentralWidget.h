#ifndef AWCENTRALWIDGET_H
#define AWCENTRALWIDGET_H

#include <QSplitter>

class AwCentralWidget : public QSplitter
{
	Q_OBJECT

public:
	AwCentralWidget(QWidget *parent);
	~AwCentralWidget();
protected:
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);
private:
	
};

#endif // AWCENTRALWIDGET_H
