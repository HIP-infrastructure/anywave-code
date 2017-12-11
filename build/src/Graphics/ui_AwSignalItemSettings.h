/********************************************************************************
** Form generated from reading UI file 'AwSignalItemSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWSIGNALITEMSETTINGS_H
#define UI_AWSIGNALITEMSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SignalSettingsClass
{
public:
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_7;
    QCheckBox *checkHighPass;
    QComboBox *cb_Couleur;
    QLabel *label_Couleur;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *checkLowPass;
    QLabel *labelChannel;
    QSpacerItem *horizontalSpacer_10;
    QLabel *label;
    QDoubleSpinBox *sbLow;
    QLabel *labelType;
    QSpacerItem *horizontalSpacer_2;
    QDoubleSpinBox *sbHigh;
    QDoubleSpinBox *spGainLevel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonApply;
    QPushButton *buttonCancel;

    void setupUi(QDialog *SignalSettingsClass)
    {
        if (SignalSettingsClass->objectName().isEmpty())
            SignalSettingsClass->setObjectName(QStringLiteral("SignalSettingsClass"));
        SignalSettingsClass->resize(344, 259);
        SignalSettingsClass->setMinimumSize(QSize(344, 259));
        SignalSettingsClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        SignalSettingsClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        SignalSettingsClass->setWindowIcon(icon);
        gridLayout_3 = new QGridLayout(SignalSettingsClass);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(SignalSettingsClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer_7 = new QSpacerItem(113, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 3, 1, 1, 3);

        checkHighPass = new QCheckBox(groupBox);
        checkHighPass->setObjectName(QStringLiteral("checkHighPass"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(checkHighPass->sizePolicy().hasHeightForWidth());
        checkHighPass->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkHighPass, 3, 0, 1, 1);

        cb_Couleur = new QComboBox(groupBox);
        cb_Couleur->setObjectName(QStringLiteral("cb_Couleur"));
        sizePolicy.setHeightForWidth(cb_Couleur->sizePolicy().hasHeightForWidth());
        cb_Couleur->setSizePolicy(sizePolicy);
        cb_Couleur->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(cb_Couleur, 2, 1, 1, 1);

        label_Couleur = new QLabel(groupBox);
        label_Couleur->setObjectName(QStringLiteral("label_Couleur"));
        sizePolicy.setHeightForWidth(label_Couleur->sizePolicy().hasHeightForWidth());
        label_Couleur->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_Couleur, 2, 0, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(72, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_9, 2, 2, 1, 2);

        horizontalSpacer_3 = new QSpacerItem(113, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 5, 1, 1, 3);

        checkLowPass = new QCheckBox(groupBox);
        checkLowPass->setObjectName(QStringLiteral("checkLowPass"));
        sizePolicy.setHeightForWidth(checkLowPass->sizePolicy().hasHeightForWidth());
        checkLowPass->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkLowPass, 5, 0, 1, 1);

        labelChannel = new QLabel(groupBox);
        labelChannel->setObjectName(QStringLiteral("labelChannel"));
        sizePolicy.setHeightForWidth(labelChannel->sizePolicy().hasHeightForWidth());
        labelChannel->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(14);
        labelChannel->setFont(font1);
        labelChannel->setScaledContents(true);

        gridLayout->addWidget(labelChannel, 0, 0, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(72, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_10, 1, 2, 1, 2);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setScaledContents(true);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        sbLow = new QDoubleSpinBox(groupBox);
        sbLow->setObjectName(QStringLiteral("sbLow"));
        sizePolicy.setHeightForWidth(sbLow->sizePolicy().hasHeightForWidth());
        sbLow->setSizePolicy(sizePolicy);
        sbLow->setDecimals(3);
        sbLow->setMinimum(0.001);
        sbLow->setMaximum(1000);

        gridLayout->addWidget(sbLow, 6, 1, 1, 1);

        labelType = new QLabel(groupBox);
        labelType->setObjectName(QStringLiteral("labelType"));
        sizePolicy.setHeightForWidth(labelType->sizePolicy().hasHeightForWidth());
        labelType->setSizePolicy(sizePolicy);

        gridLayout->addWidget(labelType, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 2);

        sbHigh = new QDoubleSpinBox(groupBox);
        sbHigh->setObjectName(QStringLiteral("sbHigh"));
        sizePolicy.setHeightForWidth(sbHigh->sizePolicy().hasHeightForWidth());
        sbHigh->setSizePolicy(sizePolicy);
        sbHigh->setDecimals(3);
        sbHigh->setMinimum(0.001);
        sbHigh->setMaximum(1000);

        gridLayout->addWidget(sbHigh, 4, 1, 1, 1);

        spGainLevel = new QDoubleSpinBox(groupBox);
        spGainLevel->setObjectName(QStringLiteral("spGainLevel"));
        spGainLevel->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spGainLevel->setKeyboardTracking(false);
        spGainLevel->setDecimals(4);
        spGainLevel->setMaximum(1e+66);

        gridLayout->addWidget(spGainLevel, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonApply = new QPushButton(SignalSettingsClass);
        buttonApply->setObjectName(QStringLiteral("buttonApply"));

        horizontalLayout->addWidget(buttonApply);

        buttonCancel = new QPushButton(SignalSettingsClass);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));

        horizontalLayout->addWidget(buttonCancel);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout_3->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(SignalSettingsClass);
        QObject::connect(buttonApply, SIGNAL(clicked()), SignalSettingsClass, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), SignalSettingsClass, SLOT(reject()));
        QObject::connect(checkHighPass, SIGNAL(toggled(bool)), sbHigh, SLOT(setEnabled(bool)));
        QObject::connect(checkLowPass, SIGNAL(toggled(bool)), sbLow, SLOT(setEnabled(bool)));

        buttonApply->setDefault(true);


        QMetaObject::connectSlotsByName(SignalSettingsClass);
    } // setupUi

    void retranslateUi(QDialog *SignalSettingsClass)
    {
        SignalSettingsClass->setWindowTitle(QApplication::translate("SignalSettingsClass", "Channel Settings", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("SignalSettingsClass", "Settings", Q_NULLPTR));
        checkHighPass->setText(QApplication::translate("SignalSettingsClass", "High Pass", Q_NULLPTR));
        label_Couleur->setText(QApplication::translate("SignalSettingsClass", "Color:", Q_NULLPTR));
        checkLowPass->setText(QApplication::translate("SignalSettingsClass", "Low Pass", Q_NULLPTR));
        labelChannel->setText(QApplication::translate("SignalSettingsClass", "A122", Q_NULLPTR));
        label->setText(QApplication::translate("SignalSettingsClass", "Gain level:", Q_NULLPTR));
        sbLow->setSuffix(QApplication::translate("SignalSettingsClass", "Hz", Q_NULLPTR));
        labelType->setText(QApplication::translate("SignalSettingsClass", "Type:", Q_NULLPTR));
        sbHigh->setSuffix(QApplication::translate("SignalSettingsClass", "Hz", Q_NULLPTR));
        buttonApply->setText(QApplication::translate("SignalSettingsClass", "Apply", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("SignalSettingsClass", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SignalSettingsClass: public Ui_SignalSettingsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWSIGNALITEMSETTINGS_H
