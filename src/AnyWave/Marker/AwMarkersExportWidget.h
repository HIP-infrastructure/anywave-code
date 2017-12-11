#pragma once

#include <QDialog>
#include "ui_AwMarkersExportWidget.h"
#include <AwMarker.h>
#include <AwChannel.h>

class AwMarkersExportWidget : public QDialog
{
	Q_OBJECT

public:
	AwMarkersExportWidget(const AwMarkerList& selection, const AwMarkerList& markers, QWidget *parent = Q_NULLPTR);
	~AwMarkersExportWidget();
	inline bool concatenate() { return m_concatenate;  }
	AwChannelList& channels() { return m_channels; }
	inline QString& file() { return m_file; }
	AwMarkerList& markers() { return m_markers; }
	inline bool isMatlabFormat() { return m_ui.radioMATLAB->isChecked(); }
public slots:
	void selectFile();
	void accept() override;
private:
	bool checkFile();
	Ui::AwMarkersExportWidgetUi m_ui;
	AwMarkerList m_selected, m_all, m_markers;
	AwChannelList m_channels;
	bool m_concatenate;
	QString m_file;
};
