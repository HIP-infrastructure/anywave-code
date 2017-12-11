/********************************************************************************
** Form generated from reading UI file 'AwMontageDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMONTAGEDIAL_H
#define UI_AWMONTAGEDIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include "Montage/AwMontageTableView.h"

QT_BEGIN_NAMESPACE

class Ui_MontageDialClass
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupAsRecorded;
    QVBoxLayout *verticalLayout_3;
    QTableView *tvChannelsAsRecorded;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *buttonEEGSEEG;
    QSpacerItem *horizontalSpacer_7;
    QGroupBox *groupTransfer;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *buttonAddChannels;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_2;
    QPushButton *buttonAddTypes;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupMontage;
    QVBoxLayout *verticalLayout;
    AwMontageTableView *tvDisplay;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonRemove;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonUp;
    QPushButton *buttonDown;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonReset;
    QPushButton *buttonClean;
    QPushButton *buttonSortName;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonSave;
    QPushButton *buttonLoad;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *buttonAutomaticMontage;
    QPushButton *buttonAVG;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonApply;
    QPushButton *buttonCancel;

    void setupUi(QDialog *MontageDialClass)
    {
        if (MontageDialClass->objectName().isEmpty())
            MontageDialClass->setObjectName(QStringLiteral("MontageDialClass"));
        MontageDialClass->resize(1115, 811);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        MontageDialClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MontageDialClass->setWindowIcon(icon);
        verticalLayout_4 = new QVBoxLayout(MontageDialClass);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        groupAsRecorded = new QGroupBox(MontageDialClass);
        groupAsRecorded->setObjectName(QStringLiteral("groupAsRecorded"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupAsRecorded->sizePolicy().hasHeightForWidth());
        groupAsRecorded->setSizePolicy(sizePolicy);
        groupAsRecorded->setMinimumSize(QSize(220, 570));
        groupAsRecorded->setMaximumSize(QSize(16777215, 16777215));
        groupAsRecorded->setFlat(false);
        verticalLayout_3 = new QVBoxLayout(groupAsRecorded);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tvChannelsAsRecorded = new QTableView(groupAsRecorded);
        tvChannelsAsRecorded->setObjectName(QStringLiteral("tvChannelsAsRecorded"));
        sizePolicy.setHeightForWidth(tvChannelsAsRecorded->sizePolicy().hasHeightForWidth());
        tvChannelsAsRecorded->setSizePolicy(sizePolicy);
        tvChannelsAsRecorded->setMinimumSize(QSize(200, 0));
        tvChannelsAsRecorded->setMaximumSize(QSize(16777215, 16777215));
        tvChannelsAsRecorded->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        tvChannelsAsRecorded->setDragEnabled(true);
        tvChannelsAsRecorded->setDragDropOverwriteMode(true);
        tvChannelsAsRecorded->setDragDropMode(QAbstractItemView::DragOnly);
        tvChannelsAsRecorded->setDefaultDropAction(Qt::CopyAction);
        tvChannelsAsRecorded->setAlternatingRowColors(true);
        tvChannelsAsRecorded->setSortingEnabled(true);
        tvChannelsAsRecorded->horizontalHeader()->setStretchLastSection(false);

        verticalLayout_3->addWidget(tvChannelsAsRecorded);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        buttonEEGSEEG = new QPushButton(groupAsRecorded);
        buttonEEGSEEG->setObjectName(QStringLiteral("buttonEEGSEEG"));

        horizontalLayout_6->addWidget(buttonEEGSEEG);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);


        verticalLayout_3->addLayout(horizontalLayout_6);


        horizontalLayout_7->addWidget(groupAsRecorded);

        groupTransfer = new QGroupBox(MontageDialClass);
        groupTransfer->setObjectName(QStringLiteral("groupTransfer"));
        verticalLayout_2 = new QVBoxLayout(groupTransfer);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        buttonAddChannels = new QPushButton(groupTransfer);
        buttonAddChannels->setObjectName(QStringLiteral("buttonAddChannels"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonAddChannels->sizePolicy().hasHeightForWidth());
        buttonAddChannels->setSizePolicy(sizePolicy1);
        buttonAddChannels->setMinimumSize(QSize(50, 50));
        buttonAddChannels->setMaximumSize(QSize(50, 50));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/AddToMontage48x48.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonAddChannels->setIcon(icon1);
        buttonAddChannels->setIconSize(QSize(48, 48));
        buttonAddChannels->setFlat(true);

        horizontalLayout_5->addWidget(buttonAddChannels);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        buttonAddTypes = new QPushButton(groupTransfer);
        buttonAddTypes->setObjectName(QStringLiteral("buttonAddTypes"));

        verticalLayout_2->addWidget(buttonAddTypes);

        verticalSpacer = new QSpacerItem(20, 205, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_7->addWidget(groupTransfer);

        groupMontage = new QGroupBox(MontageDialClass);
        groupMontage->setObjectName(QStringLiteral("groupMontage"));
        sizePolicy.setHeightForWidth(groupMontage->sizePolicy().hasHeightForWidth());
        groupMontage->setSizePolicy(sizePolicy);
        groupMontage->setMinimumSize(QSize(620, 570));
        groupMontage->setFlat(false);
        verticalLayout = new QVBoxLayout(groupMontage);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tvDisplay = new AwMontageTableView(groupMontage);
        tvDisplay->setObjectName(QStringLiteral("tvDisplay"));
        tvDisplay->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(2);
        sizePolicy2.setHeightForWidth(tvDisplay->sizePolicy().hasHeightForWidth());
        tvDisplay->setSizePolicy(sizePolicy2);
        tvDisplay->setMinimumSize(QSize(300, 0));
        tvDisplay->setAcceptDrops(true);
        tvDisplay->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        tvDisplay->setDragEnabled(true);
        tvDisplay->setDragDropMode(QAbstractItemView::DragDrop);
        tvDisplay->setDefaultDropAction(Qt::MoveAction);
        tvDisplay->setAlternatingRowColors(true);
        tvDisplay->setSelectionBehavior(QAbstractItemView::SelectRows);
        tvDisplay->setSortingEnabled(false);
        tvDisplay->setWordWrap(true);
        tvDisplay->setCornerButtonEnabled(false);
        tvDisplay->horizontalHeader()->setStretchLastSection(true);
        tvDisplay->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tvDisplay);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonRemove = new QPushButton(groupMontage);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buttonRemove->sizePolicy().hasHeightForWidth());
        buttonRemove->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(buttonRemove);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonUp = new QPushButton(groupMontage);
        buttonUp->setObjectName(QStringLiteral("buttonUp"));
        sizePolicy3.setHeightForWidth(buttonUp->sizePolicy().hasHeightForWidth());
        buttonUp->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(buttonUp);

        buttonDown = new QPushButton(groupMontage);
        buttonDown->setObjectName(QStringLiteral("buttonDown"));
        sizePolicy3.setHeightForWidth(buttonDown->sizePolicy().hasHeightForWidth());
        buttonDown->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(buttonDown);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        buttonReset = new QPushButton(groupMontage);
        buttonReset->setObjectName(QStringLiteral("buttonReset"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(buttonReset->sizePolicy().hasHeightForWidth());
        buttonReset->setSizePolicy(sizePolicy4);

        horizontalLayout_2->addWidget(buttonReset);

        buttonClean = new QPushButton(groupMontage);
        buttonClean->setObjectName(QStringLiteral("buttonClean"));
        sizePolicy4.setHeightForWidth(buttonClean->sizePolicy().hasHeightForWidth());
        buttonClean->setSizePolicy(sizePolicy4);
        buttonClean->setIconSize(QSize(16, 16));

        horizontalLayout_2->addWidget(buttonClean);

        buttonSortName = new QPushButton(groupMontage);
        buttonSortName->setObjectName(QStringLiteral("buttonSortName"));

        horizontalLayout_2->addWidget(buttonSortName);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonSave = new QPushButton(groupMontage);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        sizePolicy3.setHeightForWidth(buttonSave->sizePolicy().hasHeightForWidth());
        buttonSave->setSizePolicy(sizePolicy3);
        buttonSave->setMinimumSize(QSize(0, 0));
        buttonSave->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(buttonSave);

        buttonLoad = new QPushButton(groupMontage);
        buttonLoad->setObjectName(QStringLiteral("buttonLoad"));
        sizePolicy3.setHeightForWidth(buttonLoad->sizePolicy().hasHeightForWidth());
        buttonLoad->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(buttonLoad);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        buttonAutomaticMontage = new QPushButton(groupMontage);
        buttonAutomaticMontage->setObjectName(QStringLiteral("buttonAutomaticMontage"));
        sizePolicy4.setHeightForWidth(buttonAutomaticMontage->sizePolicy().hasHeightForWidth());
        buttonAutomaticMontage->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(buttonAutomaticMontage);

        buttonAVG = new QPushButton(groupMontage);
        buttonAVG->setObjectName(QStringLiteral("buttonAVG"));
        sizePolicy4.setHeightForWidth(buttonAVG->sizePolicy().hasHeightForWidth());
        buttonAVG->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(buttonAVG);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_7->addWidget(groupMontage);

        horizontalLayout_7->setStretch(2, 1);

        verticalLayout_4->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        buttonApply = new QPushButton(MontageDialClass);
        buttonApply->setObjectName(QStringLiteral("buttonApply"));

        horizontalLayout_4->addWidget(buttonApply);

        buttonCancel = new QPushButton(MontageDialClass);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));

        horizontalLayout_4->addWidget(buttonCancel);


        verticalLayout_4->addLayout(horizontalLayout_4);


        retranslateUi(MontageDialClass);
        QObject::connect(buttonApply, SIGNAL(clicked()), MontageDialClass, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), MontageDialClass, SLOT(reject()));

        buttonApply->setDefault(true);


        QMetaObject::connectSlotsByName(MontageDialClass);
    } // setupUi

    void retranslateUi(QDialog *MontageDialClass)
    {
        MontageDialClass->setWindowTitle(QApplication::translate("MontageDialClass", "Setup Display and Montage", Q_NULLPTR));
        groupAsRecorded->setTitle(QApplication::translate("MontageDialClass", "As Recorded Channels:", Q_NULLPTR));
        buttonEEGSEEG->setText(QApplication::translate("MontageDialClass", "Convert EEG to SEEG", Q_NULLPTR));
        groupTransfer->setTitle(QApplication::translate("MontageDialClass", "As recorded to Montage", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonAddChannels->setToolTip(QApplication::translate("MontageDialClass", "Select channels in the left list and click here to add them to the current montage.\n"
"Dragging channels from As Recorded list directly to the montage will also add them.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonAddChannels->setText(QString());
        buttonAddTypes->setText(QApplication::translate("MontageDialClass", "Add by types", Q_NULLPTR));
        groupMontage->setTitle(QApplication::translate("MontageDialClass", "Montage", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonRemove->setToolTip(QApplication::translate("MontageDialClass", "Remove selected channels from Montage", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonRemove->setText(QApplication::translate("MontageDialClass", "Remove Selected Channels", Q_NULLPTR));
        buttonRemove->setShortcut(QApplication::translate("MontageDialClass", "Del", Q_NULLPTR));
        buttonUp->setText(QApplication::translate("MontageDialClass", "Up", Q_NULLPTR));
        buttonDown->setText(QApplication::translate("MontageDialClass", "Down", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonReset->setToolTip(QApplication::translate("MontageDialClass", "Restore the default Montage", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonReset->setText(QApplication::translate("MontageDialClass", "Restore Default", Q_NULLPTR));
        buttonReset->setShortcut(QApplication::translate("MontageDialClass", "Backspace", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonClean->setToolTip(QApplication::translate("MontageDialClass", "Clear the current Montage", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonClean->setText(QApplication::translate("MontageDialClass", "Clear", Q_NULLPTR));
        buttonClean->setShortcut(QApplication::translate("MontageDialClass", "Shift+Del", Q_NULLPTR));
        buttonSortName->setText(QApplication::translate("MontageDialClass", "Sort by name", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonSave->setToolTip(QApplication::translate("MontageDialClass", "Save the Montage", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonSave->setText(QApplication::translate("MontageDialClass", "Save", Q_NULLPTR));
        buttonSave->setShortcut(QApplication::translate("MontageDialClass", "Ctrl+S", Q_NULLPTR));
        buttonLoad->setText(QApplication::translate("MontageDialClass", "Load", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonAutomaticMontage->setToolTip(QApplication::translate("MontageDialClass", "Click to reset the current montage and to build a SEEG bipolar one", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonAutomaticMontage->setText(QApplication::translate("MontageDialClass", "Make SEEG as Bipolar", Q_NULLPTR));
        buttonAVG->setText(QApplication::translate("MontageDialClass", "Make EEG AVG Ref.", Q_NULLPTR));
        buttonApply->setText(QApplication::translate("MontageDialClass", "&Apply", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("MontageDialClass", "&Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MontageDialClass: public Ui_MontageDialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMONTAGEDIAL_H
