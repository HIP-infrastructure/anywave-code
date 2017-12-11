/********************************************************************************
** Form generated from reading UI file 'AwProcessWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPROCESSWIDGET_H
#define UI_AWPROCESSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwProcessWidgetUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelName;
    QSpacerItem *horizontalSpacer_3;
    QLabel *labelMessage;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonStop;
    QProgressBar *progress;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonRemove;
    QPushButton *buttonShowResults;
    QPushButton *buttonShowLog;
    QSpacerItem *horizontalSpacer;
    QLabel *labelTime;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *AwProcessWidgetUi)
    {
        if (AwProcessWidgetUi->objectName().isEmpty())
            AwProcessWidgetUi->setObjectName(QStringLiteral("AwProcessWidgetUi"));
        AwProcessWidgetUi->resize(400, 132);
        gridLayout = new QGridLayout(AwProcessWidgetUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, -1, -1, 5);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout_3->setContentsMargins(-1, 5, -1, 5);
        labelName = new QLabel(AwProcessWidgetUi);
        labelName->setObjectName(QStringLiteral("labelName"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelName->sizePolicy().hasHeightForWidth());
        labelName->setSizePolicy(sizePolicy);
        labelName->setMinimumSize(QSize(0, 20));
        labelName->setStyleSheet(QStringLiteral("font: bold;"));

        horizontalLayout_3->addWidget(labelName);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        labelMessage = new QLabel(AwProcessWidgetUi);
        labelMessage->setObjectName(QStringLiteral("labelMessage"));
        sizePolicy.setHeightForWidth(labelMessage->sizePolicy().hasHeightForWidth());
        labelMessage->setSizePolicy(sizePolicy);
        labelMessage->setMinimumSize(QSize(0, 20));
        labelMessage->setStyleSheet(QLatin1String("font: bold;\n"
"color: blue;"));

        horizontalLayout_3->addWidget(labelMessage);

        horizontalLayout_3->setStretch(2, 1);

        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout_2->setContentsMargins(-1, 5, -1, 5);
        buttonStop = new QPushButton(AwProcessWidgetUi);
        buttonStop->setObjectName(QStringLiteral("buttonStop"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonStop->sizePolicy().hasHeightForWidth());
        buttonStop->setSizePolicy(sizePolicy1);
        buttonStop->setMinimumSize(QSize(0, 20));
        buttonStop->setMaximumSize(QSize(16777215, 16777215));
        buttonStop->setStyleSheet(QStringLiteral(""));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/cross.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonStop->setIcon(icon);
        buttonStop->setFlat(false);

        horizontalLayout_2->addWidget(buttonStop);

        progress = new QProgressBar(AwProcessWidgetUi);
        progress->setObjectName(QStringLiteral("progress"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(progress->sizePolicy().hasHeightForWidth());
        progress->setSizePolicy(sizePolicy2);
        progress->setMinimumSize(QSize(0, 20));
        progress->setValue(24);

        horizontalLayout_2->addWidget(progress);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout_2->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout->setContentsMargins(-1, 5, -1, 5);
        buttonRemove = new QPushButton(AwProcessWidgetUi);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        sizePolicy1.setHeightForWidth(buttonRemove->sizePolicy().hasHeightForWidth());
        buttonRemove->setSizePolicy(sizePolicy1);
        buttonRemove->setMinimumSize(QSize(0, 20));

        horizontalLayout->addWidget(buttonRemove);

        buttonShowResults = new QPushButton(AwProcessWidgetUi);
        buttonShowResults->setObjectName(QStringLiteral("buttonShowResults"));
        sizePolicy1.setHeightForWidth(buttonShowResults->sizePolicy().hasHeightForWidth());
        buttonShowResults->setSizePolicy(sizePolicy1);
        buttonShowResults->setMinimumSize(QSize(0, 20));

        horizontalLayout->addWidget(buttonShowResults);

        buttonShowLog = new QPushButton(AwProcessWidgetUi);
        buttonShowLog->setObjectName(QStringLiteral("buttonShowLog"));
        sizePolicy1.setHeightForWidth(buttonShowLog->sizePolicy().hasHeightForWidth());
        buttonShowLog->setSizePolicy(sizePolicy1);
        buttonShowLog->setMinimumSize(QSize(0, 20));

        horizontalLayout->addWidget(buttonShowLog);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelTime = new QLabel(AwProcessWidgetUi);
        labelTime->setObjectName(QStringLiteral("labelTime"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(labelTime->sizePolicy().hasHeightForWidth());
        labelTime->setSizePolicy(sizePolicy3);
        labelTime->setMinimumSize(QSize(0, 20));

        horizontalLayout->addWidget(labelTime);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);


        retranslateUi(AwProcessWidgetUi);

        buttonStop->setDefault(true);


        QMetaObject::connectSlotsByName(AwProcessWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwProcessWidgetUi)
    {
        AwProcessWidgetUi->setWindowTitle(QApplication::translate("AwProcessWidgetUi", "Processes", Q_NULLPTR));
        labelName->setText(QApplication::translate("AwProcessWidgetUi", "Process Name", Q_NULLPTR));
        labelMessage->setText(QApplication::translate("AwProcessWidgetUi", "Message", Q_NULLPTR));
        buttonStop->setText(QApplication::translate("AwProcessWidgetUi", "Stop", Q_NULLPTR));
        buttonRemove->setText(QApplication::translate("AwProcessWidgetUi", "Remove", Q_NULLPTR));
        buttonShowResults->setText(QApplication::translate("AwProcessWidgetUi", "Show Results", Q_NULLPTR));
        buttonShowLog->setText(QApplication::translate("AwProcessWidgetUi", "Show Log", Q_NULLPTR));
        labelTime->setText(QApplication::translate("AwProcessWidgetUi", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwProcessWidgetUi: public Ui_AwProcessWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPROCESSWIDGET_H
