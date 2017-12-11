/********************************************************************************
** Form generated from reading UI file 'AwAddPredefinedMarker.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWADDPREDEFINEDMARKER_H
#define UI_AWADDPREDEFINEDMARKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwAddPredefinedMarkerUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEditLabel;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpinBox *spinBoxValue;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QComboBox *comboBoxColor;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *AwAddPredefinedMarkerUi)
    {
        if (AwAddPredefinedMarkerUi->objectName().isEmpty())
            AwAddPredefinedMarkerUi->setObjectName(QStringLiteral("AwAddPredefinedMarkerUi"));
        AwAddPredefinedMarkerUi->resize(321, 135);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwAddPredefinedMarkerUi->setWindowIcon(icon);
        AwAddPredefinedMarkerUi->setSizeGripEnabled(true);
        AwAddPredefinedMarkerUi->setModal(true);
        gridLayout = new QGridLayout(AwAddPredefinedMarkerUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwAddPredefinedMarkerUi);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        lineEditLabel = new QLineEdit(AwAddPredefinedMarkerUi);
        lineEditLabel->setObjectName(QStringLiteral("lineEditLabel"));
        sizePolicy.setHeightForWidth(lineEditLabel->sizePolicy().hasHeightForWidth());
        lineEditLabel->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(lineEditLabel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(AwAddPredefinedMarkerUi);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_2);

        spinBoxValue = new QSpinBox(AwAddPredefinedMarkerUi);
        spinBoxValue->setObjectName(QStringLiteral("spinBoxValue"));
        sizePolicy.setHeightForWidth(spinBoxValue->sizePolicy().hasHeightForWidth());
        spinBoxValue->setSizePolicy(sizePolicy);
        spinBoxValue->setMinimum(-32768);
        spinBoxValue->setMaximum(32768);
        spinBoxValue->setSingleStep(1);
        spinBoxValue->setValue(-1);

        horizontalLayout_2->addWidget(spinBoxValue);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(AwAddPredefinedMarkerUi);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        comboBoxColor = new QComboBox(AwAddPredefinedMarkerUi);
        comboBoxColor->setObjectName(QStringLiteral("comboBoxColor"));

        horizontalLayout_4->addWidget(comboBoxColor);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        pushButton = new QPushButton(AwAddPredefinedMarkerUi);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pushButton);

        pushButton_2 = new QPushButton(AwAddPredefinedMarkerUi);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwAddPredefinedMarkerUi);
        QObject::connect(pushButton, SIGNAL(clicked()), AwAddPredefinedMarkerUi, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), AwAddPredefinedMarkerUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwAddPredefinedMarkerUi);
    } // setupUi

    void retranslateUi(QDialog *AwAddPredefinedMarkerUi)
    {
        AwAddPredefinedMarkerUi->setWindowTitle(QApplication::translate("AwAddPredefinedMarkerUi", "Add predefined marker", Q_NULLPTR));
        label->setText(QApplication::translate("AwAddPredefinedMarkerUi", "Label:", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwAddPredefinedMarkerUi", "Value:", Q_NULLPTR));
        label_3->setText(QApplication::translate("AwAddPredefinedMarkerUi", "Color:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("AwAddPredefinedMarkerUi", "Add", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("AwAddPredefinedMarkerUi", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwAddPredefinedMarkerUi: public Ui_AwAddPredefinedMarkerUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWADDPREDEFINEDMARKER_H
