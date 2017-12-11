/********************************************************************************
** Form generated from reading UI file 'AwMarkerRuleManageDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMARKERRULEMANAGEDIAL_H
#define UI_AWMARKERRULEMANAGEDIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwMarkerRuleManageDialUI
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listRules;
    QVBoxLayout *verticalLayout;
    QPushButton *buttonNew;
    QPushButton *buttonModify;
    QPushButton *buttonRemove;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *AwMarkerRuleManageDialUI)
    {
        if (AwMarkerRuleManageDialUI->objectName().isEmpty())
            AwMarkerRuleManageDialUI->setObjectName(QStringLiteral("AwMarkerRuleManageDialUI"));
        AwMarkerRuleManageDialUI->resize(411, 259);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        font.setItalic(false);
        AwMarkerRuleManageDialUI->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwMarkerRuleManageDialUI->setWindowIcon(icon);
        AwMarkerRuleManageDialUI->setSizeGripEnabled(true);
        AwMarkerRuleManageDialUI->setModal(true);
        gridLayout = new QGridLayout(AwMarkerRuleManageDialUI);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwMarkerRuleManageDialUI);
        label->setObjectName(QStringLiteral("label"));
        QFont font1;
        font1.setItalic(false);
        label->setFont(font1);
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listRules = new QListWidget(AwMarkerRuleManageDialUI);
        listRules->setObjectName(QStringLiteral("listRules"));
        listRules->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listRules->setAlternatingRowColors(true);
        listRules->setSelectionBehavior(QAbstractItemView::SelectRows);

        horizontalLayout_2->addWidget(listRules);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        buttonNew = new QPushButton(AwMarkerRuleManageDialUI);
        buttonNew->setObjectName(QStringLiteral("buttonNew"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonNew->sizePolicy().hasHeightForWidth());
        buttonNew->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(buttonNew);

        buttonModify = new QPushButton(AwMarkerRuleManageDialUI);
        buttonModify->setObjectName(QStringLiteral("buttonModify"));
        buttonModify->setEnabled(false);
        sizePolicy.setHeightForWidth(buttonModify->sizePolicy().hasHeightForWidth());
        buttonModify->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(buttonModify);

        buttonRemove = new QPushButton(AwMarkerRuleManageDialUI);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        buttonRemove->setEnabled(false);
        sizePolicy.setHeightForWidth(buttonRemove->sizePolicy().hasHeightForWidth());
        buttonRemove->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(buttonRemove);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(AwMarkerRuleManageDialUI);

        QMetaObject::connectSlotsByName(AwMarkerRuleManageDialUI);
    } // setupUi

    void retranslateUi(QDialog *AwMarkerRuleManageDialUI)
    {
        AwMarkerRuleManageDialUI->setWindowTitle(QApplication::translate("AwMarkerRuleManageDialUI", "Marker rules", Q_NULLPTR));
        label->setText(QApplication::translate("AwMarkerRuleManageDialUI", "Rules:", Q_NULLPTR));
        buttonNew->setText(QApplication::translate("AwMarkerRuleManageDialUI", "New", Q_NULLPTR));
        buttonModify->setText(QApplication::translate("AwMarkerRuleManageDialUI", "Modify", Q_NULLPTR));
        buttonRemove->setText(QApplication::translate("AwMarkerRuleManageDialUI", "Remove", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwMarkerRuleManageDialUI: public Ui_AwMarkerRuleManageDialUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMARKERRULEMANAGEDIAL_H
