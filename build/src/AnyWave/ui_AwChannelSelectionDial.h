/********************************************************************************
** Form generated from reading UI file 'AwChannelSelectionDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWCHANNELSELECTIONDIAL_H
#define UI_AWCHANNELSELECTIONDIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_AwChannelSelectionDialClass
{
public:
    QGridLayout *gridLayout;
    QTableView *tvChannels;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonApply;

    void setupUi(QDialog *AwChannelSelectionDialClass)
    {
        if (AwChannelSelectionDialClass->objectName().isEmpty())
            AwChannelSelectionDialClass->setObjectName(QStringLiteral("AwChannelSelectionDialClass"));
        AwChannelSelectionDialClass->setWindowModality(Qt::ApplicationModal);
        AwChannelSelectionDialClass->resize(245, 452);
        AwChannelSelectionDialClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        AwChannelSelectionDialClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwChannelSelectionDialClass->setWindowIcon(icon);
        AwChannelSelectionDialClass->setModal(true);
        gridLayout = new QGridLayout(AwChannelSelectionDialClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        tvChannels = new QTableView(AwChannelSelectionDialClass);
        tvChannels->setObjectName(QStringLiteral("tvChannels"));
        tvChannels->setFocusPolicy(Qt::ClickFocus);
        tvChannels->setContextMenuPolicy(Qt::CustomContextMenu);
        tvChannels->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tvChannels->setEditTriggers(QAbstractItemView::SelectedClicked);
        tvChannels->setSelectionMode(QAbstractItemView::NoSelection);
        tvChannels->setCornerButtonEnabled(false);
        tvChannels->horizontalHeader()->setStretchLastSection(true);
        tvChannels->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tvChannels, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonApply = new QPushButton(AwChannelSelectionDialClass);
        buttonApply->setObjectName(QStringLiteral("buttonApply"));
        buttonApply->setAutoDefault(false);

        horizontalLayout->addWidget(buttonApply);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(AwChannelSelectionDialClass);
        QObject::connect(buttonApply, SIGNAL(clicked()), AwChannelSelectionDialClass, SLOT(accept()));

        buttonApply->setDefault(true);


        QMetaObject::connectSlotsByName(AwChannelSelectionDialClass);
    } // setupUi

    void retranslateUi(QDialog *AwChannelSelectionDialClass)
    {
        AwChannelSelectionDialClass->setWindowTitle(QApplication::translate("AwChannelSelectionDialClass", "Channels Selection", Q_NULLPTR));
        buttonApply->setText(QApplication::translate("AwChannelSelectionDialClass", "OK", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwChannelSelectionDialClass: public Ui_AwChannelSelectionDialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWCHANNELSELECTIONDIAL_H
