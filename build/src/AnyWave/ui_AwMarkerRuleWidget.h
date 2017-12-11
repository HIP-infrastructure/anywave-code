/********************************************************************************
** Form generated from reading UI file 'AwMarkerRuleWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMARKERRULEWIDGET_H
#define UI_AWMARKERRULEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwMarkerRuleWidgetUI
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboType;
    QComboBox *comboCondition;
    QComboBox *comboConditionValue;
    QPushButton *buttonAdd;
    QPushButton *buttonRemove;

    void setupUi(QWidget *AwMarkerRuleWidgetUI)
    {
        if (AwMarkerRuleWidgetUI->objectName().isEmpty())
            AwMarkerRuleWidgetUI->setObjectName(QStringLiteral("AwMarkerRuleWidgetUI"));
        AwMarkerRuleWidgetUI->resize(485, 44);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwMarkerRuleWidgetUI->setFont(font);
        gridLayout = new QGridLayout(AwMarkerRuleWidgetUI);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        comboType = new QComboBox(AwMarkerRuleWidgetUI);
        comboType->setObjectName(QStringLiteral("comboType"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboType->sizePolicy().hasHeightForWidth());
        comboType->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(comboType);

        comboCondition = new QComboBox(AwMarkerRuleWidgetUI);
        comboCondition->setObjectName(QStringLiteral("comboCondition"));
        sizePolicy.setHeightForWidth(comboCondition->sizePolicy().hasHeightForWidth());
        comboCondition->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(comboCondition);

        comboConditionValue = new QComboBox(AwMarkerRuleWidgetUI);
        comboConditionValue->setObjectName(QStringLiteral("comboConditionValue"));
        sizePolicy.setHeightForWidth(comboConditionValue->sizePolicy().hasHeightForWidth());
        comboConditionValue->setSizePolicy(sizePolicy);
        comboConditionValue->setEditable(true);
        comboConditionValue->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout->addWidget(comboConditionValue);

        buttonAdd = new QPushButton(AwMarkerRuleWidgetUI);
        buttonAdd->setObjectName(QStringLiteral("buttonAdd"));
        sizePolicy.setHeightForWidth(buttonAdd->sizePolicy().hasHeightForWidth());
        buttonAdd->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(buttonAdd);

        buttonRemove = new QPushButton(AwMarkerRuleWidgetUI);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        sizePolicy.setHeightForWidth(buttonRemove->sizePolicy().hasHeightForWidth());
        buttonRemove->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(buttonRemove);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwMarkerRuleWidgetUI);

        QMetaObject::connectSlotsByName(AwMarkerRuleWidgetUI);
    } // setupUi

    void retranslateUi(QWidget *AwMarkerRuleWidgetUI)
    {
        AwMarkerRuleWidgetUI->setWindowTitle(QApplication::translate("AwMarkerRuleWidgetUI", "AwMarkerRuleWidget", Q_NULLPTR));
        buttonAdd->setText(QApplication::translate("AwMarkerRuleWidgetUI", "+", Q_NULLPTR));
        buttonRemove->setText(QApplication::translate("AwMarkerRuleWidgetUI", "-", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwMarkerRuleWidgetUI: public Ui_AwMarkerRuleWidgetUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMARKERRULEWIDGET_H
