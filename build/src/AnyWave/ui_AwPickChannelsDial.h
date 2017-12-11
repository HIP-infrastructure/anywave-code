/********************************************************************************
** Form generated from reading UI file 'AwPickChannelsDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPICKCHANNELSDIAL_H
#define UI_AWPICKCHANNELSDIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwPickChannelsDialUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *listWidgetTargets;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonClear;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QPushButton *buttonAdd;
    QPushButton *buttonRemove;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QListWidget *listWidgetAvailable;
    QFrame *line;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_4;

    void setupUi(QDialog *AwPickChannelsDialUi)
    {
        if (AwPickChannelsDialUi->objectName().isEmpty())
            AwPickChannelsDialUi->setObjectName(QStringLiteral("AwPickChannelsDialUi"));
        AwPickChannelsDialUi->resize(620, 366);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwPickChannelsDialUi->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwPickChannelsDialUi->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwPickChannelsDialUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(AwPickChannelsDialUi);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        listWidgetTargets = new QListWidget(AwPickChannelsDialUi);
        listWidgetTargets->setObjectName(QStringLiteral("listWidgetTargets"));
        listWidgetTargets->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidgetTargets->setSelectionMode(QAbstractItemView::ExtendedSelection);

        verticalLayout->addWidget(listWidgetTargets);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonClear = new QPushButton(AwPickChannelsDialUi);
        buttonClear->setObjectName(QStringLiteral("buttonClear"));

        horizontalLayout_2->addWidget(buttonClear);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        buttonAdd = new QPushButton(AwPickChannelsDialUi);
        buttonAdd->setObjectName(QStringLiteral("buttonAdd"));

        verticalLayout_3->addWidget(buttonAdd);

        buttonRemove = new QPushButton(AwPickChannelsDialUi);
        buttonRemove->setObjectName(QStringLiteral("buttonRemove"));

        verticalLayout_3->addWidget(buttonRemove);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(AwPickChannelsDialUi);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        listWidgetAvailable = new QListWidget(AwPickChannelsDialUi);
        listWidgetAvailable->setObjectName(QStringLiteral("listWidgetAvailable"));
        listWidgetAvailable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidgetAvailable->setSelectionMode(QAbstractItemView::ExtendedSelection);

        verticalLayout_2->addWidget(listWidgetAvailable);


        horizontalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_4->addLayout(horizontalLayout_3);

        line = new QFrame(AwPickChannelsDialUi);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_4->addWidget(line);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(AwPickChannelsDialUi);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_4 = new QPushButton(AwPickChannelsDialUi);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);


        verticalLayout_4->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout_4, 0, 0, 1, 1);


        retranslateUi(AwPickChannelsDialUi);
        QObject::connect(pushButton_4, SIGNAL(clicked()), AwPickChannelsDialUi, SLOT(accept()));
        QObject::connect(buttonAdd, SIGNAL(clicked()), AwPickChannelsDialUi, SLOT(addChannels()));
        QObject::connect(buttonRemove, SIGNAL(clicked()), AwPickChannelsDialUi, SLOT(removeChannels()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), AwPickChannelsDialUi, SLOT(reject()));
        QObject::connect(pushButton, SIGNAL(clicked()), AwPickChannelsDialUi, SLOT(accept()));
        QObject::connect(buttonClear, SIGNAL(clicked()), AwPickChannelsDialUi, SLOT(clearTargets()));

        QMetaObject::connectSlotsByName(AwPickChannelsDialUi);
    } // setupUi

    void retranslateUi(QDialog *AwPickChannelsDialUi)
    {
        AwPickChannelsDialUi->setWindowTitle(QApplication::translate("AwPickChannelsDialUi", "Pick channels", Q_NULLPTR));
        label->setText(QApplication::translate("AwPickChannelsDialUi", "Targeted channel(s):", Q_NULLPTR));
        buttonClear->setText(QApplication::translate("AwPickChannelsDialUi", "Clear", Q_NULLPTR));
        buttonAdd->setText(QApplication::translate("AwPickChannelsDialUi", "<<", Q_NULLPTR));
        buttonRemove->setText(QApplication::translate("AwPickChannelsDialUi", ">>", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwPickChannelsDialUi", "Available channels:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AwPickChannelsDialUi", "Apply", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("AwPickChannelsDialUi", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwPickChannelsDialUi: public Ui_AwPickChannelsDialUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPICKCHANNELSDIAL_H
