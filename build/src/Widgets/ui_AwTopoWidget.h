/********************************************************************************
** Form generated from reading UI file 'AwTopoWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWTOPOWIDGET_H
#define UI_AWTOPOWIDGET_H

#include <AwTopoLabelImage.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwTopoWidgetUi
{
public:
    QVBoxLayout *verticalLayout;
    AwTopoLabelImage *labelImage;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QLabel *labelLatency;
    QLabel *labelValue;
    QLabel *labelFlags;
    QPushButton *buttonOptions;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *AwTopoWidgetUi)
    {
        if (AwTopoWidgetUi->objectName().isEmpty())
            AwTopoWidgetUi->setObjectName(QStringLiteral("AwTopoWidgetUi"));
        AwTopoWidgetUi->resize(226, 250);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AwTopoWidgetUi->sizePolicy().hasHeightForWidth());
        AwTopoWidgetUi->setSizePolicy(sizePolicy);
        AwTopoWidgetUi->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwTopoWidgetUi->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(AwTopoWidgetUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        labelImage = new AwTopoLabelImage(AwTopoWidgetUi);
        labelImage->setObjectName(QStringLiteral("labelImage"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(labelImage->sizePolicy().hasHeightForWidth());
        labelImage->setSizePolicy(sizePolicy1);
        labelImage->setScaledContents(false);
        labelImage->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelImage);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label = new QLabel(AwTopoWidgetUi);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        labelLatency = new QLabel(AwTopoWidgetUi);
        labelLatency->setObjectName(QStringLiteral("labelLatency"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labelLatency->sizePolicy().hasHeightForWidth());
        labelLatency->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(labelLatency);

        labelValue = new QLabel(AwTopoWidgetUi);
        labelValue->setObjectName(QStringLiteral("labelValue"));
        sizePolicy2.setHeightForWidth(labelValue->sizePolicy().hasHeightForWidth());
        labelValue->setSizePolicy(sizePolicy2);
        labelValue->setScaledContents(true);

        horizontalLayout->addWidget(labelValue);

        labelFlags = new QLabel(AwTopoWidgetUi);
        labelFlags->setObjectName(QStringLiteral("labelFlags"));
        labelFlags->setStyleSheet(QLatin1String("font : bold;\n"
"color : red;"));

        horizontalLayout->addWidget(labelFlags);

        buttonOptions = new QPushButton(AwTopoWidgetUi);
        buttonOptions->setObjectName(QStringLiteral("buttonOptions"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buttonOptions->sizePolicy().hasHeightForWidth());
        buttonOptions->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(buttonOptions);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(AwTopoWidgetUi);

        QMetaObject::connectSlotsByName(AwTopoWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwTopoWidgetUi)
    {
        AwTopoWidgetUi->setWindowTitle(QApplication::translate("AwTopoWidgetUi", "TopoWidget", Q_NULLPTR));
        labelImage->setText(QApplication::translate("AwTopoWidgetUi", "No Mapping Available", Q_NULLPTR));
        label->setText(QApplication::translate("AwTopoWidgetUi", "No Label", Q_NULLPTR));
        labelLatency->setText(QApplication::translate("AwTopoWidgetUi", "Latency:", Q_NULLPTR));
        labelValue->setText(QApplication::translate("AwTopoWidgetUi", "Value:", Q_NULLPTR));
        labelFlags->setText(QApplication::translate("AwTopoWidgetUi", "FLAGS", Q_NULLPTR));
        buttonOptions->setText(QApplication::translate("AwTopoWidgetUi", "Options", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwTopoWidgetUi: public Ui_AwTopoWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWTOPOWIDGET_H
