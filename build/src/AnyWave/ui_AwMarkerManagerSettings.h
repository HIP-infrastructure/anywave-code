/********************************************************************************
** Form generated from reading UI file 'AwMarkerManagerSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMARKERMANAGERSETTINGS_H
#define UI_AWMARKERMANAGERSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Marker/AwMarkerTableView.h"

QT_BEGIN_NAMESPACE

class Ui_MarkersManagerSettingsClass
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    AwMarkerTableView *tvMarkers;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QCheckBox *checkLabel;
    QCheckBox *checkValue;
    QCheckBox *checkPos;
    QCheckBox *checkDuration;
    QCheckBox *checkColor;
    QCheckBox *checkTargets;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *groupBoxDisplayRules;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboFilters;
    QPushButton *buttonEditRules;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBoxQuickNav;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLabel *labelTotal;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QComboBox *comboNames;
    QLabel *labelNames;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *buttonPrevName;
    QPushButton *buttonGoName;
    QPushButton *buttonNextName;
    QLabel *label_4;
    QComboBox *comboValues;
    QLabel *labelValues;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *buttonPrevValue;
    QPushButton *buttonGoValue;
    QPushButton *buttonNextValue;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonExportWizard;
    QPushButton *buttonClearTriggerChannel;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonLoad;
    QPushButton *buttonSave;

    void setupUi(QWidget *MarkersManagerSettingsClass)
    {
        if (MarkersManagerSettingsClass->objectName().isEmpty())
            MarkersManagerSettingsClass->setObjectName(QStringLiteral("MarkersManagerSettingsClass"));
        MarkersManagerSettingsClass->resize(543, 623);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MarkersManagerSettingsClass->sizePolicy().hasHeightForWidth());
        MarkersManagerSettingsClass->setSizePolicy(sizePolicy);
        MarkersManagerSettingsClass->setMinimumSize(QSize(0, 0));
        MarkersManagerSettingsClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        MarkersManagerSettingsClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        MarkersManagerSettingsClass->setWindowIcon(icon);
        verticalLayout_3 = new QVBoxLayout(MarkersManagerSettingsClass);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(3, 3, 3, 3);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        groupBox = new QGroupBox(MarkersManagerSettingsClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(2, 2, 2, 2);
        tvMarkers = new AwMarkerTableView(groupBox);
        tvMarkers->setObjectName(QStringLiteral("tvMarkers"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tvMarkers->sizePolicy().hasHeightForWidth());
        tvMarkers->setSizePolicy(sizePolicy2);
        tvMarkers->setFrameShape(QFrame::NoFrame);
        tvMarkers->setFrameShadow(QFrame::Plain);
        tvMarkers->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
        tvMarkers->setAutoScroll(true);
        tvMarkers->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        tvMarkers->setDragEnabled(false);
        tvMarkers->setDragDropOverwriteMode(false);
        tvMarkers->setDragDropMode(QAbstractItemView::DragDrop);
        tvMarkers->setAlternatingRowColors(true);
        tvMarkers->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tvMarkers->setSelectionBehavior(QAbstractItemView::SelectRows);
        tvMarkers->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
        tvMarkers->setSortingEnabled(true);
        tvMarkers->setCornerButtonEnabled(false);
        tvMarkers->horizontalHeader()->setCascadingSectionResizes(true);
        tvMarkers->horizontalHeader()->setDefaultSectionSize(100);
        tvMarkers->horizontalHeader()->setMinimumSectionSize(100);
        tvMarkers->horizontalHeader()->setStretchLastSection(true);
        tvMarkers->verticalHeader()->setVisible(false);

        gridLayout_4->addWidget(tvMarkers, 1, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_4->addWidget(label_5);

        checkLabel = new QCheckBox(groupBox);
        checkLabel->setObjectName(QStringLiteral("checkLabel"));
        checkLabel->setChecked(true);

        horizontalLayout_4->addWidget(checkLabel);

        checkValue = new QCheckBox(groupBox);
        checkValue->setObjectName(QStringLiteral("checkValue"));

        horizontalLayout_4->addWidget(checkValue);

        checkPos = new QCheckBox(groupBox);
        checkPos->setObjectName(QStringLiteral("checkPos"));
        checkPos->setChecked(true);

        horizontalLayout_4->addWidget(checkPos);

        checkDuration = new QCheckBox(groupBox);
        checkDuration->setObjectName(QStringLiteral("checkDuration"));
        checkDuration->setChecked(true);

        horizontalLayout_4->addWidget(checkDuration);

        checkColor = new QCheckBox(groupBox);
        checkColor->setObjectName(QStringLiteral("checkColor"));

        horizontalLayout_4->addWidget(checkColor);

        checkTargets = new QCheckBox(groupBox);
        checkTargets->setObjectName(QStringLiteral("checkTargets"));

        horizontalLayout_4->addWidget(checkTargets);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        gridLayout_4->addLayout(horizontalLayout_4, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        groupBoxDisplayRules = new QGroupBox(MarkersManagerSettingsClass);
        groupBoxDisplayRules->setObjectName(QStringLiteral("groupBoxDisplayRules"));
        sizePolicy.setHeightForWidth(groupBoxDisplayRules->sizePolicy().hasHeightForWidth());
        groupBoxDisplayRules->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(groupBoxDisplayRules);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(groupBoxDisplayRules);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setScaledContents(true);

        horizontalLayout_2->addWidget(label);

        comboFilters = new QComboBox(groupBoxDisplayRules);
        comboFilters->setObjectName(QStringLiteral("comboFilters"));
        sizePolicy.setHeightForWidth(comboFilters->sizePolicy().hasHeightForWidth());
        comboFilters->setSizePolicy(sizePolicy);
        comboFilters->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout_2->addWidget(comboFilters);

        buttonEditRules = new QPushButton(groupBoxDisplayRules);
        buttonEditRules->setObjectName(QStringLiteral("buttonEditRules"));
        sizePolicy.setHeightForWidth(buttonEditRules->sizePolicy().hasHeightForWidth());
        buttonEditRules->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(buttonEditRules);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBoxDisplayRules);

        groupBoxQuickNav = new QGroupBox(MarkersManagerSettingsClass);
        groupBoxQuickNav->setObjectName(QStringLiteral("groupBoxQuickNav"));
        gridLayout_3 = new QGridLayout(groupBoxQuickNav);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(groupBoxQuickNav);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setScaledContents(true);

        horizontalLayout_3->addWidget(label_2);

        labelTotal = new QLabel(groupBoxQuickNav);
        labelTotal->setObjectName(QStringLiteral("labelTotal"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(labelTotal->sizePolicy().hasHeightForWidth());
        labelTotal->setSizePolicy(sizePolicy3);
        labelTotal->setStyleSheet(QStringLiteral("color: rgb(0, 85, 255);"));

        horizontalLayout_3->addWidget(labelTotal);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_3 = new QLabel(groupBoxQuickNav);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        comboNames = new QComboBox(groupBoxQuickNav);
        comboNames->setObjectName(QStringLiteral("comboNames"));
        sizePolicy3.setHeightForWidth(comboNames->sizePolicy().hasHeightForWidth());
        comboNames->setSizePolicy(sizePolicy3);
        comboNames->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_2->addWidget(comboNames, 0, 1, 1, 1);

        labelNames = new QLabel(groupBoxQuickNav);
        labelNames->setObjectName(QStringLiteral("labelNames"));
        sizePolicy3.setHeightForWidth(labelNames->sizePolicy().hasHeightForWidth());
        labelNames->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(labelNames, 0, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 3, 1, 1);

        buttonPrevName = new QPushButton(groupBoxQuickNav);
        buttonPrevName->setObjectName(QStringLiteral("buttonPrevName"));
        sizePolicy3.setHeightForWidth(buttonPrevName->sizePolicy().hasHeightForWidth());
        buttonPrevName->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(buttonPrevName, 0, 4, 1, 1);

        buttonGoName = new QPushButton(groupBoxQuickNav);
        buttonGoName->setObjectName(QStringLiteral("buttonGoName"));
        sizePolicy3.setHeightForWidth(buttonGoName->sizePolicy().hasHeightForWidth());
        buttonGoName->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(buttonGoName, 0, 5, 1, 1);

        buttonNextName = new QPushButton(groupBoxQuickNav);
        buttonNextName->setObjectName(QStringLiteral("buttonNextName"));
        sizePolicy3.setHeightForWidth(buttonNextName->sizePolicy().hasHeightForWidth());
        buttonNextName->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(buttonNextName, 0, 6, 1, 1);

        label_4 = new QLabel(groupBoxQuickNav);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        comboValues = new QComboBox(groupBoxQuickNav);
        comboValues->setObjectName(QStringLiteral("comboValues"));
        sizePolicy3.setHeightForWidth(comboValues->sizePolicy().hasHeightForWidth());
        comboValues->setSizePolicy(sizePolicy3);
        comboValues->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_2->addWidget(comboValues, 1, 1, 1, 1);

        labelValues = new QLabel(groupBoxQuickNav);
        labelValues->setObjectName(QStringLiteral("labelValues"));
        sizePolicy3.setHeightForWidth(labelValues->sizePolicy().hasHeightForWidth());
        labelValues->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(labelValues, 1, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 1, 3, 1, 1);

        buttonPrevValue = new QPushButton(groupBoxQuickNav);
        buttonPrevValue->setObjectName(QStringLiteral("buttonPrevValue"));
        sizePolicy3.setHeightForWidth(buttonPrevValue->sizePolicy().hasHeightForWidth());
        buttonPrevValue->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(buttonPrevValue, 1, 4, 1, 1);

        buttonGoValue = new QPushButton(groupBoxQuickNav);
        buttonGoValue->setObjectName(QStringLiteral("buttonGoValue"));
        sizePolicy3.setHeightForWidth(buttonGoValue->sizePolicy().hasHeightForWidth());
        buttonGoValue->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(buttonGoValue, 1, 5, 1, 1);

        buttonNextValue = new QPushButton(groupBoxQuickNav);
        buttonNextValue->setObjectName(QStringLiteral("buttonNextValue"));
        sizePolicy3.setHeightForWidth(buttonNextValue->sizePolicy().hasHeightForWidth());
        buttonNextValue->setSizePolicy(sizePolicy3);

        gridLayout_2->addWidget(buttonNextValue, 1, 6, 1, 1);


        verticalLayout->addLayout(gridLayout_2);


        gridLayout_3->addLayout(verticalLayout, 0, 0, 1, 1);


        verticalLayout_2->addWidget(groupBoxQuickNav);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonExportWizard = new QPushButton(MarkersManagerSettingsClass);
        buttonExportWizard->setObjectName(QStringLiteral("buttonExportWizard"));
        sizePolicy3.setHeightForWidth(buttonExportWizard->sizePolicy().hasHeightForWidth());
        buttonExportWizard->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(buttonExportWizard);

        buttonClearTriggerChannel = new QPushButton(MarkersManagerSettingsClass);
        buttonClearTriggerChannel->setObjectName(QStringLiteral("buttonClearTriggerChannel"));
        sizePolicy3.setHeightForWidth(buttonClearTriggerChannel->sizePolicy().hasHeightForWidth());
        buttonClearTriggerChannel->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(buttonClearTriggerChannel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonLoad = new QPushButton(MarkersManagerSettingsClass);
        buttonLoad->setObjectName(QStringLiteral("buttonLoad"));
        sizePolicy3.setHeightForWidth(buttonLoad->sizePolicy().hasHeightForWidth());
        buttonLoad->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(buttonLoad);

        buttonSave = new QPushButton(MarkersManagerSettingsClass);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        sizePolicy3.setHeightForWidth(buttonSave->sizePolicy().hasHeightForWidth());
        buttonSave->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(buttonSave);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(MarkersManagerSettingsClass);

        QMetaObject::connectSlotsByName(MarkersManagerSettingsClass);
    } // setupUi

    void retranslateUi(QWidget *MarkersManagerSettingsClass)
    {
        MarkersManagerSettingsClass->setWindowTitle(QApplication::translate("MarkersManagerSettingsClass", "Markers Manager", Q_NULLPTR));
        groupBox->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        tvMarkers->setToolTip(QApplication::translate("MarkersManagerSettingsClass", "Markers currently shown depending on Display rule used", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("MarkersManagerSettingsClass", "Show:", Q_NULLPTR));
        checkLabel->setText(QApplication::translate("MarkersManagerSettingsClass", "Label", Q_NULLPTR));
        checkValue->setText(QApplication::translate("MarkersManagerSettingsClass", "Value", Q_NULLPTR));
        checkPos->setText(QApplication::translate("MarkersManagerSettingsClass", "Pos.", Q_NULLPTR));
        checkDuration->setText(QApplication::translate("MarkersManagerSettingsClass", "Duration", Q_NULLPTR));
        checkColor->setText(QApplication::translate("MarkersManagerSettingsClass", "Color", Q_NULLPTR));
        checkTargets->setText(QApplication::translate("MarkersManagerSettingsClass", "Targets", Q_NULLPTR));
        groupBoxDisplayRules->setTitle(QApplication::translate("MarkersManagerSettingsClass", "Display rules", Q_NULLPTR));
        label->setText(QApplication::translate("MarkersManagerSettingsClass", "Use rule:", Q_NULLPTR));
        buttonEditRules->setText(QApplication::translate("MarkersManagerSettingsClass", "Edit Rules", Q_NULLPTR));
        groupBoxQuickNav->setTitle(QApplication::translate("MarkersManagerSettingsClass", "Quick navigation", Q_NULLPTR));
        label_2->setText(QApplication::translate("MarkersManagerSettingsClass", "Number of markers:", Q_NULLPTR));
        labelTotal->setText(QString());
        label_3->setText(QApplication::translate("MarkersManagerSettingsClass", "By name:", Q_NULLPTR));
        labelNames->setText(QApplication::translate("MarkersManagerSettingsClass", "TextLabel", Q_NULLPTR));
        buttonPrevName->setText(QApplication::translate("MarkersManagerSettingsClass", "Prev.", Q_NULLPTR));
        buttonGoName->setText(QApplication::translate("MarkersManagerSettingsClass", "Center On", Q_NULLPTR));
        buttonNextName->setText(QApplication::translate("MarkersManagerSettingsClass", "Next", Q_NULLPTR));
        label_4->setText(QApplication::translate("MarkersManagerSettingsClass", "By value:", Q_NULLPTR));
        labelValues->setText(QApplication::translate("MarkersManagerSettingsClass", "TextLabel", Q_NULLPTR));
        buttonPrevValue->setText(QApplication::translate("MarkersManagerSettingsClass", "Prev.", Q_NULLPTR));
        buttonGoValue->setText(QApplication::translate("MarkersManagerSettingsClass", "Center On", Q_NULLPTR));
        buttonNextValue->setText(QApplication::translate("MarkersManagerSettingsClass", "Next", Q_NULLPTR));
        buttonExportWizard->setText(QApplication::translate("MarkersManagerSettingsClass", "Export Wizard", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonClearTriggerChannel->setToolTip(QApplication::translate("MarkersManagerSettingsClass", "Overwrite currently open file's trigger channel with zeros.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonClearTriggerChannel->setText(QApplication::translate("MarkersManagerSettingsClass", "Clear Trigger Channel", Q_NULLPTR));
        buttonLoad->setText(QApplication::translate("MarkersManagerSettingsClass", "Load Markers", Q_NULLPTR));
        buttonSave->setText(QApplication::translate("MarkersManagerSettingsClass", "Save Markers", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MarkersManagerSettingsClass: public Ui_MarkersManagerSettingsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMARKERMANAGERSETTINGS_H
