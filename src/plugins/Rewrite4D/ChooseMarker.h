#ifndef CHOOSEMARKER_H
#define CHOOSEMARKER_H

#include <QDialog>
#include "ui_ChooseMarker.h"
#include <AwMarker.h>

class ChooseMarkerDialog : public QDialog
{
	Q_OBJECT

public:
	ChooseMarkerDialog(AwMarkerList *markers, QWidget *parent = 0);
	~ChooseMarkerDialog();
	inline float value() { return m_value; }
	inline int before() { return m_beforems; }
	inline int after() { return m_afterms; }
	QStringList selectedMarkers() { return m_selectedMarkers; }
	QVector<int>& channelTypes() { return m_types; }
public slots:
	void accept();

private:
	Ui::ChooseMarkerUi m_ui;
	QStringList m_markersNames;
	QStringList m_selectedMarkers;
	float m_value;	// overwrite value in pT
	int m_beforems;
	int m_afterms;
	QVector<int> m_types;	// channel types on which to apply overwriting.
};

#endif // CHOOSEMARKER_H
