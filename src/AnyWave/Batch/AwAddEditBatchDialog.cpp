#include "AwAddEditBatchDialog.h"

AwAddEditBatchDialog::AwAddEditBatchDialog(AwBatchModelItem *item, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_item = item;
}

AwAddEditBatchDialog::~AwAddEditBatchDialog()
{
}
