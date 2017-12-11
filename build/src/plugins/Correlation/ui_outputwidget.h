/********************************************************************************
** Form generated from reading UI file 'outputwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUTWIDGET_H
#define UI_OUTPUTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qwt_plot.h>

QT_BEGIN_NAMESPACE

class Ui_OutputWidgetUi
{
public:
    QVBoxLayout *verticalLayout;
    QwtPlot *plot;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDoubleSpinBox *spinThreshold;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonSaveToMat;

    void setupUi(QWidget *OutputWidgetUi)
    {
        if (OutputWidgetUi->objectName().isEmpty())
            OutputWidgetUi->setObjectName(QStringLiteral("OutputWidgetUi"));
        OutputWidgetUi->resize(554, 513);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        OutputWidgetUi->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(OutputWidgetUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        plot = new QwtPlot(OutputWidgetUi);
        plot->setObjectName(QStringLiteral("plot"));

        verticalLayout->addWidget(plot);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(OutputWidgetUi);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        spinThreshold = new QDoubleSpinBox(OutputWidgetUi);
        spinThreshold->setObjectName(QStringLiteral("spinThreshold"));
        spinThreshold->setMaximum(1);
        spinThreshold->setSingleStep(0.1);

        horizontalLayout->addWidget(spinThreshold);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonSaveToMat = new QPushButton(OutputWidgetUi);
        buttonSaveToMat->setObjectName(QStringLiteral("buttonSaveToMat"));

        horizontalLayout->addWidget(buttonSaveToMat);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);

        retranslateUi(OutputWidgetUi);
        QObject::connect(buttonSaveToMat, SIGNAL(clicked()), OutputWidgetUi, SLOT(saveToMat()));
        QObject::connect(spinThreshold, SIGNAL(valueChanged(double)), OutputWidgetUi, SLOT(applyThreshold(double)));

        QMetaObject::connectSlotsByName(OutputWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *OutputWidgetUi)
    {
        OutputWidgetUi->setWindowTitle(QApplication::translate("OutputWidgetUi", "Correlation Matrix", Q_NULLPTR));
        label->setText(QApplication::translate("OutputWidgetUi", "Threshold (abs):", Q_NULLPTR));
        buttonSaveToMat->setText(QApplication::translate("OutputWidgetUi", "Save to .mat", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class OutputWidgetUi: public Ui_OutputWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUTWIDGET_H
