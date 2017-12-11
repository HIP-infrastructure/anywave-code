/********************************************************************************
** Form generated from reading UI file 'AwExportMarkersWizard.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWEXPORTMARKERSWIZARD_H
#define UI_AWEXPORTMARKERSWIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
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

class Ui_ExportWizardDialClass
{
public:
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout;
    QRadioButton *radioButtonName;
    QComboBox *comboBoxName;
    QSpacerItem *horizontalSpacer_2;
    QRadioButton *radioButtonCode;
    QComboBox *comboBoxCode;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBoxLatency;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QSpinBox *spinBoxPre;
    QSpacerItem *horizontalSpacer;
    QLabel *label_2;
    QSpinBox *spinBoxPost;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ExportWizardDialClass)
    {
        if (ExportWizardDialClass->objectName().isEmpty())
            ExportWizardDialClass->setObjectName(QStringLiteral("ExportWizardDialClass"));
        ExportWizardDialClass->resize(243, 241);
        ExportWizardDialClass->setMinimumSize(QSize(0, 0));
        ExportWizardDialClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        ExportWizardDialClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ExportWizardDialClass->setWindowIcon(icon);
        gridLayout_3 = new QGridLayout(ExportWizardDialClass);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(ExportWizardDialClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        radioButtonName = new QRadioButton(groupBox);
        radioButtonName->setObjectName(QStringLiteral("radioButtonName"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(radioButtonName->sizePolicy().hasHeightForWidth());
        radioButtonName->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(radioButtonName, 0, 0, 1, 1);

        comboBoxName = new QComboBox(groupBox);
        comboBoxName->setObjectName(QStringLiteral("comboBoxName"));
        sizePolicy1.setHeightForWidth(comboBoxName->sizePolicy().hasHeightForWidth());
        comboBoxName->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(comboBoxName, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        radioButtonCode = new QRadioButton(groupBox);
        radioButtonCode->setObjectName(QStringLiteral("radioButtonCode"));
        sizePolicy1.setHeightForWidth(radioButtonCode->sizePolicy().hasHeightForWidth());
        radioButtonCode->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(radioButtonCode, 1, 0, 1, 1);

        comboBoxCode = new QComboBox(groupBox);
        comboBoxCode->setObjectName(QStringLiteral("comboBoxCode"));
        sizePolicy1.setHeightForWidth(comboBoxCode->sizePolicy().hasHeightForWidth());
        comboBoxCode->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(comboBoxCode, 1, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 2, 1, 1);


        gridLayout_5->addLayout(gridLayout, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBoxLatency = new QGroupBox(ExportWizardDialClass);
        groupBoxLatency->setObjectName(QStringLiteral("groupBoxLatency"));
        sizePolicy.setHeightForWidth(groupBoxLatency->sizePolicy().hasHeightForWidth());
        groupBoxLatency->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(groupBoxLatency);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label = new QLabel(groupBoxLatency);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        spinBoxPre = new QSpinBox(groupBoxLatency);
        spinBoxPre->setObjectName(QStringLiteral("spinBoxPre"));
        sizePolicy1.setHeightForWidth(spinBoxPre->sizePolicy().hasHeightForWidth());
        spinBoxPre->setSizePolicy(sizePolicy1);
        spinBoxPre->setMaximum(1000000000);

        gridLayout_2->addWidget(spinBoxPre, 0, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 3, 1, 1);

        label_2 = new QLabel(groupBoxLatency);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        spinBoxPost = new QSpinBox(groupBoxLatency);
        spinBoxPost->setObjectName(QStringLiteral("spinBoxPost"));
        sizePolicy1.setHeightForWidth(spinBoxPost->sizePolicy().hasHeightForWidth());
        spinBoxPost->setSizePolicy(sizePolicy1);
        spinBoxPost->setMaximum(999999999);

        gridLayout_2->addWidget(spinBoxPost, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 1, 2, 1, 2);


        gridLayout_4->addLayout(gridLayout_2, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBoxLatency);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(ExportWizardDialClass);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(ExportWizardDialClass);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        verticalLayout->addLayout(hboxLayout);


        gridLayout_3->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(ExportWizardDialClass);
        QObject::connect(okButton, SIGNAL(clicked()), ExportWizardDialClass, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), ExportWizardDialClass, SLOT(reject()));

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(ExportWizardDialClass);
    } // setupUi

    void retranslateUi(QDialog *ExportWizardDialClass)
    {
        ExportWizardDialClass->setWindowTitle(QApplication::translate("ExportWizardDialClass", "Export Markers", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("ExportWizardDialClass", "Export using markers", Q_NULLPTR));
        radioButtonName->setText(QApplication::translate("ExportWizardDialClass", "Name", Q_NULLPTR));
        radioButtonCode->setText(QApplication::translate("ExportWizardDialClass", "Value", Q_NULLPTR));
        groupBoxLatency->setTitle(QApplication::translate("ExportWizardDialClass", "Latencies", Q_NULLPTR));
        label->setText(QApplication::translate("ExportWizardDialClass", "Before (ms):", Q_NULLPTR));
        label_2->setText(QApplication::translate("ExportWizardDialClass", "After (ms):", Q_NULLPTR));
        okButton->setText(QApplication::translate("ExportWizardDialClass", "Export", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("ExportWizardDialClass", "&Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ExportWizardDialClass: public Ui_ExportWizardDialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWEXPORTMARKERSWIZARD_H
