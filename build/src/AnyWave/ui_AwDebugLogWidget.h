/********************************************************************************
** Form generated from reading UI file 'AwDebugLogWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWDEBUGLOGWIDGET_H
#define UI_AWDEBUGLOGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwDebugLogWidgetUi
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboLogType;
    QSpacerItem *horizontalSpacer;
    QTextEdit *textEditLogs;

    void setupUi(QWidget *AwDebugLogWidgetUi)
    {
        if (AwDebugLogWidgetUi->objectName().isEmpty())
            AwDebugLogWidgetUi->setObjectName(QStringLiteral("AwDebugLogWidgetUi"));
        AwDebugLogWidgetUi->resize(522, 466);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        AwDebugLogWidgetUi->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AwDebugLogWidgetUi->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwDebugLogWidgetUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(5, 5, 5, 5);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(AwDebugLogWidgetUi);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label);

        comboLogType = new QComboBox(AwDebugLogWidgetUi);
        comboLogType->setObjectName(QStringLiteral("comboLogType"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboLogType->sizePolicy().hasHeightForWidth());
        comboLogType->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(comboLogType);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        textEditLogs = new QTextEdit(AwDebugLogWidgetUi);
        textEditLogs->setObjectName(QStringLiteral("textEditLogs"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(14);
        textEditLogs->setFont(font1);
        textEditLogs->setReadOnly(true);

        verticalLayout->addWidget(textEditLogs);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(AwDebugLogWidgetUi);
        QObject::connect(comboLogType, SIGNAL(currentIndexChanged(QString)), AwDebugLogWidgetUi, SLOT(updateLogs(QString)));

        QMetaObject::connectSlotsByName(AwDebugLogWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwDebugLogWidgetUi)
    {
        AwDebugLogWidgetUi->setWindowTitle(QApplication::translate("AwDebugLogWidgetUi", "Debug Logs", Q_NULLPTR));
        label->setText(QApplication::translate("AwDebugLogWidgetUi", "Show debug logs for:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwDebugLogWidgetUi: public Ui_AwDebugLogWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWDEBUGLOGWIDGET_H
