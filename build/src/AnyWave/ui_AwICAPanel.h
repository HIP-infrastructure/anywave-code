/********************************************************************************
** Form generated from reading UI file 'AwICAPanel.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWICAPANEL_H
#define UI_AWICAPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwICAPanelUI
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioAll;
    QRadioButton *radioSelected;
    QSpacerItem *horizontalSpacer;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;

    void setupUi(QWidget *AwICAPanelUI)
    {
        if (AwICAPanelUI->objectName().isEmpty())
            AwICAPanelUI->setObjectName(QStringLiteral("AwICAPanelUI"));
        AwICAPanelUI->resize(1013, 695);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwICAPanelUI->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(AwICAPanelUI);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        radioAll = new QRadioButton(AwICAPanelUI);
        radioAll->setObjectName(QStringLiteral("radioAll"));
        radioAll->setChecked(true);

        horizontalLayout->addWidget(radioAll);

        radioSelected = new QRadioButton(AwICAPanelUI);
        radioSelected->setObjectName(QStringLiteral("radioSelected"));

        horizontalLayout->addWidget(radioSelected);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        scrollArea = new QScrollArea(AwICAPanelUI);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 993, 647));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));

        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(AwICAPanelUI);

        QMetaObject::connectSlotsByName(AwICAPanelUI);
    } // setupUi

    void retranslateUi(QWidget *AwICAPanelUI)
    {
        AwICAPanelUI->setWindowTitle(QApplication::translate("AwICAPanelUI", "Components", Q_NULLPTR));
        radioAll->setText(QApplication::translate("AwICAPanelUI", "Show All", Q_NULLPTR));
        radioSelected->setText(QApplication::translate("AwICAPanelUI", "Show Selected", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwICAPanelUI: public Ui_AwICAPanelUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWICAPANEL_H
