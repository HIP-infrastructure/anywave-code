#include "AwRefreshDialog.h"

AwRefreshDialog::AwRefreshDialog(const AwBIDSItems& subjects, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_items = subjects;
}

AwRefreshDialog::~AwRefreshDialog()
{
}

int AwRefreshDialog::exec()
{
	QStringList labels;
	for (auto item : m_items)
		labels << item->subjectName();

	m_ui.listWidget->insertItems(0, labels);
	return QDialog::exec();
}

void AwRefreshDialog::accept()
{
	for (const auto& item : m_ui.listWidget->selectedItems())
		subjects << m_items.at(m_ui.listWidget->row(item));
	if (subjects.size())
		QDialog::accept();
}