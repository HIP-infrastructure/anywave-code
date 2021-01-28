/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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

class AwMarkerManager;
class AwDisplaySetup;
class AwGainManager;
class AwFileIO;
class AwCentralWidget;

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

	void newFile(AwFileIO *reader);
	void closeFile();
	void quit();
	void setAddMarkerDock(QDockWidget *dock);

	AwSignalView *addSignalView(AwViewSetup *setup);	// Add a new SignalView 
	AwChannelList displayedChannels();	// Return the channels currently displayed in the views
	static AwDisplay *instance();
	static void setInstance(AwDisplay *d);
private:
	QDockWidget *m_dockAddMarker;
	FileType fileType;
	AwDisplaySetup *m_setup;
	QMainWindow *m_mainWindow;
	QList<AwSignalView *> m_signalViews;	// The views displayed.
	AwChannelList m_allSelectedChannels;	// channels selected in the different views	
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

	void saveChannelSelections();
	void loadChannelSelections();
signals:
	void newMarker(float pos, float duration);
	void selectedChannelsChanged(const AwChannelList& selection);
	void gainLevelsUpdated(float eeg, float seeg, float meg, float ecg, float trigger);
	void clickedAtLatency(float latency);
	void displayedChannelsChanged(const AwChannelList &channels);
	void resetMarkerMode();
	void setupChanged(AwDisplaySetup *setup, int flags);
	void mappingTimeSelectionDone(float pos, float duration);
	void draggedCursorPositionChanged(float pos);
	void QTSModeEnded();
public slots:
	void updateSetup(AwDisplaySetup *setup, int flags);
	void executeCommand(int command, const QVariantList& args);
	void handleCommand(const QVariantMap& map);
	void synchronizeMappingCursorPos(float position);
	void addNewSignalView();
	void removeView();
	void alignViewsVerticaly();
	void alignViewsHorizontaly();
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
	/** remove ICA channels from views **/
	void removeICAChannels();
	void setChannels(const AwChannelList& montage);
	/** processHasFinished -
	recoit les messages de fin de process de tous les plugins de type Display actifs sur l'affichage courant **/ 
	void processHasFinished();
	void addMarkerModeChanged(bool on);
	void cursorModeChanged(bool on);
	void setQTSMode(bool on);
	void changeCurrentSetup(AwDisplaySetup *newSetup);
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