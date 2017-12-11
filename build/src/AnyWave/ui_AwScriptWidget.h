/********************************************************************************
** Form generated from reading UI file 'AwScriptWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWSCRIPTWIDGET_H
#define UI_AWSCRIPTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwScriptWidgetUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelName;
    QSpacerItem *horizontalSpacer;
    QLabel *labelMessage;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonRemove;
    QPushButton *buttonShowLog;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelTime;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *AwScriptWidgetUi)
    {
        if (AwScriptWidgetUi->objectName().isEmpty())
            AwScriptWidgetUi->setObjectName(QStringLiteral("AwScriptWidgetUi"));
        AwScriptWidgetUi->resize(339, 123);
        gridLayout = new QGridLayout(AwScriptWidgetUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        labelName = new QLabel(AwScriptWidgetUi);
        labelName->setObjectName(QStringLiteral("labelName"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelName->sizePolicy().hasHeightForWidth());
        labelName->setSizePolicy(sizePolicy);
        labelName->setMinimumSize(QSize(0, 20));
        labelName->setStyleSheet(QStringLiteral("font: bold;"));

        horizontalLayout_2->addWidget(labelName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        labelMessage = new QLabel(AwScriptWidgetUi);
        labelMessage->setObjectName(QStringLiteral("labelMessage"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelMessage->sizePolicy().hasHeightForWidth());
        labelMessage->setSizePolicy(sizePolicy1);
        labelMessage->setMinimumSize(QSize(0, 20));
        labelMessage->setStyleSheet(QLatin1String("font: bold;\n"
"color: blue;"));

        horizontalLayout_2->addWidget(labelMessage);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonRemove = new QPushButton(AwScriptWidgetUi);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        sizePolicy.setHeightForWidth(buttonRemove->sizePolicy().hasHeightForWidth());
        buttonRemove->setSizePolicy(sizePolicy);
        buttonRemove->setMinimumSize(QSize(0, 20));

        horizontalLayout->addWidget(buttonRemove);

        buttonShowLog = new QPushButton(AwScriptWidgetUi);
        buttonShowLog->setObjectName(QStringLiteral("buttonShowLog"));
        sizePolicy.setHeightForWidth(buttonShowLog->sizePolicy().hasHeightForWidth());
        buttonShowLog->setSizePolicy(sizePolicy);
        buttonShowLog->setMinimumSize(QSize(0, 20));

        horizontalLayout->addWidget(buttonShowLog);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        labelTime = new QLabel(AwScriptWidgetUi);
        labelTime->setObjectName(QStringLiteral("labelTime"));
        sizePolicy.setHeightForWidth(labelTime->sizePolicy().hasHeightForWidth());
        labelTime->setSizePolicy(sizePolicy);
        labelTime->setMinimumSize(QSize(0, 20));

        horizontalLayout->addWidget(labelTime);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);


        retranslateUi(AwScriptWidgetUi);

        QMetaObject::connectSlotsByName(AwScriptWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwScriptWidgetUi)
    {
        AwScriptWidgetUi->setWindowTitle(QApplication::translate("AwScriptWidgetUi", "Script", Q_NULLPTR));
        labelName->setText(QApplication::translate("AwScriptWidgetUi", "Script file", Q_NULLPTR));
        labelMessage->setText(QApplication::translate("AwScriptWidgetUi", "Message", Q_NULLPTR));
        buttonRemove->setText(QApplication::translate("AwScriptWidgetUi", "Remove", Q_NULLPTR));
        buttonShowLog->setText(QApplication::translate("AwScriptWidgetUi", "Show Log", Q_NULLPTR));
        labelTime->setText(QApplication::translate("AwScriptWidgetUi", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwScriptWidgetUi: public Ui_AwScriptWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWSCRIPTWIDGET_H
