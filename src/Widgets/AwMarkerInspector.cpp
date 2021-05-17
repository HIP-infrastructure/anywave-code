#include <widget/AwMarkerInspector.h>
#include "AwAddPredefinedMarker.h"
#include "AwAddTargetChannels.h"
#include <QMessageBox>
#include "ui_AwMarkerInspector.h"

AwMarkerInspector::AwMarkerInspector(const AwMarkerList& markers, const QStringList& targets, QWidget *parent)
	: QWidget(parent)
{
	// if settings is NULL than instantiate new settings
	m_ui = new Ui::AwMarkerInspectorUI();
	m_ui->setupUi(this);

	// by default, the inspector suggest to add single marker:
	// so, hide the Predefined markers groupbox
	m_ui->groupBoxPredefined->hide();
	// by default markers are globals so hide targeted channels list
	m_ui->listWidgetTargets->hide();
	m_ui->buttonAddChannels->hide();
	m_ui->buttonClearTargets->hide();
	m_onlySingle = false;
	m_ui->lineEditName->setText(tr("New Marker"));

	// predefined markers
	QTableWidgetItem *itemName = new QTableWidgetItem(tr("Label"));
	QTableWidgetItem *itemValue = new QTableWidgetItem(tr("Value"));
	QTableWidgetItem *itemColor = new QTableWidgetItem(tr("Color"));
	m_ui->table->setHorizontalHeaderItem(0, itemName);
	m_ui->table->setHorizontalHeaderItem(1, itemValue);
	m_ui->table->setHorizontalHeaderItem(2, itemColor);

	connect(m_ui->radioSingle, SIGNAL(clicked(bool)), this, SLOT(changeMarkerType(bool)));
	connect(m_ui->radioSelection, SIGNAL(clicked(bool)), this, SLOT(changeMarkerType(bool)));
	connect(m_ui->lineEditName, SIGNAL(textEdited(const QString&)), this, SLOT(changeLabel(const QString&)));
	connect(m_ui->spinValue, SIGNAL(valueChanged(double)), this, SLOT(changeValue(double)));
	connect(m_ui->checkAutoInc, SIGNAL(toggled(bool)), this, SLOT(changeAutoInc(bool)));	

	// init combo color
	// default color at first index
	m_ui->comboBoxColor->insertItem(0, "Default", 0);
	for (int i = 0; i < QColor::colorNames().size(); i++)	{
		m_ui->comboBoxColor->insertItem(i + 1, QColor::colorNames().at(i), i);
		m_ui->comboBoxColor->setItemData(i + 1, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
	}
	connect(m_ui->comboBoxColor, SIGNAL(currentIndexChanged(int)), this, SLOT(changeColor()));
	setMarkers(markers);
	setTargets(targets);
}

AwMarkerInspector::~AwMarkerInspector()
{
	delete m_ui;
}

void AwMarkerInspector::setTargets(const QStringList& targets)
{
	m_targets = targets;
	m_ui->radioSpecific->setEnabled(!m_targets.isEmpty());
}

void AwMarkerInspector::setTargets(const AwChannelList& channels)
{
	m_targets.clear();
	for (auto c : channels) {
		QString target = c->fullName();

		if (!m_targets.contains(target))
			m_targets << target;
	}
	m_ui->radioSpecific->setEnabled(!m_targets.isEmpty());
}

void AwMarkerInspector::setMarkers(const AwMarkerList& markers)
{
	m_markers = markers;
}

void AwMarkerInspector::setSingleOnly(bool flag)
{
	m_onlySingle = flag;
	if (flag) 	{
		m_ui->radioSingle->setChecked(true);
		m_ui->radioSelection->setEnabled(false);
	}
	else
		m_ui->radioSelection->setEnabled(true);
}


void AwMarkerInspector::changeColor()
{
	int index = m_ui->comboBoxColor->currentIndex();
	if (index == 0) // Default color
		m_settings.color = QString();
	else 
		m_settings.color = QColor(QColor::colorNames().at(index - 1)).name(QColor::HexRgb);
}


void AwMarkerInspector::changeMarkerType(bool flag)
{
	QRadioButton *button = (QRadioButton *)sender();
	if (button == NULL)
		return;

	if (button == m_ui->radioSingle && flag == true)
		m_settings.type = AwMarker::Single;
	if (button == m_ui->radioSelection && flag == true)
		m_settings.type = AwMarker::Selection;
	emit settingsChanged(&m_settings);
}

void AwMarkerInspector::changeUseShortcut(bool flag)
{
	m_settings.usingSpacebar = flag;
}


void AwMarkerInspector::changeTargetChannels(bool flag)
{
	QRadioButton *button = (QRadioButton *)sender();
	if (button == NULL)
		return;

	if (button == m_ui->radioGlobal && flag == true)
		m_settings.isTargettingChannels = false;
	if (button == m_ui->radioSpecific && flag == true)
		m_settings.isTargettingChannels = true;
	if (button == m_ui->radioAutoDetect)
		m_settings.autoTargetChannel = flag;
}


void AwMarkerInspector::changeLabel(const QString& text)
{
	m_settings.label = text;
}

void AwMarkerInspector::changeValue(double value)
{
	m_settings.value = (float)value;
}

void AwMarkerInspector::changeAutoInc(bool flag)
{
	m_settings.isAutoInc = flag;
}

void AwMarkerInspector::changeUsePredefined(bool flag)
{
	m_settings.isUsingList = flag;
}

void AwMarkerInspector::setPredefinedMarkers(const AwMarkerList& markers)
{
	m_ui->table->clearContents();
	m_ui->table->setRowCount(0);
	while (!m_settings.predefinedMarkers.isEmpty())
		delete m_settings.predefinedMarkers.takeFirst();
	for (auto m : markers) {
		m_settings.predefinedMarkers.append(m);
		QTableWidgetItem *itemName = new QTableWidgetItem(m->label());
		QTableWidgetItem *itemValue = new QTableWidgetItem(QString::number(m->value()));
		QTableWidgetItem *itemColor;
		if (m->color().isEmpty())
			itemColor = new QTableWidgetItem("Default");
		else {
			itemColor = new QTableWidgetItem(m->color());
			itemColor->setData(Qt::DecorationRole, m->color());
		}
		m_ui->table->insertRow(m_ui->table->rowCount());
		m_ui->table->setItem(m_ui->table->rowCount() - 1, 0, itemName);
		m_ui->table->setItem(m_ui->table->rowCount() - 1, 1, itemValue);
		m_ui->table->setItem(m_ui->table->rowCount() - 1, 2, itemColor);
	}
//	m_ui->checkPredefined->setChecked(true);   // Do not activate the predefined markers automatically
}

AwMarkerList& AwMarkerInspector::predefinedMarkers()
{
	return m_settings.predefinedMarkers;
}

void AwMarkerInspector::addPredefinedMarker()
{
	AwAddPredefinedMarker dlg(this);
	if (dlg.exec() == QDialog::Accepted) {
		AwMarker *m = dlg.marker();
		m_settings.predefinedMarkers.append(m);
		QTableWidgetItem *itemName = new QTableWidgetItem(m->label());
		QTableWidgetItem *itemValue = new QTableWidgetItem(QString::number(m->value()));
		QTableWidgetItem *itemColor;
		if (m->color().isEmpty())
			itemColor = new QTableWidgetItem("Default");
		else {
			itemColor = new QTableWidgetItem(m->color());
			itemColor->setData(Qt::DecorationRole, m->color());
		}
		m_ui->table->insertRow(m_ui->table->rowCount());
		m_ui->table->setItem(m_ui->table->rowCount() - 1, 0, itemName);
		m_ui->table->setItem(m_ui->table->rowCount() - 1, 1, itemValue);
		m_ui->table->setItem(m_ui->table->rowCount() - 1, 2, itemColor);
	}
	emit predefinedMarkersChanged(m_settings.predefinedMarkers);
}

void AwMarkerInspector::clearPredefinedMarkers()
{
	if (m_ui->table->rowCount() == 0)
		return;
	if (QMessageBox::question(this, tr("Clear predefined markers"), tr("Clear the list?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
		m_ui->table->clearContents();
		m_ui->table->setRowCount(0);
		while (!m_settings.predefinedMarkers.isEmpty())
			delete m_settings.predefinedMarkers.takeFirst();
		emit predefinedMarkersChanged(m_settings.predefinedMarkers);
	}
}

void AwMarkerInspector::removeSelectedPredefinedMarkers()
{
	QList<QTableWidgetItem *> selection = m_ui->table->selectedItems();
	if (!selection.isEmpty()) {
		AwMarkerList markers;
		QVector<int> rows;
		foreach (QTableWidgetItem *item, selection) // get selected rows
			if (!rows.contains(item->row()))
				rows << item->row();
		foreach (int row, rows) {
			markers << m_settings.predefinedMarkers.at(row);
			m_ui->table->removeRow(row);
		}
				
		foreach (AwMarker *m, markers) {
			m_settings.predefinedMarkers.removeOne(m);
			delete m;
		}
	}
}

void AwMarkerInspector::clearTargets()
{
	m_ui->listWidgetTargets->clear();
	m_targetedChannels.clear();
	m_settings.isTargettingChannels = false;
}

void AwMarkerInspector::addTargets()
{
	if (m_targets.isEmpty())
		return;

	AwAddTargetChannels dlg(m_targets);
	if (dlg.exec() == QDialog::Accepted) 	{
		QStringList selection = dlg.selectedChannels();
		if (!selection.isEmpty()) {
			// add selection to current list.
			// remove existing items to avoid duplicates.
			foreach (QString s, selection)	{
				if (!m_targetedChannels.contains(s)) {
					m_targetedChannels << s;
					m_ui->listWidgetTargets->addItem(s);
				}
			}
		}
		if (!m_targetedChannels.isEmpty()) 	{
			m_settings.targets = m_targetedChannels;
			m_settings.isTargettingChannels = true;
		}
		else
			m_settings.isTargettingChannels = false;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwMarkerInspector::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui->retranslateUi(this);
	}
}
