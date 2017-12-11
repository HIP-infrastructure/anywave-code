/********************************************************************************
** Form generated from reading UI file 'AwProcessLogWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWPROCESSLOGWINDOW_H
#define UI_AWPROCESSLOGWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwProcessLogWindowUi
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit;

    void setupUi(QWidget *AwProcessLogWindowUi)
    {
        if (AwProcessLogWindowUi->objectName().isEmpty())
            AwProcessLogWindowUi->setObjectName(QStringLiteral("AwProcessLogWindowUi"));
        AwProcessLogWindowUi->resize(297, 432);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwProcessLogWindowUi->setWindowIcon(icon);
        gridLayout = new QGridLayout(AwProcessLogWindowUi);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textEdit = new QTextEdit(AwProcessLogWindowUi);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        gridLayout->addWidget(textEdit, 0, 0, 1, 1);


        retranslateUi(AwProcessLogWindowUi);

        QMetaObject::connectSlotsByName(AwProcessLogWindowUi);
    } // setupUi

    void retranslateUi(QWidget *AwProcessLogWindowUi)
    {
        AwProcessLogWindowUi->setWindowTitle(QApplication::translate("AwProcessLogWindowUi", "Log Window", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwProcessLogWindowUi: public Ui_AwProcessLogWindowUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWPROCESSLOGWINDOW_H
