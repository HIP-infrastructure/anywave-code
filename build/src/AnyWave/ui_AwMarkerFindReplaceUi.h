/********************************************************************************
** Form generated from reading UI file 'AwMarkerFindReplaceUi.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMARKERFINDREPLACEUI_H
#define UI_AWMARKERFINDREPLACEUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwMarkerFindReplaceUiClass
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QRadioButton *radioValue;
    QLabel *label_2;
    QSpinBox *spinBoxValue;
    QRadioButton *radioLabel;
    QLabel *label;
    QLineEdit *leLabel;
    QFrame *line;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QDialogButtonBox *buttonBox_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *AwMarkerFindReplaceUiClass)
    {
        if (AwMarkerFindReplaceUiClass->objectName().isEmpty())
            AwMarkerFindReplaceUiClass->setObjectName(QStringLiteral("AwMarkerFindReplaceUiClass"));
        AwMarkerFindReplaceUiClass->resize(433, 115);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        AwMarkerFindReplaceUiClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwMarkerFindReplaceUiClass->setWindowIcon(icon);
        AwMarkerFindReplaceUiClass->setSizeGripEnabled(true);
        AwMarkerFindReplaceUiClass->setModal(true);
        gridLayout_2 = new QGridLayout(AwMarkerFindReplaceUiClass);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        radioValue = new QRadioButton(AwMarkerFindReplaceUiClass);
        radioValue->setObjectName(QStringLiteral("radioValue"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(radioValue->sizePolicy().hasHeightForWidth());
        radioValue->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(10);
        radioValue->setFont(font1);
        radioValue->setChecked(true);

        gridLayout->addWidget(radioValue, 0, 0, 1, 1);

        label_2 = new QLabel(AwMarkerFindReplaceUiClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setFont(font1);
        label_2->setScaledContents(true);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        spinBoxValue = new QSpinBox(AwMarkerFindReplaceUiClass);
        spinBoxValue->setObjectName(QStringLiteral("spinBoxValue"));
        sizePolicy.setHeightForWidth(spinBoxValue->sizePolicy().hasHeightForWidth());
        spinBoxValue->setSizePolicy(sizePolicy);
        spinBoxValue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxValue->setMinimum(0);
        spinBoxValue->setMaximum(65001);

        gridLayout->addWidget(spinBoxValue, 0, 2, 1, 1);

        radioLabel = new QRadioButton(AwMarkerFindReplaceUiClass);
        radioLabel->setObjectName(QStringLiteral("radioLabel"));
        sizePolicy.setHeightForWidth(radioLabel->sizePolicy().hasHeightForWidth());
        radioLabel->setSizePolicy(sizePolicy);
        radioLabel->setFont(font1);

        gridLayout->addWidget(radioLabel, 1, 0, 1, 1);

        label = new QLabel(AwMarkerFindReplaceUiClass);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setFont(font1);
        label->setScaledContents(true);

        gridLayout->addWidget(label, 1, 1, 1, 1);

        leLabel = new QLineEdit(AwMarkerFindReplaceUiClass);
        leLabel->setObjectName(QStringLiteral("leLabel"));
        sizePolicy.setHeightForWidth(leLabel->sizePolicy().hasHeightForWidth());
        leLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(leLabel, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        line = new QFrame(AwMarkerFindReplaceUiClass);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        buttonBox_2 = new QDialogButtonBox(AwMarkerFindReplaceUiClass);
        buttonBox_2->setObjectName(QStringLiteral("buttonBox_2"));
        sizePolicy.setHeightForWidth(buttonBox_2->sizePolicy().hasHeightForWidth());
        buttonBox_2->setSizePolicy(sizePolicy);
        buttonBox_2->setFont(font1);
        buttonBox_2->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_3->addWidget(buttonBox_2);


        verticalLayout->addLayout(horizontalLayout_3);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);


        retranslateUi(AwMarkerFindReplaceUiClass);
        QObject::connect(buttonBox_2, SIGNAL(accepted()), AwMarkerFindReplaceUiClass, SLOT(accept()));
        QObject::connect(buttonBox_2, SIGNAL(rejected()), AwMarkerFindReplaceUiClass, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwMarkerFindReplaceUiClass);
    } // setupUi

    void retranslateUi(QDialog *AwMarkerFindReplaceUiClass)
    {
        AwMarkerFindReplaceUiClass->setWindowTitle(QApplication::translate("AwMarkerFindReplaceUiClass", "Edit Selected Markers", Q_NULLPTR));
        radioValue->setText(QApplication::translate("AwMarkerFindReplaceUiClass", "Replace all values", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwMarkerFindReplaceUiClass", "with:", Q_NULLPTR));
        radioLabel->setText(QApplication::translate("AwMarkerFindReplaceUiClass", "Replace all labels", Q_NULLPTR));
        label->setText(QApplication::translate("AwMarkerFindReplaceUiClass", "with:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwMarkerFindReplaceUiClass: public Ui_AwMarkerFindReplaceUiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMARKERFINDREPLACEUI_H
