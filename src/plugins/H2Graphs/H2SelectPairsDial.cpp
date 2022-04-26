#include "H2SelectPairsDial.h"
#include <QMessageBox>

H2SelectPairsDial::H2SelectPairsDial(const QStringList& labels, const QStringList& selectedPairs, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.comboC1->addItems(labels);
	m_ui.comboC2->addItems(labels);
	connect(m_ui.buttonAdd, &QPushButton::clicked, this, &H2SelectPairsDial::addPair);
	connect(m_ui.buttonClear, &QPushButton::clicked, this, &H2SelectPairsDial::clear);
	if (selectedPairs.size()) 
		m_ui.listWidget->addItems(selectedPairs);
	m_pairs = selectedPairs;
	setWindowTitle("Create pairs of channels"); 
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));
}

H2SelectPairsDial::~H2SelectPairsDial()
{
}


void H2SelectPairsDial::addPair()
{
	QString c1 = m_ui.comboC1->currentText();
	QString c2 = m_ui.comboC2->currentText();
	if (c1 == c2) {
		QMessageBox::information(this, "Pairing channels", "Pease select two different channels.");
		return;
	}
	// make the pair
	QString pair = QString("%1>%2").arg(c1).arg(c2);
	if (!m_pairs.contains(pair)) {
		m_pairs.append(pair);
		m_ui.listWidget->addItem(pair);
	}
}

void H2SelectPairsDial::clear()
{
	m_ui.listWidget->clear();
	m_pairs.clear();
}

void H2SelectPairsDial::accept()
{
	if (m_pairs.size())
		QDialog::accept();
}