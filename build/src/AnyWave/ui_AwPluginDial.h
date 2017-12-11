/********************************************************************************
** Form generated from reading UI file 'AwPluginDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPLUGINDIAL_H
#define UI_AWPLUGINDIAL_H

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
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PluginDialClass
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;

    void setupUi(QDialog *PluginDialClass)
    {
        if (PluginDialClass->objectName().isEmpty())
            PluginDialClass->setObjectName(QStringLiteral("PluginDialClass"));
        PluginDialClass->resize(481, 327);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        PluginDialClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        PluginDialClass->setWindowIcon(icon);
        gridLayout = new QGridLayout(PluginDialClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        treeWidget = new QTreeWidget(PluginDialClass);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget->setColumnCount(2);

        verticalLayout->addWidget(treeWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(298, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(PluginDialClass);
        okButton->setObjectName(QStringLiteral("okButton"));

        horizontalLayout->addWidget(okButton);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(PluginDialClass);

        QMetaObject::connectSlotsByName(PluginDialClass);
    } // setupUi

    void retranslateUi(QDialog *PluginDialClass)
    {
        PluginDialClass->setWindowTitle(QApplication::translate("PluginDialClass", "PluginDial", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("PluginDialClass", "2", Q_NULLPTR));
        ___qtreewidgetitem->setText(0, QApplication::translate("PluginDialClass", "1", Q_NULLPTR));
        okButton->setText(QApplication::translate("PluginDialClass", "OK", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PluginDialClass: public Ui_PluginDialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPLUGINDIAL_H
