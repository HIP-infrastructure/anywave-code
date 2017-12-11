/********************************************************************************
** Form generated from reading UI file 'settings.ui'
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

class Ui_settingsUi
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboMatrix;
    QLabel *label_2;
    QComboBox *comboMarker;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *settingsUi)
    {
        if (settingsUi->objectName().isEmpty())
            settingsUi->setObjectName(QStringLiteral("settingsUi"));
        settingsUi->resize(552, 111);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsUi->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(settingsUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(settingsUi);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        comboMatrix = new QComboBox(settingsUi);
        comboMatrix->setObjectName(QStringLiteral("comboMatrix"));

        horizontalLayout_2->addWidget(comboMatrix);

        label_2 = new QLabel(settingsUi);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        comboMarker = new QComboBox(settingsUi);
        comboMarker->setObjectName(QStringLiteral("comboMarker"));

        horizontalLayout_2->addWidget(comboMarker);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        pushButton = new QPushButton(settingsUi);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(settingsUi);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(settingsUi);
        QObject::connect(pushButton, SIGNAL(clicked()), settingsUi, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), settingsUi, SLOT(reject()));

        QMetaObject::connectSlotsByName(settingsUi);
    } // setupUi

    void retranslateUi(QDialog *settingsUi)
    {
        settingsUi->setWindowTitle(QApplication::translate("settingsUi", "Input/Output settings", Q_NULLPTR));
        label->setText(QApplication::translate("settingsUi", "Result Matrix:", Q_NULLPTR));
        label_2->setText(QApplication::translate("settingsUi", "Data:", Q_NULLPTR));
        comboMarker->clear();
        comboMarker->insertItems(0, QStringList()
         << QApplication::translate("settingsUi", "Whole data", Q_NULLPTR)
        );
        pushButton->setText(QApplication::translate("settingsUi", "Ok", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("settingsUi", "Cancel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class settingsUi: public Ui_settingsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
