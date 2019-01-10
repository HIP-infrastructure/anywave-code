#include "AwBIDSValidateDialog.h"

AwBIDSValidateDialog::AwBIDSValidateDialog(const QMap<int, QString>& mods, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.tableView->setModel(new AwBIDSValidateModel(mods, m_ui.tableView));
}

AwBIDSValidateDialog::~AwBIDSValidateDialog()
{
}
