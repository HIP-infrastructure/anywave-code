/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
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
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_settingsUi
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidgetIN;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonAddFiles;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_8;
    QComboBox *comboBoxWriters;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEditOutputDir;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioNoFiltering;
    QRadioButton *radioButton_2;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *groupBoxFilters;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_6;
    QLabel *label_4;
    QDoubleSpinBox *spEEGHP;
    QDoubleSpinBox *spMEGHP;
    QLabel *label_5;
    QDoubleSpinBox *spMEGLP;
    QDoubleSpinBox *spEEGNotch;
    QLabel *label_3;
    QLabel *label_7;
    QDoubleSpinBox *spMEGNotch;
    QLabel *label_2;
    QDoubleSpinBox *spEEGLP;
    QLabel *label_9;
    QDoubleSpinBox *spEMGHP;
    QDoubleSpinBox *spEMGLP;
    QDoubleSpinBox *spEMGNotch;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *buttonConvert;

    void setupUi(QDialog *settingsUi)
    {
        if (settingsUi->objectName().isEmpty())
            settingsUi->setObjectName(QStringLiteral("settingsUi"));
        settingsUi->resize(891, 486);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsUi->setWindowIcon(icon);
        verticalLayout_4 = new QVBoxLayout(settingsUi);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        groupBox = new QGroupBox(settingsUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidgetIN = new QTableWidget(groupBox);
        if (tableWidgetIN->columnCount() < 2)
            tableWidgetIN->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetIN->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetIN->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidgetIN->setObjectName(QStringLiteral("tableWidgetIN"));
        tableWidgetIN->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidgetIN->setAlternatingRowColors(true);
        tableWidgetIN->setShowGrid(false);
        tableWidgetIN->setCornerButtonEnabled(false);
        tableWidgetIN->horizontalHeader()->setVisible(true);
        tableWidgetIN->horizontalHeader()->setMinimumSectionSize(100);
        tableWidgetIN->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableWidgetIN);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonAddFiles = new QPushButton(groupBox);
        buttonAddFiles->setObjectName(QStringLiteral("buttonAddFiles"));

        horizontalLayout->addWidget(buttonAddFiles);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_5->addWidget(groupBox);

        groupBox_2 = new QGroupBox(settingsUi);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_6->addWidget(label_8);

        comboBoxWriters = new QComboBox(groupBox_2);
        comboBoxWriters->setObjectName(QStringLiteral("comboBoxWriters"));

        horizontalLayout_6->addWidget(comboBoxWriters);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        lineEditOutputDir = new QLineEdit(groupBox_2);
        lineEditOutputDir->setObjectName(QStringLiteral("lineEditOutputDir"));

        horizontalLayout_2->addWidget(lineEditOutputDir);

        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        radioNoFiltering = new QRadioButton(groupBox_2);
        radioNoFiltering->setObjectName(QStringLiteral("radioNoFiltering"));
        radioNoFiltering->setChecked(true);

        horizontalLayout_3->addWidget(radioNoFiltering);

        radioButton_2 = new QRadioButton(groupBox_2);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        horizontalLayout_3->addWidget(radioButton_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_3);

        groupBoxFilters = new QGroupBox(groupBox_2);
        groupBoxFilters->setObjectName(QStringLiteral("groupBoxFilters"));
        verticalLayout_2 = new QVBoxLayout(groupBoxFilters);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_6 = new QLabel(groupBoxFilters);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 0, 1, 1, 1);

        label_4 = new QLabel(groupBoxFilters);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        spEEGHP = new QDoubleSpinBox(groupBoxFilters);
        spEEGHP->setObjectName(QStringLiteral("spEEGHP"));
        spEEGHP->setMaximum(1e+10);

        gridLayout->addWidget(spEEGHP, 1, 1, 2, 1);

        spMEGHP = new QDoubleSpinBox(groupBoxFilters);
        spMEGHP->setObjectName(QStringLiteral("spMEGHP"));
        spMEGHP->setMaximum(1e+10);

        gridLayout->addWidget(spMEGHP, 3, 1, 1, 1);

        label_5 = new QLabel(groupBoxFilters);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        spMEGLP = new QDoubleSpinBox(groupBoxFilters);
        spMEGLP->setObjectName(QStringLiteral("spMEGLP"));
        spMEGLP->setMaximum(1e+10);

        gridLayout->addWidget(spMEGLP, 3, 2, 1, 1);

        spEEGNotch = new QDoubleSpinBox(groupBoxFilters);
        spEEGNotch->setObjectName(QStringLiteral("spEEGNotch"));
        spEEGNotch->setMaximum(1e+10);

        gridLayout->addWidget(spEEGNotch, 1, 3, 2, 1);

        label_3 = new QLabel(groupBoxFilters);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 2, 1);

        label_7 = new QLabel(groupBoxFilters);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 0, 3, 1, 1);

        spMEGNotch = new QDoubleSpinBox(groupBoxFilters);
        spMEGNotch->setObjectName(QStringLiteral("spMEGNotch"));
        spMEGNotch->setMaximum(1e+10);

        gridLayout->addWidget(spMEGNotch, 3, 3, 1, 1);

        label_2 = new QLabel(groupBoxFilters);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        spEEGLP = new QDoubleSpinBox(groupBoxFilters);
        spEEGLP->setObjectName(QStringLiteral("spEEGLP"));
        spEEGLP->setMaximum(1e+10);

        gridLayout->addWidget(spEEGLP, 1, 2, 2, 1);

        label_9 = new QLabel(groupBoxFilters);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout->addWidget(label_9, 4, 0, 1, 1);

        spEMGHP = new QDoubleSpinBox(groupBoxFilters);
        spEMGHP->setObjectName(QStringLiteral("spEMGHP"));
        spEMGHP->setMaximum(1e+10);

        gridLayout->addWidget(spEMGHP, 4, 1, 1, 1);

        spEMGLP = new QDoubleSpinBox(groupBoxFilters);
        spEMGLP->setObjectName(QStringLiteral("spEMGLP"));
        spEMGLP->setMaximum(1e+10);

        gridLayout->addWidget(spEMGLP, 4, 2, 1, 1);

        spEMGNotch = new QDoubleSpinBox(groupBoxFilters);
        spEMGNotch->setObjectName(QStringLiteral("spEMGNotch"));
        spEMGNotch->setMaximum(1e+10);

        gridLayout->addWidget(spEMGNotch, 4, 3, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


        verticalLayout_3->addWidget(groupBoxFilters);

        verticalSpacer = new QSpacerItem(20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout_5->addWidget(groupBox_2);


        verticalLayout_4->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        buttonConvert = new QPushButton(settingsUi);
        buttonConvert->setObjectName(QStringLiteral("buttonConvert"));
        buttonConvert->setEnabled(false);

        horizontalLayout_4->addWidget(buttonConvert);


        verticalLayout_4->addLayout(horizontalLayout_4);


        retranslateUi(settingsUi);
        QObject::connect(radioNoFiltering, SIGNAL(toggled(bool)), groupBoxFilters, SLOT(setHidden(bool)));
        QObject::connect(radioButton_2, SIGNAL(clicked(bool)), groupBoxFilters, SLOT(setVisible(bool)));
        QObject::connect(comboBoxWriters, SIGNAL(currentIndexChanged(int)), settingsUi, SLOT(changeCurrentWriter(int)));
        QObject::connect(buttonAddFiles, SIGNAL(clicked()), settingsUi, SLOT(addFiles()));
        QObject::connect(pushButton, SIGNAL(clicked()), settingsUi, SLOT(changeOutputDir()));
        QObject::connect(buttonConvert, SIGNAL(clicked()), settingsUi, SLOT(accept()));

        QMetaObject::connectSlotsByName(settingsUi);
    } // setupUi

    void retranslateUi(QDialog *settingsUi)
    {
        settingsUi->setWindowTitle(QApplication::translate("settingsUi", "File Converter Settings", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("settingsUi", "IN", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetIN->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("settingsUi", "File", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetIN->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("settingsUi", "Status", Q_NULLPTR));
        buttonAddFiles->setText(QApplication::translate("settingsUi", "Add Files", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("settingsUi", "OUT", Q_NULLPTR));
        label_8->setText(QApplication::translate("settingsUi", "Writer:", Q_NULLPTR));
        label->setText(QApplication::translate("settingsUi", "Output directory:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("settingsUi", "Change", Q_NULLPTR));
        radioNoFiltering->setText(QApplication::translate("settingsUi", "No Filtering", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("settingsUi", "Apply filters", Q_NULLPTR));
        groupBoxFilters->setTitle(QApplication::translate("settingsUi", "Filters", Q_NULLPTR));
        label_6->setText(QApplication::translate("settingsUi", "HP", Q_NULLPTR));
        label_4->setText(QApplication::translate("settingsUi", "Type", Q_NULLPTR));
        spEEGHP->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        spMEGHP->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        label_5->setText(QApplication::translate("settingsUi", "LP", Q_NULLPTR));
        spMEGLP->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        spEEGNotch->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        label_3->setText(QApplication::translate("settingsUi", "MEG:", Q_NULLPTR));
        label_7->setText(QApplication::translate("settingsUi", "Notch", Q_NULLPTR));
        spMEGNotch->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        label_2->setText(QApplication::translate("settingsUi", "EEG/SEEG:", Q_NULLPTR));
        spEEGLP->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        label_9->setText(QApplication::translate("settingsUi", "EMG/ECG:", Q_NULLPTR));
        spEMGHP->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        spEMGLP->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        spEMGNotch->setSuffix(QApplication::translate("settingsUi", "Hz", Q_NULLPTR));
        buttonConvert->setText(QApplication::translate("settingsUi", "CONVERT", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class settingsUi: public Ui_settingsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
