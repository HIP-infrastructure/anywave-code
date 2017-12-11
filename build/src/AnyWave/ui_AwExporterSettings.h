/********************************************************************************
** Form generated from reading UI file 'AwExporterSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWEXPORTERSETTINGS_H
#define UI_AWEXPORTERSETTINGS_H

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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwExporterSettingsUi
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboWriters;
    QLabel *label_2;
    QLineEdit *lineEditFile;
    QPushButton *buttonFile;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QCheckBox *checkBoxCurrentMontage;
    QPushButton *buttonSelectChannels;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBoxICA;
    QPushButton *buttonSelectICA;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonMarkers;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_6;
    QDoubleSpinBox *sbDownSample;
    QSpacerItem *horizontalSpacer_6;
    QGridLayout *gridLayout_2;
    QLabel *label_9;
    QLabel *label_7;
    QDoubleSpinBox *spinMEGLP;
    QLabel *label_4;
    QDoubleSpinBox *spinMEGHP;
    QLabel *label_8;
    QLabel *label_3;
    QDoubleSpinBox *spinEEGNotch;
    QDoubleSpinBox *spinMEGNotch;
    QDoubleSpinBox *spinEMGHP;
    QDoubleSpinBox *spinEMGLP;
    QDoubleSpinBox *spinEEGLP;
    QDoubleSpinBox *spinEEGHP;
    QDoubleSpinBox *spinEMGNotch;
    QLabel *label_5;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *AwExporterSettingsUi)
    {
        if (AwExporterSettingsUi->objectName().isEmpty())
            AwExporterSettingsUi->setObjectName(QStringLiteral("AwExporterSettingsUi"));
        AwExporterSettingsUi->resize(998, 387);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwExporterSettingsUi->setWindowIcon(icon);
        AwExporterSettingsUi->setModal(true);
        verticalLayout_4 = new QVBoxLayout(AwExporterSettingsUi);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(AwExporterSettingsUi);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        comboWriters = new QComboBox(AwExporterSettingsUi);
        comboWriters->setObjectName(QStringLiteral("comboWriters"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboWriters->sizePolicy().hasHeightForWidth());
        comboWriters->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(comboWriters);

        label_2 = new QLabel(AwExporterSettingsUi);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEditFile = new QLineEdit(AwExporterSettingsUi);
        lineEditFile->setObjectName(QStringLiteral("lineEditFile"));

        horizontalLayout_2->addWidget(lineEditFile);

        buttonFile = new QPushButton(AwExporterSettingsUi);
        buttonFile->setObjectName(QStringLiteral("buttonFile"));
        sizePolicy.setHeightForWidth(buttonFile->sizePolicy().hasHeightForWidth());
        buttonFile->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(buttonFile);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(AwExporterSettingsUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        checkBoxCurrentMontage = new QCheckBox(groupBox);
        checkBoxCurrentMontage->setObjectName(QStringLiteral("checkBoxCurrentMontage"));
        checkBoxCurrentMontage->setChecked(true);

        gridLayout->addWidget(checkBoxCurrentMontage, 0, 0, 1, 1);

        buttonSelectChannels = new QPushButton(groupBox);
        buttonSelectChannels->setObjectName(QStringLiteral("buttonSelectChannels"));
        sizePolicy.setHeightForWidth(buttonSelectChannels->sizePolicy().hasHeightForWidth());
        buttonSelectChannels->setSizePolicy(sizePolicy);

        gridLayout->addWidget(buttonSelectChannels, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        checkBoxICA = new QCheckBox(groupBox);
        checkBoxICA->setObjectName(QStringLiteral("checkBoxICA"));

        gridLayout->addWidget(checkBoxICA, 1, 0, 1, 1);

        buttonSelectICA = new QPushButton(groupBox);
        buttonSelectICA->setObjectName(QStringLiteral("buttonSelectICA"));
        sizePolicy.setHeightForWidth(buttonSelectICA->sizePolicy().hasHeightForWidth());
        buttonSelectICA->setSizePolicy(sizePolicy);

        gridLayout->addWidget(buttonSelectICA, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_3->addWidget(groupBox);

        groupBox_3 = new QGroupBox(AwExporterSettingsUi);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        buttonMarkers = new QPushButton(groupBox_3);
        buttonMarkers->setObjectName(QStringLiteral("buttonMarkers"));

        horizontalLayout_3->addWidget(buttonMarkers);

        horizontalSpacer_5 = new QSpacerItem(257, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);


        verticalLayout_3->addWidget(groupBox_3);


        horizontalLayout_5->addLayout(verticalLayout_3);

        groupBox_2 = new QGroupBox(AwExporterSettingsUi);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_4->addWidget(label_6);

        sbDownSample = new QDoubleSpinBox(groupBox_2);
        sbDownSample->setObjectName(QStringLiteral("sbDownSample"));
        sbDownSample->setDecimals(0);
        sbDownSample->setMaximum(1e+11);

        horizontalLayout_4->addWidget(sbDownSample);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_4);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 0, 3, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_7, 0, 1, 1, 1);

        spinMEGLP = new QDoubleSpinBox(groupBox_2);
        spinMEGLP->setObjectName(QStringLiteral("spinMEGLP"));
        sizePolicy.setHeightForWidth(spinMEGLP->sizePolicy().hasHeightForWidth());
        spinMEGLP->setSizePolicy(sizePolicy);
        spinMEGLP->setDecimals(3);
        spinMEGLP->setMaximum(1e+15);

        gridLayout_2->addWidget(spinMEGLP, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        spinMEGHP = new QDoubleSpinBox(groupBox_2);
        spinMEGHP->setObjectName(QStringLiteral("spinMEGHP"));
        spinMEGHP->setDecimals(3);
        spinMEGHP->setMaximum(1e+15);

        gridLayout_2->addWidget(spinMEGHP, 1, 2, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_8, 0, 2, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        spinEEGNotch = new QDoubleSpinBox(groupBox_2);
        spinEEGNotch->setObjectName(QStringLiteral("spinEEGNotch"));
        spinEEGNotch->setDecimals(1);
        spinEEGNotch->setMaximum(1e+15);

        gridLayout_2->addWidget(spinEEGNotch, 3, 3, 1, 1);

        spinMEGNotch = new QDoubleSpinBox(groupBox_2);
        spinMEGNotch->setObjectName(QStringLiteral("spinMEGNotch"));
        spinMEGNotch->setDecimals(1);
        spinMEGNotch->setMaximum(1e+15);

        gridLayout_2->addWidget(spinMEGNotch, 1, 3, 1, 1);

        spinEMGHP = new QDoubleSpinBox(groupBox_2);
        spinEMGHP->setObjectName(QStringLiteral("spinEMGHP"));
        spinEMGHP->setDecimals(3);
        spinEMGHP->setMaximum(1e+15);

        gridLayout_2->addWidget(spinEMGHP, 5, 2, 1, 1);

        spinEMGLP = new QDoubleSpinBox(groupBox_2);
        spinEMGLP->setObjectName(QStringLiteral("spinEMGLP"));
        spinEMGLP->setDecimals(3);
        spinEMGLP->setMaximum(1e+15);

        gridLayout_2->addWidget(spinEMGLP, 5, 1, 1, 1);

        spinEEGLP = new QDoubleSpinBox(groupBox_2);
        spinEEGLP->setObjectName(QStringLiteral("spinEEGLP"));
        sizePolicy.setHeightForWidth(spinEEGLP->sizePolicy().hasHeightForWidth());
        spinEEGLP->setSizePolicy(sizePolicy);
        spinEEGLP->setDecimals(3);
        spinEEGLP->setMaximum(1e+15);

        gridLayout_2->addWidget(spinEEGLP, 3, 1, 1, 1);

        spinEEGHP = new QDoubleSpinBox(groupBox_2);
        spinEEGHP->setObjectName(QStringLiteral("spinEEGHP"));
        spinEEGHP->setDecimals(3);
        spinEEGHP->setMaximum(1e+15);

        gridLayout_2->addWidget(spinEEGHP, 3, 2, 1, 1);

        spinEMGNotch = new QDoubleSpinBox(groupBox_2);
        spinEMGNotch->setObjectName(QStringLiteral("spinEMGNotch"));
        spinEMGNotch->setDecimals(1);
        spinEMGNotch->setMaximum(1e+15);

        gridLayout_2->addWidget(spinEMGNotch, 5, 3, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 5, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);


        horizontalLayout_5->addWidget(groupBox_2);


        verticalLayout_4->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        pushButton = new QPushButton(AwExporterSettingsUi);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(AwExporterSettingsUi);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout_4->addLayout(horizontalLayout);


        retranslateUi(AwExporterSettingsUi);
        QObject::connect(pushButton, SIGNAL(clicked()), AwExporterSettingsUi, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), AwExporterSettingsUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwExporterSettingsUi);
    } // setupUi

    void retranslateUi(QDialog *AwExporterSettingsUi)
    {
        AwExporterSettingsUi->setWindowTitle(QApplication::translate("AwExporterSettingsUi", "Save Data", Q_NULLPTR));
        label->setText(QApplication::translate("AwExporterSettingsUi", "File Format:", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwExporterSettingsUi", "Save as:", Q_NULLPTR));
        buttonFile->setText(QApplication::translate("AwExporterSettingsUi", "Choose File", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwExporterSettingsUi", "Channels", Q_NULLPTR));
        checkBoxCurrentMontage->setText(QApplication::translate("AwExporterSettingsUi", "Export Current Montage or", Q_NULLPTR));
        buttonSelectChannels->setText(QApplication::translate("AwExporterSettingsUi", "Select Channels", Q_NULLPTR));
        checkBoxICA->setText(QApplication::translate("AwExporterSettingsUi", "Export All ICA Channels or", Q_NULLPTR));
        buttonSelectICA->setText(QApplication::translate("AwExporterSettingsUi", "Select Channels", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("AwExporterSettingsUi", "Markers", Q_NULLPTR));
        buttonMarkers->setText(QApplication::translate("AwExporterSettingsUi", "Skip Markers", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AwExporterSettingsUi", "Filtering", Q_NULLPTR));
        label_6->setText(QApplication::translate("AwExporterSettingsUi", "Down sample to :", Q_NULLPTR));
        sbDownSample->setSuffix(QApplication::translate("AwExporterSettingsUi", "Hz", Q_NULLPTR));
        label_9->setText(QApplication::translate("AwExporterSettingsUi", "Notch", Q_NULLPTR));
        label_7->setText(QApplication::translate("AwExporterSettingsUi", "Low Pass", Q_NULLPTR));
        label_4->setText(QApplication::translate("AwExporterSettingsUi", "EEG/SEEG:", Q_NULLPTR));
        label_8->setText(QApplication::translate("AwExporterSettingsUi", "High Pass", Q_NULLPTR));
        label_3->setText(QApplication::translate("AwExporterSettingsUi", "MEG:", Q_NULLPTR));
        label_5->setText(QApplication::translate("AwExporterSettingsUi", "ECG/EMG:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AwExporterSettingsUi", "OK", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("AwExporterSettingsUi", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwExporterSettingsUi: public Ui_AwExporterSettingsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWEXPORTERSETTINGS_H
