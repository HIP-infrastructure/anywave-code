/********************************************************************************
** Form generated from reading UI file 'AwNavigationBarv2.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWNAVIGATIONBARV2_H
#define UI_AWNAVIGATIONBARV2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <widget/AwAmplitudeWidget.h>

QT_BEGIN_NAMESPACE

class Ui_AwNavigationBarClass
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonStart;
    QPushButton *buttonPrevious;
    QLabel *labelTime;
    QLabel *labelnChannels;
    QLabel *labelnSelectedChannels;
    QPushButton *buttonForward;
    QPushButton *buttonEnd;
    QComboBox *comboSecsPerCm;
    QLabel *label;
    AwAmplitudeWidget *widgetGains;
    QPushButton *buttonViewSettings;
    QPushButton *buttonMarker;
    QScrollBar *horizontalScrollBar;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *AwNavigationBarClass)
    {
        if (AwNavigationBarClass->objectName().isEmpty())
            AwNavigationBarClass->setObjectName(QStringLiteral("AwNavigationBarClass"));
        AwNavigationBarClass->resize(966, 34);
        AwNavigationBarClass->setMinimumSize(QSize(0, 0));
        AwNavigationBarClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwNavigationBarClass->setFont(font);
        gridLayout = new QGridLayout(AwNavigationBarClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonStart = new QPushButton(AwNavigationBarClass);
        buttonStart->setObjectName(QStringLiteral("buttonStart"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(32);
        sizePolicy.setVerticalStretch(32);
        sizePolicy.setHeightForWidth(buttonStart->sizePolicy().hasHeightForWidth());
        buttonStart->setSizePolicy(sizePolicy);
        buttonStart->setMinimumSize(QSize(32, 32));
        buttonStart->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/start32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonStart->setIcon(icon);
        buttonStart->setIconSize(QSize(32, 32));
        buttonStart->setFlat(true);

        horizontalLayout->addWidget(buttonStart);

        buttonPrevious = new QPushButton(AwNavigationBarClass);
        buttonPrevious->setObjectName(QStringLiteral("buttonPrevious"));
        sizePolicy.setHeightForWidth(buttonPrevious->sizePolicy().hasHeightForWidth());
        buttonPrevious->setSizePolicy(sizePolicy);
        buttonPrevious->setMinimumSize(QSize(32, 32));
        buttonPrevious->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/back32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonPrevious->setIcon(icon1);
        buttonPrevious->setIconSize(QSize(32, 32));
        buttonPrevious->setFlat(true);

        horizontalLayout->addWidget(buttonPrevious);

        labelTime = new QLabel(AwNavigationBarClass);
        labelTime->setObjectName(QStringLiteral("labelTime"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelTime->sizePolicy().hasHeightForWidth());
        labelTime->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        labelTime->setFont(font1);
        labelTime->setStyleSheet(QLatin1String("color : darkblue;\n"
"font : bold;"));
        labelTime->setScaledContents(true);

        horizontalLayout->addWidget(labelTime);

        labelnChannels = new QLabel(AwNavigationBarClass);
        labelnChannels->setObjectName(QStringLiteral("labelnChannels"));
        sizePolicy1.setHeightForWidth(labelnChannels->sizePolicy().hasHeightForWidth());
        labelnChannels->setSizePolicy(sizePolicy1);
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(11);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(50);
        labelnChannels->setFont(font2);
        labelnChannels->setStyleSheet(QStringLiteral("color : blue;"));
        labelnChannels->setScaledContents(true);

        horizontalLayout->addWidget(labelnChannels);

        labelnSelectedChannels = new QLabel(AwNavigationBarClass);
        labelnSelectedChannels->setObjectName(QStringLiteral("labelnSelectedChannels"));
        sizePolicy1.setHeightForWidth(labelnSelectedChannels->sizePolicy().hasHeightForWidth());
        labelnSelectedChannels->setSizePolicy(sizePolicy1);
        QFont font3;
        font3.setFamily(QStringLiteral("Arial"));
        font3.setPointSize(11);
        labelnSelectedChannels->setFont(font3);
        labelnSelectedChannels->setStyleSheet(QStringLiteral("color : red;"));

        horizontalLayout->addWidget(labelnSelectedChannels);

        buttonForward = new QPushButton(AwNavigationBarClass);
        buttonForward->setObjectName(QStringLiteral("buttonForward"));
        sizePolicy.setHeightForWidth(buttonForward->sizePolicy().hasHeightForWidth());
        buttonForward->setSizePolicy(sizePolicy);
        buttonForward->setMinimumSize(QSize(32, 32));
        buttonForward->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/fwd32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonForward->setIcon(icon2);
        buttonForward->setIconSize(QSize(32, 32));
        buttonForward->setFlat(true);

        horizontalLayout->addWidget(buttonForward);

        buttonEnd = new QPushButton(AwNavigationBarClass);
        buttonEnd->setObjectName(QStringLiteral("buttonEnd"));
        sizePolicy.setHeightForWidth(buttonEnd->sizePolicy().hasHeightForWidth());
        buttonEnd->setSizePolicy(sizePolicy);
        buttonEnd->setMinimumSize(QSize(32, 32));
        buttonEnd->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/end32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonEnd->setIcon(icon3);
        buttonEnd->setIconSize(QSize(32, 32));
        buttonEnd->setFlat(true);

        horizontalLayout->addWidget(buttonEnd);

        comboSecsPerCm = new QComboBox(AwNavigationBarClass);
        comboSecsPerCm->setObjectName(QStringLiteral("comboSecsPerCm"));
        comboSecsPerCm->setToolTipDuration(-1);
        comboSecsPerCm->setEditable(true);
        comboSecsPerCm->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout->addWidget(comboSecsPerCm);

        label = new QLabel(AwNavigationBarClass);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        widgetGains = new AwAmplitudeWidget(AwNavigationBarClass);
        widgetGains->setObjectName(QStringLiteral("widgetGains"));

        horizontalLayout->addWidget(widgetGains);

        buttonViewSettings = new QPushButton(AwNavigationBarClass);
        buttonViewSettings->setObjectName(QStringLiteral("buttonViewSettings"));
        sizePolicy.setHeightForWidth(buttonViewSettings->sizePolicy().hasHeightForWidth());
        buttonViewSettings->setSizePolicy(sizePolicy);
        buttonViewSettings->setMinimumSize(QSize(32, 32));
        buttonViewSettings->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/ox_eye_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonViewSettings->setIcon(icon4);
        buttonViewSettings->setIconSize(QSize(32, 32));
        buttonViewSettings->setFlat(true);

        horizontalLayout->addWidget(buttonViewSettings);

        buttonMarker = new QPushButton(AwNavigationBarClass);
        buttonMarker->setObjectName(QStringLiteral("buttonMarker"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/edit-24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonMarker->setIcon(icon5);
        buttonMarker->setIconSize(QSize(24, 24));
        buttonMarker->setFlat(true);

        horizontalLayout->addWidget(buttonMarker);

        horizontalScrollBar = new QScrollBar(AwNavigationBarClass);
        horizontalScrollBar->setObjectName(QStringLiteral("horizontalScrollBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(horizontalScrollBar->sizePolicy().hasHeightForWidth());
        horizontalScrollBar->setSizePolicy(sizePolicy2);
        horizontalScrollBar->setMaximum(999999999);
        horizontalScrollBar->setSliderPosition(0);
        horizontalScrollBar->setTracking(false);
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalScrollBar);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalLayout->setStretch(12, 1);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwNavigationBarClass);

        comboSecsPerCm->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AwNavigationBarClass);
    } // setupUi

    void retranslateUi(QWidget *AwNavigationBarClass)
    {
        AwNavigationBarClass->setWindowTitle(QApplication::translate("AwNavigationBarClass", "AwNavigationBar", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonStart->setToolTip(QApplication::translate("AwNavigationBarClass", "Beginning of file", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonStart->setText(QString());
        buttonStart->setShortcut(QApplication::translate("AwNavigationBarClass", "Shift+Left", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonPrevious->setToolTip(QApplication::translate("AwNavigationBarClass", "One page backward", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonPrevious->setText(QString());
#ifndef QT_NO_TOOLTIP
        labelTime->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        labelTime->setText(QApplication::translate("AwNavigationBarClass", "[0 - 0]", Q_NULLPTR));
        labelnChannels->setText(QString());
        labelnSelectedChannels->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonForward->setToolTip(QApplication::translate("AwNavigationBarClass", "Go one page ahead ", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonForward->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonEnd->setToolTip(QApplication::translate("AwNavigationBarClass", "End of file", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonEnd->setText(QString());
        buttonEnd->setShortcut(QApplication::translate("AwNavigationBarClass", "Shift+Right", Q_NULLPTR));
        comboSecsPerCm->clear();
        comboSecsPerCm->insertItems(0, QStringList()
         << QApplication::translate("AwNavigationBarClass", "3", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "2", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "1", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "0.5", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "0.4", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "0.3", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "0.2", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "0.1", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "0.05", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "0.02", Q_NULLPTR)
         << QApplication::translate("AwNavigationBarClass", "0.01", Q_NULLPTR)
        );
#ifndef QT_NO_TOOLTIP
        comboSecsPerCm->setToolTip(QApplication::translate("AwNavigationBarClass", "The time scale. Can be modified manually", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("AwNavigationBarClass", "s/cm", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonViewSettings->setToolTip(QApplication::translate("AwNavigationBarClass", "Click here to edit display options", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonViewSettings->setText(QString());
        buttonMarker->setText(QString());
#ifndef QT_NO_TOOLTIP
        horizontalScrollBar->setToolTip(QApplication::translate("AwNavigationBarClass", "Navigate through the file", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class AwNavigationBarClass: public Ui_AwNavigationBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWNAVIGATIONBARV2_H
