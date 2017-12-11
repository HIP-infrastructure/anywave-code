/********************************************************************************
** Form generated from reading UI file 'AwMappingToolBar.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMAPPINGTOOLBAR_H
#define UI_AWMAPPINGTOOLBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwMappingToolBarUI
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonMapping;
    QPushButton *buttonGARDEL;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *AwMappingToolBarUI)
    {
        if (AwMappingToolBarUI->objectName().isEmpty())
            AwMappingToolBarUI->setObjectName(QStringLiteral("AwMappingToolBarUI"));
        AwMappingToolBarUI->resize(94, 42);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwMappingToolBarUI->setFont(font);
        gridLayout = new QGridLayout(AwMappingToolBarUI);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonMapping = new QPushButton(AwMappingToolBarUI);
        buttonMapping->setObjectName(QStringLiteral("buttonMapping"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(32);
        sizePolicy.setVerticalStretch(32);
        sizePolicy.setHeightForWidth(buttonMapping->sizePolicy().hasHeightForWidth());
        buttonMapping->setSizePolicy(sizePolicy);
        buttonMapping->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/carto_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonMapping->setIcon(icon);
        buttonMapping->setIconSize(QSize(32, 32));
        buttonMapping->setFlat(true);

        horizontalLayout->addWidget(buttonMapping);

        buttonGARDEL = new QPushButton(AwMappingToolBarUI);
        buttonGARDEL->setObjectName(QStringLiteral("buttonGARDEL"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/GARDEL_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonGARDEL->setIcon(icon1);
        buttonGARDEL->setIconSize(QSize(32, 32));
        buttonGARDEL->setFlat(true);

        horizontalLayout->addWidget(buttonGARDEL);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwMappingToolBarUI);

        QMetaObject::connectSlotsByName(AwMappingToolBarUI);
    } // setupUi

    void retranslateUi(QWidget *AwMappingToolBarUI)
    {
        AwMappingToolBarUI->setWindowTitle(QApplication::translate("AwMappingToolBarUI", "AwMappingToolBar", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonMapping->setToolTip(QApplication::translate("AwMappingToolBarUI", "Launch 3D Mapping", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonMapping->setText(QString());
        buttonGARDEL->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AwMappingToolBarUI: public Ui_AwMappingToolBarUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMAPPINGTOOLBAR_H
