#ifndef AW_WAIT_H
#define AW_WAIT_H
#include <qapplication.h>

class AwWait
{
public:
	AwWait() { QApplication::setOverrideCursor(QCursor(Qt::WaitCursor)); }
	~AwWait() { QApplication::restoreOverrideCursor(); }
};
#endif