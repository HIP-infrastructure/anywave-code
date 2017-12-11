/********************************************************************************
** Form generated from reading UI file 'AwExporterSelMarkers.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWEXPORTERSELMARKERS_H
#define UI_AWEXPORTERSELMARKERS_H

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
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwExporterSelMarkersUi
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QDialog *AwExporterSelMarkersUi)
    {
        if (AwExporterSelMarkersUi->objectName().isEmpty())
            AwExporterSelMarkersUi->setObjectName(QStringLiteral("AwExporterSelMarkersUi"));
        AwExporterSelMarkersUi->resize(655, 253);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwExporterSelMarkersUi->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(AwExporterSelMarkersUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tableView = new QTableView(AwExporterSelMarkersUi);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::NoSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setCornerButtonEnabled(false);

        gridLayout->addWidget(tableView, 0, 0, 2, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_3 = new QPushButton(AwExporterSelMarkersUi);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(AwExporterSelMarkersUi);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(AwExporterSelMarkersUi);
        QObject::connect(pushButton_3, SIGNAL(clicked()), AwExporterSelMarkersUi, SLOT(accept()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), AwExporterSelMarkersUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwExporterSelMarkersUi);
    } // setupUi

    void retranslateUi(QDialog *AwExporterSelMarkersUi)
    {
        AwExporterSelMarkersUi->setWindowTitle(QApplication::translate("AwExporterSelMarkersUi", "Select Markers", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("AwExporterSelMarkersUi", "OK", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("AwExporterSelMarkersUi", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwExporterSelMarkersUi: public Ui_AwExporterSelMarkersUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWEXPORTERSELMARKERS_H
