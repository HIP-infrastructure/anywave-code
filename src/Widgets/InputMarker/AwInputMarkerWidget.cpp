#include <widget/AwInputMarkerWidget.h>
#include "ui_AwInputMarkerWidget.h"
#include "InputMarkerModel.h"
#include <widget/AwMessageBox.h>

AwInputMarkerWidget::AwInputMarkerWidget(QWidget *parent)
	: QDialog(parent)
{
	m_ui = new Ui::AwInputMarkerWidgetUi();
	m_ui->setupUi(this);
	// Default flags => show all marker properties and allow multi selection
	InputMarkerModel *model = new InputMarkerModel(this);
	m_ui->tableView->setModel(model);
	m_ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	setFlags(ShowAllProperties | MultiSelection);
	m_ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_post = m_pre = 0.;
}

AwInputMarkerWidget::~AwInputMarkerWidget()
{
	delete m_ui;
}

void AwInputMarkerWidget::setMarkers(const AwMarkerList& markers)
{
	InputMarkerModel *m = qobject_cast<InputMarkerModel *>(m_ui->tableView->model());
	m_displayedMarkers = m_allMarkers = markers;
		
	// build a private list depending on flags
	if (m_flags & AwInputMarkerWidget::UniqueMarkerLabel) {
		AwMarkerList temp;
		QStringList labels;
		foreach(AwMarker *m, markers) {
			if (!labels.contains(m->label())) {
				temp << m;
				labels << m->label();
			}
		}
		m_displayedMarkers = temp;
	}

	if (m_flags & AwInputMarkerWidget::OnlySelectionMarkers) {
		foreach(AwMarker *m, m_displayedMarkers) {
			if (m->duration() == 0) {
				m_displayedMarkers.removeAll(m);
			}
		}
	}

	if (m_flags & AwInputMarkerWidget::OnlySingleMarkers) {
		foreach(AwMarker *m, m_displayedMarkers) {
			if (m->duration()) {
				m_displayedMarkers.removeAll(m);
			}
		}
	}

	m->setMarkers(m_displayedMarkers);
}

void AwInputMarkerWidget::setFlags(int flags)
{
	m_flags = flags;
	if (m_flags & AwInputMarkerWidget::MultiSelection)
		m_ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
	else
		m_ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

	m_ui->tableView->setColumnHidden(MARKER_MODEL_COLUMN_LABEL, !(m_flags & AwInputMarkerWidget::ShowLabel));
	m_ui->tableView->setColumnHidden(MARKER_MODEL_COLUMN_DUR, !(m_flags & AwInputMarkerWidget::ShowDuration));
	m_ui->tableView->setColumnHidden(MARKER_MODEL_COLUMN_VALUE, !(m_flags & AwInputMarkerWidget::ShowValue));
	m_ui->tableView->setColumnHidden(MARKER_MODEL_COLUMN_COLOR, !(m_flags & AwInputMarkerWidget::ShowColor));
	m_ui->tableView->setColumnHidden(MARKER_MODEL_COLUMN_START, !(m_flags & AwInputMarkerWidget::ShowPosition));
	m_ui->tableView->setColumnHidden(MARKER_MODEL_COLUMN_TARGETS, !(m_flags & AwInputMarkerWidget::ShowTargets));
}



void AwInputMarkerWidget::accept()
{
	QModelIndexList indexList = m_ui->tableView->selectionModel()->selectedIndexes();
	if (indexList.isEmpty())	{
		AwMessageBox::critical(this, "Invalid Selection", "Select at least one marker");
		return;
	}
	InputMarkerModel *m = qobject_cast<InputMarkerModel *>(m_ui->tableView->model());
	AwMarkerList markers = m->getList();

	foreach(QModelIndex index, indexList) {
		if (index.column() == 0) 
			m_selectedMarkers << markers.at(index.row());
	}

	m_pre = (float)m_ui->sbPre->value() / 1000;
	m_post = (float)m_ui->sbPost->value() / 1000;

	QDialog::accept();
}