#include "AwBIDSGUIOptionsDialog.h"
#include "AwBIDSManager.h"

constexpr int max_cols_used = 3;

AwBIDSGUIOptionsDialog::AwBIDSGUIOptionsDialog(const QStringList& extraColumns, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	auto bidsManager = AwBIDSManager::instance();
	//auto settingsDir = AwSettings::getInstance()->value(aws::settings_dir).toString();
	//QString jsonPath = QString("%1/bids.json").arg(settingsDir);
	//// do nothing if the file does not exist
	//if (QFile::exists(jsonPath)) {
	//	// check for bids.json file in Settings
	//	auto dict = AwUtilities::json::fromJsonFileToHash(jsonPath);
	//	// base encore the current bids dir
	//	auto bidsDir = bidsManager->rootDir();
	//	auto encodedDir = AwUtilities::base64Encode(bidsDir);

	//	if (!dict.isEmpty() && dict.contains(encodedDir)) {

	//	}
	//}
	// init cols layout with the first max_cols_used columns of participants
	auto cols = bidsManager->settings().value(bids::participant_cols).toStringList();
	// remove first col (participant id)
	cols.takeFirst();
	m_selectecColumns = extraColumns;
	int index = 0;
	int row = 0, col = 0;
	for (auto c : cols) {
		if (index == max_cols_used)
			break;
		auto widget = new QCheckBox(c);
		widget->setChecked(m_selectecColumns.contains(c));
		widget->setProperty("column", c);
		if (col == 4) {
			col = 0;
			row++;
		}
		m_ui.columnsLayout->addWidget(widget, row, col++, 1, 1);
		m_columnsWigets << widget;
		index++;
	}
	// add a clear all button
	row++;
	auto widget = new QPushButton("Reset");
	connect(widget, &QPushButton::clicked, this, &AwBIDSGUIOptionsDialog::reset);
	m_ui.columnsLayout->addWidget(widget, row, max_cols_used, 1, 1);
}

AwBIDSGUIOptionsDialog::~AwBIDSGUIOptionsDialog()
{

}


void AwBIDSGUIOptionsDialog::accept()
{
	m_selectecColumns.clear();
	for (auto w : m_columnsWigets) {
		if (w->isChecked())
			m_selectecColumns.append(w->property("column").toString());
	}
	return QDialog::accept();
}



void AwBIDSGUIOptionsDialog::reset()
{
	for (auto w : m_columnsWigets) 
		w->setChecked(false);
}