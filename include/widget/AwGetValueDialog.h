#pragma once
#include <AwGlobal.h>
#include <QDialog>

namespace Ui { class AwGetValueDialogUi; };

class AW_WIDGETS_EXPORT AwGetValueDialog : public QDialog
{
	Q_OBJECT

public:
	AwGetValueDialog(const QString& title = QString(), QWidget *parent = Q_NULLPTR);
	~AwGetValueDialog();

	QString value();
private:
	Ui::AwGetValueDialogUi *m_ui;
};
