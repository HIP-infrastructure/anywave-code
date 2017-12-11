/********************************************************************************
** Form generated from reading UI file 'maingui.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINGUI_H
#define UI_MAINGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainguiUi
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonSave;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *mainguiUi)
    {
        if (mainguiUi->objectName().isEmpty())
            mainguiUi->setObjectName(QStringLiteral("mainguiUi"));
        mainguiUi->resize(342, 80);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        mainguiUi->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(mainguiUi);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(mainguiUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonSave = new QPushButton(groupBox);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonSave->sizePolicy().hasHeightForWidth());
        buttonSave->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(buttonSave);

        horizontalSpacer = new QSpacerItem(208, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(mainguiUi);
        QObject::connect(buttonSave, SIGNAL(clicked()), mainguiUi, SLOT(saveToMatlab()));

        QMetaObject::connectSlotsByName(mainguiUi);
    } // setupUi

    void retranslateUi(QWidget *mainguiUi)
    {
        mainguiUi->setWindowTitle(QApplication::translate("mainguiUi", "Save Results", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("mainguiUi", "File Output", Q_NULLPTR));
        buttonSave->setText(QApplication::translate("mainguiUi", "Average to .mat", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainguiUi: public Ui_mainguiUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINGUI_H
