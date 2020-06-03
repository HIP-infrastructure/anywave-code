/********************************************************************************
** Form generated from reading UI file 'MFVGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MFVGUI_H
#define UI_MFVGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_zH0GUI
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QGridLayout *signalsLayout;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonAdd;
    QPushButton *buttonClear;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonApply;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *zH0GUI)
    {
        if (zH0GUI->objectName().isEmpty())
            zH0GUI->setObjectName(QLatin1String("zH0GUI"));
        zH0GUI->resize(1031, 920);
        QIcon icon;
        icon.addFile(QLatin1String(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        zH0GUI->setWindowIcon(icon);
        verticalLayout_3 = new QVBoxLayout(zH0GUI);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QLatin1String("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QLatin1String("verticalLayout_2"));
        signalsLayout = new QGridLayout();
        signalsLayout->setSpacing(6);
        signalsLayout->setObjectName(QLatin1String("signalsLayout"));

        verticalLayout_2->addLayout(signalsLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QLatin1String("horizontalLayout_2"));
        groupBox = new QGroupBox(zH0GUI);
        groupBox->setObjectName(QLatin1String("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QLatin1String("verticalLayout"));
        tableView = new QTableView(groupBox);
        tableView->setObjectName(QLatin1String("tableView"));
        tableView->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);

        verticalLayout->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QLatin1String("horizontalLayout"));
        buttonAdd = new QPushButton(groupBox);
        buttonAdd->setObjectName(QLatin1String("buttonAdd"));

        horizontalLayout->addWidget(buttonAdd);

        buttonClear = new QPushButton(groupBox);
        buttonClear->setObjectName(QLatin1String("buttonClear"));

        horizontalLayout->addWidget(buttonClear);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonApply = new QPushButton(groupBox);
        buttonApply->setObjectName(QLatin1String("buttonApply"));

        horizontalLayout->addWidget(buttonApply);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addWidget(groupBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalLayout_2->setStretch(0, 1);

        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(zH0GUI);

        QMetaObject::connectSlotsByName(zH0GUI);
    } // setupUi

    void retranslateUi(QWidget *zH0GUI)
    {
        zH0GUI->setWindowTitle(QApplication::translate("zH0GUI", "Multi Filters View", nullptr));
        groupBox->setTitle(QApplication::translate("zH0GUI", "Filtering comparison", nullptr));
#ifndef QT_NO_TOOLTIP
        tableView->setToolTip(QApplication::translate("zH0GUI", "Click Add then set the filters you want to see on signals", nullptr));
#endif // QT_NO_TOOLTIP
        buttonAdd->setText(QApplication::translate("zH0GUI", "Add", nullptr));
        buttonClear->setText(QApplication::translate("zH0GUI", "Clear All", nullptr));
#ifndef QT_NO_TOOLTIP
        buttonApply->setToolTip(QApplication::translate("zH0GUI", "Creates a copy of each signals and apply them the different filtering options", nullptr));
#endif // QT_NO_TOOLTIP
        buttonApply->setText(QApplication::translate("zH0GUI", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class zH0GUI: public Ui_zH0GUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MFVGUI_H
