#ifndef AWADDTARGETCHANNELS_H
#define AWADDTARGETCHANNELS_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwAddTargetChannels.h"

class AW_WIDGETS_EXPORT AwAddTargetChannels : public QDialog
{
	Q_OBJECT

public:
	AwAddTargetChannels(const QStringList& labels, QWidget *parent = 0);
	inline QStringList& selectedChannels() { return m_selectedChannels; }
protected:
	void changeEvent(QEvent *);
public slots:
	void accept();
private:
	Ui::AwAddTargetChannelsUI m_ui;
	QStringList m_selectedChannels;
};

#endif // AWADDTARGETCHANNELS_H
