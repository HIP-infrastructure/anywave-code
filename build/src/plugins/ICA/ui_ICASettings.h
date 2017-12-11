/********************************************************************************
** Form generated from reading UI file 'ICASettings.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ICASETTINGS_H
#define UI_ICASETTINGS_H

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
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ICASettingsUI
{
public:
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QComboBox *comboAlgo;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QDoubleSpinBox *spinHPF;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_3;
    QDoubleSpinBox *spinLPF;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_4;
    QCheckBox *checkIgnoreMarker;
    QComboBox *comboModality;
    QLabel *modalityLabel;
    QComboBox *comboIgnoredMarkers;
    QCheckBox *ignoreBads;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelNC;
    QSpinBox *spinNC;
    QCheckBox *cbAll;
    QLabel *labelTotalIC;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QLineEdit *saveFileName;
    QPushButton *pickSaveFileName;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *ICASettingsUI)
    {
        if (ICASettingsUI->objectName().isEmpty())
            ICASettingsUI->setObjectName(QStringLiteral("ICASettingsUI"));
        ICASettingsUI->resize(436, 403);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ICASettingsUI->setWindowIcon(icon);
        ICASettingsUI->setSizeGripEnabled(true);
        ICASettingsUI->setModal(true);
        gridLayout_6 = new QGridLayout(ICASettingsUI);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox_4 = new QGroupBox(ICASettingsUI);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_4->addWidget(label_5);

        comboAlgo = new QComboBox(groupBox_4);
        comboAlgo->setObjectName(QStringLiteral("comboAlgo"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboAlgo->sizePolicy().hasHeightForWidth());
        comboAlgo->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(comboAlgo);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);


        gridLayout_5->addLayout(horizontalLayout_4, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_4);

        groupBox = new QGroupBox(ICASettingsUI);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_2);

        spinHPF = new QDoubleSpinBox(groupBox);
        spinHPF->setObjectName(QStringLiteral("spinHPF"));
        sizePolicy.setHeightForWidth(spinHPF->sizePolicy().hasHeightForWidth());
        spinHPF->setSizePolicy(sizePolicy);
        spinHPF->setFrame(true);
        spinHPF->setDecimals(1);
        spinHPF->setMinimum(0);
        spinHPF->setMaximum(1000);

        horizontalLayout->addWidget(spinHPF);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_3);

        spinLPF = new QDoubleSpinBox(groupBox);
        spinLPF->setObjectName(QStringLiteral("spinLPF"));
        sizePolicy.setHeightForWidth(spinLPF->sizePolicy().hasHeightForWidth());
        spinLPF->setSizePolicy(sizePolicy);
        spinLPF->setFrame(true);
        spinLPF->setDecimals(1);
        spinLPF->setMinimum(0);
        spinLPF->setMaximum(1000);

        horizontalLayout->addWidget(spinLPF);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(ICASettingsUI);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        checkIgnoreMarker = new QCheckBox(groupBox_2);
        checkIgnoreMarker->setObjectName(QStringLiteral("checkIgnoreMarker"));
        sizePolicy.setHeightForWidth(checkIgnoreMarker->sizePolicy().hasHeightForWidth());
        checkIgnoreMarker->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(checkIgnoreMarker, 1, 0, 1, 1);

        comboModality = new QComboBox(groupBox_2);
        comboModality->setObjectName(QStringLiteral("comboModality"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboModality->sizePolicy().hasHeightForWidth());
        comboModality->setSizePolicy(sizePolicy2);

        gridLayout_4->addWidget(comboModality, 0, 1, 1, 1);

        modalityLabel = new QLabel(groupBox_2);
        modalityLabel->setObjectName(QStringLiteral("modalityLabel"));
        sizePolicy.setHeightForWidth(modalityLabel->sizePolicy().hasHeightForWidth());
        modalityLabel->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(modalityLabel, 0, 0, 1, 1);

        comboIgnoredMarkers = new QComboBox(groupBox_2);
        comboIgnoredMarkers->setObjectName(QStringLiteral("comboIgnoredMarkers"));
        sizePolicy2.setHeightForWidth(comboIgnoredMarkers->sizePolicy().hasHeightForWidth());
        comboIgnoredMarkers->setSizePolicy(sizePolicy2);

        gridLayout_4->addWidget(comboIgnoredMarkers, 1, 1, 1, 1);

        ignoreBads = new QCheckBox(groupBox_2);
        ignoreBads->setObjectName(QStringLiteral("ignoreBads"));
        sizePolicy.setHeightForWidth(ignoreBads->sizePolicy().hasHeightForWidth());
        ignoreBads->setSizePolicy(sizePolicy);
        ignoreBads->setChecked(true);

        gridLayout_4->addWidget(ignoreBads, 2, 0, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_6, 1, 2, 1, 1);


        gridLayout_3->addLayout(gridLayout_4, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(ICASettingsUI);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        labelNC = new QLabel(groupBox_3);
        labelNC->setObjectName(QStringLiteral("labelNC"));
        sizePolicy.setHeightForWidth(labelNC->sizePolicy().hasHeightForWidth());
        labelNC->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(labelNC);

        spinNC = new QSpinBox(groupBox_3);
        spinNC->setObjectName(QStringLiteral("spinNC"));
        sizePolicy.setHeightForWidth(spinNC->sizePolicy().hasHeightForWidth());
        spinNC->setSizePolicy(sizePolicy);
        spinNC->setMinimum(1);
        spinNC->setMaximum(999);
        spinNC->setSingleStep(10);
        spinNC->setValue(50);

        horizontalLayout_2->addWidget(spinNC);

        cbAll = new QCheckBox(groupBox_3);
        cbAll->setObjectName(QStringLiteral("cbAll"));
        sizePolicy.setHeightForWidth(cbAll->sizePolicy().hasHeightForWidth());
        cbAll->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(cbAll);

        labelTotalIC = new QLabel(groupBox_3);
        labelTotalIC->setObjectName(QStringLiteral("labelTotalIC"));
        sizePolicy.setHeightForWidth(labelTotalIC->sizePolicy().hasHeightForWidth());
        labelTotalIC->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(labelTotalIC);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(label_4);

        saveFileName = new QLineEdit(groupBox_3);
        saveFileName->setObjectName(QStringLiteral("saveFileName"));

        horizontalLayout_3->addWidget(saveFileName);

        pickSaveFileName = new QPushButton(groupBox_3);
        pickSaveFileName->setObjectName(QStringLiteral("pickSaveFileName"));
        sizePolicy.setHeightForWidth(pickSaveFileName->sizePolicy().hasHeightForWidth());
        pickSaveFileName->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pickSaveFileName);


        verticalLayout->addLayout(horizontalLayout_3);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(17, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        pushButton = new QPushButton(ICASettingsUI);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_6->addWidget(pushButton);

        pushButton_2 = new QPushButton(ICASettingsUI);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_6->addWidget(pushButton_2);


        verticalLayout_2->addLayout(horizontalLayout_6);


        gridLayout_6->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(ICASettingsUI);
        QObject::connect(pushButton, SIGNAL(clicked()), ICASettingsUI, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), ICASettingsUI, SLOT(reject()));
        QObject::connect(cbAll, SIGNAL(toggled(bool)), spinNC, SLOT(setDisabled(bool)));
        QObject::connect(cbAll, SIGNAL(toggled(bool)), labelTotalIC, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(ICASettingsUI);
    } // setupUi

    void retranslateUi(QDialog *ICASettingsUI)
    {
        ICASettingsUI->setWindowTitle(QApplication::translate("ICASettingsUI", "ICA Settings", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("ICASettingsUI", "Algorithm", Q_NULLPTR));
        label_5->setText(QApplication::translate("ICASettingsUI", "Use:", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("ICASettingsUI", "Data Filtering", Q_NULLPTR));
        label_2->setText(QApplication::translate("ICASettingsUI", "High Pass:", Q_NULLPTR));
        spinHPF->setSuffix(QApplication::translate("ICASettingsUI", "Hz", Q_NULLPTR));
        label_3->setText(QApplication::translate("ICASettingsUI", "Low Pass:", Q_NULLPTR));
        spinLPF->setSuffix(QApplication::translate("ICASettingsUI", "Hz", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("ICASettingsUI", "Data Input", Q_NULLPTR));
        checkIgnoreMarker->setText(QApplication::translate("ICASettingsUI", "Ignore selection markers labeled", Q_NULLPTR));
        modalityLabel->setText(QApplication::translate("ICASettingsUI", "Modality:", Q_NULLPTR));
        ignoreBads->setText(QApplication::translate("ICASettingsUI", "Ignore channels marked as bad", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("ICASettingsUI", "Output", Q_NULLPTR));
        labelNC->setText(QApplication::translate("ICASettingsUI", "Number of components: ", Q_NULLPTR));
        cbAll->setText(QApplication::translate("ICASettingsUI", "All components", Q_NULLPTR));
        labelTotalIC->setText(QApplication::translate("ICASettingsUI", "(248)", Q_NULLPTR));
        label_4->setText(QApplication::translate("ICASettingsUI", "ICA file:", Q_NULLPTR));
        pickSaveFileName->setText(QApplication::translate("ICASettingsUI", "Choose...", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ICASettingsUI", "Compute", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("ICASettingsUI", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ICASettingsUI: public Ui_ICASettingsUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ICASETTINGS_H
