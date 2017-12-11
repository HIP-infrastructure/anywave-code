/********************************************************************************
** Form generated from reading UI file 'AnyWave.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANYWAVE_H
#define UI_ANYWAVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AnyWaveClass
{
public:
    QAction *actionOpen;
    QAction *actionPlugins;
    QAction *actionMontage;
    QAction *actionQuit;
    QAction *actionMarkers;
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionHistory;
    QAction *actionPreferences;
    QAction *actionCarto3D;
    QAction *actionAbout_AnyWave;
    QAction *actionFileProperties;
    QAction *actionMarkers_Bar;
    QAction *actionProcesses;
    QAction *actExportChannelsInfo;
    QAction *actionExport_to_SVG;
    QAction *actionImport_mrk_file;
    QAction *actionStart_Matlab_Server;
    QAction *actionDebug_Logs;
    QAction *actionShow_Mappings;
    QAction *actionImport_Layout_File;
    QAction *actionExecuteScript;
    QAction *actionLoadICA;
    QAction *actionSave_display_to_PDF;
    QAction *actionComponentsMaps;
    QAction *actionShow_map_on_signal;
    QAction *actionLoad_Mesh;
    QAction *actionLoadBeamFormer;
    QAction *actionCreateEpoch;
    QAction *actionVisualiseEpoch;
    QAction *actionAveraging;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *mainWinLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuRecent_files;
    QMenu *menuEdit;
    QMenu *menuView_;
    QMenu *menuMapping;
    QMenu *menuICA;
    QMenu *menuEpoch;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AnyWaveClass)
    {
        if (AnyWaveClass->objectName().isEmpty())
            AnyWaveClass->setObjectName(QStringLiteral("AnyWaveClass"));
        AnyWaveClass->resize(1003, 434);
        QFont font;
        font.setPointSize(10);
        AnyWaveClass->setFont(font);
        AnyWaveClass->setFocusPolicy(Qt::StrongFocus);
        AnyWaveClass->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/AnyWave_logo.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        AnyWaveClass->setWindowIcon(icon);
        actionOpen = new QAction(AnyWaveClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/fileopen-32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionOpen->setShortcutContext(Qt::ApplicationShortcut);
        actionPlugins = new QAction(AnyWaveClass);
        actionPlugins->setObjectName(QStringLiteral("actionPlugins"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/ox_plugin_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlugins->setIcon(icon2);
        actionMontage = new QAction(AnyWaveClass);
        actionMontage->setObjectName(QStringLiteral("actionMontage"));
        actionMontage->setShortcutContext(Qt::ApplicationShortcut);
        actionQuit = new QAction(AnyWaveClass);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/ox_application_exit_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuit->setIcon(icon3);
        actionMarkers = new QAction(AnyWaveClass);
        actionMarkers->setObjectName(QStringLiteral("actionMarkers"));
        actionSave = new QAction(AnyWaveClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/ox_document_save_as_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon4);
        actionSave_as = new QAction(AnyWaveClass);
        actionSave_as->setObjectName(QStringLiteral("actionSave_as"));
        actionSave_as->setIcon(icon4);
        actionHistory = new QAction(AnyWaveClass);
        actionHistory->setObjectName(QStringLiteral("actionHistory"));
        actionPreferences = new QAction(AnyWaveClass);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        actionCarto3D = new QAction(AnyWaveClass);
        actionCarto3D->setObjectName(QStringLiteral("actionCarto3D"));
        QFont font1;
        actionCarto3D->setFont(font1);
        actionAbout_AnyWave = new QAction(AnyWaveClass);
        actionAbout_AnyWave->setObjectName(QStringLiteral("actionAbout_AnyWave"));
        actionFileProperties = new QAction(AnyWaveClass);
        actionFileProperties->setObjectName(QStringLiteral("actionFileProperties"));
        actionMarkers_Bar = new QAction(AnyWaveClass);
        actionMarkers_Bar->setObjectName(QStringLiteral("actionMarkers_Bar"));
        actionMarkers_Bar->setCheckable(true);
        actionMarkers_Bar->setChecked(true);
        actionProcesses = new QAction(AnyWaveClass);
        actionProcesses->setObjectName(QStringLiteral("actionProcesses"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/system_run_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionProcesses->setIcon(icon5);
        actExportChannelsInfo = new QAction(AnyWaveClass);
        actExportChannelsInfo->setObjectName(QStringLiteral("actExportChannelsInfo"));
        actionExport_to_SVG = new QAction(AnyWaveClass);
        actionExport_to_SVG->setObjectName(QStringLiteral("actionExport_to_SVG"));
        actionImport_mrk_file = new QAction(AnyWaveClass);
        actionImport_mrk_file->setObjectName(QStringLiteral("actionImport_mrk_file"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/import_file_32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImport_mrk_file->setIcon(icon6);
        actionStart_Matlab_Server = new QAction(AnyWaveClass);
        actionStart_Matlab_Server->setObjectName(QStringLiteral("actionStart_Matlab_Server"));
        actionDebug_Logs = new QAction(AnyWaveClass);
        actionDebug_Logs->setObjectName(QStringLiteral("actionDebug_Logs"));
        actionShow_Mappings = new QAction(AnyWaveClass);
        actionShow_Mappings->setObjectName(QStringLiteral("actionShow_Mappings"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/carto_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShow_Mappings->setIcon(icon7);
        actionImport_Layout_File = new QAction(AnyWaveClass);
        actionImport_Layout_File->setObjectName(QStringLiteral("actionImport_Layout_File"));
        actionImport_Layout_File->setIcon(icon6);
        actionExecuteScript = new QAction(AnyWaveClass);
        actionExecuteScript->setObjectName(QStringLiteral("actionExecuteScript"));
        actionExecuteScript->setIcon(icon5);
        actionLoadICA = new QAction(AnyWaveClass);
        actionLoadICA->setObjectName(QStringLiteral("actionLoadICA"));
        actionLoadICA->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/importICA32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadICA->setIcon(icon8);
        actionSave_display_to_PDF = new QAction(AnyWaveClass);
        actionSave_display_to_PDF->setObjectName(QStringLiteral("actionSave_display_to_PDF"));
        actionSave_display_to_PDF->setEnabled(false);
        actionComponentsMaps = new QAction(AnyWaveClass);
        actionComponentsMaps->setObjectName(QStringLiteral("actionComponentsMaps"));
        actionComponentsMaps->setIcon(icon8);
        actionShow_map_on_signal = new QAction(AnyWaveClass);
        actionShow_map_on_signal->setObjectName(QStringLiteral("actionShow_map_on_signal"));
        actionShow_map_on_signal->setCheckable(true);
        actionShow_map_on_signal->setChecked(false);
        actionLoad_Mesh = new QAction(AnyWaveClass);
        actionLoad_Mesh->setObjectName(QStringLiteral("actionLoad_Mesh"));
        actionLoadBeamFormer = new QAction(AnyWaveClass);
        actionLoadBeamFormer->setObjectName(QStringLiteral("actionLoadBeamFormer"));
        actionLoadBeamFormer->setEnabled(false);
        actionCreateEpoch = new QAction(AnyWaveClass);
        actionCreateEpoch->setObjectName(QStringLiteral("actionCreateEpoch"));
        actionVisualiseEpoch = new QAction(AnyWaveClass);
        actionVisualiseEpoch->setObjectName(QStringLiteral("actionVisualiseEpoch"));
        actionAveraging = new QAction(AnyWaveClass);
        actionAveraging->setObjectName(QStringLiteral("actionAveraging"));
        centralWidget = new QWidget(AnyWaveClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        mainWinLayout = new QHBoxLayout();
        mainWinLayout->setSpacing(6);
        mainWinLayout->setObjectName(QStringLiteral("mainWinLayout"));
        mainWinLayout->setSizeConstraint(QLayout::SetNoConstraint);

        horizontalLayout_2->addLayout(mainWinLayout);

        AnyWaveClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AnyWaveClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1003, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRecent_files = new QMenu(menuFile);
        menuRecent_files->setObjectName(QStringLiteral("menuRecent_files"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuView_ = new QMenu(menuBar);
        menuView_->setObjectName(QStringLiteral("menuView_"));
        menuMapping = new QMenu(menuBar);
        menuMapping->setObjectName(QStringLiteral("menuMapping"));
        menuICA = new QMenu(menuBar);
        menuICA->setObjectName(QStringLiteral("menuICA"));
        menuEpoch = new QMenu(menuBar);
        menuEpoch->setObjectName(QStringLiteral("menuEpoch"));
        AnyWaveClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(AnyWaveClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        AnyWaveClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView_->menuAction());
        menuBar->addAction(menuICA->menuAction());
        menuBar->addAction(menuMapping->menuAction());
        menuBar->addAction(menuEpoch->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_as);
        menuFile->addSeparator();
        menuFile->addAction(actionLoad_Mesh);
        menuFile->addAction(actionImport_mrk_file);
        menuFile->addAction(actionLoadBeamFormer);
        menuFile->addAction(actionLoadICA);
        menuFile->addSeparator();
        menuFile->addAction(actionExecuteScript);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_display_to_PDF);
        menuFile->addAction(actionExport_to_SVG);
        menuFile->addSeparator();
        menuFile->addAction(actionFileProperties);
        menuFile->addSeparator();
        menuFile->addAction(menuRecent_files->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuEdit->addAction(actionMontage);
        menuEdit->addAction(actionMarkers);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPreferences);
        menuEdit->addSeparator();
        menuView_->addAction(actionProcesses);
        menuView_->addAction(actionPlugins);
        menuView_->addSeparator();
        menuView_->addAction(actionDebug_Logs);
        menuView_->addSeparator();
        menuView_->addAction(actionAbout_AnyWave);
        menuMapping->addAction(actionShow_Mappings);
        menuICA->addAction(actionComponentsMaps);
        menuICA->addAction(actionShow_map_on_signal);
        menuEpoch->addAction(actionCreateEpoch);
        menuEpoch->addAction(actionVisualiseEpoch);
        menuEpoch->addAction(actionAveraging);

        retranslateUi(AnyWaveClass);

        QMetaObject::connectSlotsByName(AnyWaveClass);
    } // setupUi

    void retranslateUi(QMainWindow *AnyWaveClass)
    {
        AnyWaveClass->setWindowTitle(QApplication::translate("AnyWaveClass", "AnyWave", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("AnyWaveClass", "&Open", Q_NULLPTR));
        actionOpen->setShortcut(QApplication::translate("AnyWaveClass", "Ctrl+O", Q_NULLPTR));
        actionPlugins->setText(QApplication::translate("AnyWaveClass", "Plugins", Q_NULLPTR));
        actionMontage->setText(QApplication::translate("AnyWaveClass", "&Montage", Q_NULLPTR));
        actionMontage->setShortcut(QApplication::translate("AnyWaveClass", "Ctrl+M", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("AnyWaveClass", "Quit", Q_NULLPTR));
        actionQuit->setShortcut(QApplication::translate("AnyWaveClass", "Ctrl+Q", Q_NULLPTR));
        actionMarkers->setText(QApplication::translate("AnyWaveClass", "Markers", Q_NULLPTR));
        actionMarkers->setShortcut(QApplication::translate("AnyWaveClass", "Ctrl+E", Q_NULLPTR));
        actionSave->setText(QApplication::translate("AnyWaveClass", "Save", Q_NULLPTR));
        actionSave_as->setText(QApplication::translate("AnyWaveClass", "Save as...", Q_NULLPTR));
        actionHistory->setText(QApplication::translate("AnyWaveClass", "History", Q_NULLPTR));
        actionPreferences->setText(QApplication::translate("AnyWaveClass", "Preferences", Q_NULLPTR));
        actionCarto3D->setText(QApplication::translate("AnyWaveClass", "Mapping", Q_NULLPTR));
        actionAbout_AnyWave->setText(QApplication::translate("AnyWaveClass", "About AnyWave", Q_NULLPTR));
        actionFileProperties->setText(QApplication::translate("AnyWaveClass", "Properties", Q_NULLPTR));
        actionFileProperties->setShortcut(QApplication::translate("AnyWaveClass", "Ctrl+I", Q_NULLPTR));
        actionMarkers_Bar->setText(QApplication::translate("AnyWaveClass", "Markers Bar", Q_NULLPTR));
        actionProcesses->setText(QApplication::translate("AnyWaveClass", "Processes", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        actionProcesses->setToolTip(QApplication::translate("AnyWaveClass", "Show Processes History", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        actionProcesses->setShortcut(QApplication::translate("AnyWaveClass", "Ctrl+P", Q_NULLPTR));
        actExportChannelsInfo->setText(QApplication::translate("AnyWaveClass", "Export Channels Info.", Q_NULLPTR));
        actionExport_to_SVG->setText(QApplication::translate("AnyWaveClass", "Save display to SVG", Q_NULLPTR));
        actionImport_mrk_file->setText(QApplication::translate("AnyWaveClass", "Import marker file", Q_NULLPTR));
        actionStart_Matlab_Server->setText(QApplication::translate("AnyWaveClass", "Start Matlab Server", Q_NULLPTR));
        actionDebug_Logs->setText(QApplication::translate("AnyWaveClass", "Debug logs", Q_NULLPTR));
        actionShow_Mappings->setText(QApplication::translate("AnyWaveClass", "Show mappings", Q_NULLPTR));
        actionImport_Layout_File->setText(QApplication::translate("AnyWaveClass", "Import layout file", Q_NULLPTR));
        actionExecuteScript->setText(QApplication::translate("AnyWaveClass", "Execute script", Q_NULLPTR));
        actionLoadICA->setText(QApplication::translate("AnyWaveClass", "Load ICA components", Q_NULLPTR));
        actionSave_display_to_PDF->setText(QApplication::translate("AnyWaveClass", "Save display to PDF", Q_NULLPTR));
        actionComponentsMaps->setText(QApplication::translate("AnyWaveClass", "Review components maps", Q_NULLPTR));
        actionShow_map_on_signal->setText(QApplication::translate("AnyWaveClass", "Show map on signal", Q_NULLPTR));
        actionLoad_Mesh->setText(QApplication::translate("AnyWaveClass", "Load Mesh", Q_NULLPTR));
        actionLoadBeamFormer->setText(QApplication::translate("AnyWaveClass", "Load Beamformer matrix", Q_NULLPTR));
        actionCreateEpoch->setText(QApplication::translate("AnyWaveClass", "Edit conditions", Q_NULLPTR));
        actionVisualiseEpoch->setText(QApplication::translate("AnyWaveClass", "Review conditions", Q_NULLPTR));
        actionAveraging->setText(QApplication::translate("AnyWaveClass", "Averaging", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("AnyWaveClass", "File", Q_NULLPTR));
        menuRecent_files->setTitle(QApplication::translate("AnyWaveClass", "Recent files...", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("AnyWaveClass", "Edit", Q_NULLPTR));
        menuView_->setTitle(QApplication::translate("AnyWaveClass", "View", Q_NULLPTR));
        menuMapping->setTitle(QApplication::translate("AnyWaveClass", "Mapping", Q_NULLPTR));
        menuICA->setTitle(QApplication::translate("AnyWaveClass", "ICA", Q_NULLPTR));
        menuEpoch->setTitle(QApplication::translate("AnyWaveClass", "Epoch", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AnyWaveClass: public Ui_AnyWaveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANYWAVE_H
