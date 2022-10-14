// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <QMainWindow>
#include <QDir>
#include "ui_AnyWave.h"
#include <QListWidget>
#include <QLabel>
#include <AwChannel.h>
#include <QTranslator>
#include <AwMarker.h>
#include <filter/AwFilterSettings.h>

#define AW_CUSTOM_MONTAGES_DIR "/AnyWave/Montages"     // sous chemin � ajouter � homeDir pour avoir le chemin complet

// Mapping
#define AW_MAX_CARTO	2

class AwFileIOPlugin;
class AwFileIO;
class AwDisplay;
class AwCursorMarkerToolBar;
class AwDataInfoReader;
class AwDebugLogWidget;
class AwMeshManager;
class AwLayoutManager;
class AwSEEGViewer;
class AwVideoPlayer;
class AwUpdateManager;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AnyWave
///
/// Classe	Application.
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AnyWave : public QMainWindow, private Ui::AnyWaveClass
{
	Q_OBJECT

public:
	AnyWave(const QVariantMap& args, QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~AnyWave();

	inline AwDisplay *displayManager() { return m_display; }
protected:
	void closeEvent(QCloseEvent *e);
	void dragMoveEvent(QDragMoveEvent *e);
	void dragLeaveEvent(QDragLeaveEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);
	void changeEvent(QEvent *e);

private:
	QMap<QString, QDockWidget *> m_dockWidgets;
	QString m_openFileName;				// Full path to current open file.
	QString m_lastDirOpen;				// Keep path to last directory used when opening a file
	QString m_saveFileName;
	QMenu *m_recentFilesMenu, *m_recentBIDSMenu;
	// widgets
	QList<QWidget *> m_toolBarWidgets;
	QList<QWidget *> m_openWidgets;	 // list tracking all widget open by the user (and that must be closed when the application ends.)
	QList<QAction *> m_actions;
	QStatusBar *m_sBar;
	AwCursorMarkerToolBar *m_cursorToolBar;
	AwDebugLogWidget *m_debugLogWidget;
	// internal objects
	AwDisplay *m_display;				// Pointer to main AwDisplay object.
	AwSEEGViewer *m_SEEGViewer;			// Pointer to SEEGViewer
	AwMeshManager *m_meshManager;
	AwLayoutManager *m_layoutManager;
	AwUpdateManager* m_updateManager;
	// VIDEO Widget
	AwVideoPlayer *m_player;
	// flags
	bool m_currentFileModified;
	// methods
	void initToolBarsAndMenu();
	void closeFile();
	bool checkForModified();
	bool checkForRunningProcesses();
	void initMatlab();
	bool searchForMatlab();
	void searchForPython();
	bool checkAndCreateFolder(const QString& root, const QString& name);
	void applyNewLanguage();
	void writeSettings();
	void readSettings();

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
	void createMatlabShellScript(const QString& path);
	void emptyMatlabShellScript();
#endif
	void checkMatlabAndMCRInit();
	void quit();

signals:
	void newFile(AwDataInfoReader *infos);
	/** Signal �mis indiquant que de nouveaux events sont charg�s. La liste des markers est pass�e. */
	/** Signal sent when data is ready to be exported **/
	void markersReadyToExport(const AwMarkerList& markers);
	/** Send a log message **/
	void log(const QString&);
//	void closingFile();
public slots:
	/** Position le flag de fichier modifie */
	void setModified(bool f = true) { m_currentFileModified = f; }
	/** Launch 3D mapping if signals are suitable **/
	void runMapping();
	/** Close mapping **/
	void stopMapping();
	void displayReaderTriggerStatus(bool ok, int number);
	void openFile(const QString& path = QString());
	void openFileFromBIDS(const QString& path);
	bool openBIDS(const QString& path);
	void updateRecentFiles(const QStringList& files);
	void updateRecentBIDS(const QStringList& files);
	void openRecentFile();
	void openRecentBIDS();
	/** Menu View->Processes **/
	void showProcessDock();
	/** Export to SVG **/
	void exportToSVG();
	void exportToPDF();
	/** Import a marker file **/
	void importMrkFile();
	void editVideoSyncSettings();

private slots:
	void initPluginsHelpMenu();
	// slot for plugin helps
	void openPluginHelpUrl();
	void on_actionCreate_batch_script_triggered();
	void on_actionSave_as_triggered();
	void on_actionMarkers_triggered();
	void on_actionQuit_triggered();
	void on_actionMontage_triggered();
	void on_actionOpen_triggered();
	void on_actionPreferences_triggered();
	void on_actionShow_Mappings_triggered();
	void on_actionHelpAnyWave_triggered();
	void on_actionAbout_AnyWave_triggered();
	void on_actionDebug_Logs_triggered();
	void on_actionLoadICA_triggered();
	void on_actionOpen_video_triggered();
	void on_actionCreate_new_MATLAB_plugin_triggered();
	void on_actionCheck_for_available_plugins_triggered();
	void on_actionView_Change_Log_triggered();
	void loadBeamformer();
	void reviewComponentsMaps();
	void showFileProperties();
	void runGARDEL();
	void openNewAnyWave();
	void doEpoch();
	void visualiseEpoch();
	void averageEpoch();
	void openBIDS();
};
