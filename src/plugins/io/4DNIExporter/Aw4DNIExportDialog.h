#pragma once

#include <QDialog>
#include "ui_DialogButtonBottom.h"

class Aw4DNIExportDialog : public QDialog
{
	Q_OBJECT

public:
	Aw4DNIExportDialog(QWidget *parent = nullptr);
	~Aw4DNIExportDialog();

	QString selectedModification;
public slots:
	void accept() override;
private:
	Ui::Aw4DNIExporDialogUi m_ui;
};
