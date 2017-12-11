/********************************************************************************
** Form generated from reading UI file 'AwDownSamplingWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWDOWNSAMPLINGWIDGET_H
#define UI_AWDOWNSAMPLINGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwDownSamplingWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *spMaxSR;
    QLabel *label_2;
    QComboBox *comboNewSR;
    QLabel *labelAA;
    QDoubleSpinBox *spAA;

    void setupUi(QWidget *AwDownSamplingWidget)
    {
        if (AwDownSamplingWidget->objectName().isEmpty())
            AwDownSamplingWidget->setObjectName(QStringLiteral("AwDownSamplingWidget"));
        AwDownSamplingWidget->resize(288, 90);
        gridLayout = new QGridLayout(AwDownSamplingWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(AwDownSamplingWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setScaledContents(true);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        spMaxSR = new QDoubleSpinBox(AwDownSamplingWidget);
        spMaxSR->setObjectName(QStringLiteral("spMaxSR"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(spMaxSR->sizePolicy().hasHeightForWidth());
        spMaxSR->setSizePolicy(sizePolicy);
        spMaxSR->setAlignment(Qt::AlignCenter);
        spMaxSR->setReadOnly(true);
        spMaxSR->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spMaxSR->setKeyboardTracking(false);
        spMaxSR->setProperty("showGroupSeparator", QVariant(false));
        spMaxSR->setMaximum(1e+71);

        gridLayout->addWidget(spMaxSR, 0, 1, 1, 2);

        label_2 = new QLabel(AwDownSamplingWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setScaledContents(true);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        comboNewSR = new QComboBox(AwDownSamplingWidget);
        comboNewSR->setObjectName(QStringLiteral("comboNewSR"));
        sizePolicy.setHeightForWidth(comboNewSR->sizePolicy().hasHeightForWidth());
        comboNewSR->setSizePolicy(sizePolicy);
        comboNewSR->setInputMethodHints(Qt::ImhPreferUppercase);
        comboNewSR->setFrame(true);

        gridLayout->addWidget(comboNewSR, 1, 1, 1, 2);

        labelAA = new QLabel(AwDownSamplingWidget);
        labelAA->setObjectName(QStringLiteral("labelAA"));
        labelAA->setScaledContents(true);

        gridLayout->addWidget(labelAA, 2, 0, 1, 2);

        spAA = new QDoubleSpinBox(AwDownSamplingWidget);
        spAA->setObjectName(QStringLiteral("spAA"));
        sizePolicy.setHeightForWidth(spAA->sizePolicy().hasHeightForWidth());
        spAA->setSizePolicy(sizePolicy);
        spAA->setAlignment(Qt::AlignCenter);
        spAA->setReadOnly(true);
        spAA->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spAA->setMaximum(1e+71);

        gridLayout->addWidget(spAA, 2, 2, 1, 1);


        retranslateUi(AwDownSamplingWidget);

        QMetaObject::connectSlotsByName(AwDownSamplingWidget);
    } // setupUi

    void retranslateUi(QWidget *AwDownSamplingWidget)
    {
        AwDownSamplingWidget->setWindowTitle(QApplication::translate("AwDownSamplingWidget", "AwDownSamplingWidget", Q_NULLPTR));
        label->setText(QApplication::translate("AwDownSamplingWidget", "Max. sampling rate :", Q_NULLPTR));
        spMaxSR->setSuffix(QApplication::translate("AwDownSamplingWidget", "Hz", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwDownSamplingWidget", "New sampling rate:", Q_NULLPTR));
        labelAA->setText(QApplication::translate("AwDownSamplingWidget", "Anti-aliasing filter will be:", Q_NULLPTR));
        spAA->setSuffix(QApplication::translate("AwDownSamplingWidget", "Hz", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwDownSamplingWidget: public Ui_AwDownSamplingWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWDOWNSAMPLINGWIDGET_H
