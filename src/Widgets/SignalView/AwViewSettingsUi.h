#ifndef AWVIEWSETTINGSUI_H
#define AWVIEWSETTINGSUI_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwViewSettingsUi.h"

class AwViewSettings;

class AW_WIDGETS_EXPORT AwViewSettingsUi : public QDialog, public Ui::AwViewSettingsUiClass
{
	Q_OBJECT

public:
	AwViewSettingsUi(AwViewSettings *settings, QWidget *parent = 0);
public slots:
	void selectAllFilters();
	void unselectAllFilters();
	void updateMaxChannels(bool flag);
	void accept();
	int exec();
signals:
	void settingsChanged(AwViewSettings *settings, int flags);
protected:
	AwViewSettings *m_settings;
	QMap<QCheckBox *, int> m_checkBoxes;
};

#endif // AwViewSettingsUi
