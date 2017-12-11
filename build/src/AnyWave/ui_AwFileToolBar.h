/********************************************************************************
** Form generated from reading UI file 'AwFileToolBar.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWFILETOOLBAR_H
#define UI_AWFILETOOLBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwFileToolBarClass
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonOpen;
    QPushButton *buttonSave;
    QPushButton *buttonICA;

    void setupUi(QWidget *AwFileToolBarClass)
    {
        if (AwFileToolBarClass->objectName().isEmpty())
            AwFileToolBarClass->setObjectName(QStringLiteral("AwFileToolBarClass"));
        AwFileToolBarClass->resize(110, 34);
        gridLayout = new QGridLayout(AwFileToolBarClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonOpen = new QPushButton(AwFileToolBarClass);
        buttonOpen->setObjectName(QStringLiteral("buttonOpen"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(32);
        sizePolicy.setVerticalStretch(32);
        sizePolicy.setHeightForWidth(buttonOpen->sizePolicy().hasHeightForWidth());
        buttonOpen->setSizePolicy(sizePolicy);
        buttonOpen->setMaximumSize(QSize(32, 32));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        buttonOpen->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/fileopen-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonOpen->setIcon(icon);
        buttonOpen->setIconSize(QSize(32, 32));
        buttonOpen->setFlat(true);

        horizontalLayout->addWidget(buttonOpen);

        buttonSave = new QPushButton(AwFileToolBarClass);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        buttonSave->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(32);
        sizePolicy1.setVerticalStretch(32);
        sizePolicy1.setHeightForWidth(buttonSave->sizePolicy().hasHeightForWidth());
        buttonSave->setSizePolicy(sizePolicy1);
        buttonSave->setMaximumSize(QSize(32, 32));
        buttonSave->setFont(font);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/ox_document_save_as_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSave->setIcon(icon1);
        buttonSave->setIconSize(QSize(32, 32));
        buttonSave->setFlat(true);

        horizontalLayout->addWidget(buttonSave);

        buttonICA = new QPushButton(AwFileToolBarClass);
        buttonICA->setObjectName(QStringLiteral("buttonICA"));
        buttonICA->setEnabled(false);
        sizePolicy1.setHeightForWidth(buttonICA->sizePolicy().hasHeightForWidth());
        buttonICA->setSizePolicy(sizePolicy1);
        buttonICA->setMaximumSize(QSize(32, 32));
        buttonICA->setFont(font);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/importICA32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonICA->setIcon(icon2);
        buttonICA->setIconSize(QSize(32, 32));
        buttonICA->setFlat(true);

        horizontalLayout->addWidget(buttonICA);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwFileToolBarClass);

        QMetaObject::connectSlotsByName(AwFileToolBarClass);
    } // setupUi

    void retranslateUi(QWidget *AwFileToolBarClass)
    {
        AwFileToolBarClass->setWindowTitle(QApplication::translate("AwFileToolBarClass", "AwFileToolBar", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonOpen->setToolTip(QApplication::translate("AwFileToolBarClass", "Open data file", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonOpen->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonSave->setToolTip(QApplication::translate("AwFileToolBarClass", "Save or export data", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonSave->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonICA->setToolTip(QApplication::translate("AwFileToolBarClass", "Save or export data", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonICA->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AwFileToolBarClass: public Ui_AwFileToolBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWFILETOOLBAR_H
