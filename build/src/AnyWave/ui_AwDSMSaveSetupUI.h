/********************************************************************************
** Form generated from reading UI file 'AwDSMSaveSetupUI.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWDSMSAVESETUPUI_H
#define UI_AWDSMSAVESETUPUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwDSMSaveSetupUIClass
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditName;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AwDSMSaveSetupUIClass)
    {
        if (AwDSMSaveSetupUIClass->objectName().isEmpty())
            AwDSMSaveSetupUIClass->setObjectName(QStringLiteral("AwDSMSaveSetupUIClass"));
        AwDSMSaveSetupUIClass->resize(215, 73);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwDSMSaveSetupUIClass->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwDSMSaveSetupUIClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwDSMSaveSetupUIClass);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEditName = new QLineEdit(AwDSMSaveSetupUIClass);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));

        horizontalLayout->addWidget(lineEditName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonBox = new QDialogButtonBox(AwDSMSaveSetupUIClass);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwDSMSaveSetupUIClass);
        QObject::connect(buttonBox, SIGNAL(accepted()), AwDSMSaveSetupUIClass, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AwDSMSaveSetupUIClass, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwDSMSaveSetupUIClass);
    } // setupUi

    void retranslateUi(QDialog *AwDSMSaveSetupUIClass)
    {
        AwDSMSaveSetupUIClass->setWindowTitle(QApplication::translate("AwDSMSaveSetupUIClass", "Save Display Setup", Q_NULLPTR));
        label->setText(QApplication::translate("AwDSMSaveSetupUIClass", "Name:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwDSMSaveSetupUIClass: public Ui_AwDSMSaveSetupUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWDSMSAVESETUPUI_H
