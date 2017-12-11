/********************************************************************************
** Form generated from reading UI file 'TPUi.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TPUI_H
#define UI_TPUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TPUiClass
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupChannels;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkMask;
    QSpinBox *spinBoxValue;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkNegative;
    QSpacerItem *horizontalSpacer_2;
    QFrame *line;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TPUiClass)
    {
        if (TPUiClass->objectName().isEmpty())
            TPUiClass->setObjectName(QStringLiteral("TPUiClass"));
        TPUiClass->setWindowModality(Qt::ApplicationModal);
        TPUiClass->resize(481, 308);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        TPUiClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        TPUiClass->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(TPUiClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupChannels = new QGroupBox(TPUiClass);
        groupChannels->setObjectName(QStringLiteral("groupChannels"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupChannels->sizePolicy().hasHeightForWidth());
        groupChannels->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(groupChannels);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        checkMask = new QCheckBox(TPUiClass);
        checkMask->setObjectName(QStringLiteral("checkMask"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(checkMask->sizePolicy().hasHeightForWidth());
        checkMask->setSizePolicy(sizePolicy1);
        checkMask->setChecked(true);

        horizontalLayout->addWidget(checkMask);

        spinBoxValue = new QSpinBox(TPUiClass);
        spinBoxValue->setObjectName(QStringLiteral("spinBoxValue"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(spinBoxValue->sizePolicy().hasHeightForWidth());
        spinBoxValue->setSizePolicy(sizePolicy2);
        spinBoxValue->setMinimum(1);
        spinBoxValue->setMaximum(999999999);
        spinBoxValue->setValue(8192);

        horizontalLayout->addWidget(spinBoxValue);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        checkNegative = new QCheckBox(TPUiClass);
        checkNegative->setObjectName(QStringLiteral("checkNegative"));
        sizePolicy2.setHeightForWidth(checkNegative->sizePolicy().hasHeightForWidth());
        checkNegative->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(checkNegative);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        line = new QFrame(TPUiClass);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        buttonBox = new QDialogButtonBox(TPUiClass);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_3->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(TPUiClass);
        QObject::connect(checkMask, SIGNAL(toggled(bool)), spinBoxValue, SLOT(setEnabled(bool)));
        QObject::connect(buttonBox, SIGNAL(accepted()), TPUiClass, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TPUiClass, SLOT(reject()));

        QMetaObject::connectSlotsByName(TPUiClass);
    } // setupUi

    void retranslateUi(QDialog *TPUiClass)
    {
        TPUiClass->setWindowTitle(QApplication::translate("TPUiClass", "Detection parameters", Q_NULLPTR));
        groupChannels->setTitle(QApplication::translate("TPUiClass", "Channels", Q_NULLPTR));
        checkMask->setText(QApplication::translate("TPUiClass", "Mask value", Q_NULLPTR));
        checkNegative->setText(QApplication::translate("TPUiClass", "Search for negative values", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TPUiClass: public Ui_TPUiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TPUI_H
