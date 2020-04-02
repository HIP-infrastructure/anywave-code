#include "AwBIDSGUIOptionsDialog.h"
#include "AwBIDSManager.h"
#include "Prefs/AwSettings.h"
#include <utils/AwUtilities.h>
#include <utils/json.h>

constexpr int max_cols_used = 3;

// we will use here a bids.json file with the following structure:
// encodedBIDSdir : { 
// participants_cols_used : [ "col1" , "col2"]
//		   }

AwBIDSGUIOptionsDialog::AwBIDSGUIOptionsDialog(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	auto bidsManager = AwBIDSManager::instance();
	auto settingsDir = AwSettings::getInstance()->value(aws::settings_dir).toString();
	QString jsonPath = QString("%1/bids.json").arg(settingsDir);
	// do nothing if the file does not exist
	if (QFile::exists(jsonPath)) {
		// check for bids.json file in Settings
		auto dict = AwUtilities::json::fromJsonFileToHash(jsonPath);
		// base encore the current bids dir
		auto bidsDir = bidsManager->rootDir();
		auto encodedDir = AwUtilities::base64Encode(bidsDir);

		if (!dict.isEmpty() && dict.contains(encodedDir)) {

		}
	}
	// init cols layout with the first max_cols_used columns of participants
	auto cols = bidsManager->settings().value(bids::participant_cols).toStringList();

	int index = 0;
	int row = 0, col = 0;
	for (auto c : cols) {
		if (index == max_cols_used)
			break;
		auto widget = new QCheckBox(c);
		widget->setProperty("column", c);
		if (col == 4) {
			col = 0;
			row++;
		}
		m_ui.columnsLayout->addWidget(widget, row, col++, 1, 1);
		connect(widget, &QCheckBox::toggle, this, &AwBIDSGUIOptionsDialog::updateSubPreview);
		m_columnsWigets << widget;
		index++;
	}
	// add a clear all button
	row++;
	auto widget = new QPushButton("Reset");
	connect(widget, &QPushButton::clicked, this, &AwBIDSGUIOptionsDialog::resetSubPreview);
	m_ui.columnsLayout->addWidget(widget, row, max_cols_used, 1, 1);
}

AwBIDSGUIOptionsDialog::~AwBIDSGUIOptionsDialog()
{

}


void AwBIDSGUIOptionsDialog::updateSubPreview()
{
	QString res = "sub-ID";
	QString extras;
	for (auto widget : m_columnsWigets) {
		if (widget->isChecked())
			extras = QString("%1|%2").arg(extras).arg(widget->property("column").toString());
	}
	if (!extras.isEmpty())
		res = QString("%1(%2)").arg(res).arg(extras);
	m_ui.editPreview->setText(res);
}

void AwBIDSGUIOptionsDialog::resetSubPreview()
{
	m_ui.editPreview->setText("sub-ID");
}