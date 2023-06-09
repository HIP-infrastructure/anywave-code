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
#ifndef AW_DISPLAY_H
#define AW_DISPLAY_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLinearLayout>
#include <QLayout>
#include <QMainWindow>
#include <AwChannel.h>
#include <QSplitter>
#include "AwSignalView.h"
#include "AwDisplaySetup.h"

class AwMarkerManager;
class AwDisplaySetup;
class AwGainManager;
class AwFileIO;
class AwCentralWidget;
class AwGainLevels;

#define TOP_LEFT				0
#define BOTTOM_RIGHT			1
#define AW_MAX_VIEW_SCENE		2
#define AW_CALIBRATION_WIDTH	200	// 200 pixels line width reference

class AwDisplay : public QObject
{
	Q_OBJECT
public:
	enum FileType { Continous, Epoch };

	AwDisplay(QMainWindow *window);
	~AwDisplay();
	AwChannelList selectedChannels();
	inline QToolBar *toolBar() { return m_toolBar; }

	//void newFile(AwFileIO *reader);
	void newFile(); // called when a file is open
	void closeFile();
	void quit();
	void setAddMarkerDock(QDockWidget *dock);

	AwSignalView* addSignalView(AwViewSettings* settings);
	AwChannelList displayedChannels();	// Return the channels currently displayed in the views
	static AwDisplay *instance();
	static void setInstance(AwDisplay *d);
	bool containsChannels(int type);
	AwChannelList getChannels(int type);
private:
	QDockWidget *m_dockAddMarker;
	FileType fileType;
	AwGainLevels* m_gainLevels;
	QMainWindow *m_mainWindow;
	QList<AwSignalView *> m_signalViews;	// The views displayed.
	AwChannelList m_virtualChannels;		// List of virtual channels generated by processes.
	QStringList m_selectedLabels;			// labels of selected channels.
	QToolBar *m_toolBar;
	AwChannelList m_channels;
	QSplitter* m_centralWidget;
	bool m_dontSynchronize;
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	QScreen *m_screen;
#endif
	static AwDisplay *m_instance;
	AwDisplaySetup m_displaySetup;

	void loadViewSettings();
	void saveViewSettings();
	void removeView(int index);
	void updateGUI(); // after loading new display setup

signals:
	void newMarker(float pos, float duration);
	void selectedChannelsChanged(const AwChannelList& selection);
	void gainLevelsUpdated(float eeg, float seeg, float meg, float ecg, float trigger);
	void clickedAtLatency(float latency);
	void displayedChannelsChanged(const AwChannelList &channels);
	void resetMarkerMode();
	void newDisplaySetupLoaded(AwDisplaySetup*);
	void mappingTimeSelectionDone(float pos, float duration);
	void draggedCursorPositionChanged(float pos);
	void QTSModeEnded();
public slots:
	void processEvent(QSharedPointer<AwEvent>);
	void executeCommand(int command, const QVariantList& args);
	void handleCommand(const QVariantMap& map);
	void synchronizeMappingCursorPos(float position);
	void addNewSignalView();
	void removeView();
	void alignViewsVerticaly();
	void alignViewsHorizontaly();
	void setSynchronized(bool flag) { m_displaySetup.setSynchronized(flag); }
	void synchronizeViews(float position);
	void synchronizeOnCursor(float position);	// reposition the views based on the cursor position from a particular view.
	void setCursorPosition(float position);     // change the cursor position in the view. Can make the view change its position.
	void synchronizeCursorPos(float position);
	void showPositionInViews(float position);
	void highlightMarker(AwMarker *m);
	void captureViews();
	void addVideoCursor();
	void removeVideoCursor();
	void handleVideoCursor(bool flag);
	void setVideoPosition(float pos);


	/** Change selected state of one or more channels, referenced by name **/
	void changeChannelsSelectionState(const QString& name, bool selected);
	void makeChannelVisible(const QString& label);
	void setSelectedChannelsFromLabels(const QStringList& labels);
	/** Add custom channels instanciated by a process **/
	void addVirtualChannels(AwChannelList *channels);
	/** remove custom channels **/
	void removeVirtualChannels(AwChannelList *channels);
	void setChannels(const AwChannelList& montage);
	/** processHasFinished -
	recoit les messages de fin de process de tous les plugins de type Display actifs sur l'affichage courant **/ 
	void processHasFinished();
	void addMarkerModeChanged(bool on);
	void cursorModeChanged(bool on);
	void setQTSMode(bool on);
	/** Updates markers color **/
	void updateMarkersColor(const QStringList& colors);
	void updateDisplay();
	// switch mapping mode in scenes.
	void setMappingModeOn(bool on);

	// ICA
	void showICAMapOverChannel(bool flag);
	void updateICAChannelRejected(const QString& label, bool rejected);
};

#endif