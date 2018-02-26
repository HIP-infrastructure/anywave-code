#pragma once

#include <QWidget>
#include "ui_AverageUi.h"
#include <armadillo>
using namespace arma;
#include <widget/SignalView/AwSimpleSignalView.h>

class AverageUi : public QWidget
{
	Q_OBJECT

public:
	AverageUi(fmat &matrix, const AwChannelList& channels, AwMarker *marker,  QWidget *parent = Q_NULLPTR);
	~AverageUi();

private:
	Ui::AverageUiUi m_ui;
	AwSimpleSignalView *m_signalView;
	AwMarker *m_marker;	// marker used to average
};
