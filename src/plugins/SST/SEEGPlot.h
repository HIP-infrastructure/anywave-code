#pragma once
#include <AwChannel.h>
#include <qwt_plot.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_matrix_raster_data.h>

class SEEGPlot : public QwtPlot
{
	Q_OBJECT

public:
	SEEGPlot(AwChannelList& channels, QWidget *parent);
	~SEEGPlot();
	enum Plots { Hilbert, Amplitudes };
public slots:
	void changePlot(int type);
	void changeCMap(int cmap);
	void updateCurrentPlot();
protected:
	void updatePlot();
	void updateHilbertPlot();

	QwtPlotSpectrogram *m_spectro;
	QwtMatrixRasterData *m_matrix;
	AwChannelList m_channels;
	int m_plotType;
};
