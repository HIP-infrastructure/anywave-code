/********************************************************************************
** Form generated from reading UI file 'SSTWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SSTWIDGET_H
#define UI_SSTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <widget/AwComboCMap.h>

QT_BEGIN_NAMESPACE

class Ui_SSTWidgetUi
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    AwComboCMap *comboBox;
    QPushButton *buttonBack;
    QPushButton *buttonForward;
    QDoubleSpinBox *spinDuration;
    QPushButton *buttonUpdate;
    QComboBox *comboBoxPlots;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *SSTWidgetUi)
    {
        if (SSTWidgetUi->objectName().isEmpty())
            SSTWidgetUi->setObjectName(QStringLiteral("SSTWidgetUi"));
        SSTWidgetUi->resize(975, 610);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        SSTWidgetUi->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(SSTWidgetUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget = new QWidget(SSTWidgetUi);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(widget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(SSTWidgetUi);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        comboBox = new AwComboCMap(SSTWidgetUi);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout->addWidget(comboBox);

        buttonBack = new QPushButton(SSTWidgetUi);
        buttonBack->setObjectName(QStringLiteral("buttonBack"));
        buttonBack->setMinimumSize(QSize(32, 31));
        buttonBack->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/back32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonBack->setIcon(icon1);
        buttonBack->setIconSize(QSize(32, 32));
        buttonBack->setFlat(true);

        horizontalLayout->addWidget(buttonBack);

        buttonForward = new QPushButton(SSTWidgetUi);
        buttonForward->setObjectName(QStringLiteral("buttonForward"));
        buttonForward->setMinimumSize(QSize(32, 31));
        buttonForward->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/fwd32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonForward->setIcon(icon2);
        buttonForward->setIconSize(QSize(32, 32));
        buttonForward->setFlat(true);

        horizontalLayout->addWidget(buttonForward);

        spinDuration = new QDoubleSpinBox(SSTWidgetUi);
        spinDuration->setObjectName(QStringLiteral("spinDuration"));
        spinDuration->setDecimals(1);
        spinDuration->setMaximum(100);
        spinDuration->setValue(5);

        horizontalLayout->addWidget(spinDuration);

        buttonUpdate = new QPushButton(SSTWidgetUi);
        buttonUpdate->setObjectName(QStringLiteral("buttonUpdate"));

        horizontalLayout->addWidget(buttonUpdate);

        comboBoxPlots = new QComboBox(SSTWidgetUi);
        comboBoxPlots->setObjectName(QStringLiteral("comboBoxPlots"));

        horizontalLayout->addWidget(comboBoxPlots);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 1);

        retranslateUi(SSTWidgetUi);

        QMetaObject::connectSlotsByName(SSTWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *SSTWidgetUi)
    {
        SSTWidgetUi->setWindowTitle(QApplication::translate("SSTWidgetUi", "Spatio-Temporal representation", Q_NULLPTR));
        label->setText(QApplication::translate("SSTWidgetUi", "Color map:", Q_NULLPTR));
        buttonBack->setText(QString());
        buttonForward->setText(QString());
        spinDuration->setSuffix(QApplication::translate("SSTWidgetUi", "s", Q_NULLPTR));
        buttonUpdate->setText(QApplication::translate("SSTWidgetUi", "Update Plot", Q_NULLPTR));
        comboBoxPlots->clear();
        comboBoxPlots->insertItems(0, QStringList()
         << QApplication::translate("SSTWidgetUi", "Show Hilbert Transformation", Q_NULLPTR)
         << QApplication::translate("SSTWidgetUi", "Show Amplitudes", Q_NULLPTR)
        );
    } // retranslateUi

};

namespace Ui {
    class SSTWidgetUi: public Ui_SSTWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSTWIDGET_H
