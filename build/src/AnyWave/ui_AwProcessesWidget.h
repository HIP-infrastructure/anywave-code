/********************************************************************************
** Form generated from reading UI file 'AwProcessesWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPROCESSESWIDGET_H
#define UI_AWPROCESSESWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwProcessesWidgetUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonClear;

    void setupUi(QWidget *AwProcessesWidgetUi)
    {
        if (AwProcessesWidgetUi->objectName().isEmpty())
            AwProcessesWidgetUi->setObjectName(QStringLiteral("AwProcessesWidgetUi"));
        AwProcessesWidgetUi->resize(504, 171);
        AwProcessesWidgetUi->setMinimumSize(QSize(504, 0));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwProcessesWidgetUi->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwProcessesWidgetUi->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwProcessesWidgetUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(AwProcessesWidgetUi);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setTabKeyNavigation(false);
        listWidget->setResizeMode(QListView::Adjust);

        verticalLayout->addWidget(listWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonClear = new QPushButton(AwProcessesWidgetUi);
        buttonClear->setObjectName(QStringLiteral("buttonClear"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonClear->sizePolicy().hasHeightForWidth());
        buttonClear->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(buttonClear);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwProcessesWidgetUi);

        buttonClear->setDefault(true);


        QMetaObject::connectSlotsByName(AwProcessesWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwProcessesWidgetUi)
    {
        AwProcessesWidgetUi->setWindowTitle(QApplication::translate("AwProcessesWidgetUi", "Processes", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonClear->setToolTip(QApplication::translate("AwProcessesWidgetUi", "Remove Finished Processes", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonClear->setText(QApplication::translate("AwProcessesWidgetUi", "Clear List", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwProcessesWidgetUi: public Ui_AwProcessesWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPROCESSESWIDGET_H
