#pragma once

#include <QWidget>
#include "ui_SIWidget.h"
//#include <widget/SignalView/AwBaseSignalView.h>
#include <process/AwProcessGUIWidget.h>
//#define QCUSTOMPLOT_USE_LIBRARY
//#include "qcustomplot.h"
//#include <vtkSmartPointer.h>
//#include <vtkContextView.h>
//#include <vtkContextScene.h>
//#if VTK_MAJOR_VERSION >= 8
//#include <QVTKOpenGLWidget.h>
//#include <vtkGenericOpenGLRenderWindow.h>  
//#define QVTK_CLASS QVTKOpenGLWidget		
//#else
//#include <QVTKWidget.h>
//#define QVTK_CLASS  QVTKWidget		
//#endif
#include <aw_armadillo.h>
//#include <qwt_plot.h>
#include <AwChannel.h>

// data structure to hold all fft iterations for a channel
class FFTIterations
{
public:
	FFTIterations(AwChannel* chan) { channel = chan; m_iterationIndex = 0; }
	void setNumberOfIterations(uword count, uword timeWindowPts) { m_iterations = arma::zeros(count, timeWindowPts); }
	void appendIteration(const arma::vec& iteration);

	inline mat& results() { return m_iterations; }
protected:
	AwChannel* channel;
	arma::mat m_iterations;	// each row of iterations are ONE fft iteration
	uword m_iterationIndex;
};

class SIWidget : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	SIWidget(AwGUIProcess *process, QWidget *parent = nullptr);
	~SIWidget();

	enum Windows { None,  Hanning, Hamming };
public slots:
	void compute();
private:
	void plot(const arma::mat& data);

	Ui::SIWidgetUi  m_ui;
	//int m_layoutRow;
	int m_window; // type of windowing to use, 0 = none
	//QwtPlot* m_widget;
	QMap<AwChannel*, FFTIterations*> m_results;
};
