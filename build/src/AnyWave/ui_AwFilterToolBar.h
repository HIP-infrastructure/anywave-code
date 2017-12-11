/********************************************************************************
** Form generated from reading UI file 'AwFilterToolBar.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWFILTERTOOLBAR_H
#define UI_AWFILTERTOOLBAR_H

#include <Filter/AwFilterSwitchButton.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwFilterToolBarClass
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonFilter;
    AwFilterSwitchButton *buttonSwitch;
    QLabel *labelICA;
    QRadioButton *radioICA_ON;
    QRadioButton *radioICA_OFF;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *AwFilterToolBarClass)
    {
        if (AwFilterToolBarClass->objectName().isEmpty())
            AwFilterToolBarClass->setObjectName(QStringLiteral("AwFilterToolBarClass"));
        AwFilterToolBarClass->resize(430, 34);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        AwFilterToolBarClass->setFont(font);
        gridLayout = new QGridLayout(AwFilterToolBarClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonFilter = new QPushButton(AwFilterToolBarClass);
        buttonFilter->setObjectName(QStringLiteral("buttonFilter"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(32);
        sizePolicy.setVerticalStretch(32);
        sizePolicy.setHeightForWidth(buttonFilter->sizePolicy().hasHeightForWidth());
        buttonFilter->setSizePolicy(sizePolicy);
        buttonFilter->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/ox_view_filter_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonFilter->setIcon(icon);
        buttonFilter->setIconSize(QSize(32, 32));
        buttonFilter->setFlat(true);

        horizontalLayout->addWidget(buttonFilter);

        buttonSwitch = new AwFilterSwitchButton(AwFilterToolBarClass);
        buttonSwitch->setObjectName(QStringLiteral("buttonSwitch"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonSwitch->sizePolicy().hasHeightForWidth());
        buttonSwitch->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(buttonSwitch);

        labelICA = new QLabel(AwFilterToolBarClass);
        labelICA->setObjectName(QStringLiteral("labelICA"));

        horizontalLayout->addWidget(labelICA);

        radioICA_ON = new QRadioButton(AwFilterToolBarClass);
        radioICA_ON->setObjectName(QStringLiteral("radioICA_ON"));
        sizePolicy1.setHeightForWidth(radioICA_ON->sizePolicy().hasHeightForWidth());
        radioICA_ON->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(radioICA_ON);

        radioICA_OFF = new QRadioButton(AwFilterToolBarClass);
        radioICA_OFF->setObjectName(QStringLiteral("radioICA_OFF"));
        sizePolicy1.setHeightForWidth(radioICA_OFF->sizePolicy().hasHeightForWidth());
        radioICA_OFF->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(radioICA_OFF);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwFilterToolBarClass);

        QMetaObject::connectSlotsByName(AwFilterToolBarClass);
    } // setupUi

    void retranslateUi(QWidget *AwFilterToolBarClass)
    {
        AwFilterToolBarClass->setWindowTitle(QApplication::translate("AwFilterToolBarClass", "AwFilterToolBar", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonFilter->setToolTip(QApplication::translate("AwFilterToolBarClass", "Open Filter Settings UI", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonFilter->setText(QString());
        buttonSwitch->setText(QApplication::translate("AwFilterToolBarClass", "No Filters", Q_NULLPTR));
        labelICA->setText(QApplication::translate("AwFilterToolBarClass", "ICA:", Q_NULLPTR));
        radioICA_ON->setText(QApplication::translate("AwFilterToolBarClass", "Filtering", Q_NULLPTR));
        radioICA_OFF->setText(QApplication::translate("AwFilterToolBarClass", "No Filtering", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwFilterToolBarClass: public Ui_AwFilterToolBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWFILTERTOOLBAR_H
