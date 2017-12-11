/********************************************************************************
** Form generated from reading UI file 'AwPleaseWaitWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPLEASEWAITWIDGET_H
#define UI_AWPLEASEWAITWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PleaseWaitWidgetClass
{
public:
    QGridLayout *gridLayout;
    QLabel *labelText;

    void setupUi(QWidget *PleaseWaitWidgetClass)
    {
        if (PleaseWaitWidgetClass->objectName().isEmpty())
            PleaseWaitWidgetClass->setObjectName(QStringLiteral("PleaseWaitWidgetClass"));
        PleaseWaitWidgetClass->resize(537, 57);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PleaseWaitWidgetClass->sizePolicy().hasHeightForWidth());
        PleaseWaitWidgetClass->setSizePolicy(sizePolicy);
        PleaseWaitWidgetClass->setMinimumSize(QSize(399, 57));
        PleaseWaitWidgetClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        PleaseWaitWidgetClass->setFont(font);
        PleaseWaitWidgetClass->setContextMenuPolicy(Qt::NoContextMenu);
        gridLayout = new QGridLayout(PleaseWaitWidgetClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        labelText = new QLabel(PleaseWaitWidgetClass);
        labelText->setObjectName(QStringLiteral("labelText"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelText->sizePolicy().hasHeightForWidth());
        labelText->setSizePolicy(sizePolicy1);
        labelText->setMinimumSize(QSize(100, 15));
        labelText->setScaledContents(false);
        labelText->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labelText, 0, 0, 1, 1);


        retranslateUi(PleaseWaitWidgetClass);

        QMetaObject::connectSlotsByName(PleaseWaitWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *PleaseWaitWidgetClass)
    {
        PleaseWaitWidgetClass->setWindowTitle(QApplication::translate("PleaseWaitWidgetClass", "Please Wait", Q_NULLPTR));
        labelText->setText(QApplication::translate("PleaseWaitWidgetClass", "Creating Mesh...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PleaseWaitWidgetClass: public Ui_PleaseWaitWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPLEASEWAITWIDGET_H
