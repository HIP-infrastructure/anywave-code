/********************************************************************************
** Form generated from reading UI file 'AwExportSelChannels.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWEXPORTSELCHANNELS_H
#define UI_AWEXPORTSELCHANNELS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwExportSelChannelsUi
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *AwExportSelChannelsUi)
    {
        if (AwExportSelChannelsUi->objectName().isEmpty())
            AwExportSelChannelsUi->setObjectName(QStringLiteral("AwExportSelChannelsUi"));
        AwExportSelChannelsUi->setWindowModality(Qt::WindowModal);
        AwExportSelChannelsUi->resize(321, 414);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwExportSelChannelsUi->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(AwExportSelChannelsUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(AwExportSelChannelsUi);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(AwExportSelChannelsUi);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(AwExportSelChannelsUi);
        QObject::connect(pushButton, SIGNAL(clicked()), AwExportSelChannelsUi, SLOT(accept()));

        QMetaObject::connectSlotsByName(AwExportSelChannelsUi);
    } // setupUi

    void retranslateUi(QDialog *AwExportSelChannelsUi)
    {
        AwExportSelChannelsUi->setWindowTitle(QApplication::translate("AwExportSelChannelsUi", "Select Channels", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AwExportSelChannelsUi", "OK", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwExportSelChannelsUi: public Ui_AwExportSelChannelsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWEXPORTSELCHANNELS_H
