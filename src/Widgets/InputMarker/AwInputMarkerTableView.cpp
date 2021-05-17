#include <widget/AwInputMarkerTableView.h>
#include "InputMarkerModel.h"

AwInputMarkerTableView::AwInputMarkerTableView(QWidget *parent) : QTableView(parent)
{
	// Default flags => show all marker properties and allow multi selection
	InputMarkerModel *model = new InputMarkerModel(this);
	setModel(model);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	setFlags(ShowAll | MultiSelection);
}


void AwInputMarkerTableView::setMarkers(const AwMarkerList& markers)
{
	InputMarkerModel *m = qobject_cast<InputMarkerModel *>(this->model());
	m->setMarkers(markers);
}

void AwInputMarkerTableView::setFlags(int flags)
{
	m_flags = flags;
	if (m_flags & AwInputMarkerTableView::MultiSelection)
		this->setSelectionMode(QAbstractItemView::MultiSelection);
	else
		this->setSelectionMode(QAbstractItemView::SingleSelection);

	this->setColumnHidden(MARKER_MODEL_COLUMN_LABEL, !(m_flags & AwInputMarkerTableView::ShowLabel));
	this->setColumnHidden(MARKER_MODEL_COLUMN_DUR, !(m_flags & AwInputMarkerTableView::ShowDuration));
	this->setColumnHidden(MARKER_MODEL_COLUMN_VALUE, !(m_flags & AwInputMarkerTableView::ShowValue));
	this->setColumnHidden(MARKER_MODEL_COLUMN_COLOR, !(m_flags & AwInputMarkerTableView::ShowColor));
	this->setColumnHidden(MARKER_MODEL_COLUMN_START, !(m_flags & AwInputMarkerTableView::ShowPosition));
	this->setColumnHidden(MARKER_MODEL_COLUMN_TARGETS, !(m_flags & AwInputMarkerTableView::ShowTargets));

}