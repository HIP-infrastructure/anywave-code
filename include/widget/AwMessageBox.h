#ifndef AW_MESSAGEBOX_H
#define AW_MESSAGEBOX_H
#include <QMessageBox>
#include <AwGlobal.h>

class  AW_WIDGETS_EXPORT AwMessageBox {
public:
	static int information(QWidget *parent, const QString& title, const QString& text,
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,	QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	static int critical(QWidget *parent, const QString& title, const QString& text,
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,	QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	static int question(QWidget *parent, const QString& title, const QString& text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Yes|QMessageBox::No, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
};

#endif