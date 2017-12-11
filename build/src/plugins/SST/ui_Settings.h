/********************************************************************************
** Form generated from reading UI file 'Settings.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingsUi
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *SettingsUi)
    {
        if (SettingsUi->objectName().isEmpty())
            SettingsUi->setObjectName(QStringLiteral("SettingsUi"));
        SettingsUi->resize(576, 97);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        SettingsUi->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(SettingsUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(SettingsUi);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(SettingsUi);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout->addWidget(comboBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(SettingsUi);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        pushButton_2 = new QPushButton(SettingsUi);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(SettingsUi);
        QObject::connect(pushButton, SIGNAL(clicked()), SettingsUi, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), SettingsUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(SettingsUi);
    } // setupUi

    void retranslateUi(QDialog *SettingsUi)
    {
        SettingsUi->setWindowTitle(QApplication::translate("SettingsUi", "Choose Electrode", Q_NULLPTR));
        label->setText(QApplication::translate("SettingsUi", "Select eletrode:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("SettingsUi", "OK", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("SettingsUi", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SettingsUi: public Ui_SettingsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
