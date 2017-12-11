/********************************************************************************
** Form generated from reading UI file 'AwExportDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWEXPORTDIAL_H
#define UI_AWEXPORTDIAL_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ExportDialogClass
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QComboBox *comboFormat;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;
    QPushButton *pushButton;
    QTextEdit *teInfos;

    void setupUi(QDialog *ExportDialogClass)
    {
        if (ExportDialogClass->objectName().isEmpty())
            ExportDialogClass->setObjectName(QStringLiteral("ExportDialogClass"));
        ExportDialogClass->resize(559, 507);
        ExportDialogClass->setMinimumSize(QSize(338, 417));
        ExportDialogClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        ExportDialogClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        ExportDialogClass->setWindowIcon(icon);
        gridLayout = new QGridLayout(ExportDialogClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(ExportDialogClass);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 70));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        comboFormat = new QComboBox(groupBox);
        comboFormat->setObjectName(QStringLiteral("comboFormat"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboFormat->sizePolicy().hasHeightForWidth());
        comboFormat->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(comboFormat, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 1, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(ExportDialogClass);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy2);
        pushButton_2->setAutoDefault(false);

        horizontalLayout->addWidget(pushButton_2);

        pushButton = new QPushButton(ExportDialogClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        teInfos = new QTextEdit(ExportDialogClass);
        teInfos->setObjectName(QStringLiteral("teInfos"));

        gridLayout->addWidget(teInfos, 1, 0, 1, 1);


        retranslateUi(ExportDialogClass);
        QObject::connect(pushButton_2, SIGNAL(clicked()), ExportDialogClass, SLOT(accept()));
        QObject::connect(pushButton, SIGNAL(clicked()), ExportDialogClass, SLOT(reject()));

        pushButton_2->setDefault(true);


        QMetaObject::connectSlotsByName(ExportDialogClass);
    } // setupUi

    void retranslateUi(QDialog *ExportDialogClass)
    {
        ExportDialogClass->setWindowTitle(QApplication::translate("ExportDialogClass", "Export Data", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("ExportDialogClass", "Writer", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("ExportDialogClass", "&Export", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ExportDialogClass", "&Close", Q_NULLPTR));
        teInfos->setHtml(QApplication::translate("ExportDialogClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; text-decoration: underline;\">Note about channels selection:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial';\">If there are selected channels in the one of the views, only that channels will be exported.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Arial';\"><br /></p>\n"
"<"
                        "p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; text-decoration: underline;\">Note about output file(s):</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial';\">If you export several selections then several output files will be created.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Arial';\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Arial'; text-decoration: underline;\">Note about Montage:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-fa"
                        "mily:'Arial';\">You may loose channels montage during export. Correct values will be saved on file but information about montage will be lost. Be carefull before exporting bipolar channels.</span></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ExportDialogClass: public Ui_ExportDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWEXPORTDIAL_H
