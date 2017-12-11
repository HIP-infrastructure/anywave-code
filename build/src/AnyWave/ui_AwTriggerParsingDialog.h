/********************************************************************************
** Form generated from reading UI file 'AwTriggerParsingDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWTRIGGERPARSINGDIALOG_H
#define UI_AWTRIGGERPARSINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwTriggerParsingDialogUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *checkDontAsk;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonOK;
    QPushButton *buttonClose;

    void setupUi(QDialog *AwTriggerParsingDialogUi)
    {
        if (AwTriggerParsingDialogUi->objectName().isEmpty())
            AwTriggerParsingDialogUi->setObjectName(QStringLiteral("AwTriggerParsingDialogUi"));
        AwTriggerParsingDialogUi->resize(435, 100);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwTriggerParsingDialogUi->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwTriggerParsingDialogUi->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwTriggerParsingDialogUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(AwTriggerParsingDialogUi);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(10);
        label->setFont(font1);

        verticalLayout->addWidget(label);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        checkDontAsk = new QCheckBox(AwTriggerParsingDialogUi);
        checkDontAsk->setObjectName(QStringLiteral("checkDontAsk"));

        horizontalLayout_2->addWidget(checkDontAsk);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonOK = new QPushButton(AwTriggerParsingDialogUi);
        buttonOK->setObjectName(QStringLiteral("buttonOK"));
        buttonOK->setAutoDefault(false);

        horizontalLayout->addWidget(buttonOK);

        buttonClose = new QPushButton(AwTriggerParsingDialogUi);
        buttonClose->setObjectName(QStringLiteral("buttonClose"));

        horizontalLayout->addWidget(buttonClose);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwTriggerParsingDialogUi);
        QObject::connect(buttonOK, SIGNAL(clicked()), AwTriggerParsingDialogUi, SLOT(accept()));
        QObject::connect(buttonClose, SIGNAL(clicked()), AwTriggerParsingDialogUi, SLOT(reject()));

        buttonOK->setDefault(true);


        QMetaObject::connectSlotsByName(AwTriggerParsingDialogUi);
    } // setupUi

    void retranslateUi(QDialog *AwTriggerParsingDialogUi)
    {
        AwTriggerParsingDialogUi->setWindowTitle(QApplication::translate("AwTriggerParsingDialogUi", "Trigger Channels Detected", Q_NULLPTR));
        label->setText(QApplication::translate("AwTriggerParsingDialogUi", "Would you like to extract events/markers?", Q_NULLPTR));
        checkDontAsk->setText(QApplication::translate("AwTriggerParsingDialogUi", "Don't Ask Again", Q_NULLPTR));
        buttonOK->setText(QApplication::translate("AwTriggerParsingDialogUi", "Yes", Q_NULLPTR));
        buttonClose->setText(QApplication::translate("AwTriggerParsingDialogUi", "No", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwTriggerParsingDialogUi: public Ui_AwTriggerParsingDialogUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWTRIGGERPARSINGDIALOG_H
