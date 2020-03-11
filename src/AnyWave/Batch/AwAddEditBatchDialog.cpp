#include "AwAddEditBatchDialog.h"
#include "Plugin/AwPluginManager.h"
#include <AwProcessInterface.h>
#include "AwBatchModelItem.h"
#include <utils/json.h>
#include <QDoubleSpinBox>
#include <QComboBox>

AwAddEditBatchDialog::AwAddEditBatchDialog(AwBatchModelItem *item, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_item = item;

	setupParamsUi();
}

AwAddEditBatchDialog::~AwAddEditBatchDialog()
{
}

void AwAddEditBatchDialog::setupParamsUi()
{
	auto gridLayout = new QGridLayout;
	m_ui.groupParameters->setLayout(gridLayout);

	// get the json ui
	QString error;
	auto jsonUi = AwUtilities::json::hashFromJsonString(m_item->plugin()->settings()[processio::json_ui].toString(), error);
	auto jsonDefaults = AwUtilities::json::hashFromJsonString(m_item->plugin()->settings()[processio::json_defaults].toString(), error);

	int row = 0;
	for (auto k : jsonUi.keys()) {
		auto value = jsonUi[k].toString();
		if (value == "string") {
			auto label = new QLabel(QString("%1:").arg(k));
			gridLayout->addWidget(label, row, 0);
			auto edit = new QLineEdit;
			edit->setText(jsonDefaults[k].toString());
			gridLayout->addWidget(edit, row, 1);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (value == "number") {
			auto label = new QLabel(QString("%1:").arg(k));
			gridLayout->addWidget(label, row, 0);
			auto edit = new QDoubleSpinBox;
			edit->setMaximum(std::numeric_limits<double>::max());
			edit->setValue(jsonDefaults[k].toDouble());
			gridLayout->addWidget(edit, row, 1);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (value == "list") {
			auto label = new QLabel(QString("%1:").arg(k));
			gridLayout->addWidget(label, row, 0);
			auto edit = new QComboBox;
			edit->addItems(jsonDefaults[k].toStringList());
			gridLayout->addWidget(edit, row, 1);
			row++;
			m_widgets.insert(k, edit);
		}
	}

}