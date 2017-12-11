/********************************************************************************
** Form generated from reading UI file 'AwMontageToolBar.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMONTAGETOOLBAR_H
#define UI_AWMONTAGETOOLBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwMontageToolBarUi
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *buttonLoad;
    QPushButton *buttonEdit;

    void setupUi(QWidget *AwMontageToolBarUi)
    {
        if (AwMontageToolBarUi->objectName().isEmpty())
            AwMontageToolBarUi->setObjectName(QStringLiteral("AwMontageToolBarUi"));
        AwMontageToolBarUi->resize(244, 26);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwMontageToolBarUi->setFont(font);
        gridLayout = new QGridLayout(AwMontageToolBarUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwMontageToolBarUi);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        buttonLoad = new QPushButton(AwMontageToolBarUi);
        buttonLoad->setObjectName(QStringLiteral("buttonLoad"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonLoad->sizePolicy().hasHeightForWidth());
        buttonLoad->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(buttonLoad);

        buttonEdit = new QPushButton(AwMontageToolBarUi);
        buttonEdit->setObjectName(QStringLiteral("buttonEdit"));
        sizePolicy1.setHeightForWidth(buttonEdit->sizePolicy().hasHeightForWidth());
        buttonEdit->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(buttonEdit);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwMontageToolBarUi);

        QMetaObject::connectSlotsByName(AwMontageToolBarUi);
    } // setupUi

    void retranslateUi(QWidget *AwMontageToolBarUi)
    {
        AwMontageToolBarUi->setWindowTitle(QApplication::translate("AwMontageToolBarUi", "AwMontageToolBar", Q_NULLPTR));
        label->setText(QApplication::translate("AwMontageToolBarUi", "Montages:", Q_NULLPTR));
        buttonLoad->setText(QApplication::translate("AwMontageToolBarUi", "Load Montage", Q_NULLPTR));
        buttonEdit->setText(QApplication::translate("AwMontageToolBarUi", "Edit Montage", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwMontageToolBarUi: public Ui_AwMontageToolBarUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMONTAGETOOLBAR_H
