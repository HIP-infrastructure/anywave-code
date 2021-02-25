#include "SIWidget.h"
#include <QPushButton>
#include <QMessageBox>


SIWidget::SIWidget(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	
	m_ui.comboWindow->addItem("None");
	m_ui.comboWindow->addItem("Hanning");
	m_ui.comboWindow->addItem("Hamming");
	m_ui.comboWindow->setCurrentIndex(1);
}

SIWidget::~SIWidget()
{

}

void SIWidget::clear()
{

}

void SIWidget::accept()
{
	timeWindow = m_ui.spinWindow->value();
	overlap = m_ui.spinOverlap->value();
	windowing = m_ui.comboWindow->currentText();
	QDialog::accept();
}




