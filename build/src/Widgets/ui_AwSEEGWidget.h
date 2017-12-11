/********************************************************************************
** Form generated from reading UI file 'AwSEEGWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWSEEGWIDGET_H
#define UI_AWSEEGWIDGET_H

#include <QVTKWidget.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwSEEGWidgetUi
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QVTKWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *sliderOpacity;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonLoadMesh;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBoxFast;
    QCheckBox *checkBoxSmooth;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonLoadElectrodes;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *checkBoxShowLabels;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *AwSEEGWidgetUi)
    {
        if (AwSEEGWidgetUi->objectName().isEmpty())
            AwSEEGWidgetUi->setObjectName(QStringLiteral("AwSEEGWidgetUi"));
        AwSEEGWidgetUi->resize(978, 516);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwSEEGWidgetUi->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwSEEGWidgetUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        widget = new QVTKWidget(AwSEEGWidgetUi);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(widget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(AwSEEGWidgetUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(label);

        sliderOpacity = new QSlider(groupBox);
        sliderOpacity->setObjectName(QStringLiteral("sliderOpacity"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(sliderOpacity->sizePolicy().hasHeightForWidth());
        sliderOpacity->setSizePolicy(sizePolicy3);
        sliderOpacity->setMaximum(100);
        sliderOpacity->setValue(40);
        sliderOpacity->setOrientation(Qt::Horizontal);
        sliderOpacity->setTickPosition(QSlider::NoTicks);

        horizontalLayout->addWidget(sliderOpacity);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        buttonLoadMesh = new QPushButton(groupBox);
        buttonLoadMesh->setObjectName(QStringLiteral("buttonLoadMesh"));

        horizontalLayout->addWidget(buttonLoadMesh);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        checkBoxFast = new QCheckBox(groupBox);
        checkBoxFast->setObjectName(QStringLiteral("checkBoxFast"));

        horizontalLayout_3->addWidget(checkBoxFast);

        checkBoxSmooth = new QCheckBox(groupBox);
        checkBoxSmooth->setObjectName(QStringLiteral("checkBoxSmooth"));
        checkBoxSmooth->setChecked(true);

        horizontalLayout_3->addWidget(checkBoxSmooth);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(AwSEEGWidgetUi);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        horizontalLayout_5 = new QHBoxLayout(groupBox_2);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        buttonLoadElectrodes = new QPushButton(groupBox_2);
        buttonLoadElectrodes->setObjectName(QStringLiteral("buttonLoadElectrodes"));
        sizePolicy3.setHeightForWidth(buttonLoadElectrodes->sizePolicy().hasHeightForWidth());
        buttonLoadElectrodes->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(buttonLoadElectrodes);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        checkBoxShowLabels = new QCheckBox(groupBox_2);
        checkBoxShowLabels->setObjectName(QStringLiteral("checkBoxShowLabels"));
        sizePolicy3.setHeightForWidth(checkBoxShowLabels->sizePolicy().hasHeightForWidth());
        checkBoxShowLabels->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(checkBoxShowLabels);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        horizontalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(groupBox_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_4->addLayout(verticalLayout_2);

        horizontalLayout_4->setStretch(0, 1);

        gridLayout->addLayout(horizontalLayout_4, 0, 0, 1, 1);


        retranslateUi(AwSEEGWidgetUi);
        QObject::connect(buttonLoadElectrodes, SIGNAL(clicked()), AwSEEGWidgetUi, SLOT(loadElectrodes()));
        QObject::connect(sliderOpacity, SIGNAL(valueChanged(int)), AwSEEGWidgetUi, SLOT(changeMeshOpacity(int)));
        QObject::connect(buttonLoadMesh, SIGNAL(clicked()), AwSEEGWidgetUi, SLOT(loadMesh()));
        QObject::connect(checkBoxFast, SIGNAL(toggled(bool)), AwSEEGWidgetUi, SLOT(changeFastRendering(bool)));
        QObject::connect(checkBoxSmooth, SIGNAL(toggled(bool)), AwSEEGWidgetUi, SLOT(changeSmoothRendering(bool)));
        QObject::connect(checkBoxShowLabels, SIGNAL(toggled(bool)), AwSEEGWidgetUi, SLOT(showElectrodesLabels(bool)));

        QMetaObject::connectSlotsByName(AwSEEGWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwSEEGWidgetUi)
    {
        AwSEEGWidgetUi->setWindowTitle(QApplication::translate("AwSEEGWidgetUi", "SEEG Viewer", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwSEEGWidgetUi", "Mesh Options", Q_NULLPTR));
        label->setText(QApplication::translate("AwSEEGWidgetUi", "Opacity:", Q_NULLPTR));
        buttonLoadMesh->setText(QApplication::translate("AwSEEGWidgetUi", "Load Mesh", Q_NULLPTR));
        checkBoxFast->setText(QApplication::translate("AwSEEGWidgetUi", "Fast Rendering", Q_NULLPTR));
        checkBoxSmooth->setText(QApplication::translate("AwSEEGWidgetUi", "Smooth Rendering", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AwSEEGWidgetUi", "Electrodes Options", Q_NULLPTR));
        buttonLoadElectrodes->setText(QApplication::translate("AwSEEGWidgetUi", "Load Elec.", Q_NULLPTR));
        checkBoxShowLabels->setText(QApplication::translate("AwSEEGWidgetUi", "Show Labels", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwSEEGWidgetUi: public Ui_AwSEEGWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWSEEGWIDGET_H
