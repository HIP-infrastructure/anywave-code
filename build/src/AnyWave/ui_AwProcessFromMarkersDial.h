/********************************************************************************
** Form generated from reading UI file 'AwProcessFromMarkersDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPROCESSFROMMARKERSDIAL_H
#define UI_AWPROCESSFROMMARKERSDIAL_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwProcessFromMarkersDialClass
{
public:
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboProcess;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelSelectMarker;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonType;
    QComboBox *comboType;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *radioButtonName;
    QComboBox *comboName;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *radioButtonCode;
    QComboBox *comboCode;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_9;
    QLabel *labelSelectedMarkers;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *verticalSpacer_2;
    QTableView *tvDroppedMarkers;
    QSpacerItem *verticalSpacer_3;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_6;
    QDoubleSpinBox *sbPreMarkerTime;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_7;
    QDoubleSpinBox *sbPostMarkerTime;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *spacer;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *AwProcessFromMarkersDialClass)
    {
        if (AwProcessFromMarkersDialClass->objectName().isEmpty())
            AwProcessFromMarkersDialClass->setObjectName(QStringLiteral("AwProcessFromMarkersDialClass"));
        AwProcessFromMarkersDialClass->setWindowModality(Qt::WindowModal);
        AwProcessFromMarkersDialClass->resize(345, 524);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AwProcessFromMarkersDialClass->sizePolicy().hasHeightForWidth());
        AwProcessFromMarkersDialClass->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwProcessFromMarkersDialClass->setFont(font);
        AwProcessFromMarkersDialClass->setAcceptDrops(true);
        AwProcessFromMarkersDialClass->setSizeGripEnabled(true);
        AwProcessFromMarkersDialClass->setModal(true);
        gridLayout_4 = new QGridLayout(AwProcessFromMarkersDialClass);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        groupBox = new QGroupBox(AwProcessFromMarkersDialClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);

        comboProcess = new QComboBox(groupBox);
        comboProcess->setObjectName(QStringLiteral("comboProcess"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboProcess->sizePolicy().hasHeightForWidth());
        comboProcess->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(comboProcess);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(AwProcessFromMarkersDialClass);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        labelSelectMarker = new QLabel(groupBox_2);
        labelSelectMarker->setObjectName(QStringLiteral("labelSelectMarker"));
        labelSelectMarker->setScaledContents(true);

        horizontalLayout_3->addWidget(labelSelectMarker);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        radioButtonType = new QRadioButton(groupBox_2);
        radioButtonType->setObjectName(QStringLiteral("radioButtonType"));
        sizePolicy2.setHeightForWidth(radioButtonType->sizePolicy().hasHeightForWidth());
        radioButtonType->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(radioButtonType);

        comboType = new QComboBox(groupBox_2);
        comboType->setObjectName(QStringLiteral("comboType"));
        sizePolicy2.setHeightForWidth(comboType->sizePolicy().hasHeightForWidth());
        comboType->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(comboType);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        radioButtonName = new QRadioButton(groupBox_2);
        radioButtonName->setObjectName(QStringLiteral("radioButtonName"));
        sizePolicy2.setHeightForWidth(radioButtonName->sizePolicy().hasHeightForWidth());
        radioButtonName->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(radioButtonName);

        comboName = new QComboBox(groupBox_2);
        comboName->setObjectName(QStringLiteral("comboName"));
        sizePolicy2.setHeightForWidth(comboName->sizePolicy().hasHeightForWidth());
        comboName->setSizePolicy(sizePolicy2);

        horizontalLayout_4->addWidget(comboName);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        radioButtonCode = new QRadioButton(groupBox_2);
        radioButtonCode->setObjectName(QStringLiteral("radioButtonCode"));
        sizePolicy2.setHeightForWidth(radioButtonCode->sizePolicy().hasHeightForWidth());
        radioButtonCode->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(radioButtonCode);

        comboCode = new QComboBox(groupBox_2);
        comboCode->setObjectName(QStringLiteral("comboCode"));
        sizePolicy2.setHeightForWidth(comboCode->sizePolicy().hasHeightForWidth());
        comboCode->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(comboCode);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        labelSelectedMarkers = new QLabel(groupBox_2);
        labelSelectedMarkers->setObjectName(QStringLiteral("labelSelectedMarkers"));
        labelSelectedMarkers->setScaledContents(true);

        horizontalLayout_9->addWidget(labelSelectedMarkers);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);


        verticalLayout->addLayout(horizontalLayout_9);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        tvDroppedMarkers = new QTableView(groupBox_2);
        tvDroppedMarkers->setObjectName(QStringLiteral("tvDroppedMarkers"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(tvDroppedMarkers->sizePolicy().hasHeightForWidth());
        tvDroppedMarkers->setSizePolicy(sizePolicy3);
        tvDroppedMarkers->setAcceptDrops(true);
        tvDroppedMarkers->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tvDroppedMarkers->setDragEnabled(true);
        tvDroppedMarkers->setDragDropOverwriteMode(false);
        tvDroppedMarkers->setDragDropMode(QAbstractItemView::DropOnly);
        tvDroppedMarkers->setDefaultDropAction(Qt::CopyAction);
        tvDroppedMarkers->setSelectionMode(QAbstractItemView::ContiguousSelection);
        tvDroppedMarkers->setSelectionBehavior(QAbstractItemView::SelectRows);
        tvDroppedMarkers->horizontalHeader()->setVisible(true);

        verticalLayout->addWidget(tvDroppedMarkers);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_2, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(AwProcessFromMarkersDialClass);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        sizePolicy3.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy3);
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy4);
        label_6->setScaledContents(true);

        horizontalLayout_11->addWidget(label_6);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_9);


        verticalLayout_3->addLayout(horizontalLayout_11);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        sbPreMarkerTime = new QDoubleSpinBox(groupBox_3);
        sbPreMarkerTime->setObjectName(QStringLiteral("sbPreMarkerTime"));
        sizePolicy4.setHeightForWidth(sbPreMarkerTime->sizePolicy().hasHeightForWidth());
        sbPreMarkerTime->setSizePolicy(sizePolicy4);
        sbPreMarkerTime->setDecimals(3);
        sbPreMarkerTime->setMaximum(1e+9);
        sbPreMarkerTime->setSingleStep(0.1);

        horizontalLayout_6->addWidget(sbPreMarkerTime);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy4.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy4);

        horizontalLayout_6->addWidget(label_3);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        sbPostMarkerTime = new QDoubleSpinBox(groupBox_3);
        sbPostMarkerTime->setObjectName(QStringLiteral("sbPostMarkerTime"));
        sizePolicy4.setHeightForWidth(sbPostMarkerTime->sizePolicy().hasHeightForWidth());
        sbPostMarkerTime->setSizePolicy(sizePolicy4);
        sbPostMarkerTime->setDecimals(3);
        sbPostMarkerTime->setMaximum(1e+9);
        sbPostMarkerTime->setSingleStep(0.1);

        horizontalLayout_7->addWidget(sbPostMarkerTime);

        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy4.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy4);

        horizontalLayout_7->addWidget(label_4);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);


        verticalLayout_3->addLayout(horizontalLayout_7);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        gridLayout_3->addLayout(verticalLayout_3, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_3, 2, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        spacer = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(spacer);

        okButton = new QPushButton(AwProcessFromMarkersDialClass);
        okButton->setObjectName(QStringLiteral("okButton"));

        horizontalLayout_8->addWidget(okButton);

        cancelButton = new QPushButton(AwProcessFromMarkersDialClass);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout_8->addWidget(cancelButton);


        gridLayout_4->addLayout(horizontalLayout_8, 3, 0, 1, 1);


        retranslateUi(AwProcessFromMarkersDialClass);
        QObject::connect(okButton, SIGNAL(clicked()), AwProcessFromMarkersDialClass, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), AwProcessFromMarkersDialClass, SLOT(reject()));

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(AwProcessFromMarkersDialClass);
    } // setupUi

    void retranslateUi(QDialog *AwProcessFromMarkersDialClass)
    {
        AwProcessFromMarkersDialClass->setWindowTitle(QApplication::translate("AwProcessFromMarkersDialClass", "Launch a Process", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwProcessFromMarkersDialClass", "Step 1: Choose a process", Q_NULLPTR));
        label->setText(QApplication::translate("AwProcessFromMarkersDialClass", "Process:", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AwProcessFromMarkersDialClass", " Step 2: Select data for process using markers", Q_NULLPTR));
        labelSelectMarker->setText(QApplication::translate("AwProcessFromMarkersDialClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: underline;\">Select marker by:</span></p></body></html>", Q_NULLPTR));
        radioButtonType->setText(QApplication::translate("AwProcessFromMarkersDialClass", "Type", Q_NULLPTR));
        radioButtonName->setText(QApplication::translate("AwProcessFromMarkersDialClass", "Name", Q_NULLPTR));
        radioButtonCode->setText(QApplication::translate("AwProcessFromMarkersDialClass", "Value", Q_NULLPTR));
        labelSelectedMarkers->setText(QApplication::translate("AwProcessFromMarkersDialClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: underline;\">Selected markers below will be used:</span></p></body></html>", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("AwProcessFromMarkersDialClass", "Step 3: Duration", Q_NULLPTR));
        label_6->setText(QApplication::translate("AwProcessFromMarkersDialClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: underline;\">Latencies (Single markers only):</span></p></body></html>", Q_NULLPTR));
        sbPreMarkerTime->setSuffix(QApplication::translate("AwProcessFromMarkersDialClass", "s", Q_NULLPTR));
        label_3->setText(QApplication::translate("AwProcessFromMarkersDialClass", "Before", Q_NULLPTR));
        sbPostMarkerTime->setSuffix(QApplication::translate("AwProcessFromMarkersDialClass", "s", Q_NULLPTR));
        label_4->setText(QApplication::translate("AwProcessFromMarkersDialClass", "After", Q_NULLPTR));
        okButton->setText(QApplication::translate("AwProcessFromMarkersDialClass", "&Launch", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("AwProcessFromMarkersDialClass", "&Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwProcessFromMarkersDialClass: public Ui_AwProcessFromMarkersDialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPROCESSFROMMARKERSDIAL_H
