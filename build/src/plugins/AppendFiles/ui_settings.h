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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_settingsUi
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidgetIN;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *buttonAddFiles;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_8;
    QComboBox *comboBoxWriters;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEditOutputFile;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonAppend;

    void setupUi(QDialog *settingsUi)
    {
        if (settingsUi->objectName().isEmpty())
            settingsUi->setObjectName(QStringLiteral("settingsUi"));
        settingsUi->setWindowModality(Qt::WindowModal);
        settingsUi->resize(1090, 702);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsUi->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(settingsUi);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        groupBox = new QGroupBox(settingsUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidgetIN = new QTableWidget(groupBox);
        if (tableWidgetIN->columnCount() < 2)
            tableWidgetIN->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetIN->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetIN->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidgetIN->setObjectName(QStringLiteral("tableWidgetIN"));
        tableWidgetIN->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidgetIN->setAlternatingRowColors(true);
        tableWidgetIN->setShowGrid(false);
        tableWidgetIN->setCornerButtonEnabled(false);
        tableWidgetIN->horizontalHeader()->setVisible(true);
        tableWidgetIN->horizontalHeader()->setMinimumSectionSize(100);
        tableWidgetIN->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableWidgetIN);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonAddFiles = new QPushButton(groupBox);
        buttonAddFiles->setObjectName(QStringLiteral("buttonAddFiles"));

        horizontalLayout->addWidget(buttonAddFiles);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(settingsUi);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_6->addWidget(label_8);

        comboBoxWriters = new QComboBox(groupBox_2);
        comboBoxWriters->setObjectName(QStringLiteral("comboBoxWriters"));

        horizontalLayout_6->addWidget(comboBoxWriters);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);


        verticalLayout_3->addLayout(horizontalLayout_6);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        lineEditOutputFile = new QLineEdit(groupBox_2);
        lineEditOutputFile->setObjectName(QStringLiteral("lineEditOutputFile"));

        horizontalLayout_2->addWidget(lineEditOutputFile);

        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout_4->addWidget(groupBox_2);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        buttonAppend = new QPushButton(settingsUi);
        buttonAppend->setObjectName(QStringLiteral("buttonAppend"));
        buttonAppend->setEnabled(false);

        horizontalLayout_3->addWidget(buttonAppend);


        verticalLayout_2->addLayout(horizontalLayout_3);


        retranslateUi(settingsUi);
        QObject::connect(buttonAddFiles, SIGNAL(clicked()), settingsUi, SLOT(addFiles()));
        QObject::connect(buttonAppend, SIGNAL(clicked()), settingsUi, SLOT(accept()));
        QObject::connect(pushButton, SIGNAL(clicked()), settingsUi, SLOT(changeOutputFile()));

        QMetaObject::connectSlotsByName(settingsUi);
    } // setupUi

    void retranslateUi(QDialog *settingsUi)
    {
        settingsUi->setWindowTitle(QApplication::translate("settingsUi", "Append Files", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("settingsUi", "IN", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetIN->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("settingsUi", "File", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetIN->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("settingsUi", "Status", Q_NULLPTR));
        buttonAddFiles->setText(QApplication::translate("settingsUi", "Add Files", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("settingsUi", "OUT", Q_NULLPTR));
        label_8->setText(QApplication::translate("settingsUi", "Writer:", Q_NULLPTR));
        label->setText(QApplication::translate("settingsUi", "Output file:", Q_NULLPTR));
        pushButton->setText(QApplication::translate("settingsUi", "Change", Q_NULLPTR));
        buttonAppend->setText(QApplication::translate("settingsUi", "Append", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class settingsUi: public Ui_settingsUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
