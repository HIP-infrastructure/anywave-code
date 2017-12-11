/********************************************************************************
** Form generated from reading UI file 'AwStatusBarProcessIndicatorWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWSTATUSBARPROCESSINDICATORWIDGET_H
#define UI_AWSTATUSBARPROCESSINDICATORWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "Process/AwProgressIndicator.h"

QT_BEGIN_NAMESPACE

class Ui_AwStatusBarProcessIndicatorWidgetClass
{
public:
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    AwProgressIndicator *processIndicator;
    QLabel *labelProcess;
    QLabel *labelMessage;
    QToolButton *buttonMenu;

    void setupUi(QWidget *AwStatusBarProcessIndicatorWidgetClass)
    {
        if (AwStatusBarProcessIndicatorWidgetClass->objectName().isEmpty())
            AwStatusBarProcessIndicatorWidgetClass->setObjectName(QStringLiteral("AwStatusBarProcessIndicatorWidgetClass"));
        AwStatusBarProcessIndicatorWidgetClass->resize(532, 31);
        AwStatusBarProcessIndicatorWidgetClass->setMaximumSize(QSize(16777215, 31));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwStatusBarProcessIndicatorWidgetClass->setFont(font);
        AwStatusBarProcessIndicatorWidgetClass->setStyleSheet(QStringLiteral(""));
        gridLayout = new QGridLayout(AwStatusBarProcessIndicatorWidgetClass);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        frame = new QFrame(AwStatusBarProcessIndicatorWidgetClass);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setStyleSheet(QStringLiteral(""));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Plain);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        processIndicator = new AwProgressIndicator(frame);
        processIndicator->setObjectName(QStringLiteral("processIndicator"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(processIndicator->sizePolicy().hasHeightForWidth());
        processIndicator->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(processIndicator);

        labelProcess = new QLabel(frame);
        labelProcess->setObjectName(QStringLiteral("labelProcess"));
        sizePolicy.setHeightForWidth(labelProcess->sizePolicy().hasHeightForWidth());
        labelProcess->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        labelProcess->setFont(font1);
        labelProcess->setStyleSheet(QStringLiteral("font:bold;"));
        labelProcess->setScaledContents(true);
        labelProcess->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(labelProcess);

        labelMessage = new QLabel(frame);
        labelMessage->setObjectName(QStringLiteral("labelMessage"));
        sizePolicy.setHeightForWidth(labelMessage->sizePolicy().hasHeightForWidth());
        labelMessage->setSizePolicy(sizePolicy);
        labelMessage->setFont(font1);
        labelMessage->setStyleSheet(QStringLiteral("font:bold;"));
        labelMessage->setScaledContents(true);
        labelMessage->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(labelMessage);

        buttonMenu = new QToolButton(frame);
        buttonMenu->setObjectName(QStringLiteral("buttonMenu"));
        buttonMenu->setContextMenuPolicy(Qt::DefaultContextMenu);
        buttonMenu->setCheckable(false);
        buttonMenu->setPopupMode(QToolButton::InstantPopup);
        buttonMenu->setToolButtonStyle(Qt::ToolButtonTextOnly);

        horizontalLayout->addWidget(buttonMenu);


        gridLayout_2->addLayout(horizontalLayout, 0, 2, 1, 1);


        gridLayout->addWidget(frame, 0, 0, 1, 1);


        retranslateUi(AwStatusBarProcessIndicatorWidgetClass);

        QMetaObject::connectSlotsByName(AwStatusBarProcessIndicatorWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *AwStatusBarProcessIndicatorWidgetClass)
    {
        AwStatusBarProcessIndicatorWidgetClass->setWindowTitle(QApplication::translate("AwStatusBarProcessIndicatorWidgetClass", "AwProcessIndicatorWidget", Q_NULLPTR));
        labelProcess->setText(QApplication::translate("AwStatusBarProcessIndicatorWidgetClass", "Time Frequency", Q_NULLPTR));
        labelMessage->setText(QApplication::translate("AwStatusBarProcessIndicatorWidgetClass", "Running...", Q_NULLPTR));
        buttonMenu->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AwStatusBarProcessIndicatorWidgetClass: public Ui_AwStatusBarProcessIndicatorWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWSTATUSBARPROCESSINDICATORWIDGET_H
