/********************************************************************************
** Form generated from reading UI file 'AwNewMarkerRuleDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWNEWMARKERRULEDIAL_H
#define UI_AWNEWMARKERRULEDIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwNewMarkerRuleDialUI
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *leName;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QScrollArea *scrollAreaRules;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *layoutRules;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *AwNewMarkerRuleDialUI)
    {
        if (AwNewMarkerRuleDialUI->objectName().isEmpty())
            AwNewMarkerRuleDialUI->setObjectName(QStringLiteral("AwNewMarkerRuleDialUI"));
        AwNewMarkerRuleDialUI->resize(567, 305);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwNewMarkerRuleDialUI->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwNewMarkerRuleDialUI->setWindowIcon(icon);
        AwNewMarkerRuleDialUI->setSizeGripEnabled(true);
        AwNewMarkerRuleDialUI->setModal(true);
        gridLayout_2 = new QGridLayout(AwNewMarkerRuleDialUI);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwNewMarkerRuleDialUI);
        label->setObjectName(QStringLiteral("label"));
        label->setScaledContents(true);

        horizontalLayout->addWidget(label);

        leName = new QLineEdit(AwNewMarkerRuleDialUI);
        leName->setObjectName(QStringLiteral("leName"));

        horizontalLayout->addWidget(leName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        groupBox = new QGroupBox(AwNewMarkerRuleDialUI);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        scrollAreaRules = new QScrollArea(groupBox);
        scrollAreaRules->setObjectName(QStringLiteral("scrollAreaRules"));
        scrollAreaRules->setFrameShape(QFrame::NoFrame);
        scrollAreaRules->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 547, 207));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        layoutRules = new QVBoxLayout();
        layoutRules->setSpacing(6);
        layoutRules->setObjectName(QStringLiteral("layoutRules"));

        gridLayout_3->addLayout(layoutRules, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 152, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 0, 1, 1);

        scrollAreaRules->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollAreaRules, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonOk = new QPushButton(AwNewMarkerRuleDialUI);
        buttonOk->setObjectName(QStringLiteral("buttonOk"));

        horizontalLayout_2->addWidget(buttonOk);

        buttonCancel = new QPushButton(AwNewMarkerRuleDialUI);
        buttonCancel->setObjectName(QStringLiteral("buttonCancel"));

        horizontalLayout_2->addWidget(buttonCancel);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 0, 1, 1);


        retranslateUi(AwNewMarkerRuleDialUI);
        QObject::connect(buttonOk, SIGNAL(clicked()), AwNewMarkerRuleDialUI, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), AwNewMarkerRuleDialUI, SLOT(reject()));

        QMetaObject::connectSlotsByName(AwNewMarkerRuleDialUI);
    } // setupUi

    void retranslateUi(QDialog *AwNewMarkerRuleDialUI)
    {
        AwNewMarkerRuleDialUI->setWindowTitle(QApplication::translate("AwNewMarkerRuleDialUI", "Modify Filter", Q_NULLPTR));
        label->setText(QApplication::translate("AwNewMarkerRuleDialUI", "Rule name:", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwNewMarkerRuleDialUI", "Constraints", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwNewMarkerRuleDialUI", "NOTE: The rules below will be used to show markers which match conditions.", Q_NULLPTR));
        buttonOk->setText(QApplication::translate("AwNewMarkerRuleDialUI", "Validate", Q_NULLPTR));
        buttonCancel->setText(QApplication::translate("AwNewMarkerRuleDialUI", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwNewMarkerRuleDialUI: public Ui_AwNewMarkerRuleDialUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWNEWMARKERRULEDIAL_H
