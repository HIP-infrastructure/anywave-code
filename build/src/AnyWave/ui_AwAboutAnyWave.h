/********************************************************************************
** Form generated from reading UI file 'AwAboutAnyWave.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWABOUTANYWAVE_H
#define UI_AWABOUTANYWAVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutAnyWaveClass
{
public:
    QGridLayout *gridLayout_2;
    QLabel *label;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *labelQtVersion;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *labelVtkVersion;
    QLabel *label_3;
    QLabel *label_9;
    QLabel *labelArmadillo;
    QLabel *label_2;
    QLabel *labelBuildDate;
    QPushButton *buttonDetails;
    QHBoxLayout *horizontalLayout;
    QLabel *labelUpdateAvailable;
    QPushButton *buttonGetLastVersion;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *AboutAnyWaveClass)
    {
        if (AboutAnyWaveClass->objectName().isEmpty())
            AboutAnyWaveClass->setObjectName(QStringLiteral("AboutAnyWaveClass"));
        AboutAnyWaveClass->resize(808, 641);
        AboutAnyWaveClass->setMinimumSize(QSize(520, 522));
        AboutAnyWaveClass->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AboutAnyWaveClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AboutAnyWaveClass->setWindowIcon(icon);
        AboutAnyWaveClass->setStyleSheet(QStringLiteral(""));
        gridLayout_2 = new QGridLayout(AboutAnyWaveClass);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        label = new QLabel(AboutAnyWaveClass);
        label->setObjectName(QStringLiteral("label"));
        label->setOpenExternalLinks(true);
        label->setTextInteractionFlags(Qt::TextBrowserInteraction);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
        label_5 = new QLabel(AboutAnyWaveClass);
        label_5->setObjectName(QStringLiteral("label_5"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(175, 139));
        label_5->setMaximumSize(QSize(175, 139));
        label_5->setPixmap(QPixmap(QString::fromUtf8(":/images/Built_with_Qt_logo.png")));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        labelQtVersion = new QLabel(AboutAnyWaveClass);
        labelQtVersion->setObjectName(QStringLiteral("labelQtVersion"));
        labelQtVersion->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));
        labelQtVersion->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labelQtVersion, 2, 0, 1, 1);

        label_4 = new QLabel(AboutAnyWaveClass);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setPixmap(QPixmap(QString::fromUtf8(":/images/logo_ins.png")));

        gridLayout->addWidget(label_4, 4, 0, 1, 4);

        label_6 = new QLabel(AboutAnyWaveClass);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setPixmap(QPixmap(QString::fromUtf8(":/images/VTK_logo.png")));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 1, 1, 1, 1);

        labelVtkVersion = new QLabel(AboutAnyWaveClass);
        labelVtkVersion->setObjectName(QStringLiteral("labelVtkVersion"));
        labelVtkVersion->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));
        labelVtkVersion->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labelVtkVersion, 2, 1, 1, 1);

        label_3 = new QLabel(AboutAnyWaveClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(307, 106));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/images/logo_amu_rvb.png")));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 5, 2, 1, 2);

        label_9 = new QLabel(AboutAnyWaveClass);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setScaledContents(false);
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_9, 2, 2, 1, 2);

        labelArmadillo = new QLabel(AboutAnyWaveClass);
        labelArmadillo->setObjectName(QStringLiteral("labelArmadillo"));
        labelArmadillo->setMinimumSize(QSize(182, 139));
        labelArmadillo->setPixmap(QPixmap(QString::fromUtf8(":/images/armadillo.png")));
        labelArmadillo->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(labelArmadillo, 1, 2, 1, 2);

        label_2 = new QLabel(AboutAnyWaveClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(355, 89));
        label_2->setMaximumSize(QSize(16777215, 16777215));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/images/INSERM_logo.png")));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 5, 0, 1, 2);

        labelBuildDate = new QLabel(AboutAnyWaveClass);
        labelBuildDate->setObjectName(QStringLiteral("labelBuildDate"));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        labelBuildDate->setFont(font1);
        labelBuildDate->setStyleSheet(QStringLiteral("font: 12pt \"Arial\";"));

        gridLayout->addWidget(labelBuildDate, 6, 0, 1, 1);

        buttonDetails = new QPushButton(AboutAnyWaveClass);
        buttonDetails->setObjectName(QStringLiteral("buttonDetails"));

        gridLayout->addWidget(buttonDetails, 6, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        labelUpdateAvailable = new QLabel(AboutAnyWaveClass);
        labelUpdateAvailable->setObjectName(QStringLiteral("labelUpdateAvailable"));
        labelUpdateAvailable->setScaledContents(true);
        labelUpdateAvailable->setOpenExternalLinks(true);
        labelUpdateAvailable->setTextInteractionFlags(Qt::TextBrowserInteraction);

        horizontalLayout->addWidget(labelUpdateAvailable);

        buttonGetLastVersion = new QPushButton(AboutAnyWaveClass);
        buttonGetLastVersion->setObjectName(QStringLiteral("buttonGetLastVersion"));

        horizontalLayout->addWidget(buttonGetLastVersion);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 2, 0, 1, 1);


        retranslateUi(AboutAnyWaveClass);
        QObject::connect(buttonDetails, SIGNAL(clicked()), AboutAnyWaveClass, SLOT(openLicense()));

        QMetaObject::connectSlotsByName(AboutAnyWaveClass);
    } // setupUi

    void retranslateUi(QWidget *AboutAnyWaveClass)
    {
        AboutAnyWaveClass->setWindowTitle(QApplication::translate("AboutAnyWaveClass", "About", Q_NULLPTR));
        label->setText(QApplication::translate("AboutAnyWaveClass", "AnyWave is licensed for research use only. <a href=\"http://meg.univ-amu.fr/wiki/AnyWave\">Visit our Wiki to learn more.</a>", Q_NULLPTR));
        label_5->setText(QString());
        labelQtVersion->setText(QApplication::translate("AboutAnyWaveClass", "TextLabel", Q_NULLPTR));
        label_4->setText(QString());
        label_6->setText(QString());
        labelVtkVersion->setText(QApplication::translate("AboutAnyWaveClass", "5.4.2", Q_NULLPTR));
        label_3->setText(QString());
        label_9->setText(QApplication::translate("AboutAnyWaveClass", "<html><head/><body><p><span style=\" font-size:12pt;\">Conrad Sanderson and Ryan Curtin.</span></p><p><a href=\"http://arma.sourceforge.net/armadillo_joss_2016.pdf\"><span style=\" text-decoration: underline; color:#0000ff;\">Armadillo: a template based C++ library for linear algebra.</span></a></p><p>Journal of Open Source Software, Vol. 1, pp. 26, 2016. </p></body></html>", Q_NULLPTR));
        labelArmadillo->setText(QString());
        label_2->setText(QString());
        labelBuildDate->setText(QApplication::translate("AboutAnyWaveClass", "TextLabel", Q_NULLPTR));
        buttonDetails->setText(QApplication::translate("AboutAnyWaveClass", "See More Details", Q_NULLPTR));
        labelUpdateAvailable->setText(QApplication::translate("AboutAnyWaveClass", "<html><head/><body><p><span style=\" font-size:16pt;\">A new version is available</span></p></body></html>", Q_NULLPTR));
        buttonGetLastVersion->setText(QApplication::translate("AboutAnyWaveClass", "Get the latest release", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutAnyWaveClass: public Ui_AboutAnyWaveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWABOUTANYWAVE_H
