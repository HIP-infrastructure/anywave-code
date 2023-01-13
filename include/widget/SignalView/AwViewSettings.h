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
#ifndef AWVIEWSETTINGS_H
#define AWVIEWSETTINGS_H
#include <AwGlobal.h>
#include <QObject>
#include <AwGainLevels.h>
#include <QVariantMap>
#include <widget/AwGraphicsObjects.h>

namespace aw {
	namespace view_settings {
		constexpr auto marker_visibility = 0;
		constexpr auto show_markers = 1;
		constexpr auto max_visible_channels = 2;
		constexpr auto limit_channel_per_view = 3;
		constexpr auto show_zero_line = 4;
		constexpr auto show_time_grid = 5;
		constexpr auto time_scale = 6;  // type of time scale: PaperLike or Fixed page duration
		constexpr auto time_representation = 7;
		constexpr auto show_seconds = 8;
		constexpr auto show_sensors = 9;
		constexpr auto stack_channels = 10;
		constexpr auto marker_bar_mode = 11;
		constexpr auto show_marker_bar = 12;
		constexpr auto channel_selection = 13;
		constexpr auto secs_per_cm = 14;
		constexpr auto fixed_page_duration = 15;
		constexpr auto old_eeg_mode = 16;  // display amplitudes upside down for EEG channels
		constexpr auto show_amplitude_scales = 17;
		// settings relative to length of data 
		constexpr auto file_duration = 18;
		constexpr auto pos_in_file = 19;
		constexpr auto page_duration = 20;
		constexpr auto start_position = 21; // zero in normal usage, but can be set to an offset position in data.
		// extra settings relative to signal view options
		constexpr auto show_all_channels = 22;  // force filter for the view to show all modalities.
		constexpr auto hide_nav_bar = 23;
		constexpr auto no_settings_button = 24;
		constexpr auto enable_marking = 25;
		constexpr auto hide_gain_levels = 26;
		constexpr auto hide_nav_buttons = 27;
		constexpr auto channels_modalities = 28;
		

		// sender objects id  (used when settings object is updated)
		constexpr int sender_signalview = 0;
		constexpr int sender_scene = 1;
		constexpr int sender_marker_bar = 2;
		constexpr int sender_nav_bar = 3;
		constexpr int sender_view = 4;
		constexpr int sender_global = -1;
	}
}

class AW_WIDGETS_EXPORT AwViewSettings : public QObject
{
	Q_OBJECT
public:
	AwViewSettings(QObject *parent = nullptr);
	enum TimeScale { PaperLike, FixedPageDuration };
	enum TimeRepresentation { ShowRelativeTime, ShowRecordedTime };
	enum MarkerBarMode { Global, Classic };
	enum MarkerVisibility { ShowLabel, ShowValue, ShowBoth, HideBoth };

	QList<int> filters;
	QStringList channelSelection;
	AwGainLevels *gainLevels;
	bool showAmplScales;
	bool showMarkers;
	bool limitChannelPerView;
	bool showZeroLine;
	bool showTimeGrid;
	bool showSeconds;   // in time grid
	bool showSensors;	// on channels
	bool showMarkerBar;
	bool stackChannels;
	bool oldEEGMode;
	bool showAllChannels; // force the view to display all channels modalities
	bool hideNavBar;
	bool enableMarking;
	int maxVisibleChannels;
	int timeScale;
	int timeRepresentation; // Relative or Recorded
	int markerBarMode;  // Global of Classic
	int markerViewOptions; //  Show label, value, both or none
	float fixedPageDuration;
	float pageDuration;
	float posInFile;
	float fileDuration;
	float startPosition;

	// special flags to customize the signal view
	bool showSettingsButton;
	bool showAddMarkerButton;
	bool showFilterButton;
	bool showChannelsInfoNavBar;
	bool showNavBar;
	AwDisplayPhysics physics;
	void setSecsPerCm(float v);
	inline float secsPerCm() { return m_secsPerCm; }

protected:
	float m_secsPerCm;
};

#endif