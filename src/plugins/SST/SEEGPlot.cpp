#include "SEEGPlot.h"
#include <graphics/AwQwtColorMap.h>
#include <graphics/AwColorMap.h>
#include <qwt_color_map.h>
#include <qwt_plot_layout.h>
#include <math/AwMath.h>
#ifdef MKL
#include "fftw/fftw3.h"
#else
#include <fftw3.h>
#endif


SEEGPlot::SEEGPlot(AwChannelList& channels, QWidget *parent)
	: QwtPlot(parent)
{
	enableAxis(QwtPlot::yRight, false);
	enableAxis(QwtPlot::yLeft, true);
	enableAxis(QwtPlot::xBottom, true);
	m_matrix = new QwtMatrixRasterData;
	m_spectro = new QwtPlotSpectrogram();
	m_spectro->setRenderThreadCount(0); // use system specific thread count
	m_spectro->attach(this);
	m_spectro->setColorMap(AwQwtColorMap::newMap(AwColorMap::Jet));
	m_spectro->setAxes(QwtPlot::xBottom, QwtPlot::yLeft);
	m_channels = channels;
	m_plotType = Hilbert;
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

SEEGPlot::~SEEGPlot()
{
}

void SEEGPlot::updateCurrentPlot()
{
	changePlot(m_plotType);
}

void SEEGPlot::changeCMap(int cmap)
{
	m_spectro->setColorMap(AwQwtColorMap::newMap(cmap));
	m_spectro->invalidateCache();
	replot();
}

void SEEGPlot::changePlot(int plot)
{
	switch (plot) {
	case SEEGPlot::Hilbert:
		m_plotType = Hilbert;
		updateHilbertPlot();
		break;
	case SEEGPlot::Amplitudes:
		m_plotType = Amplitudes;
		updatePlot();
		break;
	}
}

void SEEGPlot::updatePlot()
{
	// data changed, recompute min, max and axis scales
	int cols = (int)m_channels.first()->dataSize();
	QVector<double> matrix(m_channels.size() * cols);
	double *data = matrix.data();

	mat channels = AwMath::channelsToMat(m_channels);
	double min = channels.min();
	double max = channels.max();
	channels = channels.t();
	memcpy(data, channels.memptr(), channels.n_elem * sizeof(double));

	// set X Axis scale
	m_matrix->setInterval(Qt::XAxis, QwtInterval(0, (double)cols - 1));
	m_matrix->setInterval(Qt::YAxis, QwtInterval(0, m_channels.size() - 1));
	m_matrix->setInterval(Qt::ZAxis, QwtInterval(min, max));
	m_matrix->setValueMatrix(matrix, cols);
	m_spectro->setData(m_matrix);
	m_spectro->invalidateCache();
	replot();
}

void SEEGPlot::updateHilbertPlot()
{
	int cols = (int)m_channels.first()->dataSize();
	int rows = m_channels.size();
	QVector<double> matrix(rows * cols);
	double *data = matrix.data();
	double min = 0., max = 0.;

	int row = 0;
	
	qint64 n = m_channels.first()->dataSize();
	double *in = (double *)fftw_malloc(sizeof(double) * n);
	fftw_complex *fftx = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
	fftw_complex *hilbert = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * n);
	fftw_plan plan = fftw_plan_dft_r2c_1d(n, in, fftx, FFTW_ESTIMATE);
	fftw_plan plan_c2c = fftw_plan_dft_1d(n, fftx, hilbert, FFTW_BACKWARD, FFTW_ESTIMATE);
	foreach(AwChannel *c, m_channels) {
		memset(in, 0, n * sizeof(double));
		for (qint64 i = 0; i < c->dataSize(); i++)
			in[i] = c->data()[i];
		fftw_execute_dft_r2c(plan, in, fftx);
		
		// clear second half of fftx
		for (qint64 i = n / 2; i < n; i++) {
			fftx[i][0] = 0.;
			fftx[i][1] = 0.;
		}

		fftw_execute_dft(plan_c2c, fftx, hilbert);
		for (qint64 i = 0; i < n; i++) {
			double modulus = sqrt(hilbert[i][0] * hilbert[i][0] + hilbert[i][1] * hilbert[i][1]);
			if (modulus > max)
				max = modulus;
			data[row * cols + i] = modulus;
		}
		row++;
	}
	fftw_destroy_plan(plan);
	fftw_destroy_plan(plan_c2c);
	
	// set X Axis scale
	m_matrix->setInterval(Qt::XAxis, QwtInterval(0, (double)cols));
	m_matrix->setInterval(Qt::YAxis, QwtInterval(0, m_channels.size()));
	m_matrix->setInterval(Qt::ZAxis, QwtInterval(min, max));
	m_matrix->setValueMatrix(matrix, cols);
	m_spectro->setData(m_matrix);
	m_spectro->invalidateCache();
	replot();
}