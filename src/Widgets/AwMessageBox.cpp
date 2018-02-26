#include <widget/AwMessageBox.h>
#include <QIcon>
#define icon_path ":/images/AnyWave_icon.png"
int AwMessageBox::information(QWidget *parent, const QString& title, const QString& text, 
		QMessageBox::StandardButtons buttons,	QMessageBox::StandardButton defaultButton)
{
	QMessageBox box(QMessageBox::NoIcon, title, text, buttons, parent);
	box.setIconPixmap(QPixmap::fromImage(QImage(icon_path)));
	box.setDefaultButton(defaultButton);
	return box.exec();
}
int AwMessageBox::critical(QWidget *parent, const QString& title, const QString& text, 
		QMessageBox::StandardButtons buttons,	QMessageBox::StandardButton defaultButton)
{
	QMessageBox box(QMessageBox::NoIcon, title, text, buttons, parent);
	box.setIconPixmap(QPixmap::fromImage(QImage(icon_path)));
	box.setDefaultButton(defaultButton);
	return box.exec();
}
int AwMessageBox::question(QWidget *parent, const QString& title, const QString& text, 
		QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
	QMessageBox box(QMessageBox::NoIcon, title, text, buttons, parent);
	box.setIconPixmap(QPixmap::fromImage(QImage(icon_path)));
	box.setDefaultButton(defaultButton);
	return box.exec();
}