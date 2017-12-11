/********************************************************************************
** Form generated from reading UI file 'AwSaveAsNewSetupDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWSAVEASNEWSETUPDIALOG_H
#define UI_AWSAVEASNEWSETUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwSaveAsNewSetupDialogUI
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *editName;
    QSpacerItem *horizontalSpacer;
    QLabel *labelInfo;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonSave;
    QPushButton *pushButton_2;

    void setupUi(QDialog *AwSaveAsNewSetupDialogUI)
    {
        if (AwSaveAsNewSetupDialogUI->objectName().isEmpty())
            AwSaveAsNewSetupDialogUI->setObjectName(QStringLiteral("AwSaveAsNewSetupDialogUI"));
        AwSaveAsNewSetupDialogUI->resize(326, 92);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        AwSaveAsNewSetupDialogUI->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwSaveAsNewSetupDialogUI->setWindowIcon(icon);
        AwSaveAsNewSetupDialogUI->setModal(true);
        gridLayout = new QGridLayout(AwSaveAsNewSetupDialogUI);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwSaveAsNewSetupDialogUI);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        editName = new QLineEdit(AwSaveAsNewSetupDialogUI);
        editName->setObjectName(QStringLiteral("editName"));

        horizontalLayout->addWidget(editName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        labelInfo = new QLabel(AwSaveAsNewSetupDialogUI);
        labelInfo->setObjectName(QStringLiteral("labelInfo"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelInfo->sizePolicy().hasHeightForWidth());
        labelInfo->setSizePolicy(sizePolicy);
        labelInfo->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelInfo->setScaledContents(true);
        labelInfo->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelInfo);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonSave = new QPushButton(AwSaveAsNewSetupDialogUI);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        buttonSave->setAutoDefault(false);

        horizontalLayout_2->addWidget(buttonSave);

        pushButton_2 = new QPushButton(AwSaveAsNewSetupDialogUI);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwSaveAsNewSetupDialogUI);
        QObject::connect(buttonSave, SIGNAL(clicked()), AwSaveAsNewSetupDialogUI, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), AwSaveAsNewSetupDialogUI, SLOT(reject()));

        buttonSave->setDefault(true);


        QMetaObject::connectSlotsByName(AwSaveAsNewSetupDialogUI);
    } // setupUi

    void retranslateUi(QDialog *AwSaveAsNewSetupDialogUI)
    {
        AwSaveAsNewSetupDialogUI->setWindowTitle(QApplication::translate("AwSaveAsNewSetupDialogUI", "Save New Display Setup", Q_NULLPTR));
        label->setText(QApplication::translate("AwSaveAsNewSetupDialogUI", "Name:", Q_NULLPTR));
        labelInfo->setText(QApplication::translate("AwSaveAsNewSetupDialogUI", "TextLabel", Q_NULLPTR));
        buttonSave->setText(QApplication::translate("AwSaveAsNewSetupDialogUI", "Save", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("AwSaveAsNewSetupDialogUI", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwSaveAsNewSetupDialogUI: public Ui_AwSaveAsNewSetupDialogUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWSAVEASNEWSETUPDIALOG_H
