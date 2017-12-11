/********************************************************************************
** Form generated from reading UI file 'AwWaitWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWWAITWIDGET_H
#define UI_AWWAITWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwWaitWidget
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QProgressBar *progressBar;

    void setupUi(QWidget *AwWaitWidget)
    {
        if (AwWaitWidget->objectName().isEmpty())
            AwWaitWidget->setObjectName(QStringLiteral("AwWaitWidget"));
        AwWaitWidget->resize(400, 69);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwWaitWidget->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwWaitWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(AwWaitWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        progressBar = new QProgressBar(AwWaitWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMinimumSize(QSize(150, 0));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(progressBar);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwWaitWidget);

        QMetaObject::connectSlotsByName(AwWaitWidget);
    } // setupUi

    void retranslateUi(QWidget *AwWaitWidget)
    {
        AwWaitWidget->setWindowTitle(QApplication::translate("AwWaitWidget", "Please wait...", Q_NULLPTR));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AwWaitWidget: public Ui_AwWaitWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWWAITWIDGET_H
