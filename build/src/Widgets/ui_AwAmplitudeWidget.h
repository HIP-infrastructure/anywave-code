/********************************************************************************
** Form generated from reading UI file 'AwAmplitudeWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWAMPLITUDEWIDGET_H
#define UI_AWAMPLITUDEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwAmplitudeWidgetUi
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonUp;
    QComboBox *comboLevels;
    QDoubleSpinBox *spValue;
    QPushButton *buttonDown;

    void setupUi(QWidget *AwAmplitudeWidgetUi)
    {
        if (AwAmplitudeWidgetUi->objectName().isEmpty())
            AwAmplitudeWidgetUi->setObjectName(QStringLiteral("AwAmplitudeWidgetUi"));
        AwAmplitudeWidgetUi->resize(279, 32);
        AwAmplitudeWidgetUi->setWindowTitle(QStringLiteral("AwAmplitudeWidget"));
        horizontalLayout = new QHBoxLayout(AwAmplitudeWidgetUi);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        buttonUp = new QPushButton(AwAmplitudeWidgetUi);
        buttonUp->setObjectName(QStringLiteral("buttonUp"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonUp->sizePolicy().hasHeightForWidth());
        buttonUp->setSizePolicy(sizePolicy);
        buttonUp->setMinimumSize(QSize(32, 32));
        buttonUp->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/plus_sign_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonUp->setIcon(icon);
        buttonUp->setIconSize(QSize(32, 32));
        buttonUp->setFlat(true);

        horizontalLayout->addWidget(buttonUp);

        comboLevels = new QComboBox(AwAmplitudeWidgetUi);
        comboLevels->setObjectName(QStringLiteral("comboLevels"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboLevels->sizePolicy().hasHeightForWidth());
        comboLevels->setSizePolicy(sizePolicy1);
        comboLevels->setMinimumSize(QSize(100, 0));
        comboLevels->setMaximumSize(QSize(150, 16777215));

        horizontalLayout->addWidget(comboLevels);

        spValue = new QDoubleSpinBox(AwAmplitudeWidgetUi);
        spValue->setObjectName(QStringLiteral("spValue"));
        sizePolicy1.setHeightForWidth(spValue->sizePolicy().hasHeightForWidth());
        spValue->setSizePolicy(sizePolicy1);
        spValue->setMinimumSize(QSize(80, 0));
        spValue->setMaximumSize(QSize(150, 16777215));
        spValue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spValue->setKeyboardTracking(false);
        spValue->setDecimals(6);
        spValue->setMaximum(1e+52);

        horizontalLayout->addWidget(spValue);

        buttonDown = new QPushButton(AwAmplitudeWidgetUi);
        buttonDown->setObjectName(QStringLiteral("buttonDown"));
        sizePolicy.setHeightForWidth(buttonDown->sizePolicy().hasHeightForWidth());
        buttonDown->setSizePolicy(sizePolicy);
        buttonDown->setMinimumSize(QSize(32, 32));
        buttonDown->setMaximumSize(QSize(32, 32));
        buttonDown->setAutoFillBackground(false);
        buttonDown->setStyleSheet(QStringLiteral(""));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/minus_sign_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDown->setIcon(icon1);
        buttonDown->setIconSize(QSize(32, 32));
        buttonDown->setFlat(true);

        horizontalLayout->addWidget(buttonDown);


        retranslateUi(AwAmplitudeWidgetUi);

        QMetaObject::connectSlotsByName(AwAmplitudeWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwAmplitudeWidgetUi)
    {
#ifndef QT_NO_TOOLTIP
        buttonUp->setToolTip(QApplication::translate("AwAmplitudeWidgetUi", "Raise Gains For All Channels", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonUp->setText(QString());
        buttonUp->setShortcut(QApplication::translate("AwAmplitudeWidgetUi", "+", Q_NULLPTR));
        comboLevels->clear();
        comboLevels->insertItems(0, QStringList()
         << QApplication::translate("AwAmplitudeWidgetUi", "All Gain Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "EEG Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "SEEG Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "MEG Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "EMG Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "ECG Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "Reference Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "Trigger Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "Other Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "ICA Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "Source Levels", Q_NULLPTR)
         << QApplication::translate("AwAmplitudeWidgetUi", "GRAD Levels", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        spValue->setToolTip(QApplication::translate("AwAmplitudeWidgetUi", "Enter gain level here", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        buttonDown->setToolTip(QApplication::translate("AwAmplitudeWidgetUi", "Lower gains for all channels", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonDown->setText(QString());
        buttonDown->setShortcut(QApplication::translate("AwAmplitudeWidgetUi", "-", Q_NULLPTR));
        Q_UNUSED(AwAmplitudeWidgetUi);
    } // retranslateUi

};

namespace Ui {
    class AwAmplitudeWidgetUi: public Ui_AwAmplitudeWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWAMPLITUDEWIDGET_H
