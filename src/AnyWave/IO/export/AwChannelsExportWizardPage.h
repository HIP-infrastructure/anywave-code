#pragma once
#include <QWizardPage>
#include <AwChannel.h>
class QTableWidget;
class QPushButton;

class AwChannelsExportWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	AwChannelsExportWizardPage(QWidget* parent = nullptr);

	AwChannelList& channels() { return m_channels; }
protected slots:
	void setAsRecorded();
	void setCurrentMontage();
	void setSelected();
	void clear();
	void addByTypes();
protected:
	void fillTable(const AwChannelList& channels);
	bool isComplete() const override;

	QTableWidget *m_table;
	QPushButton *m_selected, *m_asRecorded, *m_byTypes, *m_montage, *m_clear;
	AwChannelList m_channels;
};