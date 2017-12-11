/********************************************************************************
** Form generated from reading UI file 'AwGoToLatencyDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWGOTOLATENCYDIAL_H
#define UI_AWGOTOLATENCYDIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_GoToLatencyDialClass
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QDoubleSpinBox *spinBoxLatency;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *GoToLatencyDialClass)
    {
        if (GoToLatencyDialClass->objectName().isEmpty())
            GoToLatencyDialClass->setObjectName(QStringLiteral("GoToLatencyDialClass"));
        GoToLatencyDialClass->resize(156, 44);
        GoToLatencyDialClass->setMinimumSize(QSize(0, 0));
        GoToLatencyDialClass->setMaximumSize(QSize(156, 78));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        GoToLatencyDialClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        GoToLatencyDialClass->setWindowIcon(icon);
        gridLayout = new QGridLayout(GoToLatencyDialClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        spinBoxLatency = new QDoubleSpinBox(GoToLatencyDialClass);
        spinBoxLatency->setObjectName(QStringLiteral("spinBoxLatency"));
        spinBoxLatency->setDecimals(3);
        spinBoxLatency->setMaximum(1e+9);

        horizontalLayout->addWidget(spinBoxLatency);

        pushButton = new QPushButton(GoToLatencyDialClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(GoToLatencyDialClass);
        QObject::connect(pushButton, SIGNAL(clicked()), GoToLatencyDialClass, SLOT(accept()));

        QMetaObject::connectSlotsByName(GoToLatencyDialClass);
    } // setupUi

    void retranslateUi(QDialog *GoToLatencyDialClass)
    {
        GoToLatencyDialClass->setWindowTitle(QApplication::translate("GoToLatencyDialClass", "Go to latency...", Q_NULLPTR));
        spinBoxLatency->setSuffix(QApplication::translate("GoToLatencyDialClass", "s", Q_NULLPTR));
        pushButton->setText(QApplication::translate("GoToLatencyDialClass", "Go to", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GoToLatencyDialClass: public Ui_GoToLatencyDialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWGOTOLATENCYDIAL_H
