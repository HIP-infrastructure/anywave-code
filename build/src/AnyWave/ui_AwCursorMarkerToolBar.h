/********************************************************************************
** Form generated from reading UI file 'AwCursorMarkerToolBar.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWCURSORMARKERTOOLBAR_H
#define UI_AWCURSORMARKERTOOLBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwCursorMarkerToolBarClass
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonInsertMarker;
    QPushButton *buttonQTS;
    QPushButton *buttonCursor;
    QPushButton *buttonShowMarkers;

    void setupUi(QWidget *AwCursorMarkerToolBarClass)
    {
        if (AwCursorMarkerToolBarClass->objectName().isEmpty())
            AwCursorMarkerToolBarClass->setObjectName(QStringLiteral("AwCursorMarkerToolBarClass"));
        AwCursorMarkerToolBarClass->resize(160, 42);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        AwCursorMarkerToolBarClass->setFont(font);
        gridLayout = new QGridLayout(AwCursorMarkerToolBarClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonInsertMarker = new QPushButton(AwCursorMarkerToolBarClass);
        buttonInsertMarker->setObjectName(QStringLiteral("buttonInsertMarker"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonInsertMarker->sizePolicy().hasHeightForWidth());
        buttonInsertMarker->setSizePolicy(sizePolicy);
        buttonInsertMarker->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/edit-24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonInsertMarker->setIcon(icon);
        buttonInsertMarker->setIconSize(QSize(32, 32));
        buttonInsertMarker->setCheckable(true);
        buttonInsertMarker->setFlat(true);

        horizontalLayout->addWidget(buttonInsertMarker);

        buttonQTS = new QPushButton(AwCursorMarkerToolBarClass);
        buttonQTS->setObjectName(QStringLiteral("buttonQTS"));
        sizePolicy.setHeightForWidth(buttonQTS->sizePolicy().hasHeightForWidth());
        buttonQTS->setSizePolicy(sizePolicy);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/QTS.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonQTS->setIcon(icon1);
        buttonQTS->setIconSize(QSize(32, 32));
        buttonQTS->setCheckable(true);
        buttonQTS->setFlat(true);

        horizontalLayout->addWidget(buttonQTS);

        buttonCursor = new QPushButton(AwCursorMarkerToolBarClass);
        buttonCursor->setObjectName(QStringLiteral("buttonCursor"));
        sizePolicy.setHeightForWidth(buttonCursor->sizePolicy().hasHeightForWidth());
        buttonCursor->setSizePolicy(sizePolicy);
        buttonCursor->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/cursor_mode.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonCursor->setIcon(icon2);
        buttonCursor->setIconSize(QSize(32, 32));
        buttonCursor->setCheckable(true);
        buttonCursor->setFlat(true);

        horizontalLayout->addWidget(buttonCursor);

        buttonShowMarkers = new QPushButton(AwCursorMarkerToolBarClass);
        buttonShowMarkers->setObjectName(QStringLiteral("buttonShowMarkers"));
        sizePolicy.setHeightForWidth(buttonShowMarkers->sizePolicy().hasHeightForWidth());
        buttonShowMarkers->setSizePolicy(sizePolicy);
        buttonShowMarkers->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/list-icon_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonShowMarkers->setIcon(icon3);
        buttonShowMarkers->setIconSize(QSize(32, 32));
        buttonShowMarkers->setCheckable(false);
        buttonShowMarkers->setFlat(true);

        horizontalLayout->addWidget(buttonShowMarkers);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(AwCursorMarkerToolBarClass);

        QMetaObject::connectSlotsByName(AwCursorMarkerToolBarClass);
    } // setupUi

    void retranslateUi(QWidget *AwCursorMarkerToolBarClass)
    {
        AwCursorMarkerToolBarClass->setWindowTitle(QApplication::translate("AwCursorMarkerToolBarClass", "AwCursorMarkerToolBar", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonInsertMarker->setToolTip(QApplication::translate("AwCursorMarkerToolBarClass", "Insert Marker(s)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonInsertMarker->setText(QString());
        buttonInsertMarker->setShortcut(QApplication::translate("AwCursorMarkerToolBarClass", "F2", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonQTS->setToolTip(QApplication::translate("AwCursorMarkerToolBarClass", "Click to activate Quick Time Selection mode", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonQTS->setText(QString());
#ifndef QT_NO_TOOLTIP
        buttonCursor->setToolTip(QApplication::translate("AwCursorMarkerToolBarClass", "Activate Cursor", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonCursor->setText(QString());
        buttonCursor->setShortcut(QApplication::translate("AwCursorMarkerToolBarClass", "F3", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        buttonShowMarkers->setToolTip(QApplication::translate("AwCursorMarkerToolBarClass", "Show markers user interface", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        buttonShowMarkers->setText(QString());
        buttonShowMarkers->setShortcut(QApplication::translate("AwCursorMarkerToolBarClass", "F2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwCursorMarkerToolBarClass: public Ui_AwCursorMarkerToolBarClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWCURSORMARKERTOOLBAR_H
