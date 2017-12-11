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
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_SettingsUi
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit1;
    QPushButton *buttonSelect1;
    QLabel *label_2;
    QPushButton *buttonSelect2;
    QLineEdit *lineEdit2;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *SettingsUi)
    {
        if (SettingsUi->objectName().isEmpty())
            SettingsUi->setObjectName(QStringLiteral("SettingsUi"));
        SettingsUi->setWindowModality(Qt::NonModal);
        SettingsUi->resize(676, 109);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        SettingsUi->setWindowIcon(icon);
        SettingsUi->setSizeGripEnabled(true);
        SettingsUi->setModal(true);
        gridLayout_2 = new QGridLayout(SettingsUi);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_3 = new QPushButton(SettingsUi);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(SettingsUi);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);


        gridLayout_2->addLayout(horizontalLayout, 2, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(SettingsUi);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 2);

        lineEdit1 = new QLineEdit(SettingsUi);
        lineEdit1->setObjectName(QStringLiteral("lineEdit1"));

        gridLayout->addWidget(lineEdit1, 0, 2, 1, 1);

        buttonSelect1 = new QPushButton(SettingsUi);
        buttonSelect1->setObjectName(QStringLiteral("buttonSelect1"));

        gridLayout->addWidget(buttonSelect1, 0, 3, 1, 1);

        label_2 = new QLabel(SettingsUi);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        buttonSelect2 = new QPushButton(SettingsUi);
        buttonSelect2->setObjectName(QStringLiteral("buttonSelect2"));

        gridLayout->addWidget(buttonSelect2, 1, 3, 1, 1);

        lineEdit2 = new QLineEdit(SettingsUi);
        lineEdit2->setObjectName(QStringLiteral("lineEdit2"));

        gridLayout->addWidget(lineEdit2, 1, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);


        retranslateUi(SettingsUi);
        QObject::connect(pushButton_3, SIGNAL(clicked()), SettingsUi, SLOT(accept()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), SettingsUi, SLOT(reject()));
        QObject::connect(buttonSelect1, SIGNAL(clicked()), SettingsUi, SLOT(selectFile1()));
        QObject::connect(buttonSelect2, SIGNAL(clicked()), SettingsUi, SLOT(selectFile2()));

        QMetaObject::connectSlotsByName(SettingsUi);
    } // setupUi

    void retranslateUi(QDialog *SettingsUi)
    {
        SettingsUi->setWindowTitle(QApplication::translate("SettingsUi", "Input Files", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("SettingsUi", "Merge", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("SettingsUi", "Cancel", Q_NULLPTR));
        label->setText(QApplication::translate("SettingsUi", "File 1:", Q_NULLPTR));
        buttonSelect1->setText(QApplication::translate("SettingsUi", "Select File", Q_NULLPTR));
        label_2->setText(QApplication::translate("SettingsUi", "File 2:", Q_NULLPTR));
        buttonSelect2->setText(QApplication::translate("SettingsUi", "Select File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SettingsUi: public Ui_SettingsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
