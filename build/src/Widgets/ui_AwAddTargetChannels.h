/********************************************************************************
** Form generated from reading UI file 'AwAddTargetChannels.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWADDTARGETCHANNELS_H
#define UI_AWADDTARGETCHANNELS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwAddTargetChannelsUI
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QListWidget *listWidgetAvailable;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *AwAddTargetChannelsUI)
    {
        if (AwAddTargetChannelsUI->objectName().isEmpty())
            AwAddTargetChannelsUI->setObjectName(QStringLiteral("AwAddTargetChannelsUI"));
        AwAddTargetChannelsUI->resize(299, 325);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwAddTargetChannelsUI->setWindowIcon(icon);
        AwAddTargetChannelsUI->setSizeGripEnabled(true);
        AwAddTargetChannelsUI->setModal(true);
        gridLayout = new QGridLayout(AwAddTargetChannelsUI);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(AwAddTargetChannelsUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        listWidgetAvailable = new QListWidget(AwAddTargetChannelsUI);
        listWidgetAvailable->setObjectName(QStringLiteral("listWidgetAvailable"));
        listWidgetAvailable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidgetAvailable->setSelectionMode(QAbstractItemView::ExtendedSelection);

        verticalLayout->addWidget(listWidgetAvailable);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton = new QPushButton(AwAddTargetChannelsUI);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        pushButton_2 = new QPushButton(AwAddTargetChannelsUI);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwAddTargetChannelsUI);
        QObject::connect(pushButton, SIGNAL(clicked()), AwAddTargetChannelsUI, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), AwAddTargetChannelsUI, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwAddTargetChannelsUI);
    } // setupUi

    void retranslateUi(QDialog *AwAddTargetChannelsUI)
    {
        AwAddTargetChannelsUI->setWindowTitle(QApplication::translate("AwAddTargetChannelsUI", "Add targets", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwAddTargetChannelsUI", "Select channels:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AwAddTargetChannelsUI", "Add", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("AwAddTargetChannelsUI", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwAddTargetChannelsUI: public Ui_AwAddTargetChannelsUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWADDTARGETCHANNELS_H
