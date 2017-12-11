/********************************************************************************
** Form generated from reading UI file 'AwFilterSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWFILTERSETTINGS_H
#define UI_AWFILTERSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilterSettingsClass
{
public:
    QVBoxLayout *verticalLayout_6;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *checkEegHigh;
    QSpacerItem *horizontalSpacer_24;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_25;
    QDoubleSpinBox *spEegHP;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_19;
    QCheckBox *checkEegLow;
    QSpacerItem *horizontalSpacer_26;
    QHBoxLayout *horizontalLayout_20;
    QSpacerItem *horizontalSpacer_27;
    QDoubleSpinBox *spEegLP;
    QSpacerItem *horizontalSpacer_28;
    QHBoxLayout *horizontalLayout_13;
    QCheckBox *cbNotchEEG;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_6;
    QDoubleSpinBox *spNotch;
    QSpacerItem *horizontalSpacer_40;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_9;
    QCheckBox *checkEmgHigh;
    QSpacerItem *horizontalSpacer_29;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_30;
    QDoubleSpinBox *spEmgHP;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_21;
    QCheckBox *checkEmgLow;
    QSpacerItem *horizontalSpacer_31;
    QHBoxLayout *horizontalLayout_22;
    QSpacerItem *horizontalSpacer_32;
    QDoubleSpinBox *spEmgLP;
    QSpacerItem *horizontalSpacer_33;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *cbNotchEMG;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_9;
    QDoubleSpinBox *spNotchEMG;
    QSpacerItem *horizontalSpacer_39;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *checkMegHigh;
    QSpacerItem *horizontalSpacer_19;
    QHBoxLayout *horizontalLayout_16;
    QSpacerItem *horizontalSpacer_22;
    QDoubleSpinBox *spMegHP;
    QSpacerItem *horizontalSpacer_23;
    QHBoxLayout *horizontalLayout_15;
    QCheckBox *checkMegLow;
    QSpacerItem *horizontalSpacer_21;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QDoubleSpinBox *spMegLP;
    QSpacerItem *horizontalSpacer_20;
    QHBoxLayout *horizontalLayout_17;
    QCheckBox *cbNotchMEG;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_18;
    QSpacerItem *horizontalSpacer_11;
    QDoubleSpinBox *spNotchMEG;
    QSpacerItem *horizontalSpacer_18;
    QGroupBox *groupICA;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout_4;
    QDoubleSpinBox *spICAEEGLP;
    QDoubleSpinBox *spICAMEGLP;
    QLabel *labelEEGHP;
    QLabel *labelMEGLP;
    QLabel *labelEEGLP;
    QDoubleSpinBox *spICAEEGHP;
    QLabel *labelMEGHP;
    QDoubleSpinBox *spICAMEGHP;
    QDoubleSpinBox *spICAEMGHP;
    QLabel *labelEMGLP;
    QLabel *labelEMGHP;
    QDoubleSpinBox *spICAEMGLP;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupSource;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout_3;
    QLabel *labelSourceMEGLP;
    QLabel *labelSourceEEGLP;
    QDoubleSpinBox *spSourceEEGLP;
    QDoubleSpinBox *spSourceEEGHP;
    QDoubleSpinBox *spSourceMEGLP;
    QDoubleSpinBox *spSourceMEGHP;
    QLabel *labelSourceEEGHP;
    QLabel *labelSourceMEGHP;
    QSpacerItem *horizontalSpacer_36;
    QSpacerItem *horizontalSpacer_35;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_2;

    void setupUi(QWidget *FilterSettingsClass)
    {
        if (FilterSettingsClass->objectName().isEmpty())
            FilterSettingsClass->setObjectName(QStringLiteral("FilterSettingsClass"));
        FilterSettingsClass->resize(706, 439);
        FilterSettingsClass->setMinimumSize(QSize(400, 300));
        FilterSettingsClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        FilterSettingsClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        FilterSettingsClass->setWindowIcon(icon);
        verticalLayout_6 = new QVBoxLayout(FilterSettingsClass);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setSizeConstraint(QLayout::SetMinimumSize);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox_2 = new QGroupBox(FilterSettingsClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMinimumSize(QSize(150, 150));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        checkEegHigh = new QCheckBox(groupBox_2);
        checkEegHigh->setObjectName(QStringLiteral("checkEegHigh"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(checkEegHigh->sizePolicy().hasHeightForWidth());
        checkEegHigh->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(checkEegHigh);

        horizontalSpacer_24 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_24);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_25 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_25);

        spEegHP = new QDoubleSpinBox(groupBox_2);
        spEegHP->setObjectName(QStringLiteral("spEegHP"));
        spEegHP->setEnabled(false);
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(spEegHP->sizePolicy().hasHeightForWidth());
        spEegHP->setSizePolicy(sizePolicy2);
        spEegHP->setDecimals(3);
        spEegHP->setMinimum(0.001);
        spEegHP->setMaximum(1000);
        spEegHP->setValue(1);

        horizontalLayout_8->addWidget(spEegHP);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        checkEegLow = new QCheckBox(groupBox_2);
        checkEegLow->setObjectName(QStringLiteral("checkEegLow"));
        sizePolicy1.setHeightForWidth(checkEegLow->sizePolicy().hasHeightForWidth());
        checkEegLow->setSizePolicy(sizePolicy1);

        horizontalLayout_19->addWidget(checkEegLow);

        horizontalSpacer_26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_26);


        verticalLayout_2->addLayout(horizontalLayout_19);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        horizontalSpacer_27 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_27);

        spEegLP = new QDoubleSpinBox(groupBox_2);
        spEegLP->setObjectName(QStringLiteral("spEegLP"));
        spEegLP->setEnabled(false);
        sizePolicy.setHeightForWidth(spEegLP->sizePolicy().hasHeightForWidth());
        spEegLP->setSizePolicy(sizePolicy);
        spEegLP->setDecimals(3);
        spEegLP->setMinimum(0.001);
        spEegLP->setMaximum(1000);
        spEegLP->setValue(1);

        horizontalLayout_20->addWidget(spEegLP);

        horizontalSpacer_28 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_28);


        verticalLayout_2->addLayout(horizontalLayout_20);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        cbNotchEEG = new QCheckBox(groupBox_2);
        cbNotchEEG->setObjectName(QStringLiteral("cbNotchEEG"));
        sizePolicy1.setHeightForWidth(cbNotchEEG->sizePolicy().hasHeightForWidth());
        cbNotchEEG->setSizePolicy(sizePolicy1);

        horizontalLayout_13->addWidget(cbNotchEEG);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_7);


        verticalLayout_2->addLayout(horizontalLayout_13);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        spNotch = new QDoubleSpinBox(groupBox_2);
        spNotch->setObjectName(QStringLiteral("spNotch"));
        spNotch->setEnabled(false);
        sizePolicy.setHeightForWidth(spNotch->sizePolicy().hasHeightForWidth());
        spNotch->setSizePolicy(sizePolicy);
        spNotch->setDecimals(3);
        spNotch->setMinimum(0.001);
        spNotch->setMaximum(1000);
        spNotch->setValue(50);

        horizontalLayout_6->addWidget(spNotch);

        horizontalSpacer_40 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_40);


        verticalLayout_2->addLayout(horizontalLayout_6);


        gridLayout_2->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(FilterSettingsClass);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        groupBox_3->setMinimumSize(QSize(150, 150));
        verticalLayout_4 = new QVBoxLayout(groupBox_3);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        checkEmgHigh = new QCheckBox(groupBox_3);
        checkEmgHigh->setObjectName(QStringLiteral("checkEmgHigh"));
        sizePolicy1.setHeightForWidth(checkEmgHigh->sizePolicy().hasHeightForWidth());
        checkEmgHigh->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(checkEmgHigh);

        horizontalSpacer_29 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_29);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalSpacer_30 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_30);

        spEmgHP = new QDoubleSpinBox(groupBox_3);
        spEmgHP->setObjectName(QStringLiteral("spEmgHP"));
        spEmgHP->setEnabled(false);
        sizePolicy1.setHeightForWidth(spEmgHP->sizePolicy().hasHeightForWidth());
        spEmgHP->setSizePolicy(sizePolicy1);
        spEmgHP->setDecimals(3);
        spEmgHP->setMinimum(0.001);
        spEmgHP->setMaximum(1000);
        spEmgHP->setValue(1);

        horizontalLayout_10->addWidget(spEmgHP);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_4);


        verticalLayout_4->addLayout(horizontalLayout_10);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        checkEmgLow = new QCheckBox(groupBox_3);
        checkEmgLow->setObjectName(QStringLiteral("checkEmgLow"));
        sizePolicy1.setHeightForWidth(checkEmgLow->sizePolicy().hasHeightForWidth());
        checkEmgLow->setSizePolicy(sizePolicy1);

        horizontalLayout_21->addWidget(checkEmgLow);

        horizontalSpacer_31 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_21->addItem(horizontalSpacer_31);


        verticalLayout_4->addLayout(horizontalLayout_21);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        horizontalSpacer_32 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_22->addItem(horizontalSpacer_32);

        spEmgLP = new QDoubleSpinBox(groupBox_3);
        spEmgLP->setObjectName(QStringLiteral("spEmgLP"));
        spEmgLP->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(spEmgLP->sizePolicy().hasHeightForWidth());
        spEmgLP->setSizePolicy(sizePolicy3);
        spEmgLP->setDecimals(3);
        spEmgLP->setMinimum(0.001);
        spEmgLP->setMaximum(1000);
        spEmgLP->setValue(1);

        horizontalLayout_22->addWidget(spEmgLP);

        horizontalSpacer_33 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_22->addItem(horizontalSpacer_33);


        verticalLayout_4->addLayout(horizontalLayout_22);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        cbNotchEMG = new QCheckBox(groupBox_3);
        cbNotchEMG->setObjectName(QStringLiteral("cbNotchEMG"));
        sizePolicy1.setHeightForWidth(cbNotchEMG->sizePolicy().hasHeightForWidth());
        cbNotchEMG->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(cbNotchEMG);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_8);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_9);

        spNotchEMG = new QDoubleSpinBox(groupBox_3);
        spNotchEMG->setObjectName(QStringLiteral("spNotchEMG"));
        spNotchEMG->setEnabled(false);
        sizePolicy1.setHeightForWidth(spNotchEMG->sizePolicy().hasHeightForWidth());
        spNotchEMG->setSizePolicy(sizePolicy1);
        spNotchEMG->setDecimals(3);
        spNotchEMG->setMinimum(0.001);
        spNotchEMG->setMaximum(1000);
        spNotchEMG->setValue(50);

        horizontalLayout_14->addWidget(spNotchEMG);

        horizontalSpacer_39 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_39);


        verticalLayout_4->addLayout(horizontalLayout_14);


        gridLayout_2->addWidget(groupBox_3, 0, 2, 1, 1);

        groupBox = new QGroupBox(FilterSettingsClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(150, 150));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        checkMegHigh = new QCheckBox(groupBox);
        checkMegHigh->setObjectName(QStringLiteral("checkMegHigh"));
        sizePolicy1.setHeightForWidth(checkMegHigh->sizePolicy().hasHeightForWidth());
        checkMegHigh->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(checkMegHigh);

        horizontalSpacer_19 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_19);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        horizontalSpacer_22 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_22);

        spMegHP = new QDoubleSpinBox(groupBox);
        spMegHP->setObjectName(QStringLiteral("spMegHP"));
        spMegHP->setEnabled(false);
        sizePolicy1.setHeightForWidth(spMegHP->sizePolicy().hasHeightForWidth());
        spMegHP->setSizePolicy(sizePolicy1);
        spMegHP->setDecimals(3);
        spMegHP->setMinimum(0.001);
        spMegHP->setMaximum(1000);
        spMegHP->setValue(1);

        horizontalLayout_16->addWidget(spMegHP);

        horizontalSpacer_23 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_23);


        verticalLayout->addLayout(horizontalLayout_16);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        checkMegLow = new QCheckBox(groupBox);
        checkMegLow->setObjectName(QStringLiteral("checkMegLow"));
        sizePolicy1.setHeightForWidth(checkMegLow->sizePolicy().hasHeightForWidth());
        checkMegLow->setSizePolicy(sizePolicy1);

        horizontalLayout_15->addWidget(checkMegLow);

        horizontalSpacer_21 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_21);


        verticalLayout->addLayout(horizontalLayout_15);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        spMegLP = new QDoubleSpinBox(groupBox);
        spMegLP->setObjectName(QStringLiteral("spMegLP"));
        spMegLP->setEnabled(false);
        sizePolicy1.setHeightForWidth(spMegLP->sizePolicy().hasHeightForWidth());
        spMegLP->setSizePolicy(sizePolicy1);
        spMegLP->setDecimals(3);
        spMegLP->setMinimum(0.001);
        spMegLP->setMaximum(1000);
        spMegLP->setValue(1);

        horizontalLayout_3->addWidget(spMegLP);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_20);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        cbNotchMEG = new QCheckBox(groupBox);
        cbNotchMEG->setObjectName(QStringLiteral("cbNotchMEG"));
        sizePolicy1.setHeightForWidth(cbNotchMEG->sizePolicy().hasHeightForWidth());
        cbNotchMEG->setSizePolicy(sizePolicy1);

        horizontalLayout_17->addWidget(cbNotchMEG);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_10);


        verticalLayout->addLayout(horizontalLayout_17);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_11);

        spNotchMEG = new QDoubleSpinBox(groupBox);
        spNotchMEG->setObjectName(QStringLiteral("spNotchMEG"));
        spNotchMEG->setEnabled(false);
        sizePolicy1.setHeightForWidth(spNotchMEG->sizePolicy().hasHeightForWidth());
        spNotchMEG->setSizePolicy(sizePolicy1);
        spNotchMEG->setDecimals(3);
        spNotchMEG->setMinimum(0.001);
        spNotchMEG->setMaximum(1000);
        spNotchMEG->setValue(50);

        horizontalLayout_18->addWidget(spNotchMEG);

        horizontalSpacer_18 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_18->addItem(horizontalSpacer_18);


        verticalLayout->addLayout(horizontalLayout_18);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);


        verticalLayout_6->addLayout(gridLayout_2);

        groupICA = new QGroupBox(FilterSettingsClass);
        groupICA->setObjectName(QStringLiteral("groupICA"));
        sizePolicy.setHeightForWidth(groupICA->sizePolicy().hasHeightForWidth());
        groupICA->setSizePolicy(sizePolicy);
        groupICA->setMinimumSize(QSize(150, 150));
        verticalLayout_3 = new QVBoxLayout(groupICA);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        spICAEEGLP = new QDoubleSpinBox(groupICA);
        spICAEEGLP->setObjectName(QStringLiteral("spICAEEGLP"));
        sizePolicy1.setHeightForWidth(spICAEEGLP->sizePolicy().hasHeightForWidth());
        spICAEEGLP->setSizePolicy(sizePolicy1);
        spICAEEGLP->setStyleSheet(QStringLiteral("color: blue;"));
        spICAEEGLP->setReadOnly(true);
        spICAEEGLP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spICAEEGLP->setDecimals(3);
        spICAEEGLP->setMaximum(10000);

        gridLayout_4->addWidget(spICAEEGLP, 1, 1, 1, 1);

        spICAMEGLP = new QDoubleSpinBox(groupICA);
        spICAMEGLP->setObjectName(QStringLiteral("spICAMEGLP"));
        sizePolicy1.setHeightForWidth(spICAMEGLP->sizePolicy().hasHeightForWidth());
        spICAMEGLP->setSizePolicy(sizePolicy1);
        spICAMEGLP->setStyleSheet(QStringLiteral("color: blue;"));
        spICAMEGLP->setReadOnly(true);
        spICAMEGLP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spICAMEGLP->setDecimals(3);
        spICAMEGLP->setMaximum(10000);

        gridLayout_4->addWidget(spICAMEGLP, 0, 1, 1, 1);

        labelEEGHP = new QLabel(groupICA);
        labelEEGHP->setObjectName(QStringLiteral("labelEEGHP"));
        sizePolicy1.setHeightForWidth(labelEEGHP->sizePolicy().hasHeightForWidth());
        labelEEGHP->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(labelEEGHP, 1, 3, 1, 1);

        labelMEGLP = new QLabel(groupICA);
        labelMEGLP->setObjectName(QStringLiteral("labelMEGLP"));
        sizePolicy1.setHeightForWidth(labelMEGLP->sizePolicy().hasHeightForWidth());
        labelMEGLP->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(labelMEGLP, 0, 0, 1, 1);

        labelEEGLP = new QLabel(groupICA);
        labelEEGLP->setObjectName(QStringLiteral("labelEEGLP"));
        sizePolicy1.setHeightForWidth(labelEEGLP->sizePolicy().hasHeightForWidth());
        labelEEGLP->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(labelEEGLP, 1, 0, 1, 1);

        spICAEEGHP = new QDoubleSpinBox(groupICA);
        spICAEEGHP->setObjectName(QStringLiteral("spICAEEGHP"));
        sizePolicy1.setHeightForWidth(spICAEEGHP->sizePolicy().hasHeightForWidth());
        spICAEEGHP->setSizePolicy(sizePolicy1);
        spICAEEGHP->setStyleSheet(QStringLiteral("color: blue;"));
        spICAEEGHP->setReadOnly(true);
        spICAEEGHP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spICAEEGHP->setDecimals(3);
        spICAEEGHP->setMaximum(10000);

        gridLayout_4->addWidget(spICAEEGHP, 1, 4, 1, 1);

        labelMEGHP = new QLabel(groupICA);
        labelMEGHP->setObjectName(QStringLiteral("labelMEGHP"));
        sizePolicy1.setHeightForWidth(labelMEGHP->sizePolicy().hasHeightForWidth());
        labelMEGHP->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(labelMEGHP, 0, 3, 1, 1);

        spICAMEGHP = new QDoubleSpinBox(groupICA);
        spICAMEGHP->setObjectName(QStringLiteral("spICAMEGHP"));
        sizePolicy1.setHeightForWidth(spICAMEGHP->sizePolicy().hasHeightForWidth());
        spICAMEGHP->setSizePolicy(sizePolicy1);
        spICAMEGHP->setStyleSheet(QStringLiteral("color: blue;"));
        spICAMEGHP->setReadOnly(true);
        spICAMEGHP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spICAMEGHP->setDecimals(3);
        spICAMEGHP->setMaximum(10000);

        gridLayout_4->addWidget(spICAMEGHP, 0, 4, 1, 1);

        spICAEMGHP = new QDoubleSpinBox(groupICA);
        spICAEMGHP->setObjectName(QStringLiteral("spICAEMGHP"));
        sizePolicy1.setHeightForWidth(spICAEMGHP->sizePolicy().hasHeightForWidth());
        spICAEMGHP->setSizePolicy(sizePolicy1);
        spICAEMGHP->setStyleSheet(QStringLiteral("color: blue;"));
        spICAEMGHP->setReadOnly(true);
        spICAEMGHP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spICAEMGHP->setDecimals(3);
        spICAEMGHP->setMaximum(10000);

        gridLayout_4->addWidget(spICAEMGHP, 2, 4, 1, 1);

        labelEMGLP = new QLabel(groupICA);
        labelEMGLP->setObjectName(QStringLiteral("labelEMGLP"));
        sizePolicy1.setHeightForWidth(labelEMGLP->sizePolicy().hasHeightForWidth());
        labelEMGLP->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(labelEMGLP, 2, 0, 1, 1);

        labelEMGHP = new QLabel(groupICA);
        labelEMGHP->setObjectName(QStringLiteral("labelEMGHP"));
        sizePolicy1.setHeightForWidth(labelEMGHP->sizePolicy().hasHeightForWidth());
        labelEMGHP->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(labelEMGHP, 2, 3, 1, 1);

        spICAEMGLP = new QDoubleSpinBox(groupICA);
        spICAEMGLP->setObjectName(QStringLiteral("spICAEMGLP"));
        sizePolicy1.setHeightForWidth(spICAEMGLP->sizePolicy().hasHeightForWidth());
        spICAEMGLP->setSizePolicy(sizePolicy1);
        spICAEMGLP->setStyleSheet(QStringLiteral("color: blue;"));
        spICAEMGLP->setReadOnly(true);
        spICAEMGLP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spICAEMGLP->setDecimals(3);
        spICAEMGLP->setMaximum(10000);

        gridLayout_4->addWidget(spICAEMGLP, 2, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_5, 0, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout_4);


        verticalLayout_6->addWidget(groupICA);

        groupSource = new QGroupBox(FilterSettingsClass);
        groupSource->setObjectName(QStringLiteral("groupSource"));
        sizePolicy1.setHeightForWidth(groupSource->sizePolicy().hasHeightForWidth());
        groupSource->setSizePolicy(sizePolicy1);
        verticalLayout_5 = new QVBoxLayout(groupSource);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        labelSourceMEGLP = new QLabel(groupSource);
        labelSourceMEGLP->setObjectName(QStringLiteral("labelSourceMEGLP"));
        sizePolicy1.setHeightForWidth(labelSourceMEGLP->sizePolicy().hasHeightForWidth());
        labelSourceMEGLP->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(labelSourceMEGLP, 0, 0, 1, 1);

        labelSourceEEGLP = new QLabel(groupSource);
        labelSourceEEGLP->setObjectName(QStringLiteral("labelSourceEEGLP"));
        sizePolicy1.setHeightForWidth(labelSourceEEGLP->sizePolicy().hasHeightForWidth());
        labelSourceEEGLP->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(labelSourceEEGLP, 1, 0, 1, 1);

        spSourceEEGLP = new QDoubleSpinBox(groupSource);
        spSourceEEGLP->setObjectName(QStringLiteral("spSourceEEGLP"));
        spSourceEEGLP->setStyleSheet(QStringLiteral("color: blue;"));
        spSourceEEGLP->setReadOnly(true);
        spSourceEEGLP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spSourceEEGLP->setDecimals(3);
        spSourceEEGLP->setMaximum(10000);

        gridLayout_3->addWidget(spSourceEEGLP, 1, 1, 1, 1);

        spSourceEEGHP = new QDoubleSpinBox(groupSource);
        spSourceEEGHP->setObjectName(QStringLiteral("spSourceEEGHP"));
        spSourceEEGHP->setStyleSheet(QStringLiteral("color: blue;"));
        spSourceEEGHP->setReadOnly(true);
        spSourceEEGHP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spSourceEEGHP->setDecimals(3);
        spSourceEEGHP->setMaximum(10000);

        gridLayout_3->addWidget(spSourceEEGHP, 1, 4, 1, 1);

        spSourceMEGLP = new QDoubleSpinBox(groupSource);
        spSourceMEGLP->setObjectName(QStringLiteral("spSourceMEGLP"));
        spSourceMEGLP->setStyleSheet(QStringLiteral("color: blue;"));
        spSourceMEGLP->setReadOnly(true);
        spSourceMEGLP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spSourceMEGLP->setDecimals(3);
        spSourceMEGLP->setMaximum(10000);

        gridLayout_3->addWidget(spSourceMEGLP, 0, 1, 1, 1);

        spSourceMEGHP = new QDoubleSpinBox(groupSource);
        spSourceMEGHP->setObjectName(QStringLiteral("spSourceMEGHP"));
        spSourceMEGHP->setStyleSheet(QStringLiteral("color: blue;"));
        spSourceMEGHP->setReadOnly(true);
        spSourceMEGHP->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spSourceMEGHP->setDecimals(3);
        spSourceMEGHP->setMaximum(10000);

        gridLayout_3->addWidget(spSourceMEGHP, 0, 4, 1, 1);

        labelSourceEEGHP = new QLabel(groupSource);
        labelSourceEEGHP->setObjectName(QStringLiteral("labelSourceEEGHP"));
        sizePolicy1.setHeightForWidth(labelSourceEEGHP->sizePolicy().hasHeightForWidth());
        labelSourceEEGHP->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(labelSourceEEGHP, 1, 3, 1, 1);

        labelSourceMEGHP = new QLabel(groupSource);
        labelSourceMEGHP->setObjectName(QStringLiteral("labelSourceMEGHP"));
        sizePolicy1.setHeightForWidth(labelSourceMEGHP->sizePolicy().hasHeightForWidth());
        labelSourceMEGHP->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(labelSourceMEGHP, 0, 3, 1, 1);

        horizontalSpacer_36 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_36, 1, 2, 1, 1);

        horizontalSpacer_35 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_35, 0, 2, 1, 1);


        verticalLayout_5->addLayout(gridLayout_3);


        verticalLayout_6->addWidget(groupSource);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton_2 = new QPushButton(FilterSettingsClass);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout_6->addLayout(horizontalLayout);

        verticalLayout_6->setStretch(0, 1);
        verticalLayout_6->setStretch(1, 1);
        verticalLayout_6->setStretch(2, 1);
        verticalLayout_6->setStretch(3, 1);

        retranslateUi(FilterSettingsClass);
        QObject::connect(pushButton_2, SIGNAL(clicked()), FilterSettingsClass, SLOT(apply()));
        QObject::connect(checkMegHigh, SIGNAL(toggled(bool)), spMegHP, SLOT(setEnabled(bool)));
        QObject::connect(checkMegLow, SIGNAL(toggled(bool)), spMegLP, SLOT(setEnabled(bool)));
        QObject::connect(cbNotchMEG, SIGNAL(toggled(bool)), spNotchMEG, SLOT(setEnabled(bool)));
        QObject::connect(checkEegHigh, SIGNAL(toggled(bool)), spEegHP, SLOT(setEnabled(bool)));
        QObject::connect(checkEegLow, SIGNAL(toggled(bool)), spEegLP, SLOT(setEnabled(bool)));
        QObject::connect(cbNotchEEG, SIGNAL(toggled(bool)), spNotch, SLOT(setEnabled(bool)));
        QObject::connect(checkEmgHigh, SIGNAL(toggled(bool)), spEmgHP, SLOT(setEnabled(bool)));
        QObject::connect(checkEmgLow, SIGNAL(toggled(bool)), spEmgLP, SLOT(setEnabled(bool)));
        QObject::connect(cbNotchEMG, SIGNAL(toggled(bool)), spNotchEMG, SLOT(setEnabled(bool)));

        pushButton_2->setDefault(true);


        QMetaObject::connectSlotsByName(FilterSettingsClass);
    } // setupUi

    void retranslateUi(QWidget *FilterSettingsClass)
    {
        FilterSettingsClass->setWindowTitle(QApplication::translate("FilterSettingsClass", "Filters", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("FilterSettingsClass", "EEG/SEEG", Q_NULLPTR));
        checkEegHigh->setText(QApplication::translate("FilterSettingsClass", "High pass filter", Q_NULLPTR));
        spEegHP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        checkEegLow->setText(QApplication::translate("FilterSettingsClass", "Low pass filter", Q_NULLPTR));
        spEegLP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        cbNotchEEG->setText(QApplication::translate("FilterSettingsClass", "Notch", Q_NULLPTR));
        spNotch->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("FilterSettingsClass", "EMG/ECG", Q_NULLPTR));
        checkEmgHigh->setText(QApplication::translate("FilterSettingsClass", "High pass filter", Q_NULLPTR));
        spEmgHP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        checkEmgLow->setText(QApplication::translate("FilterSettingsClass", "Low pass filter", Q_NULLPTR));
        spEmgLP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        cbNotchEMG->setText(QApplication::translate("FilterSettingsClass", "Notch", Q_NULLPTR));
        spNotchEMG->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("FilterSettingsClass", "MEG/GRAD", Q_NULLPTR));
        checkMegHigh->setText(QApplication::translate("FilterSettingsClass", "High pass filter", Q_NULLPTR));
        spMegHP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        checkMegLow->setText(QApplication::translate("FilterSettingsClass", "Low pass filter", Q_NULLPTR));
        spMegLP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        cbNotchMEG->setText(QApplication::translate("FilterSettingsClass", "Notch", Q_NULLPTR));
        spNotchMEG->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        groupICA->setTitle(QApplication::translate("FilterSettingsClass", "ICA computation settings", Q_NULLPTR));
        spICAEEGLP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        spICAMEGLP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        labelEEGHP->setText(QApplication::translate("FilterSettingsClass", "EEG High Pass:", Q_NULLPTR));
        labelMEGLP->setText(QApplication::translate("FilterSettingsClass", "MEG Low Pass:", Q_NULLPTR));
        labelEEGLP->setText(QApplication::translate("FilterSettingsClass", "EEG Low Pass:", Q_NULLPTR));
        spICAEEGHP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        labelMEGHP->setText(QApplication::translate("FilterSettingsClass", "MEG High Pass:", Q_NULLPTR));
        spICAMEGHP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        spICAEMGHP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        labelEMGLP->setText(QApplication::translate("FilterSettingsClass", "EMG Low Pass:", Q_NULLPTR));
        labelEMGHP->setText(QApplication::translate("FilterSettingsClass", "EMG High Pass:", Q_NULLPTR));
        spICAEMGLP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        groupSource->setTitle(QApplication::translate("FilterSettingsClass", "Source computation settings", Q_NULLPTR));
        labelSourceMEGLP->setText(QApplication::translate("FilterSettingsClass", "MEG Low Pass:", Q_NULLPTR));
        labelSourceEEGLP->setText(QApplication::translate("FilterSettingsClass", "EEG Low Pass:", Q_NULLPTR));
        spSourceEEGLP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        spSourceEEGHP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        spSourceMEGLP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        spSourceMEGHP->setSuffix(QApplication::translate("FilterSettingsClass", "Hz", Q_NULLPTR));
        labelSourceEEGHP->setText(QApplication::translate("FilterSettingsClass", "EEG High Pass:", Q_NULLPTR));
        labelSourceMEGHP->setText(QApplication::translate("FilterSettingsClass", "MEG High Pass:", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("FilterSettingsClass", "Apply", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FilterSettingsClass: public Ui_FilterSettingsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWFILTERSETTINGS_H
