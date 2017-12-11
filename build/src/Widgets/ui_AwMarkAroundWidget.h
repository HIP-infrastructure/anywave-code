/********************************************************************************
** Form generated from reading UI file 'AwMarkAroundWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWMARKAROUNDWIDGET_H
#define UI_AWMARKAROUNDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AwMarkAroundWidgetUi
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_9;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_3;
    QLabel *label_4;
    QDoubleSpinBox *sbmax1;
    QDoubleSpinBox *sboffset1;
    QDoubleSpinBox *sboffset2;
    QDoubleSpinBox *sbmax2;
    QComboBox *comboMarker;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QLineEdit *lineEditPreMarker;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_7;
    QComboBox *comboColorPre;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_8;
    QSpinBox *spinBoxValuePre;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_7;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_9;
    QLineEdit *lineEditPostMarker;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_10;
    QComboBox *comboColorPost;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_11;
    QSpinBox *spinBoxValuePost;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_10;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *buttonCreatePrePost;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_10;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer_11;
    QGridLayout *gridLayout_2;
    QLabel *label_15;
    QLabel *label_14;
    QLabel *label_16;
    QDoubleSpinBox *sbBefore;
    QComboBox *comboMarker2;
    QDoubleSpinBox *sbAfter;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_17;
    QLineEdit *lineEditCreateMarker;
    QSpacerItem *horizontalSpacer_13;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_18;
    QComboBox *comboColorCreateMarker;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_19;
    QSpinBox *spinBoxValueCreate;
    QSpacerItem *horizontalSpacer_15;
    QHBoxLayout *horizontalLayout_14;
    QSpacerItem *horizontalSpacer_16;
    QPushButton *buttonCreateMarker;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *AwMarkAroundWidgetUi)
    {
        if (AwMarkAroundWidgetUi->objectName().isEmpty())
            AwMarkAroundWidgetUi->setObjectName(QStringLiteral("AwMarkAroundWidgetUi"));
        AwMarkAroundWidgetUi->resize(588, 613);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwMarkAroundWidgetUi->setWindowIcon(icon);
        verticalLayout_3 = new QVBoxLayout(AwMarkAroundWidgetUi);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);

        label_12 = new QLabel(AwMarkAroundWidgetUi);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/images/marker_pre_post.png")));

        horizontalLayout_8->addWidget(label_12);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_9);


        verticalLayout_3->addLayout(horizontalLayout_8);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(AwMarkAroundWidgetUi);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(AwMarkAroundWidgetUi);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_5 = new QLabel(AwMarkAroundWidgetUi);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        label_3 = new QLabel(AwMarkAroundWidgetUi);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_3, 0, 3, 1, 1);

        label_4 = new QLabel(AwMarkAroundWidgetUi);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 0, 4, 1, 1);

        sbmax1 = new QDoubleSpinBox(AwMarkAroundWidgetUi);
        sbmax1->setObjectName(QStringLiteral("sbmax1"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sbmax1->sizePolicy().hasHeightForWidth());
        sbmax1->setSizePolicy(sizePolicy);
        sbmax1->setMaximumSize(QSize(100, 16777215));
        sbmax1->setAlignment(Qt::AlignCenter);
        sbmax1->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        sbmax1->setDecimals(3);
        sbmax1->setMaximum(1e+59);
        sbmax1->setSingleStep(0.1);

        gridLayout->addWidget(sbmax1, 1, 0, 1, 1);

        sboffset1 = new QDoubleSpinBox(AwMarkAroundWidgetUi);
        sboffset1->setObjectName(QStringLiteral("sboffset1"));
        sizePolicy.setHeightForWidth(sboffset1->sizePolicy().hasHeightForWidth());
        sboffset1->setSizePolicy(sizePolicy);
        sboffset1->setMaximumSize(QSize(100, 16777215));
        sboffset1->setAlignment(Qt::AlignCenter);
        sboffset1->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        sboffset1->setDecimals(3);
        sboffset1->setMaximum(1e+59);
        sboffset1->setSingleStep(0.1);

        gridLayout->addWidget(sboffset1, 1, 1, 1, 1);

        sboffset2 = new QDoubleSpinBox(AwMarkAroundWidgetUi);
        sboffset2->setObjectName(QStringLiteral("sboffset2"));
        sizePolicy.setHeightForWidth(sboffset2->sizePolicy().hasHeightForWidth());
        sboffset2->setSizePolicy(sizePolicy);
        sboffset2->setMaximumSize(QSize(100, 16777215));
        sboffset2->setAlignment(Qt::AlignCenter);
        sboffset2->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        sboffset2->setDecimals(3);
        sboffset2->setMaximum(1e+59);
        sboffset2->setSingleStep(0.1);

        gridLayout->addWidget(sboffset2, 1, 3, 1, 1);

        sbmax2 = new QDoubleSpinBox(AwMarkAroundWidgetUi);
        sbmax2->setObjectName(QStringLiteral("sbmax2"));
        sizePolicy.setHeightForWidth(sbmax2->sizePolicy().hasHeightForWidth());
        sbmax2->setSizePolicy(sizePolicy);
        sbmax2->setMaximumSize(QSize(100, 16777215));
        sbmax2->setAlignment(Qt::AlignCenter);
        sbmax2->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        sbmax2->setDecimals(3);
        sbmax2->setMaximum(1e+59);
        sbmax2->setSingleStep(0.1);

        gridLayout->addWidget(sbmax2, 1, 4, 1, 1);

        comboMarker = new QComboBox(AwMarkAroundWidgetUi);
        comboMarker->setObjectName(QStringLiteral("comboMarker"));

        gridLayout->addWidget(comboMarker, 1, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        groupBox = new QGroupBox(AwMarkAroundWidgetUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout->addWidget(label_6);

        lineEditPreMarker = new QLineEdit(groupBox);
        lineEditPreMarker->setObjectName(QStringLiteral("lineEditPreMarker"));

        horizontalLayout->addWidget(lineEditPreMarker);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_2->addWidget(label_7);

        comboColorPre = new QComboBox(groupBox);
        comboColorPre->setObjectName(QStringLiteral("comboColorPre"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboColorPre->sizePolicy().hasHeightForWidth());
        comboColorPre->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(comboColorPre);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_3->addWidget(label_8);

        spinBoxValuePre = new QSpinBox(groupBox);
        spinBoxValuePre->setObjectName(QStringLiteral("spinBoxValuePre"));
        sizePolicy.setHeightForWidth(spinBoxValuePre->sizePolicy().hasHeightForWidth());
        spinBoxValuePre->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(spinBoxValuePre);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_3);


        horizontalLayout_7->addWidget(groupBox);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);

        groupBox_3 = new QGroupBox(AwMarkAroundWidgetUi);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_4->addWidget(label_9);

        lineEditPostMarker = new QLineEdit(groupBox_3);
        lineEditPostMarker->setObjectName(QStringLiteral("lineEditPostMarker"));

        horizontalLayout_4->addWidget(lineEditPostMarker);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_5->addWidget(label_10);

        comboColorPost = new QComboBox(groupBox_3);
        comboColorPost->setObjectName(QStringLiteral("comboColorPost"));
        sizePolicy1.setHeightForWidth(comboColorPost->sizePolicy().hasHeightForWidth());
        comboColorPost->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(comboColorPost);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_6->addWidget(label_11);

        spinBoxValuePost = new QSpinBox(groupBox_3);
        spinBoxValuePost->setObjectName(QStringLiteral("spinBoxValuePost"));
        sizePolicy.setHeightForWidth(spinBoxValuePost->sizePolicy().hasHeightForWidth());
        spinBoxValuePost->setSizePolicy(sizePolicy);

        horizontalLayout_6->addWidget(spinBoxValuePost);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(horizontalLayout_6);


        horizontalLayout_7->addWidget(groupBox_3);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_12);

        buttonCreatePrePost = new QPushButton(AwMarkAroundWidgetUi);
        buttonCreatePrePost->setObjectName(QStringLiteral("buttonCreatePrePost"));

        horizontalLayout_10->addWidget(buttonCreatePrePost);


        verticalLayout_3->addLayout(horizontalLayout_10);

        verticalSpacer = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        line = new QFrame(AwMarkAroundWidgetUi);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_10);

        label_13 = new QLabel(AwMarkAroundWidgetUi);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/images/marker_around.png")));

        horizontalLayout_9->addWidget(label_13);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_11);


        verticalLayout_3->addLayout(horizontalLayout_9);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_15 = new QLabel(AwMarkAroundWidgetUi);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_15, 0, 0, 1, 1);

        label_14 = new QLabel(AwMarkAroundWidgetUi);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_14, 0, 1, 1, 1);

        label_16 = new QLabel(AwMarkAroundWidgetUi);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_16, 0, 2, 1, 1);

        sbBefore = new QDoubleSpinBox(AwMarkAroundWidgetUi);
        sbBefore->setObjectName(QStringLiteral("sbBefore"));
        sizePolicy.setHeightForWidth(sbBefore->sizePolicy().hasHeightForWidth());
        sbBefore->setSizePolicy(sizePolicy);
        sbBefore->setMaximumSize(QSize(100, 16777215));
        sbBefore->setAlignment(Qt::AlignCenter);
        sbBefore->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        sbBefore->setDecimals(3);
        sbBefore->setMaximum(1e+59);
        sbBefore->setSingleStep(0.1);

        gridLayout_2->addWidget(sbBefore, 1, 0, 1, 1);

        comboMarker2 = new QComboBox(AwMarkAroundWidgetUi);
        comboMarker2->setObjectName(QStringLiteral("comboMarker2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(comboMarker2->sizePolicy().hasHeightForWidth());
        comboMarker2->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(comboMarker2, 1, 1, 1, 1);

        sbAfter = new QDoubleSpinBox(AwMarkAroundWidgetUi);
        sbAfter->setObjectName(QStringLiteral("sbAfter"));
        sizePolicy.setHeightForWidth(sbAfter->sizePolicy().hasHeightForWidth());
        sbAfter->setSizePolicy(sizePolicy);
        sbAfter->setMaximumSize(QSize(100, 16777215));
        sbAfter->setAlignment(Qt::AlignCenter);
        sbAfter->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        sbAfter->setDecimals(3);
        sbAfter->setMaximum(1e+59);
        sbAfter->setSingleStep(0.1);

        gridLayout_2->addWidget(sbAfter, 1, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);

        groupBox_2 = new QGroupBox(AwMarkAroundWidgetUi);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_11->addWidget(label_17);

        lineEditCreateMarker = new QLineEdit(groupBox_2);
        lineEditCreateMarker->setObjectName(QStringLiteral("lineEditCreateMarker"));

        horizontalLayout_11->addWidget(lineEditCreateMarker);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_13);


        verticalLayout_4->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_18 = new QLabel(groupBox_2);
        label_18->setObjectName(QStringLiteral("label_18"));

        horizontalLayout_12->addWidget(label_18);

        comboColorCreateMarker = new QComboBox(groupBox_2);
        comboColorCreateMarker->setObjectName(QStringLiteral("comboColorCreateMarker"));
        sizePolicy1.setHeightForWidth(comboColorCreateMarker->sizePolicy().hasHeightForWidth());
        comboColorCreateMarker->setSizePolicy(sizePolicy1);

        horizontalLayout_12->addWidget(comboColorCreateMarker);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_14);


        verticalLayout_4->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QStringLiteral("label_19"));

        horizontalLayout_13->addWidget(label_19);

        spinBoxValueCreate = new QSpinBox(groupBox_2);
        spinBoxValueCreate->setObjectName(QStringLiteral("spinBoxValueCreate"));
        sizePolicy.setHeightForWidth(spinBoxValueCreate->sizePolicy().hasHeightForWidth());
        spinBoxValueCreate->setSizePolicy(sizePolicy);

        horizontalLayout_13->addWidget(spinBoxValueCreate);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_15);


        verticalLayout_4->addLayout(horizontalLayout_13);


        verticalLayout_3->addWidget(groupBox_2);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_16);

        buttonCreateMarker = new QPushButton(AwMarkAroundWidgetUi);
        buttonCreateMarker->setObjectName(QStringLiteral("buttonCreateMarker"));

        horizontalLayout_14->addWidget(buttonCreateMarker);


        verticalLayout_3->addLayout(horizontalLayout_14);

        verticalSpacer_2 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        sbmax1->raise();
        sboffset1->raise();
        sboffset2->raise();
        sbmax2->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        groupBox->raise();
        groupBox_3->raise();
        label_12->raise();
        label_13->raise();
        comboMarker2->raise();
        label_14->raise();
        label_15->raise();
        sbBefore->raise();
        sbAfter->raise();
        label_16->raise();
        buttonCreatePrePost->raise();
        groupBox_2->raise();
        label_17->raise();
        lineEditCreateMarker->raise();
        label_18->raise();
        comboColorCreateMarker->raise();
        label_19->raise();
        spinBoxValueCreate->raise();
        buttonCreateMarker->raise();
        line->raise();

        retranslateUi(AwMarkAroundWidgetUi);
        QObject::connect(buttonCreatePrePost, SIGNAL(clicked()), AwMarkAroundWidgetUi, SLOT(generatePrePost()));
        QObject::connect(buttonCreateMarker, SIGNAL(clicked()), AwMarkAroundWidgetUi, SLOT(generateAround()));

        QMetaObject::connectSlotsByName(AwMarkAroundWidgetUi);
    } // setupUi

    void retranslateUi(QWidget *AwMarkAroundWidgetUi)
    {
        AwMarkAroundWidgetUi->setWindowTitle(QApplication::translate("AwMarkAroundWidgetUi", "Add Markers", Q_NULLPTR));
        label_12->setText(QString());
        label->setText(QApplication::translate("AwMarkAroundWidgetUi", "- max", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwMarkAroundWidgetUi", "- offset", Q_NULLPTR));
        label_5->setText(QApplication::translate("AwMarkAroundWidgetUi", "marker", Q_NULLPTR));
        label_3->setText(QApplication::translate("AwMarkAroundWidgetUi", "+ offset", Q_NULLPTR));
        label_4->setText(QApplication::translate("AwMarkAroundWidgetUi", "+ max", Q_NULLPTR));
        sbmax1->setPrefix(QApplication::translate("AwMarkAroundWidgetUi", "-", Q_NULLPTR));
        sbmax1->setSuffix(QApplication::translate("AwMarkAroundWidgetUi", "s", Q_NULLPTR));
        sboffset1->setPrefix(QApplication::translate("AwMarkAroundWidgetUi", "-", Q_NULLPTR));
        sboffset1->setSuffix(QApplication::translate("AwMarkAroundWidgetUi", "s", Q_NULLPTR));
        sboffset2->setPrefix(QApplication::translate("AwMarkAroundWidgetUi", "+", Q_NULLPTR));
        sboffset2->setSuffix(QApplication::translate("AwMarkAroundWidgetUi", "s", Q_NULLPTR));
        sbmax2->setPrefix(QApplication::translate("AwMarkAroundWidgetUi", "+", Q_NULLPTR));
        sbmax2->setSuffix(QApplication::translate("AwMarkAroundWidgetUi", "s", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwMarkAroundWidgetUi", "Pre Marker", Q_NULLPTR));
        label_6->setText(QApplication::translate("AwMarkAroundWidgetUi", "Label:", Q_NULLPTR));
        lineEditPreMarker->setText(QApplication::translate("AwMarkAroundWidgetUi", "Pre Marker", Q_NULLPTR));
        label_7->setText(QApplication::translate("AwMarkAroundWidgetUi", "Color:", Q_NULLPTR));
        label_8->setText(QApplication::translate("AwMarkAroundWidgetUi", "Value:", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("AwMarkAroundWidgetUi", "Post Marker", Q_NULLPTR));
        label_9->setText(QApplication::translate("AwMarkAroundWidgetUi", "Label:", Q_NULLPTR));
        lineEditPostMarker->setText(QApplication::translate("AwMarkAroundWidgetUi", "Post Marker", Q_NULLPTR));
        label_10->setText(QApplication::translate("AwMarkAroundWidgetUi", "Color:", Q_NULLPTR));
        label_11->setText(QApplication::translate("AwMarkAroundWidgetUi", "Value:", Q_NULLPTR));
        buttonCreatePrePost->setText(QApplication::translate("AwMarkAroundWidgetUi", "Create Markers", Q_NULLPTR));
        label_13->setText(QString());
        label_15->setText(QApplication::translate("AwMarkAroundWidgetUi", "before", Q_NULLPTR));
        label_14->setText(QApplication::translate("AwMarkAroundWidgetUi", "marker", Q_NULLPTR));
        label_16->setText(QApplication::translate("AwMarkAroundWidgetUi", "after", Q_NULLPTR));
        sbBefore->setPrefix(QApplication::translate("AwMarkAroundWidgetUi", "-", Q_NULLPTR));
        sbBefore->setSuffix(QApplication::translate("AwMarkAroundWidgetUi", "s", Q_NULLPTR));
        sbAfter->setPrefix(QApplication::translate("AwMarkAroundWidgetUi", "+", Q_NULLPTR));
        sbAfter->setSuffix(QApplication::translate("AwMarkAroundWidgetUi", "s", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AwMarkAroundWidgetUi", "Create Marker", Q_NULLPTR));
        label_17->setText(QApplication::translate("AwMarkAroundWidgetUi", "Label:", Q_NULLPTR));
        lineEditCreateMarker->setText(QApplication::translate("AwMarkAroundWidgetUi", "New Marker", Q_NULLPTR));
        label_18->setText(QApplication::translate("AwMarkAroundWidgetUi", "Color:", Q_NULLPTR));
        label_19->setText(QApplication::translate("AwMarkAroundWidgetUi", "Value:", Q_NULLPTR));
        buttonCreateMarker->setText(QApplication::translate("AwMarkAroundWidgetUi", "Create Markers", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwMarkAroundWidgetUi: public Ui_AwMarkAroundWidgetUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWMARKAROUNDWIDGET_H
