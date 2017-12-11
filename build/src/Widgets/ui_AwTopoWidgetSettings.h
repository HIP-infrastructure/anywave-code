/********************************************************************************
** Form generated from reading UI file 'AwTopoWidgetSettings.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWTOPOWIDGETSETTINGS_H
#define UI_AWTOPOWIDGETSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <widget/AwComboCMap.h>

QT_BEGIN_NAMESPACE

class Ui_AwTopoWidgetSettings
{
public:
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    AwComboCMap *comboColormap;
    QCheckBox *checkBoxContours;
    QLabel *label_2;
    QSpinBox *spinBoxContours;
    QCheckBox *checkBoxSensors;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBoxLabels;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonApply;
    QPushButton *pushButtonClose;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *AwTopoWidgetSettings)
    {
        if (AwTopoWidgetSettings->objectName().isEmpty())
            AwTopoWidgetSettings->setObjectName(QStringLiteral("AwTopoWidgetSettings"));
        AwTopoWidgetSettings->resize(332, 190);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwTopoWidgetSettings->setWindowIcon(icon);
        gridLayout_3 = new QGridLayout(AwTopoWidgetSettings);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(AwTopoWidgetSettings);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        comboColormap = new AwComboCMap(groupBox);
        comboColormap->setObjectName(QStringLiteral("comboColormap"));

        gridLayout->addWidget(comboColormap, 0, 1, 1, 1);

        checkBoxContours = new QCheckBox(groupBox);
        checkBoxContours->setObjectName(QStringLiteral("checkBoxContours"));

        gridLayout->addWidget(checkBoxContours, 1, 0, 1, 2);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        spinBoxContours = new QSpinBox(groupBox);
        spinBoxContours->setObjectName(QStringLiteral("spinBoxContours"));

        gridLayout->addWidget(spinBoxContours, 2, 1, 1, 1);

        checkBoxSensors = new QCheckBox(groupBox);
        checkBoxSensors->setObjectName(QStringLiteral("checkBoxSensors"));

        gridLayout->addWidget(checkBoxSensors, 3, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 2, 1, 1);

        checkBoxLabels = new QCheckBox(groupBox);
        checkBoxLabels->setObjectName(QStringLiteral("checkBoxLabels"));

        gridLayout->addWidget(checkBoxLabels, 3, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButtonApply = new QPushButton(AwTopoWidgetSettings);
        pushButtonApply->setObjectName(QStringLiteral("pushButtonApply"));

        horizontalLayout->addWidget(pushButtonApply);

        pushButtonClose = new QPushButton(AwTopoWidgetSettings);
        pushButtonClose->setObjectName(QStringLiteral("pushButtonClose"));

        horizontalLayout->addWidget(pushButtonClose);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout_3->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwTopoWidgetSettings);
        QObject::connect(pushButtonApply, SIGNAL(clicked()), AwTopoWidgetSettings, SLOT(accept()));
        QObject::connect(pushButtonClose, SIGNAL(clicked()), AwTopoWidgetSettings, SLOT(reject()));
        QObject::connect(checkBoxContours, SIGNAL(toggled(bool)), spinBoxContours, SLOT(setEnabled(bool)));
        QObject::connect(checkBoxSensors, SIGNAL(toggled(bool)), checkBoxLabels, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(AwTopoWidgetSettings);
    } // setupUi

    void retranslateUi(QDialog *AwTopoWidgetSettings)
    {
        AwTopoWidgetSettings->setWindowTitle(QApplication::translate("AwTopoWidgetSettings", "Mapping", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwTopoWidgetSettings", "Settings", Q_NULLPTR));
        label->setText(QApplication::translate("AwTopoWidgetSettings", "Colormap:", Q_NULLPTR));
        checkBoxContours->setText(QApplication::translate("AwTopoWidgetSettings", "Show Contour Lines", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwTopoWidgetSettings", "# lines:", Q_NULLPTR));
        checkBoxSensors->setText(QApplication::translate("AwTopoWidgetSettings", "Show Sensors", Q_NULLPTR));
        checkBoxLabels->setText(QApplication::translate("AwTopoWidgetSettings", "Show Labels", Q_NULLPTR));
        pushButtonApply->setText(QApplication::translate("AwTopoWidgetSettings", "Apply", Q_NULLPTR));
        pushButtonClose->setText(QApplication::translate("AwTopoWidgetSettings", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwTopoWidgetSettings: public Ui_AwTopoWidgetSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWTOPOWIDGETSETTINGS_H
