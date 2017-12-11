#include "AwCentralWidget.h"
#include <qevent.h>

AwCentralWidget::AwCentralWidget(QWidget *parent)
	: QSplitter(parent)
{
	setAcceptDrops(true);
}

AwCentralWidget::~AwCentralWidget()
{

}

void AwCentralWidget::dragEnterEvent(QDragEnterEvent *e)
{
	e->acceptProposedAction();
}

void AwCentralWidget::dropEvent(QDropEvent *e)
{
	e->acceptProposedAction();
}

