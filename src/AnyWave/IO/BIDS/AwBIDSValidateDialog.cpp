#include "AwBIDSValidateDialog.h"

AwBIDSValidateDialog::AwBIDSValidateDialog(const QMap<int, QString>& mods, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	QStringList labels = { "item", "Apply Modification" };
	m_ui.tableWidget->setHorizontalHeaderLabels(labels);
}

AwBIDSValidateDialog::~AwBIDSValidateDialog()
{
}
