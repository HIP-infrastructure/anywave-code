#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H

#include <QDialog>
namespace Ui {class SettingsUi;};
#include "tf_settings.h"
#include <AwMarker.h>

class SettingsUi : public QDialog
{
	Q_OBJECT
public:
	SettingsUi(TFSettings *settings, QWidget *parent = 0);
	~SettingsUi();

	void accept();
private:
	Ui::SettingsUi *ui;
	TFSettings *m_settings;
};

#endif // SETTINGSUI_H
