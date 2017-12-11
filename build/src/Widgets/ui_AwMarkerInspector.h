/********************************************************************************
** Form generated from reading UI file 'AwMarkerInspector.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMARKERINSPECTOR_H
#define UI_AWMARKERINSPECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwMarkerInspectorUI
{
public:
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioSelection;
    QRadioButton *radioSingle;
    QCheckBox *checkPredefined;
    QLabel *label;
    QComboBox *comboBoxColor;
    QSpacerItem *horizontalSpacer_5;
    QGroupBox *groupBoxPredefined;
    QVBoxLayout *verticalLayout;
    QTableWidget *table;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonAddMarker;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonRemoveSelection;
    QPushButton *buttonClearList;
    QGroupBox *groupBoxSettings;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioGlobal;
    QRadioButton *radioAutoDetect;
    QRadioButton *radioSpecific;
    QSpacerItem *horizontalSpacer_6;
    QListWidget *listWidgetTargets;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacerTargets;
    QPushButton *pushButton;
    QPushButton *buttonAddChannels;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labelName;
    QLineEdit *lineEditName;
    QCheckBox *checkAutoInc;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout_7;
    QLabel *labelValue;
    QDoubleSpinBox *spinValue;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBoxShortcuts;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_9;
    QCheckBox *checkBindingActive;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_10;
    QLabel *labelBrowse;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_11;
    QRadioButton *radioLabels;
    QComboBox *comboLabels;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_12;
    QRadioButton *radioValues;
    QComboBox *comboValues;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *AwMarkerInspectorUI)
    {
        if (AwMarkerInspectorUI->objectName().isEmpty())
            AwMarkerInspectorUI->setObjectName(QStringLiteral("AwMarkerInspectorUI"));
        AwMarkerInspectorUI->resize(572, 890);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AwMarkerInspectorUI->sizePolicy().hasHeightForWidth());
        AwMarkerInspectorUI->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwMarkerInspectorUI->setWindowIcon(icon);
        verticalLayout_5 = new QVBoxLayout(AwMarkerInspectorUI);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox = new QGroupBox(AwMarkerInspectorUI);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        radioSelection = new QRadioButton(groupBox);
        radioSelection->setObjectName(QStringLiteral("radioSelection"));
        sizePolicy.setHeightForWidth(radioSelection->sizePolicy().hasHeightForWidth());
        radioSelection->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(radioSelection);

        radioSingle = new QRadioButton(groupBox);
        radioSingle->setObjectName(QStringLiteral("radioSingle"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(radioSingle->sizePolicy().hasHeightForWidth());
        radioSingle->setSizePolicy(sizePolicy1);
        radioSingle->setChecked(true);

        horizontalLayout->addWidget(radioSingle);

        checkPredefined = new QCheckBox(groupBox);
        checkPredefined->setObjectName(QStringLiteral("checkPredefined"));
        sizePolicy.setHeightForWidth(checkPredefined->sizePolicy().hasHeightForWidth());
        checkPredefined->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(checkPredefined);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        comboBoxColor = new QComboBox(groupBox);
        comboBoxColor->setObjectName(QStringLiteral("comboBoxColor"));

        horizontalLayout->addWidget(comboBoxColor);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_5->addWidget(groupBox);

        groupBoxPredefined = new QGroupBox(AwMarkerInspectorUI);
        groupBoxPredefined->setObjectName(QStringLiteral("groupBoxPredefined"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBoxPredefined->sizePolicy().hasHeightForWidth());
        groupBoxPredefined->setSizePolicy(sizePolicy2);
        verticalLayout = new QVBoxLayout(groupBoxPredefined);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        table = new QTableWidget(groupBoxPredefined);
        if (table->columnCount() < 2)
            table->setColumnCount(2);
        table->setObjectName(QStringLiteral("table"));
        table->setMinimumSize(QSize(0, 150));
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setAlternatingRowColors(true);
        table->setSelectionMode(QAbstractItemView::MultiSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setColumnCount(2);
        table->horizontalHeader()->setVisible(true);
        table->horizontalHeader()->setCascadingSectionResizes(false);
        table->horizontalHeader()->setDefaultSectionSize(200);
        table->horizontalHeader()->setMinimumSectionSize(30);
        table->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        table->horizontalHeader()->setStretchLastSection(true);
        table->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(table);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        buttonAddMarker = new QPushButton(groupBoxPredefined);
        buttonAddMarker->setObjectName(QStringLiteral("buttonAddMarker"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buttonAddMarker->sizePolicy().hasHeightForWidth());
        buttonAddMarker->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(buttonAddMarker);

        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        buttonRemoveSelection = new QPushButton(groupBoxPredefined);
        buttonRemoveSelection->setObjectName(QStringLiteral("buttonRemoveSelection"));
        sizePolicy3.setHeightForWidth(buttonRemoveSelection->sizePolicy().hasHeightForWidth());
        buttonRemoveSelection->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(buttonRemoveSelection);

        buttonClearList = new QPushButton(groupBoxPredefined);
        buttonClearList->setObjectName(QStringLiteral("buttonClearList"));
        sizePolicy3.setHeightForWidth(buttonClearList->sizePolicy().hasHeightForWidth());
        buttonClearList->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(buttonClearList);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_5->addWidget(groupBoxPredefined);

        groupBoxSettings = new QGroupBox(AwMarkerInspectorUI);
        groupBoxSettings->setObjectName(QStringLiteral("groupBoxSettings"));
        sizePolicy2.setHeightForWidth(groupBoxSettings->sizePolicy().hasHeightForWidth());
        groupBoxSettings->setSizePolicy(sizePolicy2);
        verticalLayout_3 = new QVBoxLayout(groupBoxSettings);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        radioGlobal = new QRadioButton(groupBoxSettings);
        radioGlobal->setObjectName(QStringLiteral("radioGlobal"));
        sizePolicy.setHeightForWidth(radioGlobal->sizePolicy().hasHeightForWidth());
        radioGlobal->setSizePolicy(sizePolicy);
        radioGlobal->setChecked(true);

        horizontalLayout_3->addWidget(radioGlobal);

        radioAutoDetect = new QRadioButton(groupBoxSettings);
        radioAutoDetect->setObjectName(QStringLiteral("radioAutoDetect"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(radioAutoDetect->sizePolicy().hasHeightForWidth());
        radioAutoDetect->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(radioAutoDetect);

        radioSpecific = new QRadioButton(groupBoxSettings);
        radioSpecific->setObjectName(QStringLiteral("radioSpecific"));
        sizePolicy.setHeightForWidth(radioSpecific->sizePolicy().hasHeightForWidth());
        radioSpecific->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(radioSpecific);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout_3->addLayout(horizontalLayout_3);

        listWidgetTargets = new QListWidget(groupBoxSettings);
        listWidgetTargets->setObjectName(QStringLiteral("listWidgetTargets"));
        sizePolicy3.setHeightForWidth(listWidgetTargets->sizePolicy().hasHeightForWidth());
        listWidgetTargets->setSizePolicy(sizePolicy3);
        listWidgetTargets->setMinimumSize(QSize(0, 150));

        verticalLayout_3->addWidget(listWidgetTargets);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacerTargets = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacerTargets);

        pushButton = new QPushButton(groupBoxSettings);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy4.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy4);

        horizontalLayout_4->addWidget(pushButton);

        buttonAddChannels = new QPushButton(groupBoxSettings);
        buttonAddChannels->setObjectName(QStringLiteral("buttonAddChannels"));
        sizePolicy.setHeightForWidth(buttonAddChannels->sizePolicy().hasHeightForWidth());
        buttonAddChannels->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(buttonAddChannels);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        labelName = new QLabel(groupBoxSettings);
        labelName->setObjectName(QStringLiteral("labelName"));
        sizePolicy.setHeightForWidth(labelName->sizePolicy().hasHeightForWidth());
        labelName->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(labelName);

        lineEditName = new QLineEdit(groupBoxSettings);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));
        sizePolicy.setHeightForWidth(lineEditName->sizePolicy().hasHeightForWidth());
        lineEditName->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(lineEditName);

        checkAutoInc = new QCheckBox(groupBoxSettings);
        checkAutoInc->setObjectName(QStringLiteral("checkAutoInc"));
        sizePolicy.setHeightForWidth(checkAutoInc->sizePolicy().hasHeightForWidth());
        checkAutoInc->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(checkAutoInc);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));

        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        labelValue = new QLabel(groupBoxSettings);
        labelValue->setObjectName(QStringLiteral("labelValue"));
        sizePolicy.setHeightForWidth(labelValue->sizePolicy().hasHeightForWidth());
        labelValue->setSizePolicy(sizePolicy);

        horizontalLayout_7->addWidget(labelValue);

        spinValue = new QDoubleSpinBox(groupBoxSettings);
        spinValue->setObjectName(QStringLiteral("spinValue"));
        sizePolicy.setHeightForWidth(spinValue->sizePolicy().hasHeightForWidth());
        spinValue->setSizePolicy(sizePolicy);
        spinValue->setMinimum(-1);
        spinValue->setMaximum(1e+27);
        spinValue->setValue(-1);

        horizontalLayout_7->addWidget(spinValue);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_7);


        verticalLayout_5->addWidget(groupBoxSettings);

        groupBoxShortcuts = new QGroupBox(AwMarkerInspectorUI);
        groupBoxShortcuts->setObjectName(QStringLiteral("groupBoxShortcuts"));
        sizePolicy2.setHeightForWidth(groupBoxShortcuts->sizePolicy().hasHeightForWidth());
        groupBoxShortcuts->setSizePolicy(sizePolicy2);
        verticalLayout_4 = new QVBoxLayout(groupBoxShortcuts);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        checkBindingActive = new QCheckBox(groupBoxShortcuts);
        checkBindingActive->setObjectName(QStringLiteral("checkBindingActive"));
        sizePolicy3.setHeightForWidth(checkBindingActive->sizePolicy().hasHeightForWidth());
        checkBindingActive->setSizePolicy(sizePolicy3);

        horizontalLayout_9->addWidget(checkBindingActive);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_2);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        labelBrowse = new QLabel(groupBoxShortcuts);
        labelBrowse->setObjectName(QStringLiteral("labelBrowse"));

        horizontalLayout_10->addWidget(labelBrowse);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_10);


        verticalLayout_4->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        radioLabels = new QRadioButton(groupBoxShortcuts);
        radioLabels->setObjectName(QStringLiteral("radioLabels"));
        sizePolicy3.setHeightForWidth(radioLabels->sizePolicy().hasHeightForWidth());
        radioLabels->setSizePolicy(sizePolicy3);
        radioLabels->setChecked(true);

        horizontalLayout_11->addWidget(radioLabels);

        comboLabels = new QComboBox(groupBoxShortcuts);
        comboLabels->setObjectName(QStringLiteral("comboLabels"));
        sizePolicy3.setHeightForWidth(comboLabels->sizePolicy().hasHeightForWidth());
        comboLabels->setSizePolicy(sizePolicy3);

        horizontalLayout_11->addWidget(comboLabels);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_8);


        verticalLayout_4->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        radioValues = new QRadioButton(groupBoxShortcuts);
        radioValues->setObjectName(QStringLiteral("radioValues"));
        sizePolicy3.setHeightForWidth(radioValues->sizePolicy().hasHeightForWidth());
        radioValues->setSizePolicy(sizePolicy3);

        horizontalLayout_12->addWidget(radioValues);

        comboValues = new QComboBox(groupBoxShortcuts);
        comboValues->setObjectName(QStringLiteral("comboValues"));
        sizePolicy3.setHeightForWidth(comboValues->sizePolicy().hasHeightForWidth());
        comboValues->setSizePolicy(sizePolicy3);

        horizontalLayout_12->addWidget(comboValues);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_9);


        verticalLayout_4->addLayout(horizontalLayout_12);


        verticalLayout_5->addWidget(groupBoxShortcuts);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        retranslateUi(AwMarkerInspectorUI);
        QObject::connect(radioGlobal, SIGNAL(toggled(bool)), listWidgetTargets, SLOT(setHidden(bool)));
        QObject::connect(radioSpecific, SIGNAL(toggled(bool)), listWidgetTargets, SLOT(setVisible(bool)));
        QObject::connect(radioGlobal, SIGNAL(toggled(bool)), buttonAddChannels, SLOT(setHidden(bool)));
        QObject::connect(radioSpecific, SIGNAL(toggled(bool)), buttonAddChannels, SLOT(setVisible(bool)));
        QObject::connect(checkPredefined, SIGNAL(toggled(bool)), groupBoxPredefined, SLOT(setVisible(bool)));
        QObject::connect(buttonAddMarker, SIGNAL(clicked()), AwMarkerInspectorUI, SLOT(addPredefinedMarker()));
        QObject::connect(buttonClearList, SIGNAL(clicked()), AwMarkerInspectorUI, SLOT(clearPredefinedMarkers()));
        QObject::connect(buttonRemoveSelection, SIGNAL(clicked()), AwMarkerInspectorUI, SLOT(removeSelectedPredefinedMarkers()));
        QObject::connect(checkPredefined, SIGNAL(toggled(bool)), AwMarkerInspectorUI, SLOT(changeUsePredefined(bool)));
        QObject::connect(buttonAddChannels, SIGNAL(clicked()), AwMarkerInspectorUI, SLOT(addTargets()));
        QObject::connect(radioSpecific, SIGNAL(toggled(bool)), AwMarkerInspectorUI, SLOT(changeTargetChannels(bool)));
        QObject::connect(radioGlobal, SIGNAL(toggled(bool)), AwMarkerInspectorUI, SLOT(changeTargetChannels(bool)));
        QObject::connect(checkBindingActive, SIGNAL(toggled(bool)), radioLabels, SLOT(setVisible(bool)));
        QObject::connect(checkBindingActive, SIGNAL(toggled(bool)), comboLabels, SLOT(setVisible(bool)));
        QObject::connect(checkBindingActive, SIGNAL(toggled(bool)), labelBrowse, SLOT(setVisible(bool)));
        QObject::connect(checkBindingActive, SIGNAL(toggled(bool)), radioValues, SLOT(setVisible(bool)));
        QObject::connect(checkBindingActive, SIGNAL(toggled(bool)), comboValues, SLOT(setVisible(bool)));
        QObject::connect(checkBindingActive, SIGNAL(toggled(bool)), AwMarkerInspectorUI, SLOT(changeUseShortcut(bool)));
        QObject::connect(radioLabels, SIGNAL(toggled(bool)), AwMarkerInspectorUI, SLOT(changeKeyBinding()));
        QObject::connect(radioValues, SIGNAL(toggled(bool)), AwMarkerInspectorUI, SLOT(changeKeyBinding()));
        QObject::connect(comboLabels, SIGNAL(currentIndexChanged(QString)), AwMarkerInspectorUI, SLOT(changeComboBinding()));
        QObject::connect(comboValues, SIGNAL(currentIndexChanged(QString)), AwMarkerInspectorUI, SLOT(changeComboBinding()));
        QObject::connect(pushButton, SIGNAL(clicked()), AwMarkerInspectorUI, SLOT(clearTargets()));
        QObject::connect(radioAutoDetect, SIGNAL(toggled(bool)), listWidgetTargets, SLOT(setHidden(bool)));
        QObject::connect(radioAutoDetect, SIGNAL(toggled(bool)), pushButton, SLOT(setHidden(bool)));
        QObject::connect(radioGlobal, SIGNAL(toggled(bool)), pushButton, SLOT(setHidden(bool)));
        QObject::connect(radioAutoDetect, SIGNAL(toggled(bool)), buttonAddChannels, SLOT(setHidden(bool)));
        QObject::connect(radioAutoDetect, SIGNAL(toggled(bool)), AwMarkerInspectorUI, SLOT(changeTargetChannels(bool)));

        QMetaObject::connectSlotsByName(AwMarkerInspectorUI);
    } // setupUi

    void retranslateUi(QWidget *AwMarkerInspectorUI)
    {
        AwMarkerInspectorUI->setWindowTitle(QApplication::translate("AwMarkerInspectorUI", "Adding Markers", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwMarkerInspectorUI", "Marker type", Q_NULLPTR));
        radioSelection->setText(QApplication::translate("AwMarkerInspectorUI", "Selection", Q_NULLPTR));
        radioSingle->setText(QApplication::translate("AwMarkerInspectorUI", "Single", Q_NULLPTR));
        checkPredefined->setText(QApplication::translate("AwMarkerInspectorUI", "Use predefined markers", Q_NULLPTR));
        label->setText(QApplication::translate("AwMarkerInspectorUI", "Color:", Q_NULLPTR));
        groupBoxPredefined->setTitle(QApplication::translate("AwMarkerInspectorUI", "Predefined markers", Q_NULLPTR));
        buttonAddMarker->setText(QApplication::translate("AwMarkerInspectorUI", "Add Marker", Q_NULLPTR));
        buttonRemoveSelection->setText(QApplication::translate("AwMarkerInspectorUI", "Remove Selection", Q_NULLPTR));
        buttonClearList->setText(QApplication::translate("AwMarkerInspectorUI", "Clear", Q_NULLPTR));
        groupBoxSettings->setTitle(QApplication::translate("AwMarkerInspectorUI", "Properties", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        radioGlobal->setToolTip(QApplication::translate("AwMarkerInspectorUI", "Marker will be global", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        radioGlobal->setText(QApplication::translate("AwMarkerInspectorUI", "Global marker", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        radioAutoDetect->setToolTip(QApplication::translate("AwMarkerInspectorUI", "Mouse click over a channel to make it the target for the marker", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        radioAutoDetect->setText(QApplication::translate("AwMarkerInspectorUI", "Auto detect", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        radioSpecific->setToolTip(QApplication::translate("AwMarkerInspectorUI", "Marker will target specific channel(s)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        radioSpecific->setText(QApplication::translate("AwMarkerInspectorUI", "Channel specifc", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        pushButton->setToolTip(QApplication::translate("AwMarkerInspectorUI", "Clear actual targets", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pushButton->setText(QApplication::translate("AwMarkerInspectorUI", "Clear", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonAddChannels->setToolTip(QApplication::translate("AwMarkerInspectorUI", "Add channel(s) as targets for the marker", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonAddChannels->setText(QApplication::translate("AwMarkerInspectorUI", "Add Channels", Q_NULLPTR));
        labelName->setText(QApplication::translate("AwMarkerInspectorUI", "Label:", Q_NULLPTR));
        checkAutoInc->setText(QApplication::translate("AwMarkerInspectorUI", "Auto increment", Q_NULLPTR));
        labelValue->setText(QApplication::translate("AwMarkerInspectorUI", "Value:", Q_NULLPTR));
        groupBoxShortcuts->setTitle(QApplication::translate("AwMarkerInspectorUI", "Keyboard shortcuts", Q_NULLPTR));
        checkBindingActive->setText(QApplication::translate("AwMarkerInspectorUI", "Use the spacebar", Q_NULLPTR));
        labelBrowse->setText(QApplication::translate("AwMarkerInspectorUI", "Browse through:", Q_NULLPTR));
        radioLabels->setText(QApplication::translate("AwMarkerInspectorUI", "Marker's label:", Q_NULLPTR));
        radioValues->setText(QApplication::translate("AwMarkerInspectorUI", "Marker's value:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwMarkerInspectorUI: public Ui_AwMarkerInspectorUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMARKERINSPECTOR_H
