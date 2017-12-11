/********************************************************************************
** Form generated from reading UI file 'AwNewViewDial.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWNEWVIEWDIAL_H
#define UI_AWNEWVIEWDIAL_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwNewViewDialUi
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QCheckBox *checkSource;
    QCheckBox *checkICA;
    QCheckBox *checkOther;
    QCheckBox *checkTrigger;
    QCheckBox *checkECG;
    QCheckBox *checkEMG;
    QCheckBox *checkGRAD;
    QCheckBox *checkMEG;
    QCheckBox *checkSEEG;
    QCheckBox *checkEEG;
    QCheckBox *checkReference;
    QPushButton *buttonNone;
    QPushButton *buttonAll;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QDialog *AwNewViewDialUi)
    {
        if (AwNewViewDialUi->objectName().isEmpty())
            AwNewViewDialUi->setObjectName(QStringLiteral("AwNewViewDialUi"));
        AwNewViewDialUi->setWindowModality(Qt::WindowModal);
        AwNewViewDialUi->resize(510, 159);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwNewViewDialUi->setWindowIcon(icon);
        AwNewViewDialUi->setModal(true);
        verticalLayout_2 = new QVBoxLayout(AwNewViewDialUi);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(AwNewViewDialUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        checkSource = new QCheckBox(groupBox);
        checkSource->setObjectName(QStringLiteral("checkSource"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(checkSource->sizePolicy().hasHeightForWidth());
        checkSource->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkSource, 1, 0, 1, 2);

        checkICA = new QCheckBox(groupBox);
        checkICA->setObjectName(QStringLiteral("checkICA"));
        sizePolicy.setHeightForWidth(checkICA->sizePolicy().hasHeightForWidth());
        checkICA->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkICA, 1, 7, 1, 2);

        checkOther = new QCheckBox(groupBox);
        checkOther->setObjectName(QStringLiteral("checkOther"));
        sizePolicy.setHeightForWidth(checkOther->sizePolicy().hasHeightForWidth());
        checkOther->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkOther, 1, 6, 1, 1);

        checkTrigger = new QCheckBox(groupBox);
        checkTrigger->setObjectName(QStringLiteral("checkTrigger"));
        sizePolicy.setHeightForWidth(checkTrigger->sizePolicy().hasHeightForWidth());
        checkTrigger->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkTrigger, 1, 5, 1, 1);

        checkECG = new QCheckBox(groupBox);
        checkECG->setObjectName(QStringLiteral("checkECG"));
        sizePolicy.setHeightForWidth(checkECG->sizePolicy().hasHeightForWidth());
        checkECG->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkECG, 1, 4, 1, 1);

        checkEMG = new QCheckBox(groupBox);
        checkEMG->setObjectName(QStringLiteral("checkEMG"));
        sizePolicy.setHeightForWidth(checkEMG->sizePolicy().hasHeightForWidth());
        checkEMG->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkEMG, 0, 7, 1, 2);

        checkGRAD = new QCheckBox(groupBox);
        checkGRAD->setObjectName(QStringLiteral("checkGRAD"));
        sizePolicy.setHeightForWidth(checkGRAD->sizePolicy().hasHeightForWidth());
        checkGRAD->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkGRAD, 0, 6, 1, 1);

        checkMEG = new QCheckBox(groupBox);
        checkMEG->setObjectName(QStringLiteral("checkMEG"));
        sizePolicy.setHeightForWidth(checkMEG->sizePolicy().hasHeightForWidth());
        checkMEG->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkMEG, 0, 5, 1, 1);

        checkSEEG = new QCheckBox(groupBox);
        checkSEEG->setObjectName(QStringLiteral("checkSEEG"));
        sizePolicy.setHeightForWidth(checkSEEG->sizePolicy().hasHeightForWidth());
        checkSEEG->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkSEEG, 0, 4, 1, 1);

        checkEEG = new QCheckBox(groupBox);
        checkEEG->setObjectName(QStringLiteral("checkEEG"));
        sizePolicy.setHeightForWidth(checkEEG->sizePolicy().hasHeightForWidth());
        checkEEG->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkEEG, 0, 0, 1, 2);

        checkReference = new QCheckBox(groupBox);
        checkReference->setObjectName(QStringLiteral("checkReference"));
        sizePolicy.setHeightForWidth(checkReference->sizePolicy().hasHeightForWidth());
        checkReference->setSizePolicy(sizePolicy);

        gridLayout->addWidget(checkReference, 2, 0, 1, 2);

        buttonNone = new QPushButton(groupBox);
        buttonNone->setObjectName(QStringLiteral("buttonNone"));

        gridLayout->addWidget(buttonNone, 2, 7, 1, 2);

        buttonAll = new QPushButton(groupBox);
        buttonAll->setObjectName(QStringLiteral("buttonAll"));

        gridLayout->addWidget(buttonAll, 2, 5, 1, 2);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_2->addWidget(groupBox);

        verticalSpacer_2 = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        pushButton_3 = new QPushButton(AwNewViewDialUi);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_4->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(AwNewViewDialUi);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_4->addWidget(pushButton_4);


        verticalLayout_2->addLayout(horizontalLayout_4);


        retranslateUi(AwNewViewDialUi);
        QObject::connect(pushButton_3, SIGNAL(clicked()), AwNewViewDialUi, SLOT(accept()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), AwNewViewDialUi, SLOT(reject()));
        QObject::connect(buttonNone, SIGNAL(clicked()), AwNewViewDialUi, SLOT(none()));
        QObject::connect(buttonAll, SIGNAL(clicked()), AwNewViewDialUi, SLOT(all()));

        QMetaObject::connectSlotsByName(AwNewViewDialUi);
    } // setupUi

    void retranslateUi(QDialog *AwNewViewDialUi)
    {
        AwNewViewDialUi->setWindowTitle(QApplication::translate("AwNewViewDialUi", "Select what to view", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwNewViewDialUi", "Select channels", Q_NULLPTR));
        checkSource->setText(QApplication::translate("AwNewViewDialUi", "Source", Q_NULLPTR));
        checkICA->setText(QApplication::translate("AwNewViewDialUi", "ICA", Q_NULLPTR));
        checkOther->setText(QApplication::translate("AwNewViewDialUi", "Other", Q_NULLPTR));
        checkTrigger->setText(QApplication::translate("AwNewViewDialUi", "Trigger", Q_NULLPTR));
        checkECG->setText(QApplication::translate("AwNewViewDialUi", "ECG", Q_NULLPTR));
        checkEMG->setText(QApplication::translate("AwNewViewDialUi", "EMG", Q_NULLPTR));
        checkGRAD->setText(QApplication::translate("AwNewViewDialUi", "GRAD", Q_NULLPTR));
        checkMEG->setText(QApplication::translate("AwNewViewDialUi", "MEG", Q_NULLPTR));
        checkSEEG->setText(QApplication::translate("AwNewViewDialUi", "SEEG", Q_NULLPTR));
        checkEEG->setText(QApplication::translate("AwNewViewDialUi", "EEG", Q_NULLPTR));
        checkReference->setText(QApplication::translate("AwNewViewDialUi", "Reference", Q_NULLPTR));
        buttonNone->setText(QApplication::translate("AwNewViewDialUi", "None", Q_NULLPTR));
        buttonAll->setText(QApplication::translate("AwNewViewDialUi", "All", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("AwNewViewDialUi", "Add View", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("AwNewViewDialUi", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwNewViewDialUi: public Ui_AwNewViewDialUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWNEWVIEWDIAL_H
