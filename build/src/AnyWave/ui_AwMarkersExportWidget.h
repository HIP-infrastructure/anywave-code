/********************************************************************************
** Form generated from reading UI file 'AwMarkersExportWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMARKERSEXPORTWIDGET_H
#define UI_AWMARKERSEXPORTWIDGET_H

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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwMarkersExportWidgetUi
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QRadioButton *radioSelected;
    QRadioButton *radioAll;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QSpinBox *spinBefore;
    QSpinBox *spinAfter;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkApplyToSelection;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *gridLayout_2;
    QRadioButton *radioCurrentFilters;
    QRadioButton *radioRawData;
    QSpacerItem *horizontalSpacer_5;
    QRadioButton *radioCurrentMontage;
    QRadioButton *radioAllChannels;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioMATLAB;
    QRadioButton *radioADES;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *buttonSelectFile;
    QLineEdit *lineEditFile;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_2;

    void setupUi(QDialog *AwMarkersExportWidgetUi)
    {
        if (AwMarkersExportWidgetUi->objectName().isEmpty())
            AwMarkersExportWidgetUi->setObjectName(QStringLiteral("AwMarkersExportWidgetUi"));
        AwMarkersExportWidgetUi->resize(566, 285);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwMarkersExportWidgetUi->setWindowIcon(icon);
        verticalLayout_4 = new QVBoxLayout(AwMarkersExportWidgetUi);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        groupBox = new QGroupBox(AwMarkersExportWidgetUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        radioSelected = new QRadioButton(groupBox);
        radioSelected->setObjectName(QStringLiteral("radioSelected"));

        horizontalLayout->addWidget(radioSelected);

        radioAll = new QRadioButton(groupBox);
        radioAll->setObjectName(QStringLiteral("radioAll"));

        horizontalLayout->addWidget(radioAll);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 2, 1, 1);

        spinBefore = new QSpinBox(groupBox);
        spinBefore->setObjectName(QStringLiteral("spinBefore"));
        spinBefore->setMaximum(999999999);
        spinBefore->setSingleStep(100);

        gridLayout->addWidget(spinBefore, 1, 1, 1, 1);

        spinAfter = new QSpinBox(groupBox);
        spinAfter->setObjectName(QStringLiteral("spinAfter"));
        spinAfter->setMaximum(999999999);
        spinAfter->setSingleStep(100);

        gridLayout->addWidget(spinAfter, 1, 3, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 4, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 3);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        checkApplyToSelection = new QCheckBox(groupBox);
        checkApplyToSelection->setObjectName(QStringLiteral("checkApplyToSelection"));

        horizontalLayout_2->addWidget(checkApplyToSelection);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_5->addWidget(groupBox);

        groupBox_2 = new QGroupBox(AwMarkersExportWidgetUi);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        radioCurrentFilters = new QRadioButton(groupBox_2);
        buttonGroup_2 = new QButtonGroup(AwMarkersExportWidgetUi);
        buttonGroup_2->setObjectName(QStringLiteral("buttonGroup_2"));
        buttonGroup_2->addButton(radioCurrentFilters);
        radioCurrentFilters->setObjectName(QStringLiteral("radioCurrentFilters"));
        radioCurrentFilters->setChecked(true);

        gridLayout_2->addWidget(radioCurrentFilters, 1, 0, 1, 1);

        radioRawData = new QRadioButton(groupBox_2);
        buttonGroup_2->addButton(radioRawData);
        radioRawData->setObjectName(QStringLiteral("radioRawData"));

        gridLayout_2->addWidget(radioRawData, 1, 1, 1, 2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 1, 3, 1, 2);

        radioCurrentMontage = new QRadioButton(groupBox_2);
        buttonGroup = new QButtonGroup(AwMarkersExportWidgetUi);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(radioCurrentMontage);
        radioCurrentMontage->setObjectName(QStringLiteral("radioCurrentMontage"));
        radioCurrentMontage->setChecked(true);

        gridLayout_2->addWidget(radioCurrentMontage, 0, 0, 1, 1);

        radioAllChannels = new QRadioButton(groupBox_2);
        buttonGroup->addButton(radioAllChannels);
        radioAllChannels->setObjectName(QStringLiteral("radioAllChannels"));

        gridLayout_2->addWidget(radioAllChannels, 0, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 2, 1, 3);


        verticalLayout_2->addLayout(gridLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 46, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);


        horizontalLayout_5->addWidget(groupBox_2);


        verticalLayout_4->addLayout(horizontalLayout_5);

        groupBox_3 = new QGroupBox(AwMarkersExportWidgetUi);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        radioMATLAB = new QRadioButton(groupBox_3);
        radioMATLAB->setObjectName(QStringLiteral("radioMATLAB"));
        radioMATLAB->setChecked(true);

        horizontalLayout_3->addWidget(radioMATLAB);

        radioADES = new QRadioButton(groupBox_3);
        radioADES->setObjectName(QStringLiteral("radioADES"));

        horizontalLayout_3->addWidget(radioADES);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        buttonSelectFile = new QPushButton(groupBox_3);
        buttonSelectFile->setObjectName(QStringLiteral("buttonSelectFile"));

        horizontalLayout_4->addWidget(buttonSelectFile);

        lineEditFile = new QLineEdit(groupBox_3);
        lineEditFile->setObjectName(QStringLiteral("lineEditFile"));

        horizontalLayout_4->addWidget(lineEditFile);


        verticalLayout_3->addLayout(horizontalLayout_4);

        verticalSpacer_3 = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);


        verticalLayout_4->addWidget(groupBox_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_8);

        pushButton = new QPushButton(AwMarkersExportWidgetUi);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_6->addWidget(pushButton);

        pushButton_2 = new QPushButton(AwMarkersExportWidgetUi);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_6->addWidget(pushButton_2);


        verticalLayout_4->addLayout(horizontalLayout_6);


        retranslateUi(AwMarkersExportWidgetUi);
        QObject::connect(pushButton, SIGNAL(clicked()), AwMarkersExportWidgetUi, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), AwMarkersExportWidgetUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwMarkersExportWidgetUi);
    } // setupUi

    void retranslateUi(QDialog *AwMarkersExportWidgetUi)
    {
        AwMarkersExportWidgetUi->setWindowTitle(QApplication::translate("AwMarkersExportWidgetUi", "Export Marked Data", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwMarkersExportWidgetUi", "Markers Options", Q_NULLPTR));
        label->setText(QApplication::translate("AwMarkersExportWidgetUi", "Export:", Q_NULLPTR));
        radioSelected->setText(QApplication::translate("AwMarkersExportWidgetUi", "Selected", Q_NULLPTR));
        radioAll->setText(QApplication::translate("AwMarkersExportWidgetUi", "All", Q_NULLPTR));
        label_4->setText(QApplication::translate("AwMarkersExportWidgetUi", "After:", Q_NULLPTR));
        spinBefore->setSuffix(QApplication::translate("AwMarkersExportWidgetUi", "ms", Q_NULLPTR));
        spinBefore->setPrefix(QString());
        spinAfter->setSuffix(QApplication::translate("AwMarkersExportWidgetUi", "ms", Q_NULLPTR));
        spinAfter->setPrefix(QString());
        label_3->setText(QApplication::translate("AwMarkersExportWidgetUi", "Before:", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwMarkersExportWidgetUi", "Latencies:", Q_NULLPTR));
        checkApplyToSelection->setText(QApplication::translate("AwMarkersExportWidgetUi", "Apply to selection markers", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AwMarkersExportWidgetUi", "Channels and data", Q_NULLPTR));
        radioCurrentFilters->setText(QApplication::translate("AwMarkersExportWidgetUi", "Current Filters", Q_NULLPTR));
        radioRawData->setText(QApplication::translate("AwMarkersExportWidgetUi", "Raw data", Q_NULLPTR));
        radioCurrentMontage->setText(QApplication::translate("AwMarkersExportWidgetUi", "Current Montage", Q_NULLPTR));
        radioAllChannels->setText(QApplication::translate("AwMarkersExportWidgetUi", "All channels", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("AwMarkersExportWidgetUi", "File Output", Q_NULLPTR));
        radioMATLAB->setText(QApplication::translate("AwMarkersExportWidgetUi", "MATLAB (.mat)", Q_NULLPTR));
        radioADES->setText(QApplication::translate("AwMarkersExportWidgetUi", "ADES (.ades)", Q_NULLPTR));
        buttonSelectFile->setText(QApplication::translate("AwMarkersExportWidgetUi", "Select File", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AwMarkersExportWidgetUi", "Export", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("AwMarkersExportWidgetUi", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwMarkersExportWidgetUi: public Ui_AwMarkersExportWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMARKERSEXPORTWIDGET_H
