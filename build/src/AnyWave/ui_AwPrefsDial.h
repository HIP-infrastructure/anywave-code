/********************************************************************************
** Form generated from reading UI file 'AwPrefsDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPREFSDIAL_H
#define UI_AWPREFSDIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialPrefsClass
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonOK;
    QPushButton *buttonClose;
    QTabWidget *tabwidget_Prefs;
    QWidget *tab;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_12;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *comboType;
    QLabel *label_3;
    QPushButton *buttonColor;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_7;
    QLabel *labelMarkerFont;
    QPushButton *buttonMarkerFont;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QPushButton *buttonCursorColor;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QLabel *labelCursorFont;
    QPushButton *buttonCursorFont;
    QHBoxLayout *horizontalLayout_13;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_9;
    QPushButton *buttonMappingColor;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_10;
    QLabel *labelMappingFont;
    QPushButton *buttonMappingFont;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *pushButton_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_11;
    QComboBox *comboLanguage;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QRadioButton *radioTriggerParserOn;
    QRadioButton *radioTriggerParserOff;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_12;
    QRadioButton *radioHMSOff;
    QRadioButton *radioHMSOn;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_15;
    QSlider *sliderCPU;
    QSpinBox *spinCPU;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_2;
    QWidget *tab_3;
    QGridLayout *gridLayout_7;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *plainTextEdit;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_13;
    QLineEdit *lineEditMatlabPath;
    QPushButton *buttonSelectMatlabFolder;
    QHBoxLayout *horizontalLayout_19;
    QLabel *labelMCR;
    QLineEdit *lineEditMCR;
    QPushButton *buttonSelectMCR;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_4;
    QLineEdit *lineEditGARDEL;
    QPushButton *buttonSelectGARDEL;
    QSpacerItem *verticalSpacer_5;
    QWidget *tab_4;
    QGridLayout *gridLayout_12;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_16;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_14;
    QLineEdit *lineEditPythonPath;
    QPushButton *buttonSelectMatlabFolder_2;
    QSpacerItem *verticalSpacer_6;
    QWidget *tab_2;
    QFrame *lineH;
    QFrame *lineV;
    QSpinBox *spinBoxV;
    QSpinBox *spinBoxH;
    QLabel *label;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QButtonGroup *buttonGroup;
    QButtonGroup *buttonGroup_2;

    void setupUi(QDialog *DialPrefsClass)
    {
        if (DialPrefsClass->objectName().isEmpty())
            DialPrefsClass->setObjectName(QStringLiteral("DialPrefsClass"));
        DialPrefsClass->resize(957, 529);
        DialPrefsClass->setMinimumSize(QSize(340, 345));
        QFont font;
        font.setPointSize(10);
        DialPrefsClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        DialPrefsClass->setWindowIcon(icon);
        gridLayout = new QGridLayout(DialPrefsClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonOK = new QPushButton(DialPrefsClass);
        buttonOK->setObjectName(QStringLiteral("buttonOK"));
        buttonOK->setAutoDefault(false);

        horizontalLayout->addWidget(buttonOK);

        buttonClose = new QPushButton(DialPrefsClass);
        buttonClose->setObjectName(QStringLiteral("buttonClose"));

        horizontalLayout->addWidget(buttonClose);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        tabwidget_Prefs = new QTabWidget(DialPrefsClass);
        tabwidget_Prefs->setObjectName(QStringLiteral("tabwidget_Prefs"));
        QFont font1;
        font1.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font1.setPointSize(10);
        tabwidget_Prefs->setFont(font1);
        tabwidget_Prefs->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_6 = new QGridLayout(tab);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboType = new QComboBox(groupBox);
        comboType->setObjectName(QStringLiteral("comboType"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboType->sizePolicy().hasHeightForWidth());
        comboType->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(comboType);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);
        label_3->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_3);

        buttonColor = new QPushButton(groupBox);
        buttonColor->setObjectName(QStringLiteral("buttonColor"));
        sizePolicy1.setHeightForWidth(buttonColor->sizePolicy().hasHeightForWidth());
        buttonColor->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(buttonColor);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_5->addWidget(label_7);

        labelMarkerFont = new QLabel(groupBox);
        labelMarkerFont->setObjectName(QStringLiteral("labelMarkerFont"));
        sizePolicy2.setHeightForWidth(labelMarkerFont->sizePolicy().hasHeightForWidth());
        labelMarkerFont->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(labelMarkerFont);

        buttonMarkerFont = new QPushButton(groupBox);
        buttonMarkerFont->setObjectName(QStringLiteral("buttonMarkerFont"));

        horizontalLayout_5->addWidget(buttonMarkerFont);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_4);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);


        horizontalLayout_12->addWidget(groupBox);

        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        sizePolicy.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(label_6);

        buttonCursorColor = new QPushButton(groupBox_3);
        buttonCursorColor->setObjectName(QStringLiteral("buttonCursorColor"));
        sizePolicy1.setHeightForWidth(buttonCursorColor->sizePolicy().hasHeightForWidth());
        buttonCursorColor->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(buttonCursorColor);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_9->addWidget(label_8);

        labelCursorFont = new QLabel(groupBox_3);
        labelCursorFont->setObjectName(QStringLiteral("labelCursorFont"));
        sizePolicy2.setHeightForWidth(labelCursorFont->sizePolicy().hasHeightForWidth());
        labelCursorFont->setSizePolicy(sizePolicy2);

        horizontalLayout_9->addWidget(labelCursorFont);

        buttonCursorFont = new QPushButton(groupBox_3);
        buttonCursorFont->setObjectName(QStringLiteral("buttonCursorFont"));

        horizontalLayout_9->addWidget(buttonCursorFont);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_3);

        pushButton_2 = new QPushButton(groupBox_3);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);

        horizontalLayout_13->addWidget(pushButton_2);


        verticalLayout_4->addLayout(horizontalLayout_13);


        gridLayout_4->addLayout(verticalLayout_4, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_3, 1, 0, 1, 1);


        horizontalLayout_12->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        sizePolicy.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy);
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_4, 1, 0, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        sizePolicy3.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy3);

        horizontalLayout_10->addWidget(label_9);

        buttonMappingColor = new QPushButton(groupBox_4);
        buttonMappingColor->setObjectName(QStringLiteral("buttonMappingColor"));
        sizePolicy1.setHeightForWidth(buttonMappingColor->sizePolicy().hasHeightForWidth());
        buttonMappingColor->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(buttonMappingColor);


        verticalLayout_5->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_11->addWidget(label_10);

        labelMappingFont = new QLabel(groupBox_4);
        labelMappingFont->setObjectName(QStringLiteral("labelMappingFont"));
        sizePolicy2.setHeightForWidth(labelMappingFont->sizePolicy().hasHeightForWidth());
        labelMappingFont->setSizePolicy(sizePolicy2);

        horizontalLayout_11->addWidget(labelMappingFont);

        buttonMappingFont = new QPushButton(groupBox_4);
        buttonMappingFont->setObjectName(QStringLiteral("buttonMappingFont"));

        horizontalLayout_11->addWidget(buttonMappingFont);


        verticalLayout_5->addLayout(horizontalLayout_11);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_6);

        pushButton_3 = new QPushButton(groupBox_4);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        sizePolicy1.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy1);

        horizontalLayout_14->addWidget(pushButton_3);


        verticalLayout_5->addLayout(horizontalLayout_14);


        gridLayout_5->addLayout(verticalLayout_5, 0, 0, 1, 1);


        horizontalLayout_12->addWidget(groupBox_4);


        gridLayout_6->addLayout(horizontalLayout_12, 0, 0, 1, 2);

        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        verticalLayout_6 = new QVBoxLayout(groupBox_2);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QStringLiteral("label_11"));
        sizePolicy3.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(label_11);

        comboLanguage = new QComboBox(groupBox_2);
        comboLanguage->setObjectName(QStringLiteral("comboLanguage"));
        sizePolicy3.setHeightForWidth(comboLanguage->sizePolicy().hasHeightForWidth());
        comboLanguage->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(comboLanguage);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_7->addWidget(label_5);

        radioTriggerParserOn = new QRadioButton(groupBox_2);
        buttonGroup_2 = new QButtonGroup(DialPrefsClass);
        buttonGroup_2->setObjectName(QStringLiteral("buttonGroup_2"));
        buttonGroup_2->addButton(radioTriggerParserOn);
        radioTriggerParserOn->setObjectName(QStringLiteral("radioTriggerParserOn"));

        horizontalLayout_7->addWidget(radioTriggerParserOn);

        radioTriggerParserOff = new QRadioButton(groupBox_2);
        buttonGroup_2->addButton(radioTriggerParserOff);
        radioTriggerParserOff->setObjectName(QStringLiteral("radioTriggerParserOff"));

        horizontalLayout_7->addWidget(radioTriggerParserOff);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);


        verticalLayout_6->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_6->addWidget(label_12);

        radioHMSOff = new QRadioButton(groupBox_2);
        buttonGroup = new QButtonGroup(DialPrefsClass);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(radioHMSOff);
        radioHMSOff->setObjectName(QStringLiteral("radioHMSOff"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(radioHMSOff->sizePolicy().hasHeightForWidth());
        radioHMSOff->setSizePolicy(sizePolicy4);

        horizontalLayout_6->addWidget(radioHMSOff);

        radioHMSOn = new QRadioButton(groupBox_2);
        buttonGroup->addButton(radioHMSOn);
        radioHMSOn->setObjectName(QStringLiteral("radioHMSOn"));
        sizePolicy4.setHeightForWidth(radioHMSOn->sizePolicy().hasHeightForWidth());
        radioHMSOn->setSizePolicy(sizePolicy4);

        horizontalLayout_6->addWidget(radioHMSOn);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);


        verticalLayout_6->addLayout(horizontalLayout_6);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_15->addWidget(label_15);

        sliderCPU = new QSlider(groupBox_2);
        sliderCPU->setObjectName(QStringLiteral("sliderCPU"));
        sizePolicy4.setHeightForWidth(sliderCPU->sizePolicy().hasHeightForWidth());
        sliderCPU->setSizePolicy(sizePolicy4);
        sliderCPU->setMinimum(2);
        sliderCPU->setPageStep(2);
        sliderCPU->setOrientation(Qt::Horizontal);
        sliderCPU->setTickPosition(QSlider::NoTicks);
        sliderCPU->setTickInterval(5);

        horizontalLayout_15->addWidget(sliderCPU);

        spinCPU = new QSpinBox(groupBox_2);
        spinCPU->setObjectName(QStringLiteral("spinCPU"));
        spinCPU->setReadOnly(true);
        spinCPU->setButtonSymbols(QAbstractSpinBox::NoButtons);

        horizontalLayout_15->addWidget(spinCPU);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_8);


        verticalLayout_6->addLayout(horizontalLayout_15);

        verticalSpacer_2 = new QSpacerItem(20, 54, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);


        gridLayout_6->addWidget(groupBox_2, 1, 0, 1, 1);

        tabwidget_Prefs->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_7 = new QGridLayout(tab_3);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        plainTextEdit = new QPlainTextEdit(tab_3);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setReadOnly(true);

        verticalLayout_2->addWidget(plainTextEdit);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        label_13 = new QLabel(tab_3);
        label_13->setObjectName(QStringLiteral("label_13"));
        sizePolicy4.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy4);

        horizontalLayout_18->addWidget(label_13);

        lineEditMatlabPath = new QLineEdit(tab_3);
        lineEditMatlabPath->setObjectName(QStringLiteral("lineEditMatlabPath"));
        sizePolicy1.setHeightForWidth(lineEditMatlabPath->sizePolicy().hasHeightForWidth());
        lineEditMatlabPath->setSizePolicy(sizePolicy1);

        horizontalLayout_18->addWidget(lineEditMatlabPath);

        buttonSelectMatlabFolder = new QPushButton(tab_3);
        buttonSelectMatlabFolder->setObjectName(QStringLiteral("buttonSelectMatlabFolder"));
        sizePolicy3.setHeightForWidth(buttonSelectMatlabFolder->sizePolicy().hasHeightForWidth());
        buttonSelectMatlabFolder->setSizePolicy(sizePolicy3);

        horizontalLayout_18->addWidget(buttonSelectMatlabFolder);


        verticalLayout_2->addLayout(horizontalLayout_18);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        labelMCR = new QLabel(tab_3);
        labelMCR->setObjectName(QStringLiteral("labelMCR"));
        sizePolicy4.setHeightForWidth(labelMCR->sizePolicy().hasHeightForWidth());
        labelMCR->setSizePolicy(sizePolicy4);

        horizontalLayout_19->addWidget(labelMCR);

        lineEditMCR = new QLineEdit(tab_3);
        lineEditMCR->setObjectName(QStringLiteral("lineEditMCR"));
        sizePolicy1.setHeightForWidth(lineEditMCR->sizePolicy().hasHeightForWidth());
        lineEditMCR->setSizePolicy(sizePolicy1);

        horizontalLayout_19->addWidget(lineEditMCR);

        buttonSelectMCR = new QPushButton(tab_3);
        buttonSelectMCR->setObjectName(QStringLiteral("buttonSelectMCR"));
        sizePolicy3.setHeightForWidth(buttonSelectMCR->sizePolicy().hasHeightForWidth());
        buttonSelectMCR->setSizePolicy(sizePolicy3);

        horizontalLayout_19->addWidget(buttonSelectMCR);


        verticalLayout_2->addLayout(horizontalLayout_19);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        label_4 = new QLabel(tab_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy4.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy4);

        horizontalLayout_20->addWidget(label_4);

        lineEditGARDEL = new QLineEdit(tab_3);
        lineEditGARDEL->setObjectName(QStringLiteral("lineEditGARDEL"));
        sizePolicy1.setHeightForWidth(lineEditGARDEL->sizePolicy().hasHeightForWidth());
        lineEditGARDEL->setSizePolicy(sizePolicy1);

        horizontalLayout_20->addWidget(lineEditGARDEL);

        buttonSelectGARDEL = new QPushButton(tab_3);
        buttonSelectGARDEL->setObjectName(QStringLiteral("buttonSelectGARDEL"));
        sizePolicy3.setHeightForWidth(buttonSelectGARDEL->sizePolicy().hasHeightForWidth());
        buttonSelectGARDEL->setSizePolicy(sizePolicy3);

        horizontalLayout_20->addWidget(buttonSelectGARDEL);


        verticalLayout_2->addLayout(horizontalLayout_20);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_5);


        gridLayout_7->addLayout(verticalLayout_2, 0, 0, 1, 1);

        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/matlablogo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        tabwidget_Prefs->addTab(tab_3, icon1, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        gridLayout_12 = new QGridLayout(tab_4);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));

        verticalLayout_3->addLayout(horizontalLayout_16);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        label_14 = new QLabel(tab_4);
        label_14->setObjectName(QStringLiteral("label_14"));
        sizePolicy4.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy4);

        horizontalLayout_17->addWidget(label_14);

        lineEditPythonPath = new QLineEdit(tab_4);
        lineEditPythonPath->setObjectName(QStringLiteral("lineEditPythonPath"));
        sizePolicy1.setHeightForWidth(lineEditPythonPath->sizePolicy().hasHeightForWidth());
        lineEditPythonPath->setSizePolicy(sizePolicy1);

        horizontalLayout_17->addWidget(lineEditPythonPath);

        buttonSelectMatlabFolder_2 = new QPushButton(tab_4);
        buttonSelectMatlabFolder_2->setObjectName(QStringLiteral("buttonSelectMatlabFolder_2"));
        sizePolicy3.setHeightForWidth(buttonSelectMatlabFolder_2->sizePolicy().hasHeightForWidth());
        buttonSelectMatlabFolder_2->setSizePolicy(sizePolicy3);

        horizontalLayout_17->addWidget(buttonSelectMatlabFolder_2);


        verticalLayout_3->addLayout(horizontalLayout_17);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_6);


        gridLayout_12->addLayout(verticalLayout_3, 0, 0, 1, 1);

        tabwidget_Prefs->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        lineH = new QFrame(tab_2);
        lineH->setObjectName(QStringLiteral("lineH"));
        lineH->setGeometry(QRect(50, 230, 200, 5));
        lineH->setFrameShadow(QFrame::Plain);
        lineH->setLineWidth(3);
        lineH->setFrameShape(QFrame::HLine);
        lineV = new QFrame(tab_2);
        lineV->setObjectName(QStringLiteral("lineV"));
        lineV->setGeometry(QRect(30, 20, 3, 200));
        lineV->setFrameShadow(QFrame::Plain);
        lineV->setLineWidth(3);
        lineV->setFrameShape(QFrame::VLine);
        spinBoxV = new QSpinBox(tab_2);
        spinBoxV->setObjectName(QStringLiteral("spinBoxV"));
        spinBoxV->setGeometry(QRect(50, 20, 51, 22));
        spinBoxV->setMaximum(5000);
        spinBoxH = new QSpinBox(tab_2);
        spinBoxH->setObjectName(QStringLiteral("spinBoxH"));
        spinBoxH->setGeometry(QRect(200, 190, 51, 22));
        spinBoxH->setMaximum(5000);
        label = new QLabel(tab_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 20, 191, 161));
        label->setTextFormat(Qt::PlainText);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label->setWordWrap(true);
        line = new QFrame(tab_2);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(20, 220, 20, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(tab_2);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(20, 20, 20, 3));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(tab_2);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(50, 220, 3, 25));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(tab_2);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setGeometry(QRect(250, 220, 3, 25));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/display-settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabwidget_Prefs->addTab(tab_2, icon2, QString());

        gridLayout->addWidget(tabwidget_Prefs, 0, 0, 1, 1);


        retranslateUi(DialPrefsClass);
        QObject::connect(buttonOK, SIGNAL(clicked()), DialPrefsClass, SLOT(accept()));
        QObject::connect(buttonClose, SIGNAL(clicked()), DialPrefsClass, SLOT(reject()));
        QObject::connect(comboType, SIGNAL(currentIndexChanged(int)), DialPrefsClass, SLOT(changeMarkerType(int)));
        QObject::connect(buttonColor, SIGNAL(clicked()), DialPrefsClass, SLOT(chooseCustomColor()));
        QObject::connect(pushButton, SIGNAL(clicked()), DialPrefsClass, SLOT(restoreDefaultMarkersColors()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), DialPrefsClass, SLOT(restoreDefaultCursorColors()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), DialPrefsClass, SLOT(restoreDefaultMappingColors()));
        QObject::connect(buttonCursorFont, SIGNAL(clicked()), DialPrefsClass, SLOT(editCursorFont()));
        QObject::connect(buttonMappingFont, SIGNAL(clicked()), DialPrefsClass, SLOT(editMappingFont()));
        QObject::connect(buttonCursorColor, SIGNAL(clicked()), DialPrefsClass, SLOT(selectCursorColor()));
        QObject::connect(buttonMappingColor, SIGNAL(clicked()), DialPrefsClass, SLOT(selectMappingColor()));
        QObject::connect(buttonMarkerFont, SIGNAL(clicked()), DialPrefsClass, SLOT(editMarkerFont()));
        QObject::connect(buttonSelectMatlabFolder, SIGNAL(clicked()), DialPrefsClass, SLOT(pickMatlabFolder()));
        QObject::connect(buttonSelectMCR, SIGNAL(clicked()), DialPrefsClass, SLOT(pickMCRFolder()));
        QObject::connect(buttonSelectMatlabFolder_2, SIGNAL(clicked()), DialPrefsClass, SLOT(pickPython()));
        QObject::connect(buttonSelectGARDEL, SIGNAL(clicked()), DialPrefsClass, SLOT(pickGARDEL()));
        QObject::connect(sliderCPU, SIGNAL(valueChanged(int)), spinCPU, SLOT(setValue(int)));

        buttonOK->setDefault(true);
        tabwidget_Prefs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialPrefsClass);
    } // setupUi

    void retranslateUi(QDialog *DialPrefsClass)
    {
        DialPrefsClass->setWindowTitle(QApplication::translate("DialPrefsClass", "Preferences", Q_NULLPTR));
        buttonOK->setText(QApplication::translate("DialPrefsClass", "&Apply", Q_NULLPTR));
        buttonClose->setText(QApplication::translate("DialPrefsClass", "&Close", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("DialPrefsClass", "Marker settings", Q_NULLPTR));
        label_2->setText(QApplication::translate("DialPrefsClass", "Marker type:", Q_NULLPTR));
        label_3->setText(QApplication::translate("DialPrefsClass", "Color", Q_NULLPTR));
        buttonColor->setText(QString());
        label_7->setText(QApplication::translate("DialPrefsClass", "Font:", Q_NULLPTR));
        labelMarkerFont->setText(QApplication::translate("DialPrefsClass", "Helvetica 12 normal", Q_NULLPTR));
        buttonMarkerFont->setText(QApplication::translate("DialPrefsClass", "Edit", Q_NULLPTR));
        pushButton->setText(QApplication::translate("DialPrefsClass", "Restore Defaults", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("DialPrefsClass", "Cursor settings", Q_NULLPTR));
        label_6->setText(QApplication::translate("DialPrefsClass", "Color:", Q_NULLPTR));
        buttonCursorColor->setText(QString());
        label_8->setText(QApplication::translate("DialPrefsClass", "Font:", Q_NULLPTR));
        labelCursorFont->setText(QApplication::translate("DialPrefsClass", "Helvetica 12 normal", Q_NULLPTR));
        buttonCursorFont->setText(QApplication::translate("DialPrefsClass", "Edit", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("DialPrefsClass", "Restore Defaults", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("DialPrefsClass", "Mapping cursor settings", Q_NULLPTR));
        label_9->setText(QApplication::translate("DialPrefsClass", "Color:", Q_NULLPTR));
        buttonMappingColor->setText(QString());
        label_10->setText(QApplication::translate("DialPrefsClass", "Font:", Q_NULLPTR));
        labelMappingFont->setText(QApplication::translate("DialPrefsClass", "Helvetica 12 normal", Q_NULLPTR));
        buttonMappingFont->setText(QApplication::translate("DialPrefsClass", "Edit", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("DialPrefsClass", "Restore Defaults", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("DialPrefsClass", "Miscellaneous", Q_NULLPTR));
        label_11->setText(QApplication::translate("DialPrefsClass", "Language:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        label_5->setToolTip(QApplication::translate("DialPrefsClass", "Choose Yes if you want Anywave to ask for events extraction each time a file with trigger channel(s) is open.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("DialPrefsClass", "Automacillaly extract markers from trigger channel(s):", Q_NULLPTR));
        radioTriggerParserOn->setText(QApplication::translate("DialPrefsClass", "Yes", Q_NULLPTR));
        radioTriggerParserOff->setText(QApplication::translate("DialPrefsClass", "No", Q_NULLPTR));
        label_12->setText(QApplication::translate("DialPrefsClass", "Time format:", Q_NULLPTR));
        radioHMSOff->setText(QApplication::translate("DialPrefsClass", "Absolute seconds", Q_NULLPTR));
        radioHMSOn->setText(QApplication::translate("DialPrefsClass", "HMS", Q_NULLPTR));
        label_15->setText(QApplication::translate("DialPrefsClass", "CPU Cores to use:", Q_NULLPTR));
        tabwidget_Prefs->setTabText(tabwidget_Prefs->indexOf(tab), QApplication::translate("DialPrefsClass", "General", Q_NULLPTR));
        label_13->setText(QApplication::translate("DialPrefsClass", "MATLAB location:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        lineEditMatlabPath->setToolTip(QApplication::translate("DialPrefsClass", "<html><head/><body><p>This is the path to MATLAB current installation.</p><p>Change the path to the corresponding MATLAB location on your system.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        buttonSelectMatlabFolder->setToolTip(QApplication::translate("DialPrefsClass", "Choose the MATLAB folder", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonSelectMatlabFolder->setText(QApplication::translate("DialPrefsClass", "Select Folder", Q_NULLPTR));
        labelMCR->setText(QApplication::translate("DialPrefsClass", "MATLAB Runtime for compiled plug-ins:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        lineEditMCR->setToolTip(QApplication::translate("DialPrefsClass", "This is the path to the MATLAB Runtime installed on your system. You must specify a path on Linux or Mac OS X systems.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        buttonSelectMCR->setToolTip(QApplication::translate("DialPrefsClass", "Choose the MATLAB Runtime folder", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonSelectMCR->setText(QApplication::translate("DialPrefsClass", "Select Folder", Q_NULLPTR));
        label_4->setText(QApplication::translate("DialPrefsClass", "GARDEL File:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        lineEditGARDEL->setToolTip(QApplication::translate("DialPrefsClass", "<html><head/><body><p>This is the path to MATLAB current installation.</p><p>Change the path to the corresponding MATLAB location on your system.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        buttonSelectGARDEL->setToolTip(QApplication::translate("DialPrefsClass", "Choose the MATLAB folder", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonSelectGARDEL->setText(QApplication::translate("DialPrefsClass", "Select File", Q_NULLPTR));
        tabwidget_Prefs->setTabText(tabwidget_Prefs->indexOf(tab_3), QApplication::translate("DialPrefsClass", "MATLAB", Q_NULLPTR));
        label_14->setText(QApplication::translate("DialPrefsClass", "Python interpreter:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        lineEditPythonPath->setToolTip(QApplication::translate("DialPrefsClass", "<html><head/><body><p>This is the python interpreter AnyWave will call to execute Python plugins. </p><p>You may specify the full path to the interpreter to use.</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonSelectMatlabFolder_2->setText(QApplication::translate("DialPrefsClass", "Select File", Q_NULLPTR));
        tabwidget_Prefs->setTabText(tabwidget_Prefs->indexOf(tab_4), QApplication::translate("DialPrefsClass", "Python", Q_NULLPTR));
        label->setText(QApplication::translate("DialPrefsClass", "Measure the length of the two lines and enter results in corresponding boxes in mm.", Q_NULLPTR));
        tabwidget_Prefs->setTabText(tabwidget_Prefs->indexOf(tab_2), QApplication::translate("DialPrefsClass", "Screen Calibration", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DialPrefsClass: public Ui_DialPrefsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPREFSDIAL_H
