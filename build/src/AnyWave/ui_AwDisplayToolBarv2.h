/********************************************************************************
** Form generated from reading UI file 'AwDisplayToolBarv2.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWDISPLAYTOOLBARV2_H
#define UI_AWDISPLAYTOOLBARV2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwDisplayToolBarClassv2
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QPushButton *buttonAddView;
    QLabel *label;
    QComboBox *comboAlignment;
    QPushButton *buttonCapture;
    QCheckBox *checkSynchro;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QWidget *AwDisplayToolBarClassv2)
    {
        if (AwDisplayToolBarClassv2->objectName().isEmpty())
            AwDisplayToolBarClassv2->setObjectName(QStringLiteral("AwDisplayToolBarClassv2"));
        AwDisplayToolBarClassv2->resize(765, 34);
        gridLayout = new QGridLayout(AwDisplayToolBarClassv2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_3 = new QLabel(AwDisplayToolBarClassv2);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMaximumSize(QSize(16777215, 32));

        horizontalLayout->addWidget(label_3);

        buttonAddView = new QPushButton(AwDisplayToolBarClassv2);
        buttonAddView->setObjectName(QStringLiteral("buttonAddView"));
        sizePolicy.setHeightForWidth(buttonAddView->sizePolicy().hasHeightForWidth());
        buttonAddView->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(buttonAddView);

        label = new QLabel(AwDisplayToolBarClassv2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        comboAlignment = new QComboBox(AwDisplayToolBarClassv2);
        comboAlignment->setObjectName(QStringLiteral("comboAlignment"));
        comboAlignment->setMaximumSize(QSize(16777215, 24));
        comboAlignment->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout->addWidget(comboAlignment);

        buttonCapture = new QPushButton(AwDisplayToolBarClassv2);
        buttonCapture->setObjectName(QStringLiteral("buttonCapture"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonCapture->sizePolicy().hasHeightForWidth());
        buttonCapture->setSizePolicy(sizePolicy1);
        buttonCapture->setMinimumSize(QSize(32, 32));
        buttonCapture->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/Capture32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonCapture->setIcon(icon);
        buttonCapture->setIconSize(QSize(32, 32));
        buttonCapture->setFlat(true);

        horizontalLayout->addWidget(buttonCapture);

        checkSynchro = new QCheckBox(AwDisplayToolBarClassv2);
        checkSynchro->setObjectName(QStringLiteral("checkSynchro"));
        sizePolicy.setHeightForWidth(checkSynchro->sizePolicy().hasHeightForWidth());
        checkSynchro->setSizePolicy(sizePolicy);
        checkSynchro->setMaximumSize(QSize(16777215, 32));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        checkSynchro->setFont(font);

        horizontalLayout->addWidget(checkSynchro);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwDisplayToolBarClassv2);

        QMetaObject::connectSlotsByName(AwDisplayToolBarClassv2);
    } // setupUi

    void retranslateUi(QWidget *AwDisplayToolBarClassv2)
    {
        AwDisplayToolBarClassv2->setWindowTitle(QApplication::translate("AwDisplayToolBarClassv2", "Form", Q_NULLPTR));
        label_3->setText(QApplication::translate("AwDisplayToolBarClassv2", "Views:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonAddView->setToolTip(QApplication::translate("AwDisplayToolBarClassv2", "Click here to add a new signal view", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonAddView->setText(QApplication::translate("AwDisplayToolBarClassv2", "Add View", Q_NULLPTR));
        label->setText(QApplication::translate("AwDisplayToolBarClassv2", "Alignment:", Q_NULLPTR));
        comboAlignment->clear();
        comboAlignment->insertItems(0, QStringList()
         << QApplication::translate("AwDisplayToolBarClassv2", "Align Horizontaly", Q_NULLPTR)
         << QApplication::translate("AwDisplayToolBarClassv2", "Align Verticaly", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        comboAlignment->setToolTip(QApplication::translate("AwDisplayToolBarClassv2", "Align view vertically or horizontally", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        buttonCapture->setToolTip(QApplication::translate("AwDisplayToolBarClassv2", "Take a screen capture of the views and save it as as PNG file in the current data directory.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonCapture->setText(QString());
#ifndef QT_NO_TOOLTIP
        checkSynchro->setToolTip(QApplication::translate("AwDisplayToolBarClassv2", "If checked, views are synchronized.", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        checkSynchro->setText(QApplication::translate("AwDisplayToolBarClassv2", "Synchronize", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwDisplayToolBarClassv2: public Ui_AwDisplayToolBarClassv2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWDISPLAYTOOLBARV2_H
