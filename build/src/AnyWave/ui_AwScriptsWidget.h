/********************************************************************************
** Form generated from reading UI file 'AwScriptsWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWSCRIPTSWIDGET_H
#define UI_AWSCRIPTSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwScriptsWidgetUi
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonClear;

    void setupUi(QWidget *AwScriptsWidgetUi)
    {
        if (AwScriptsWidgetUi->objectName().isEmpty())
            AwScriptsWidgetUi->setObjectName(QStringLiteral("AwScriptsWidgetUi"));
        AwScriptsWidgetUi->resize(530, 190);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwScriptsWidgetUi->setWindowIcon(icon);
        horizontalLayout_2 = new QHBoxLayout(AwScriptsWidgetUi);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(AwScriptsWidgetUi);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setTabKeyNavigation(false);
        listWidget->setResizeMode(QListView::Adjust);

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonClear = new QPushButton(AwScriptsWidgetUi);
        buttonClear->setObjectName(QStringLiteral("buttonClear"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonClear->sizePolicy().hasHeightForWidth());
        buttonClear->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(buttonClear);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(AwScriptsWidgetUi);

        buttonClear->setDefault(true);


        QMetaObject::connectSlotsByName(AwScriptsWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwScriptsWidgetUi)
    {
        AwScriptsWidgetUi->setWindowTitle(QApplication::translate("AwScriptsWidgetUi", "AwScriptsWidget", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonClear->setToolTip(QApplication::translate("AwScriptsWidgetUi", "Remove Finished Processes", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonClear->setText(QApplication::translate("AwScriptsWidgetUi", "Clear List", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwScriptsWidgetUi: public Ui_AwScriptsWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWSCRIPTSWIDGET_H
