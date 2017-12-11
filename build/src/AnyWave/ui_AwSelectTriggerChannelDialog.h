/********************************************************************************
** Form generated from reading UI file 'AwSelectTriggerChannelDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWSELECTTRIGGERCHANNELDIALOG_H
#define UI_AWSELECTTRIGGERCHANNELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwSelectTriggerChannelUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboChannels;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *AwSelectTriggerChannelUi)
    {
        if (AwSelectTriggerChannelUi->objectName().isEmpty())
            AwSelectTriggerChannelUi->setObjectName(QStringLiteral("AwSelectTriggerChannelUi"));
        AwSelectTriggerChannelUi->resize(306, 92);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwSelectTriggerChannelUi->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwSelectTriggerChannelUi->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwSelectTriggerChannelUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwSelectTriggerChannelUi);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        comboChannels = new QComboBox(AwSelectTriggerChannelUi);
        comboChannels->setObjectName(QStringLiteral("comboChannels"));
        sizePolicy.setHeightForWidth(comboChannels->sizePolicy().hasHeightForWidth());
        comboChannels->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(comboChannels);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        line = new QFrame(AwSelectTriggerChannelUi);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonOk = new QPushButton(AwSelectTriggerChannelUi);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonOk->sizePolicy().hasHeightForWidth());
        buttonOk->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(buttonOk);

        buttonCancel = new QPushButton(AwSelectTriggerChannelUi);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));
        sizePolicy1.setHeightForWidth(buttonCancel->sizePolicy().hasHeightForWidth());
        buttonCancel->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(buttonCancel);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwSelectTriggerChannelUi);
        QObject::connect(buttonOk, SIGNAL(clicked()), AwSelectTriggerChannelUi, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), AwSelectTriggerChannelUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwSelectTriggerChannelUi);
    } // setupUi

    void retranslateUi(QDialog *AwSelectTriggerChannelUi)
    {
        AwSelectTriggerChannelUi->setWindowTitle(QApplication::translate("AwSelectTriggerChannelUi", "Select Trigger Channel", Q_NULLPTR));
        label->setText(QApplication::translate("AwSelectTriggerChannelUi", "Channel:", Q_NULLPTR));
        buttonOk->setText(QApplication::translate("AwSelectTriggerChannelUi", "Accept", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("AwSelectTriggerChannelUi", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwSelectTriggerChannelUi: public Ui_AwSelectTriggerChannelUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWSELECTTRIGGERCHANNELDIALOG_H
