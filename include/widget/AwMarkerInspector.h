#ifndef AWDMARKERINSPECTOR_H
#define AWDMARKERINSPECTOR_H
#include <AwGlobal.h>
#include <QWidget>
namespace Ui {class AwMarkerInspectorUI; };
#include <AwChannel.h>
#include <AwMarker.h>
#include <AwMarkingSettings.h>
#include <QCloseEvent>
#include <memory>


class AW_WIDGETS_EXPORT AwMarkerInspector : public QWidget
{
	Q_OBJECT

public:
	AwMarkerInspector(const AwMarkerList& markers = AwMarkerList(), const QStringList& targets = QStringList(), QWidget *parent = 0);
	~AwMarkerInspector();

	void setSingleOnly(bool flag = true);
	inline AwMarkingSettings& settings() { return m_settings; }
	void setPredefinedMarkers(const AwMarkerList& markers);
	AwMarkerList& predefinedMarkers();
public slots:
	void setMarkers(const AwMarkerList& markers);
	void setTargets(const QStringList& targets);
	void setTargets(const AwChannelList& channels);
protected:
	void bindOnLabel(const QString& label);
	void bindOnValue(float value);
	void closeEvent(QCloseEvent *event) { emit closed(); event->accept(); }
	void changeEvent(QEvent*);
signals:
	void settingsChanged(AwMarkingSettings *settings);
	void closed();
	void predefinedMarkersChanged(const AwMarkerList& markers);
protected slots:
	void changeMarkerType(bool flag);
	void changeLabel(const QString& text);
	void changeValue(double value);
	void changeAutoInc(bool flag);
	void changeUsePredefined(bool flag);
	void changeTargetChannels(bool flag);
	void changeUseShortcut(bool flag);
	void addPredefinedMarker();
	void clearPredefinedMarkers();
	void removeSelectedPredefinedMarkers();
	void addTargets();
	void clearTargets();
	//void changeBinding(bool on);
	//void changeKeyBinding();
	//void changeComboBinding();
	void changeColor();

private:
	Ui::AwMarkerInspectorUI *m_ui;
	AwMarkingSettings m_settings;
	AwMarkerList m_markers;
	QStringList m_targetedChannels;
	QStringList m_targets;
	QStringList m_markerLabels;
	QStringList m_markerValues;
	QString m_profilePath;		// path to predefined markers profiles. If empty, no handling of profiles.
	bool m_onlySingle;
};

#endif // AWADDMARKERINSPECTOR_H
