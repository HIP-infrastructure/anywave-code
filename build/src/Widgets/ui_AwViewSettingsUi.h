/********************************************************************************
** Form generated from reading UI file 'AwViewSettingsUi.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWVIEWSETTINGSUI_H
#define UI_AWVIEWSETTINGSUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwViewSettingsUiClass
{
public:
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *Grid;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkTime;
    QCheckBox *checkSeconds;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout_8;
    QRadioButton *radioAlways;
    QRadioButton *radioHidden;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_7;
    QVBoxLayout *verticalLayout_9;
    QCheckBox *checkMarkerLabel;
    QCheckBox *checkMarkerValue;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkLimit;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QSpinBox *sbNbChan;
    QCheckBox *checkChanName;
    QCheckBox *checkBaseLine;
    QCheckBox *checkOverlay;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_4;
    QGridLayout *gridLayout_4;
    QCheckBox *checkICA;
    QCheckBox *checkSEEG;
    QCheckBox *checkEEG;
    QCheckBox *checkReference;
    QCheckBox *checkECG;
    QCheckBox *checkMEG;
    QCheckBox *checkTrigger;
    QCheckBox *checkOther;
    QCheckBox *checkEMG;
    QCheckBox *checkGRAD;
    QCheckBox *checkSource;
    QPushButton *buttonAll;
    QPushButton *buttonNone;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonOK;
    QPushButton *buttonCancel;

    void setupUi(QDialog *AwViewSettingsUiClass)
    {
        if (AwViewSettingsUiClass->objectName().isEmpty())
            AwViewSettingsUiClass->setObjectName(QStringLiteral("AwViewSettingsUiClass"));
        AwViewSettingsUiClass->setWindowModality(Qt::WindowModal);
        AwViewSettingsUiClass->resize(602, 400);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        AwViewSettingsUiClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwViewSettingsUiClass->setWindowIcon(icon);
        AwViewSettingsUiClass->setSizeGripEnabled(true);
        AwViewSettingsUiClass->setModal(true);
        verticalLayout_3 = new QVBoxLayout(AwViewSettingsUiClass);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(AwViewSettingsUiClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(5, 5, 5, 5);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        Grid = new QGroupBox(groupBox);
        Grid->setObjectName(QStringLiteral("Grid"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Grid->sizePolicy().hasHeightForWidth());
        Grid->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(Grid);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(5, 5, 5, 5);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        checkTime = new QCheckBox(Grid);
        checkTime->setObjectName(QStringLiteral("checkTime"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(checkTime->sizePolicy().hasHeightForWidth());
        checkTime->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(checkTime);

        checkSeconds = new QCheckBox(Grid);
        checkSeconds->setObjectName(QStringLiteral("checkSeconds"));
        sizePolicy2.setHeightForWidth(checkSeconds->sizePolicy().hasHeightForWidth());
        checkSeconds->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(checkSeconds);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        verticalLayout_4->addWidget(Grid);

        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        sizePolicy1.setHeightForWidth(groupBox_5->sizePolicy().hasHeightForWidth());
        groupBox_5->setSizePolicy(sizePolicy1);
        groupBox_5->setFlat(false);
        gridLayout_6 = new QGridLayout(groupBox_5);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setSizeConstraint(QLayout::SetNoConstraint);
        gridLayout_6->setContentsMargins(5, 5, 5, 5);
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        radioAlways = new QRadioButton(groupBox_5);
        radioAlways->setObjectName(QStringLiteral("radioAlways"));
        sizePolicy2.setHeightForWidth(radioAlways->sizePolicy().hasHeightForWidth());
        radioAlways->setSizePolicy(sizePolicy2);

        verticalLayout_8->addWidget(radioAlways);

        radioHidden = new QRadioButton(groupBox_5);
        radioHidden->setObjectName(QStringLiteral("radioHidden"));
        sizePolicy2.setHeightForWidth(radioHidden->sizePolicy().hasHeightForWidth());
        radioHidden->setSizePolicy(sizePolicy2);

        verticalLayout_8->addWidget(radioHidden);


        gridLayout_6->addLayout(verticalLayout_8, 0, 0, 1, 1);


        verticalLayout_4->addWidget(groupBox_5);


        horizontalLayout_3->addLayout(verticalLayout_4);

        groupBox_6 = new QGroupBox(groupBox);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        sizePolicy1.setHeightForWidth(groupBox_6->sizePolicy().hasHeightForWidth());
        groupBox_6->setSizePolicy(sizePolicy1);
        gridLayout_7 = new QGridLayout(groupBox_6);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        checkMarkerLabel = new QCheckBox(groupBox_6);
        checkMarkerLabel->setObjectName(QStringLiteral("checkMarkerLabel"));
        checkMarkerLabel->setChecked(true);

        verticalLayout_9->addWidget(checkMarkerLabel);

        checkMarkerValue = new QCheckBox(groupBox_6);
        checkMarkerValue->setObjectName(QStringLiteral("checkMarkerValue"));
        checkMarkerValue->setChecked(true);

        verticalLayout_9->addWidget(checkMarkerValue);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer);


        gridLayout_7->addLayout(verticalLayout_9, 0, 0, 1, 1);


        horizontalLayout_3->addWidget(groupBox_6);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(5, 5, 5, 5);
        checkLimit = new QCheckBox(groupBox_2);
        checkLimit->setObjectName(QStringLiteral("checkLimit"));
        sizePolicy2.setHeightForWidth(checkLimit->sizePolicy().hasHeightForWidth());
        checkLimit->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(checkLimit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        sbNbChan = new QSpinBox(groupBox_2);
        sbNbChan->setObjectName(QStringLiteral("sbNbChan"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(sbNbChan->sizePolicy().hasHeightForWidth());
        sbNbChan->setSizePolicy(sizePolicy3);
        sbNbChan->setAlignment(Qt::AlignCenter);
        sbNbChan->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        sbNbChan->setMaximum(1000);
        sbNbChan->setValue(40);

        horizontalLayout_2->addWidget(sbNbChan);


        verticalLayout_2->addLayout(horizontalLayout_2);

        checkChanName = new QCheckBox(groupBox_2);
        checkChanName->setObjectName(QStringLiteral("checkChanName"));
        sizePolicy2.setHeightForWidth(checkChanName->sizePolicy().hasHeightForWidth());
        checkChanName->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(checkChanName);

        checkBaseLine = new QCheckBox(groupBox_2);
        checkBaseLine->setObjectName(QStringLiteral("checkBaseLine"));
        sizePolicy2.setHeightForWidth(checkBaseLine->sizePolicy().hasHeightForWidth());
        checkBaseLine->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(checkBaseLine);

        checkOverlay = new QCheckBox(groupBox_2);
        checkOverlay->setObjectName(QStringLiteral("checkOverlay"));
        sizePolicy2.setHeightForWidth(checkOverlay->sizePolicy().hasHeightForWidth());
        checkOverlay->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(checkOverlay);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        verticalLayout_5->addWidget(groupBox_2);


        horizontalLayout_3->addLayout(verticalLayout_5);


        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox);

        groupBox_3 = new QGroupBox(AwViewSettingsUiClass);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        horizontalLayout_4 = new QHBoxLayout(groupBox_3);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        checkICA = new QCheckBox(groupBox_3);
        checkICA->setObjectName(QStringLiteral("checkICA"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(checkICA->sizePolicy().hasHeightForWidth());
        checkICA->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(checkICA, 2, 0, 1, 1);

        checkSEEG = new QCheckBox(groupBox_3);
        checkSEEG->setObjectName(QStringLiteral("checkSEEG"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(checkSEEG->sizePolicy().hasHeightForWidth());
        checkSEEG->setSizePolicy(sizePolicy5);

        gridLayout_4->addWidget(checkSEEG, 0, 5, 1, 1);

        checkEEG = new QCheckBox(groupBox_3);
        checkEEG->setObjectName(QStringLiteral("checkEEG"));
        sizePolicy5.setHeightForWidth(checkEEG->sizePolicy().hasHeightForWidth());
        checkEEG->setSizePolicy(sizePolicy5);

        gridLayout_4->addWidget(checkEEG, 0, 6, 1, 1);

        checkReference = new QCheckBox(groupBox_3);
        checkReference->setObjectName(QStringLiteral("checkReference"));
        sizePolicy4.setHeightForWidth(checkReference->sizePolicy().hasHeightForWidth());
        checkReference->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(checkReference, 1, 5, 1, 1);

        checkECG = new QCheckBox(groupBox_3);
        checkECG->setObjectName(QStringLiteral("checkECG"));
        sizePolicy4.setHeightForWidth(checkECG->sizePolicy().hasHeightForWidth());
        checkECG->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(checkECG, 1, 0, 1, 1);

        checkMEG = new QCheckBox(groupBox_3);
        checkMEG->setObjectName(QStringLiteral("checkMEG"));
        sizePolicy5.setHeightForWidth(checkMEG->sizePolicy().hasHeightForWidth());
        checkMEG->setSizePolicy(sizePolicy5);

        gridLayout_4->addWidget(checkMEG, 0, 0, 1, 1);

        checkTrigger = new QCheckBox(groupBox_3);
        checkTrigger->setObjectName(QStringLiteral("checkTrigger"));
        sizePolicy4.setHeightForWidth(checkTrigger->sizePolicy().hasHeightForWidth());
        checkTrigger->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(checkTrigger, 2, 5, 1, 1);

        checkOther = new QCheckBox(groupBox_3);
        checkOther->setObjectName(QStringLiteral("checkOther"));
        sizePolicy4.setHeightForWidth(checkOther->sizePolicy().hasHeightForWidth());
        checkOther->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(checkOther, 2, 6, 1, 1);

        checkEMG = new QCheckBox(groupBox_3);
        checkEMG->setObjectName(QStringLiteral("checkEMG"));
        sizePolicy4.setHeightForWidth(checkEMG->sizePolicy().hasHeightForWidth());
        checkEMG->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(checkEMG, 1, 1, 1, 4);

        checkGRAD = new QCheckBox(groupBox_3);
        checkGRAD->setObjectName(QStringLiteral("checkGRAD"));
        sizePolicy5.setHeightForWidth(checkGRAD->sizePolicy().hasHeightForWidth());
        checkGRAD->setSizePolicy(sizePolicy5);

        gridLayout_4->addWidget(checkGRAD, 0, 1, 1, 4);

        checkSource = new QCheckBox(groupBox_3);
        checkSource->setObjectName(QStringLiteral("checkSource"));
        sizePolicy4.setHeightForWidth(checkSource->sizePolicy().hasHeightForWidth());
        checkSource->setSizePolicy(sizePolicy4);

        gridLayout_4->addWidget(checkSource, 2, 1, 1, 4);

        buttonAll = new QPushButton(groupBox_3);
        buttonAll->setObjectName(QStringLiteral("buttonAll"));
        sizePolicy5.setHeightForWidth(buttonAll->sizePolicy().hasHeightForWidth());
        buttonAll->setSizePolicy(sizePolicy5);

        gridLayout_4->addWidget(buttonAll, 3, 5, 1, 1);

        buttonNone = new QPushButton(groupBox_3);
        buttonNone->setObjectName(QStringLiteral("buttonNone"));
        sizePolicy5.setHeightForWidth(buttonNone->sizePolicy().hasHeightForWidth());
        buttonNone->setSizePolicy(sizePolicy5);

        gridLayout_4->addWidget(buttonNone, 3, 6, 1, 1);


        horizontalLayout_4->addLayout(gridLayout_4);

        horizontalSpacer_4 = new QSpacerItem(262, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout_3->addWidget(groupBox_3);

        verticalSpacer_2 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonOK = new QPushButton(AwViewSettingsUiClass);
        buttonOK->setObjectName(QStringLiteral("buttonOK"));
        buttonOK->setAutoDefault(false);

        horizontalLayout->addWidget(buttonOK);

        buttonCancel = new QPushButton(AwViewSettingsUiClass);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));
        buttonCancel->setAutoDefault(false);

        horizontalLayout->addWidget(buttonCancel);


        verticalLayout_3->addLayout(horizontalLayout);


        retranslateUi(AwViewSettingsUiClass);
        QObject::connect(buttonOK, SIGNAL(clicked()), AwViewSettingsUiClass, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), AwViewSettingsUiClass, SLOT(reject()));
        QObject::connect(checkLimit, SIGNAL(toggled(bool)), sbNbChan, SLOT(setEnabled(bool)));

        buttonOK->setDefault(true);


        QMetaObject::connectSlotsByName(AwViewSettingsUiClass);
    } // setupUi

    void retranslateUi(QDialog *AwViewSettingsUiClass)
    {
        AwViewSettingsUiClass->setWindowTitle(QApplication::translate("AwViewSettingsUiClass", "View settings", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwViewSettingsUiClass", "Display settings", Q_NULLPTR));
        Grid->setTitle(QApplication::translate("AwViewSettingsUiClass", "Grid", Q_NULLPTR));
        checkTime->setText(QApplication::translate("AwViewSettingsUiClass", "Show time grid", Q_NULLPTR));
        checkSeconds->setText(QApplication::translate("AwViewSettingsUiClass", "Show time values", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("AwViewSettingsUiClass", "Markers Bar", Q_NULLPTR));
        radioAlways->setText(QApplication::translate("AwViewSettingsUiClass", "Visible", Q_NULLPTR));
        radioHidden->setText(QApplication::translate("AwViewSettingsUiClass", "Hidden", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("AwViewSettingsUiClass", "Markers", Q_NULLPTR));
        checkMarkerLabel->setText(QApplication::translate("AwViewSettingsUiClass", "Show label", Q_NULLPTR));
        checkMarkerValue->setText(QApplication::translate("AwViewSettingsUiClass", "Show value", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AwViewSettingsUiClass", "Channels", Q_NULLPTR));
        checkLimit->setText(QApplication::translate("AwViewSettingsUiClass", "Limit the number of channels", Q_NULLPTR));
        label->setText(QApplication::translate("AwViewSettingsUiClass", "Max. channels:", Q_NULLPTR));
        checkChanName->setText(QApplication::translate("AwViewSettingsUiClass", "Show the channel name", Q_NULLPTR));
        checkBaseLine->setText(QApplication::translate("AwViewSettingsUiClass", "Show the zero line", Q_NULLPTR));
        checkOverlay->setText(QApplication::translate("AwViewSettingsUiClass", "Stack traces", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("AwViewSettingsUiClass", "Visible channels", Q_NULLPTR));
        checkICA->setText(QApplication::translate("AwViewSettingsUiClass", "ICA", Q_NULLPTR));
        checkSEEG->setText(QApplication::translate("AwViewSettingsUiClass", "SEEG", Q_NULLPTR));
        checkEEG->setText(QApplication::translate("AwViewSettingsUiClass", "EEG", Q_NULLPTR));
        checkReference->setText(QApplication::translate("AwViewSettingsUiClass", "Reference", Q_NULLPTR));
        checkECG->setText(QApplication::translate("AwViewSettingsUiClass", "ECG", Q_NULLPTR));
        checkMEG->setText(QApplication::translate("AwViewSettingsUiClass", "MEG", Q_NULLPTR));
        checkTrigger->setText(QApplication::translate("AwViewSettingsUiClass", "Trigg.", Q_NULLPTR));
        checkOther->setText(QApplication::translate("AwViewSettingsUiClass", "Other", Q_NULLPTR));
        checkEMG->setText(QApplication::translate("AwViewSettingsUiClass", "EMG", Q_NULLPTR));
        checkGRAD->setText(QApplication::translate("AwViewSettingsUiClass", "GRAD", Q_NULLPTR));
        checkSource->setText(QApplication::translate("AwViewSettingsUiClass", "Source", Q_NULLPTR));
        buttonAll->setText(QApplication::translate("AwViewSettingsUiClass", "Select All", Q_NULLPTR));
        buttonNone->setText(QApplication::translate("AwViewSettingsUiClass", "Clear", Q_NULLPTR));
        buttonOK->setText(QApplication::translate("AwViewSettingsUiClass", "Apply", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("AwViewSettingsUiClass", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwViewSettingsUiClass: public Ui_AwViewSettingsUiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWVIEWSETTINGSUI_H
