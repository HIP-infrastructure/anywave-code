#include "SIWidget.h"

#include <sigpack.h>
#ifdef MKL
#include <fftw.h>
#endif

//#include <vtkDoubleArray.h>
//#include <vtkChartXY.h>
//#include <vtkTable.h>
//#include <vtkPlot.h>
//#include <vtkContextView.h>
//#include <vtkContextScene.h>
//#include <vtkRenderer.h>
//#include <vtkFloatArray.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <math/AwMath.h>
using namespace sp;
#include <AwProcessInterface.h>
#include <qwt_plot_curve.h>
#include <AwKeys.h>

# define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


SIWidget::SIWidget(AwGUIProcess *process, QWidget *parent)
	: AwProcessGUIWidget(process, parent)
{
	m_ui.setupUi(this);
	//m_signalView = new AwBaseSignalView();
	//m_signalView->setFlags(AwBaseSignalView::NoMarkerBar | AwBaseSignalView::ViewAllChannels | AwBaseSignalView::EnableMarking);
	//m_signalView->setMinimumHeight(200);
	//connect(m_signalView, SIGNAL(dataLoaded(float, float)), this, SLOT(compute()));
	m_layoutRow = 0;
	m_window = SIWidget::Hanning;	
	m_widget = new QwtPlot();
	m_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_ui.graphicLayout->addWidget(m_widget);
	/// Set up the view
//	VTK_CREATE(vtkContextView, m_view);
//	m_view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);


}

SIWidget::~SIWidget()
{
}


void SIWidget::plot(const arma::mat& data)
{
	if (data.is_empty())
		return;
	auto nElems = data.n_rows / 2;
	auto nCols = data.n_cols;
	// get input channel labels
	auto labels = AwChannel::getLabels(m_process->pdi.input.channels());
	float max_sr = m_process->pdi.input.settings.value(keys::max_sr).toFloat();
	vec xVector = linspace<vec>(0., nElems , nElems);
	m_widget->detachItems();
	m_widget->setAxisScale(QwtPlot::xBottom, 0., nElems);
	m_widget->setAxisScale(QwtPlot::yLeft, data.min(), data.max());
	for (auto c = 0; c < nCols; c++) {
		auto curve = new QwtPlotCurve(labels.at(c));
		curve->setRenderHint(QwtPlotItem::RenderAntialiased);
		curve->setStyle(QwtPlotCurve::Lines);
		curve->setPen(Qt::red, 0.5, Qt::SolidLine);
		curve->setSamples(xVector.memptr(), data.colptr(c), nElems);
		curve->attach(m_widget);
	}
	m_widget->replot();

	//VTK_CREATE(vtkTable, table);
	//VTK_CREATE(vtkFloatArray, xArray);
	//table->AddColumn(xArray);
	//xArray->SetName("Frequency (Hz)");
	//for (auto i = 0; i < nCols; i++) {
	//	VTK_CREATE(vtkFloatArray, array);
	//	table->AddColumn(array);
	//}

	//for (auto i = 0; i < nElems; i++) {
	//	table->SetValue(i, 0, i);
	//	int col = 1;
	//	for (arma::vec vector : data) {
	//		for (auto value : vector)
	//			table->SetValue(i, col, value);
	//	}
	//}
	//// Set up the view
	//VTK_CREATE(vtkContextView, m_view);
	//m_view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	//// Add multiple line plots, setting the colors etc
	//vtkSmartPointer<vtkChartXY> chart =
	//	vtkSmartPointer<vtkChartXY>::New();
	//m_view->GetScene()->AddItem(chart);
	//for (auto i = 0; i < nCols; i++) {
	//	vtkPlot* line = chart->AddPlot(vtkChart::LINE);
	//	line->SetInputData(table, 0, i + 1);
	//	line->SetColor(0, 255, 0, 255);
	//	line->SetWidth(1.0);
	//}

	//m_view->SetRenderWindow(m_widget->GetRenderWindow());

	//// Start interactor
	//m_view->GetRenderWindow()->Render();
	//m_view->GetInteractor()->Initialize();
	//m_view->GetInteractor()->Start();

}

void SIWidget::compute()
{
	auto func = [](const arma::vec& vector, const arma::vec& window) {
		return psd(vector, window);
	};
	uword cols = m_process->pdi.input.channels().count();
	uword rows = m_process->pdi.input.channels().first()->dataSize();
	arma::mat matrix = arma::zeros(rows, cols);
	uword i = 0;
	for (auto chan : m_process->pdi.input.channels()) {
		for (uword j = 0; j < chan->dataSize(); j++)
			matrix(j, i) = (double)chan->data()[j];
		i++;
	}
	arma::vec window = arma::ones(rows);
	switch (m_window) {
	case SIWidget::Hanning:
		window = hanning(rows);
		break;
	case SIWidget::Hamming:
		window = hamming(rows);
		break;
	}
	// use the same matrix to store results

	QList<arma::vec> results;
	for (uword i = 0; i < cols; i++) {
		arma::vec res = 10 * log10(func(matrix.col(i), window));
		//matrix.col(i) = 10 * log10(func(matrix.col(i), window));
		matrix.col(i) = res;
	}

	plot(matrix);
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