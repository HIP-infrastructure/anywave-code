#include "SIWidget.h"
#include <aw_armadillo.h>
#include <sigpack.h>
#include <math/AwMath.h>
using namespace sp;


SIWidget::SIWidget(AwGUIProcess *process, QWidget *parent)
	: AwProcessGUIWidget(process, parent)
{
	m_ui.setupUi(this);
	//m_signalView = new AwBaseSignalView();
	//m_signalView->setFlags(AwBaseSignalView::NoMarkerBar | AwBaseSignalView::ViewAllChannels | AwBaseSignalView::EnableMarking);
	//m_signalView->setMinimumHeight(200);
	//connect(m_signalView, SIGNAL(dataLoaded(float, float)), this, SLOT(compute()));
	m_layoutRow = 0;
}

SIWidget::~SIWidget()
{
}

void SIWidget::compute()
{
	//auto channels = m_signalView->channels();
	//int n = 0;
	//for (auto p : m_plots) {
	//	auto graph = p->graph();
	//	auto channel = channels.value(n);
	//	float sr = channel->samplingRate();
	//	int NFFT = (int)std::floor(sr);
	//	vec data = AwMath::channelToVec(channel);
	//	vec S = 10 * log10(pwelch(data, NFFT, NFFT / 2));
	//	QVector<double> x(S.n_elem / 2), y(S.n_elem / 2);
	//	for (auto i = 0; i < S.n_elem / 2; i++) {
	//		x[i] = i;
	//		y[i] = S(i);
	//	}
	//	graph->setData(x, y);
	//	graph->rescaleAxes();
	//	auto ticker = p->xAxis->ticker();
	//	ticker->setTickCount(4);
	//	
	//	p->replot();
	//	n++;
	//}
}


//void SIWidget::setChannels(const AwChannelList& channels)
//{
//	// build layout for scroll area
//	QGridLayout *signalsLayout = m_ui.signalsLayout;
//	QGridLayout *layout = m_ui.otherLayout;
//
//	int row = 0;
//	int col = 0;
//	signalsLayout->addWidget(m_signalView, row, col);
//	signalsLayout->setRowStretch(1, 0);
//
//	// Build params for computation and add plot to the layout
//	for (auto c : channels) {
//		QCustomPlot *plot = new QCustomPlot(this);
//		plot->setMinimumHeight(200);
//		plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//		auto graph = plot->addGraph();
//		plot->plotLayout()->insertRow(0);
//		QCPTextElement *title = new QCPTextElement(plot, QString("%1 PSD").arg(c->fullName()), QFont("sans", 16, QFont::Normal));
//		plot->plotLayout()->addElement(0, 0, title);
//		graph->setName("Power Spectral Density");
//		plot->xAxis->setLabel("Frequency (Hz)");
//		plot->yAxis->setLabel(QString(QString::fromLatin1("Power (%1²/Hz)")).arg(c->unit()));
//		layout->addWidget(plot, row, col++);
//		m_plots << plot;
//	}
//	m_signalView->setChannels(channels);
//	repaint();
//}