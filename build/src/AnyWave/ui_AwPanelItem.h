/********************************************************************************
** Form generated from reading UI file 'AwPanelItem.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPANELITEM_H
#define UI_AWPANELITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <widget/AwMapWidget.h>
#include <widget/AwTopoWidget.h>

QT_BEGIN_NAMESPACE

class Ui_AwPanelItem
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    AwTopoWidget *widget;
    AwMapWidget *widget3D;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBoxSelected;
    QRadioButton *radioButton2D;
    QRadioButton *radioButton3D;
    QPushButton *buttonShowSignal;
    QPushButton *buttonRejectAdd;
    QLabel *labelRejected;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *AwPanelItem)
    {
        if (AwPanelItem->objectName().isEmpty())
            AwPanelItem->setObjectName(QStringLiteral("AwPanelItem"));
        AwPanelItem->resize(500, 300);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AwPanelItem->sizePolicy().hasHeightForWidth());
        AwPanelItem->setSizePolicy(sizePolicy);
        AwPanelItem->setMinimumSize(QSize(0, 0));
        AwPanelItem->setMaximumSize(QSize(500, 300));
        gridLayout = new QGridLayout(AwPanelItem);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        widget = new AwTopoWidget(AwPanelItem);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(200, 200));

        horizontalLayout_2->addWidget(widget);

        widget3D = new AwMapWidget(AwPanelItem);
        widget3D->setObjectName(QStringLiteral("widget3D"));
        sizePolicy.setHeightForWidth(widget3D->sizePolicy().hasHeightForWidth());
        widget3D->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(widget3D);


        gridLayout->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        checkBoxSelected = new QCheckBox(AwPanelItem);
        checkBoxSelected->setObjectName(QStringLiteral("checkBoxSelected"));

        horizontalLayout->addWidget(checkBoxSelected);

        radioButton2D = new QRadioButton(AwPanelItem);
        radioButton2D->setObjectName(QStringLiteral("radioButton2D"));
        radioButton2D->setChecked(true);

        horizontalLayout->addWidget(radioButton2D);

        radioButton3D = new QRadioButton(AwPanelItem);
        radioButton3D->setObjectName(QStringLiteral("radioButton3D"));

        horizontalLayout->addWidget(radioButton3D);

        buttonShowSignal = new QPushButton(AwPanelItem);
        buttonShowSignal->setObjectName(QStringLiteral("buttonShowSignal"));

        horizontalLayout->addWidget(buttonShowSignal);

        buttonRejectAdd = new QPushButton(AwPanelItem);
        buttonRejectAdd->setObjectName(QStringLiteral("buttonRejectAdd"));

        horizontalLayout->addWidget(buttonRejectAdd);

        labelRejected = new QLabel(AwPanelItem);
        labelRejected->setObjectName(QStringLiteral("labelRejected"));

        horizontalLayout->addWidget(labelRejected);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(AwPanelItem);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(AwPanelItem);

        QMetaObject::connectSlotsByName(AwPanelItem);
    } // setupUi

    void retranslateUi(QWidget *AwPanelItem)
    {
        AwPanelItem->setWindowTitle(QApplication::translate("AwPanelItem", "AwPanelItem", Q_NULLPTR));
        checkBoxSelected->setText(QApplication::translate("AwPanelItem", "Selected", Q_NULLPTR));
        radioButton2D->setText(QApplication::translate("AwPanelItem", "2D", Q_NULLPTR));
        radioButton3D->setText(QApplication::translate("AwPanelItem", "3D", Q_NULLPTR));
        buttonShowSignal->setText(QApplication::translate("AwPanelItem", "Show Signal", Q_NULLPTR));
        buttonRejectAdd->setText(QApplication::translate("AwPanelItem", "Reject", Q_NULLPTR));
        labelRejected->setText(QApplication::translate("AwPanelItem", "REJECTED", Q_NULLPTR));
        label->setText(QApplication::translate("AwPanelItem", "Label", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwPanelItem: public Ui_AwPanelItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPANELITEM_H
