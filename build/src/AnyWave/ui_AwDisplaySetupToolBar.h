/********************************************************************************
** Form generated from reading UI file 'AwDisplaySetupToolBar.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWDISPLAYSETUPTOOLBAR_H
#define UI_AWDISPLAYSETUPTOOLBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwDisplaySetupToolBarUI
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboSetups;
    QToolButton *buttonSave;

    void setupUi(QWidget *AwDisplaySetupToolBarUI)
    {
        if (AwDisplaySetupToolBarUI->objectName().isEmpty())
            AwDisplaySetupToolBarUI->setObjectName(QStringLiteral("AwDisplaySetupToolBarUI"));
        AwDisplaySetupToolBarUI->resize(221, 24);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwDisplaySetupToolBarUI->setFont(font);
        gridLayout = new QGridLayout(AwDisplaySetupToolBarUI);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwDisplaySetupToolBarUI);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);

        comboSetups = new QComboBox(AwDisplaySetupToolBarUI);
        comboSetups->setObjectName(QStringLiteral("comboSetups"));
        sizePolicy.setHeightForWidth(comboSetups->sizePolicy().hasHeightForWidth());
        comboSetups->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        comboSetups->setFont(font1);
        comboSetups->setEditable(false);
        comboSetups->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout->addWidget(comboSetups);

        buttonSave = new QToolButton(AwDisplaySetupToolBarUI);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        sizePolicy.setHeightForWidth(buttonSave->sizePolicy().hasHeightForWidth());
        buttonSave->setSizePolicy(sizePolicy);
        buttonSave->setMaximumSize(QSize(16777215, 16777215));
        buttonSave->setSizeIncrement(QSize(1, 1));
        QFont font2;
        font2.setPointSize(10);
        buttonSave->setFont(font2);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/tick.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSave->setIcon(icon);
        buttonSave->setPopupMode(QToolButton::InstantPopup);
        buttonSave->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttonSave->setAutoRaise(true);

        horizontalLayout->addWidget(buttonSave);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwDisplaySetupToolBarUI);

        QMetaObject::connectSlotsByName(AwDisplaySetupToolBarUI);
    } // setupUi

    void retranslateUi(QWidget *AwDisplaySetupToolBarUI)
    {
        AwDisplaySetupToolBarUI->setWindowTitle(QApplication::translate("AwDisplaySetupToolBarUI", "AwDisplaySetupToolBar", Q_NULLPTR));
        label->setText(QApplication::translate("AwDisplaySetupToolBarUI", "Display Setup:", Q_NULLPTR));
        buttonSave->setText(QApplication::translate("AwDisplaySetupToolBarUI", "Edit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwDisplaySetupToolBarUI: public Ui_AwDisplaySetupToolBarUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWDISPLAYSETUPTOOLBAR_H
