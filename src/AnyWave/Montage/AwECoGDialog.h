#pragma once

#include <QDialog>
#include "ui_AwECoGDialog.h"
#include <AwChannel.h>

class AwECoGDialog : public QDialog
{
	Q_OBJECT

public:
	AwECoGDialog(const AwChannelList& channels, QWidget *parent = Q_NULLPTR);
	~AwECoGDialog();

	AwChannelList& channels() { return m_channels; }
public slots:
	void accept() override;
	void addGridStrip();
	void reset();
protected slots:
	void addGrid();
private:
	Ui::AwECoGDialogUi ui;
	AwChannelList m_channels;
	QMap<QString, AwChannel *> m_channelsMap;
};
