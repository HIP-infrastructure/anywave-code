#include "AverageUi.h"

AverageUi::AverageUi(fmat& matrix, const AwChannelList& channels, AwMarker *marker, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_signalView = new AwSimpleSignalView(AwSimpleSignalView::NoNavButtons | AwSimpleSignalView::NoInfoLabels | AwSimpleSignalView::ViewAllChannels  
		| AwSimpleSignalView::NoHScrollBar | AwSimpleSignalView::NoMarkerBar);
	m_ui.verticalLayout->replaceWidget(m_ui.widget, m_signalView->widget());
	AwChannelList tmp;
	int index = 0;
	foreach(AwChannel *c, channels) {
		AwChannel *nc = new AwChannel(c);
		float *buf = nc->newData(matrix.n_cols);
		for (arma::uword j = 0; j < matrix.n_cols; j++)
			buf[j] = matrix(index, j);
		index++;
		tmp << nc;
	}
	m_signalView->setChannels(tmp);
	AwMarkerList markers;
	markers << marker;
	m_signalView->setMarkers(markers);
	// update settings for the view
	AwViewSettings *settings = m_signalView->settings();
	settings->showSensors = false;
	m_signalView->showMarkers(true);
	m_signalView->setSecPerCm(0.1);
	m_signalView->showElectrodesNames(false);
	m_signalView->showMarkersValues(false);
}

AverageUi::~AverageUi()
{
}
