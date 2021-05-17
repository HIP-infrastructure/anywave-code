#ifndef AWNAVIGATIONBAR_H
#define AWNAVIGATIONBAR_H
#include <AwGlobal.h>
#include <QWidget>
#include <AwChannel.h>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QWidget>
#include <widget/AwAmplitudeWidget.h>

class AwViewSettings;
class AwViewSettingsUi;
class AwAmplitudeWidget;

namespace Ui {class AwNavigationBarClass;};

class AW_WIDGETS_EXPORT AwNavigationBar : public QWidget
{
	Q_OBJECT

public:
	AwNavigationBar(QWidget *parent = 0, int flags = 0);
	~AwNavigationBar();

	void setFlags(int flags);
	inline void setTotalDuration(float dur) { m_totalDuration = dur; }
	AwAmplitudeWidget *amplitudeWidget();
public slots:
	void updatePageDuration(float dur);
	void updateNumberOfChannels(int n);
	void updateNumberOfSelectedChannels(int n);
	void updatePositionInFile(float pos);
	void updateSettings(AwViewSettings *settings, int flags);
	void setNewSettings(AwViewSettings *settings);
	void changeSecsPerCm();
	void openSettingsUi();
	/** change current settings and relay changed to connected objects **/
	void changeSettings(AwViewSettings *settings, int flags);
	void clean();
signals:
	void settingsChanged(AwViewSettings *settings, int flags);
	void startOfDataClicked();
	void endOfDataClicked();
	void pageBackwardClicked();
	void pageForwardClicked();
	void positionChanged(int value);
	void markingStarted();
	void filterButtonClicked();
protected:
	void updatePositionLabel();
	float m_totalDuration;
	float m_pageDuration;
	float m_positionInFile;
	float m_startPosition;
	int m_flags;
	AwViewSettings *m_settings;
	// horizontal scale values
	QVector<float> m_timeScale;
	// ui
	AwViewSettingsUi *m_settingsUi;
	Ui::AwNavigationBarClass *ui;

};

#endif // AWNAVIGATIONBAR_H
