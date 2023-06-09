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
#ifndef AWSIGNALVIEW_H
#define AWSIGNALVIEW_H

class QWidget;
class AwDisplaySetup;
class AwViewSettings;
class AwViewSetup;
#include <widget/SignalView/AwBaseSignalView.h>
#include <filter/AwFilterSettings.h>

class AwSignalView : public AwBaseSignalView
{
	Q_OBJECT

public:
	AwSignalView(AwViewSettings *settings, int flags = 0, QWidget *parent = 0, Qt::WindowFlags windowFlags = Qt::WindowFlags());
	~AwSignalView();

	enum Flags { NoProcessUpdate = 1, UpdateProcess = 2 };
	inline int processFlags() { return m_processFlags; }
	void setCursorPosition(float pos);
	void enableView();
	void closeFile();
	
	inline void setProcessFlags(int flags) { m_processFlags = flags; }
	void updateChannels();	// apply gains and redisplay channels
	void removeHighLigthMarker();
	void displaySelectedChannelsOnly();
	void setChannels(const AwChannelList&) override;
signals:
	void markersGUIRequested();
	void displayedChannelsUpdated(AwChannelList& channels);	// sent each time the view receives new data to display.
	void badChannelSet(const QString& label);
public slots:
	void reloadData() override;		// request a reloading for current channels in the view.
	void refresh();					// check if the view needs to reload data and refresh current display.
	void updatePositionInFile(float pos);
	void synchronizeOnPosition(float position);
	void showPosition(float position);
	void centerViewOnPosition(float position);
	void updatePageDuration(float duration);
	void goToPos(int pos);	// called when position in file has changed using the scrollbar in the navigation bar.
	void selectChannelsFromLabels(const QStringList& labels);
	void addVirtualChannels(AwChannelList& channels);
	void removeVirtualChannels(AwChannelList& channels);
	void removeICAChannels(); // only remove ICA channels if the user cancels
	/** Toggles the displays of the maping over the channel **/
	void showICAMaps(bool flag);
protected:
	// overriden methods
	void dataReceived() override;
	void applyChannelFilters();
private:
	void updateAmplitudes();
	AwChannelList m_virtualChannels;
	int m_processFlags;							// flags for the view
	bool m_isActive;
};

#endif // AWSIGNALVIEW_H
