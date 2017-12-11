/********************************************************************************
** Form generated from reading UI file 'AwFilePropertiesDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AWFILEPROPERTIESDIALOG_H
#define UI_AWFILEPROPERTIESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AwFilePropertiesDialogUi
{
public:
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *gridLayout_2;
    QLabel *labelnChannels;
    QLineEdit *lineEditnChannels;
    QLabel *labelSamplingRate;
    QLineEdit *lineEditSamplingRate;
    QLabel *labelRef;
    QLineEdit *lineEditnRefChannels;
    QLabel *labelMEG;
    QLineEdit *lineEditnMEGChannels;
    QLabel *labelEEG;
    QLineEdit *lineEditnEEGChannels;
    QLabel *labelSEEG;
    QLineEdit *lineEditnSEEGChannels;
    QLabel *labelEMG;
    QLineEdit *lineEditnEMGChannels;
    QLabel *labelECG;
    QLineEdit *lineEditnECGChannels;
    QLabel *labelTrigger;
    QLineEdit *lineEditnTriggerChannels;
    QLabel *labelOther;
    QLineEdit *lineEditnOtherChannels;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditPatient;
    QLabel *label_2;
    QLineEdit *lineEditDate;
    QLabel *label_4;
    QLineEdit *lineEditTime;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTableView *tvChannelsAsRecorded;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonExport;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *AwFilePropertiesDialogUi)
    {
        if (AwFilePropertiesDialogUi->objectName().isEmpty())
            AwFilePropertiesDialogUi->setObjectName(QStringLiteral("AwFilePropertiesDialogUi"));
        AwFilePropertiesDialogUi->resize(730, 739);
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(10);
        AwFilePropertiesDialogUi->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        AwFilePropertiesDialogUi->setWindowIcon(icon);
        verticalLayout_6 = new QVBoxLayout(AwFilePropertiesDialogUi);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox_2 = new QGroupBox(AwFilePropertiesDialogUi);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        labelnChannels = new QLabel(groupBox_2);
        labelnChannels->setObjectName(QStringLiteral("labelnChannels"));
        sizePolicy.setHeightForWidth(labelnChannels->sizePolicy().hasHeightForWidth());
        labelnChannels->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelnChannels, 0, 0, 1, 1);

        lineEditnChannels = new QLineEdit(groupBox_2);
        lineEditnChannels->setObjectName(QStringLiteral("lineEditnChannels"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEditnChannels->sizePolicy().hasHeightForWidth());
        lineEditnChannels->setSizePolicy(sizePolicy1);
        lineEditnChannels->setAlignment(Qt::AlignCenter);
        lineEditnChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnChannels, 0, 1, 1, 1);

        labelSamplingRate = new QLabel(groupBox_2);
        labelSamplingRate->setObjectName(QStringLiteral("labelSamplingRate"));
        sizePolicy.setHeightForWidth(labelSamplingRate->sizePolicy().hasHeightForWidth());
        labelSamplingRate->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelSamplingRate, 1, 0, 1, 1);

        lineEditSamplingRate = new QLineEdit(groupBox_2);
        lineEditSamplingRate->setObjectName(QStringLiteral("lineEditSamplingRate"));
        sizePolicy1.setHeightForWidth(lineEditSamplingRate->sizePolicy().hasHeightForWidth());
        lineEditSamplingRate->setSizePolicy(sizePolicy1);
        lineEditSamplingRate->setAlignment(Qt::AlignCenter);
        lineEditSamplingRate->setReadOnly(true);

        gridLayout_2->addWidget(lineEditSamplingRate, 1, 1, 1, 1);

        labelRef = new QLabel(groupBox_2);
        labelRef->setObjectName(QStringLiteral("labelRef"));
        sizePolicy.setHeightForWidth(labelRef->sizePolicy().hasHeightForWidth());
        labelRef->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelRef, 2, 0, 1, 1);

        lineEditnRefChannels = new QLineEdit(groupBox_2);
        lineEditnRefChannels->setObjectName(QStringLiteral("lineEditnRefChannels"));
        sizePolicy1.setHeightForWidth(lineEditnRefChannels->sizePolicy().hasHeightForWidth());
        lineEditnRefChannels->setSizePolicy(sizePolicy1);
        lineEditnRefChannels->setAlignment(Qt::AlignCenter);
        lineEditnRefChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnRefChannels, 2, 1, 1, 1);

        labelMEG = new QLabel(groupBox_2);
        labelMEG->setObjectName(QStringLiteral("labelMEG"));
        sizePolicy.setHeightForWidth(labelMEG->sizePolicy().hasHeightForWidth());
        labelMEG->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelMEG, 3, 0, 1, 1);

        lineEditnMEGChannels = new QLineEdit(groupBox_2);
        lineEditnMEGChannels->setObjectName(QStringLiteral("lineEditnMEGChannels"));
        sizePolicy1.setHeightForWidth(lineEditnMEGChannels->sizePolicy().hasHeightForWidth());
        lineEditnMEGChannels->setSizePolicy(sizePolicy1);
        lineEditnMEGChannels->setAlignment(Qt::AlignCenter);
        lineEditnMEGChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnMEGChannels, 3, 1, 1, 1);

        labelEEG = new QLabel(groupBox_2);
        labelEEG->setObjectName(QStringLiteral("labelEEG"));
        sizePolicy.setHeightForWidth(labelEEG->sizePolicy().hasHeightForWidth());
        labelEEG->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelEEG, 4, 0, 1, 1);

        lineEditnEEGChannels = new QLineEdit(groupBox_2);
        lineEditnEEGChannels->setObjectName(QStringLiteral("lineEditnEEGChannels"));
        sizePolicy1.setHeightForWidth(lineEditnEEGChannels->sizePolicy().hasHeightForWidth());
        lineEditnEEGChannels->setSizePolicy(sizePolicy1);
        lineEditnEEGChannels->setAlignment(Qt::AlignCenter);
        lineEditnEEGChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnEEGChannels, 4, 1, 1, 1);

        labelSEEG = new QLabel(groupBox_2);
        labelSEEG->setObjectName(QStringLiteral("labelSEEG"));
        sizePolicy.setHeightForWidth(labelSEEG->sizePolicy().hasHeightForWidth());
        labelSEEG->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelSEEG, 5, 0, 1, 1);

        lineEditnSEEGChannels = new QLineEdit(groupBox_2);
        lineEditnSEEGChannels->setObjectName(QStringLiteral("lineEditnSEEGChannels"));
        sizePolicy1.setHeightForWidth(lineEditnSEEGChannels->sizePolicy().hasHeightForWidth());
        lineEditnSEEGChannels->setSizePolicy(sizePolicy1);
        lineEditnSEEGChannels->setAlignment(Qt::AlignCenter);
        lineEditnSEEGChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnSEEGChannels, 5, 1, 1, 1);

        labelEMG = new QLabel(groupBox_2);
        labelEMG->setObjectName(QStringLiteral("labelEMG"));
        sizePolicy.setHeightForWidth(labelEMG->sizePolicy().hasHeightForWidth());
        labelEMG->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelEMG, 6, 0, 1, 1);

        lineEditnEMGChannels = new QLineEdit(groupBox_2);
        lineEditnEMGChannels->setObjectName(QStringLiteral("lineEditnEMGChannels"));
        sizePolicy1.setHeightForWidth(lineEditnEMGChannels->sizePolicy().hasHeightForWidth());
        lineEditnEMGChannels->setSizePolicy(sizePolicy1);
        lineEditnEMGChannels->setAlignment(Qt::AlignCenter);
        lineEditnEMGChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnEMGChannels, 6, 1, 1, 1);

        labelECG = new QLabel(groupBox_2);
        labelECG->setObjectName(QStringLiteral("labelECG"));
        sizePolicy.setHeightForWidth(labelECG->sizePolicy().hasHeightForWidth());
        labelECG->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelECG, 7, 0, 1, 1);

        lineEditnECGChannels = new QLineEdit(groupBox_2);
        lineEditnECGChannels->setObjectName(QStringLiteral("lineEditnECGChannels"));
        sizePolicy1.setHeightForWidth(lineEditnECGChannels->sizePolicy().hasHeightForWidth());
        lineEditnECGChannels->setSizePolicy(sizePolicy1);
        lineEditnECGChannels->setAlignment(Qt::AlignCenter);
        lineEditnECGChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnECGChannels, 7, 1, 1, 1);

        labelTrigger = new QLabel(groupBox_2);
        labelTrigger->setObjectName(QStringLiteral("labelTrigger"));
        sizePolicy.setHeightForWidth(labelTrigger->sizePolicy().hasHeightForWidth());
        labelTrigger->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelTrigger, 8, 0, 1, 1);

        lineEditnTriggerChannels = new QLineEdit(groupBox_2);
        lineEditnTriggerChannels->setObjectName(QStringLiteral("lineEditnTriggerChannels"));
        sizePolicy1.setHeightForWidth(lineEditnTriggerChannels->sizePolicy().hasHeightForWidth());
        lineEditnTriggerChannels->setSizePolicy(sizePolicy1);
        lineEditnTriggerChannels->setAlignment(Qt::AlignCenter);
        lineEditnTriggerChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnTriggerChannels, 8, 1, 1, 1);

        labelOther = new QLabel(groupBox_2);
        labelOther->setObjectName(QStringLiteral("labelOther"));
        sizePolicy.setHeightForWidth(labelOther->sizePolicy().hasHeightForWidth());
        labelOther->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(labelOther, 9, 0, 1, 1);

        lineEditnOtherChannels = new QLineEdit(groupBox_2);
        lineEditnOtherChannels->setObjectName(QStringLiteral("lineEditnOtherChannels"));
        sizePolicy1.setHeightForWidth(lineEditnOtherChannels->sizePolicy().hasHeightForWidth());
        lineEditnOtherChannels->setSizePolicy(sizePolicy1);
        lineEditnOtherChannels->setAlignment(Qt::AlignCenter);
        lineEditnOtherChannels->setReadOnly(true);

        gridLayout_2->addWidget(lineEditnOtherChannels, 9, 1, 1, 1);


        verticalLayout_4->addLayout(gridLayout_2);


        verticalLayout_5->addWidget(groupBox_2);

        groupBox = new QGroupBox(AwFilePropertiesDialogUi);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEditPatient = new QLineEdit(groupBox);
        lineEditPatient->setObjectName(QStringLiteral("lineEditPatient"));
        lineEditPatient->setAlignment(Qt::AlignCenter);
        lineEditPatient->setReadOnly(true);

        gridLayout->addWidget(lineEditPatient, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEditDate = new QLineEdit(groupBox);
        lineEditDate->setObjectName(QStringLiteral("lineEditDate"));
        lineEditDate->setAlignment(Qt::AlignCenter);
        lineEditDate->setReadOnly(true);

        gridLayout->addWidget(lineEditDate, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        lineEditTime = new QLineEdit(groupBox);
        lineEditTime->setObjectName(QStringLiteral("lineEditTime"));

        gridLayout->addWidget(lineEditTime, 2, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout);


        verticalLayout_5->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_5);

        groupBox_3 = new QGroupBox(AwFilePropertiesDialogUi);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tvChannelsAsRecorded = new QTableView(groupBox_3);
        tvChannelsAsRecorded->setObjectName(QStringLiteral("tvChannelsAsRecorded"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tvChannelsAsRecorded->sizePolicy().hasHeightForWidth());
        tvChannelsAsRecorded->setSizePolicy(sizePolicy2);
        tvChannelsAsRecorded->setMinimumSize(QSize(400, 500));
        tvChannelsAsRecorded->setMaximumSize(QSize(16777215, 16777215));
        tvChannelsAsRecorded->setEditTriggers(QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        tvChannelsAsRecorded->setDragEnabled(true);
        tvChannelsAsRecorded->setDragDropOverwriteMode(true);
        tvChannelsAsRecorded->setDragDropMode(QAbstractItemView::DragOnly);
        tvChannelsAsRecorded->setDefaultDropAction(Qt::CopyAction);
        tvChannelsAsRecorded->setAlternatingRowColors(true);
        tvChannelsAsRecorded->setSortingEnabled(true);
        tvChannelsAsRecorded->horizontalHeader()->setMinimumSectionSize(50);
        tvChannelsAsRecorded->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(tvChannelsAsRecorded);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonExport = new QPushButton(groupBox_3);
        buttonExport->setObjectName(QStringLiteral("buttonExport"));

        horizontalLayout->addWidget(buttonExport);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        horizontalLayout_2->addWidget(groupBox_3);


        verticalLayout_6->addLayout(horizontalLayout_2);

#ifndef QT_NO_SHORTCUT
        labelnChannels->setBuddy(lineEditnChannels);
        labelSamplingRate->setBuddy(lineEditSamplingRate);
        labelRef->setBuddy(lineEditnRefChannels);
        labelMEG->setBuddy(lineEditnMEGChannels);
        labelEEG->setBuddy(lineEditnEEGChannels);
        labelSEEG->setBuddy(lineEditnSEEGChannels);
        labelEMG->setBuddy(lineEditnEMGChannels);
        labelECG->setBuddy(lineEditnECGChannels);
        labelTrigger->setBuddy(lineEditnTriggerChannels);
        labelOther->setBuddy(lineEditnOtherChannels);
#endif // QT_NO_SHORTCUT

        retranslateUi(AwFilePropertiesDialogUi);
        QObject::connect(buttonExport, SIGNAL(clicked()), AwFilePropertiesDialogUi, SLOT(exportLabels()));

        QMetaObject::connectSlotsByName(AwFilePropertiesDialogUi);
    } // setupUi

    void retranslateUi(QDialog *AwFilePropertiesDialogUi)
    {
        AwFilePropertiesDialogUi->setWindowTitle(QApplication::translate("AwFilePropertiesDialogUi", "File Properties", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AwFilePropertiesDialogUi", "Technical properties", Q_NULLPTR));
        labelnChannels->setText(QApplication::translate("AwFilePropertiesDialogUi", "Number of channels :", Q_NULLPTR));
        labelSamplingRate->setText(QApplication::translate("AwFilePropertiesDialogUi", "Max. sampling rate:", Q_NULLPTR));
        labelRef->setText(QApplication::translate("AwFilePropertiesDialogUi", "Reference channels:", Q_NULLPTR));
        labelMEG->setText(QApplication::translate("AwFilePropertiesDialogUi", "MEG channels:", Q_NULLPTR));
        labelEEG->setText(QApplication::translate("AwFilePropertiesDialogUi", "EEG channels:", Q_NULLPTR));
        labelSEEG->setText(QApplication::translate("AwFilePropertiesDialogUi", "SEEG channels:", Q_NULLPTR));
        labelEMG->setText(QApplication::translate("AwFilePropertiesDialogUi", "EMG channels:", Q_NULLPTR));
        labelECG->setText(QApplication::translate("AwFilePropertiesDialogUi", "ECG channels:", Q_NULLPTR));
        labelTrigger->setText(QApplication::translate("AwFilePropertiesDialogUi", "Trigger channels:", Q_NULLPTR));
        labelOther->setText(QApplication::translate("AwFilePropertiesDialogUi", "Other channels:", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AwFilePropertiesDialogUi", "Recording properties", Q_NULLPTR));
        label->setText(QApplication::translate("AwFilePropertiesDialogUi", "Patient:", Q_NULLPTR));
        label_2->setText(QApplication::translate("AwFilePropertiesDialogUi", "Date:", Q_NULLPTR));
        label_4->setText(QApplication::translate("AwFilePropertiesDialogUi", "Time:", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("AwFilePropertiesDialogUi", "Channels", Q_NULLPTR));
        buttonExport->setText(QApplication::translate("AwFilePropertiesDialogUi", "Export Labels", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AwFilePropertiesDialogUi: public Ui_AwFilePropertiesDialogUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AWFILEPROPERTIESDIALOG_H
