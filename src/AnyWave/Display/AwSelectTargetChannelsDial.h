#pragma once
#include <qdialog.h>
#include <AwMarker.h>
#include <AwChannel.h>
class AwGraphicsScene;
class AwBaseSignalView;

class AwSelectTargetChannelsDial : public QDialog
{
	Q_OBJECT
public:
	AwSelectTargetChannelsDial(const AwSharedMarker& marker, const AwChannelList& channels, AwGraphicsScene *scene, QWidget* parent = nullptr);
	QStringList selectedChannels;
protected:
	AwSharedMarker m_marker;
	AwGraphicsScene* m_scene;
	AwBaseSignalView* m_signalView;
public slots:
	int exec() override;
	void accept() override;

};