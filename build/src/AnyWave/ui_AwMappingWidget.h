/********************************************************************************
** Form generated from reading UI file 'AwMappingWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMAPPINGWIDGET_H
#define UI_AWMAPPINGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <widget/AwComboCMap.h>
#include <widget/AwMapWidget.h>
#include <widget/AwTopoWidget.h>

QT_BEGIN_NAMESPACE

class Ui_AwMappingWidgetUi2
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    AwMapWidget *qvtkWidget3D;
    AwTopoWidget *topoWidget2D;
    QHBoxLayout *horizontalLayout;
    QToolButton *button2D;
    QToolButton *button3D;
    QToolButton *buttonBoth;
    QCheckBox *checkBoxSensors;
    QCheckBox *checkBoxLabels;
    QCheckBox *checkBoxContours;
    QLabel *label_2;
    AwComboCMap *comboCMap;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *AwMappingWidgetUi2)
    {
        if (AwMappingWidgetUi2->objectName().isEmpty())
            AwMappingWidgetUi2->setObjectName(QStringLiteral("AwMappingWidgetUi2"));
        AwMappingWidgetUi2->resize(1024, 444);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        AwMappingWidgetUi2->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwMappingWidgetUi2->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwMappingWidgetUi2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        qvtkWidget3D = new AwMapWidget(AwMappingWidgetUi2);
        qvtkWidget3D->setObjectName(QStringLiteral("qvtkWidget3D"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(qvtkWidget3D->sizePolicy().hasHeightForWidth());
        qvtkWidget3D->setSizePolicy(sizePolicy);
        qvtkWidget3D->setMinimumSize(QSize(500, 350));

        horizontalLayout_3->addWidget(qvtkWidget3D);

        topoWidget2D = new AwTopoWidget(AwMappingWidgetUi2);
        topoWidget2D->setObjectName(QStringLiteral("topoWidget2D"));
        sizePolicy.setHeightForWidth(topoWidget2D->sizePolicy().hasHeightForWidth());
        topoWidget2D->setSizePolicy(sizePolicy);
        topoWidget2D->setMinimumSize(QSize(500, 350));

        horizontalLayout_3->addWidget(topoWidget2D);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        button2D = new QToolButton(AwMappingWidgetUi2);
        buttonGroup = new QButtonGroup(AwMappingWidgetUi2);
        buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
        buttonGroup->addButton(button2D);
        button2D->setObjectName(QStringLiteral("button2D"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(button2D->sizePolicy().hasHeightForWidth());
        button2D->setSizePolicy(sizePolicy1);
        button2D->setCheckable(true);
        button2D->setAutoExclusive(true);
        button2D->setToolButtonStyle(Qt::ToolButtonTextOnly);

        horizontalLayout->addWidget(button2D);

        button3D = new QToolButton(AwMappingWidgetUi2);
        buttonGroup->addButton(button3D);
        button3D->setObjectName(QStringLiteral("button3D"));
        sizePolicy1.setHeightForWidth(button3D->sizePolicy().hasHeightForWidth());
        button3D->setSizePolicy(sizePolicy1);
        button3D->setCheckable(true);
        button3D->setChecked(true);
        button3D->setAutoExclusive(true);
        button3D->setToolButtonStyle(Qt::ToolButtonTextOnly);

        horizontalLayout->addWidget(button3D);

        buttonBoth = new QToolButton(AwMappingWidgetUi2);
        buttonGroup->addButton(buttonBoth);
        buttonBoth->setObjectName(QStringLiteral("buttonBoth"));
        sizePolicy1.setHeightForWidth(buttonBoth->sizePolicy().hasHeightForWidth());
        buttonBoth->setSizePolicy(sizePolicy1);
        buttonBoth->setCheckable(true);
        buttonBoth->setAutoExclusive(true);
        buttonBoth->setToolButtonStyle(Qt::ToolButtonTextOnly);
        buttonBoth->setAutoRaise(false);

        horizontalLayout->addWidget(buttonBoth);

        checkBoxSensors = new QCheckBox(AwMappingWidgetUi2);
        checkBoxSensors->setObjectName(QStringLiteral("checkBoxSensors"));
        sizePolicy1.setHeightForWidth(checkBoxSensors->sizePolicy().hasHeightForWidth());
        checkBoxSensors->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(checkBoxSensors);

        checkBoxLabels = new QCheckBox(AwMappingWidgetUi2);
        checkBoxLabels->setObjectName(QStringLiteral("checkBoxLabels"));

        horizontalLayout->addWidget(checkBoxLabels);

        checkBoxContours = new QCheckBox(AwMappingWidgetUi2);
        checkBoxContours->setObjectName(QStringLiteral("checkBoxContours"));
        sizePolicy1.setHeightForWidth(checkBoxContours->sizePolicy().hasHeightForWidth());
        checkBoxContours->setSizePolicy(sizePolicy1);
        checkBoxContours->setChecked(false);

        horizontalLayout->addWidget(checkBoxContours);

        label_2 = new QLabel(AwMappingWidgetUi2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        comboCMap = new AwComboCMap(AwMappingWidgetUi2);
        comboCMap->setObjectName(QStringLiteral("comboCMap"));

        horizontalLayout->addWidget(comboCMap);

        label = new QLabel(AwMappingWidgetUi2);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwMappingWidgetUi2);
        QObject::connect(button3D, SIGNAL(clicked(bool)), AwMappingWidgetUi2, SLOT(to3D(bool)));
        QObject::connect(buttonBoth, SIGNAL(clicked(bool)), AwMappingWidgetUi2, SLOT(showBoth(bool)));
        QObject::connect(button2D, SIGNAL(clicked(bool)), AwMappingWidgetUi2, SLOT(to2D(bool)));

        QMetaObject::connectSlotsByName(AwMappingWidgetUi2);
    } // setupUi

    void retranslateUi(QWidget *AwMappingWidgetUi2)
    {
        AwMappingWidgetUi2->setWindowTitle(QApplication::translate("AwMappingWidgetUi2", "Mapping", Q_NULLPTR));
        button2D->setText(QApplication::translate("AwMappingWidgetUi2", "2D", Q_NULLPTR));
        button3D->setText(QApplication::translate("AwMappingWidgetUi2", "3D", Q_NULLPTR));
        buttonBoth->setText(QApplication::translate("AwMappingWidgetUi2", "Both", Q_NULLPTR));
        checkBoxSensors->setText(QApplication::translate("AwMappingWidgetUi2", "Show sensors", Q_NULLPTR));
        checkBoxLabels->setText(QApplication::translate("AwMappingWidgetUi2", "Show Labels", Q_NULLPTR));
        checkBoxContours->setText(QApplication::translate("AwMappingWidgetUi2", "Show contours", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwMappingWidgetUi2", "Colormap:", Q_NULLPTR));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AwMappingWidgetUi2: public Ui_AwMappingWidgetUi2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMAPPINGWIDGET_H
