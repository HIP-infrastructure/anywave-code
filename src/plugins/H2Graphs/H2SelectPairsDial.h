#pragma once

#include <QDialog>
#include "ui_H2SelectPairsDial.h"

class H2SelectPairsDial : public QDialog
{
	Q_OBJECT

public:
	H2SelectPairsDial(const QStringList& labels, const QStringList& selectedPairs, QWidget *parent = Q_NULLPTR);
	~H2SelectPairsDial();

	inline const QStringList& pairs() { return m_pairs; }
public slots:
	void accept() override;
private slots:
	void addPair();
	void clear();
private:
	Ui::H2SelectPairsDialUi m_ui;
	QStringList m_pairs;
};
