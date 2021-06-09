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
#ifndef AW_GRAPHICS_OBJECTS_H
#define AW_GRAPHICS_OBJECTS_H
#include <AwGlobal.h>
#include <QDialog>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <widget/AwGraphicsDefines.h>
#include <AwChannel.h>
#include <AwMarker.h>
class QAction;
class AwViewSettings;

class AW_WIDGETS_EXPORT AwDisplayPhysics
{
public:
	AwDisplayPhysics();

	void setSecsPerCm(float secsPerCm);
	inline void setXPixPerSec(float xPixPerSec) { m_xPixPerSec = xPixPerSec; }
	inline void setPageDuration(float dur) { m_pageDuration = dur; }

	inline float xPixPerCm() { return m_xPixPerCm; }
	inline float yPixPerCm() { return m_yPixPerCm; }
	inline float xPixPerSec() { return m_xPixPerSec; }
	inline float pageDuration() { return m_pageDuration; }
	inline float secsPerCm() { return m_secsPerCm; }

protected:
	float m_xPixPerCm;
	float m_yPixPerCm;
	float m_xPixPerSec;
	float m_pageDuration;
	float m_secsPerCm;
};


class AW_WIDGETS_EXPORT AwGraphicsItem : public QObject
{
	Q_OBJECT
public:
	explicit AwGraphicsItem(AwDisplayPhysics *phys, AwViewSettings *settings);
	virtual QSize minimumSize() const { return QSize(0, 0); }	// override this method to specilfy the minimum size for a Graphics Item.
	enum { ItemHasUi = 1 };
	inline QObject *plugin() { return m_plugin; }
	inline void setPlugin(QObject *plugin) { m_plugin = plugin; }
	inline bool hasUi() { return m_flags & ItemHasUi; }
	inline void setPhysics(AwDisplayPhysics *phys) { m_physics = phys; }
	inline AwDisplayPhysics *physics() { return m_physics; }
	inline void setDisplayPhysics(AwDisplayPhysics *p) { m_physics = p; }

	inline QSize size() { return m_size; }	
	void setItemFlags(int flags) { m_flags |= flags; }
	inline int itemFlags() { return m_flags; }
	virtual void updateGeometry()  { } // override this method to compute new geometry for the item.
	// override this method to add custom actions for the item that will be displayed in the context menu.
	virtual QList<QAction *> customActions() { return QList<QAction *>(); }
	inline AwViewSettings* viewSettings() { return m_viewSettings; }
public slots:
	// executes Ui (if the object has ui)
	virtual int execUi() { return QDialog::Accepted; }

	void setNewSize(const QSize size);
signals:
	void sizeChanged();
	void filtersChanged();
	void selectionChanged(bool selected);
protected:
	AwDisplayPhysics *m_physics;
	int m_flags;
	QSize m_size;
	QObject *m_plugin;
	AwViewSettings* m_viewSettings; // a ref to the current view settings
};

/// AwGraphicsSignalItem
///
/// Base class for plugin that display signal type objects
class AW_WIDGETS_EXPORT AwBaseGraphicsSignalItem : public AwGraphicsItem
{
public:
	AwBaseGraphicsSignalItem(AwChannel *channel, AwViewSettings* settings, AwDisplayPhysics *phys );
	virtual int itemType() { return AW_BASE_GRAPHICS_ITEM_TYPE; }

	virtual void showLabel(bool flag) { m_label = flag; }
	inline bool isLabelVisible() { return m_label; }
	virtual void showBaseline(bool flag) { m_baseLine = flag;  }
	virtual AwChannel *channel() { return m_channel; }
	inline void setChannel(AwChannel *chan) { m_channel = chan; }
	// override this method to implement specific refresh for the item. 
	// That could be to repaint the item as new data have been set to.
	virtual void updateData() { 
		if (m_channel) 
			m_channel->setDataReady();
		m_repaint = true; }
	inline void endRepaint() { m_repaint = false; }
	inline bool needRepaint() { return m_repaint; }
	inline void shiftLabel(qreal pixels) { m_labelXOffset = pixels; }
	virtual QGraphicsItem *labelItem() = 0;
	virtual void setLabelHeight(int height) = 0;
protected:
	bool m_baseLine;
	bool m_label;
	AwChannel *m_channel;
	bool m_repaint;
	qreal m_labelXOffset;	// offset in pixels for the label

};

/// AwGraphicsSignalDialog
///
/// Base class for creating dialogs associated with AwGraphicsItem
class AW_WIDGETS_EXPORT AwGraphicsSignalDialog : public QDialog
{
public:
	AwGraphicsSignalDialog(AwBaseGraphicsSignalItem *item = NULL, QWidget *parent = 0, Qt::WindowFlags f = 0) : m_item(item), QDialog(parent, f) {}
	void setItem(AwBaseGraphicsSignalItem *item) { m_item = item; }
protected:
	AwBaseGraphicsSignalItem *m_item; // item is the AwGraphicsItem associated with Ui.
};

class AW_WIDGETS_EXPORT AwBaseGraphicsMarkerItem : public AwGraphicsItem
{
public:
	AwBaseGraphicsMarkerItem(AwMarker *marker, AwDisplayPhysics *phys);
	virtual int itemType() { return AW_GRAPHICS_ITEM_MARKER_TYPE; }
	inline AwMarker *marker() { return m_marker; }

protected:
	AwMarker *m_marker;
};

class AW_WIDGETS_EXPORT AwGraphicsCursorItem : public AwGraphicsItem, public QGraphicsRectItem
{
	Q_OBJECT
public:
	enum { Type = UserType + AW_GRAPHICS_ITEM_CURSOR_TYPE };
	inline int type() const { return Type; }
	AwGraphicsCursorItem(float posInFile, float cursorPosition, AwDisplayPhysics *phys = nullptr) : AwGraphicsItem(phys, nullptr)
	{ m_positionInFile = posInFile; m_currentPos = cursorPosition; this->setZValue(20); }
	inline float currentPos() { return m_currentPos; }
	/** Defines the absolute position in seconds for the cursor in the scene **/
	void setCurrentPos(float position);	
	void updatePosition();

public slots:
	virtual void setPositionInFile(float pos) { m_positionInFile = pos; }
protected:
	float m_positionInFile;	// position of the scene in file (not the cursor position). This is the offset position in seconds for cursor.
	float m_currentPos;		// position of cursor (absolute position in seconds in data).
};

class AW_WIDGETS_EXPORT AwGraphicsSignalItem : public AwBaseGraphicsSignalItem, public QGraphicsItem /* public QGraphicsPolygonItem */
{
	Q_OBJECT
public:
	enum { Type = UserType + AW_GRAPHICS_ITEM_SIGNAL_TYPE };
	int itemType() { return AW_GRAPHICS_ITEM_SIGNAL_TYPE; }
	inline int type() const { return Type; }
	AwGraphicsSignalItem(AwChannel *chan, AwViewSettings *settings, AwDisplayPhysics *phys = nullptr) : AwBaseGraphicsSignalItem(chan, settings, phys) 
	{
		setZValue(5); m_flags = 0; m_repaint = false; m_number = -1; 
	}

	/** Override this method for specific repaint code in subclass **/
	virtual void repaint() { 
		if (m_channel)
			m_channel->setDataReady(); 
		m_repaint = true; 
		update(); }
	void setDisplayFlags(int flags) { m_flags = flags; update(); }
	inline int displayFlags() { return m_flags; }
	virtual QRectF labelBoundingBox() { return QRectF(0, 0, 0, 0); }
	void setLabelXOffset(qreal offset);
	inline void setIndex(int index) { m_number = index; }
	inline int index() { return m_number; }
	QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) override;
signals:
	void selectionChanged(AwGraphicsSignalItem *item, bool selected);
protected:
	// override the QGraphicsItem mousePress method to suppress the default selecting scheme.
	void mousePressEvent(QGraphicsSceneMouseEvent *event) { }
	int m_flags;
	int m_number;	// hold the index of the item in the scene (to get the right order of the item in the scene)
};

Q_DECLARE_METATYPE(AwGraphicsSignalItem *)

class AW_WIDGETS_EXPORT AwGraphics2DPlotItem : public AwGraphicsSignalItem /*, public QGraphicsRectItem */
{
public:
	enum { Type = UserType + AW_GRAPHICS_ITEM_2DSIGNAL_TYPE };
	inline int type() const { return Type; }
	int itemType() { return AW_GRAPHICS_ITEM_2DSIGNAL_TYPE; }
	AwGraphics2DPlotItem(AwChannel *chan, AwDisplayPhysics *phys);
};

class AW_WIDGETS_EXPORT AwGraphicsMarkerItem : public AwBaseGraphicsMarkerItem, public QGraphicsRectItem
{
	Q_OBJECT
public:
	enum { Type = UserType + AW_GRAPHICS_ITEM_MARKER_TYPE };
	inline int type() const { return Type; }
	AwGraphicsMarkerItem(AwMarker *marker, AwDisplayPhysics *phys) : AwBaseGraphicsMarkerItem(marker, phys) 
	{ m_physics = phys; this->setZValue(10); setOpacity(1.0); }
};

class AW_WIDGETS_EXPORT AwGraphicsWidgetItem : public AwBaseGraphicsSignalItem, public QGraphicsProxyWidget
{
public:
	enum { Type = UserType + AW_GRAPHICS_ITEM_WIDGET_TYPE };
	inline int type() const { return Type; }
	AwGraphicsWidgetItem(AwChannel *channel, AwDisplayPhysics *phys, 
		QGraphicsItem *parent = NULL, Qt::WindowFlags wFlags = 0, QWidget *widget = NULL);
};
#endif // AW_GRAPHICS_OBJECTS_H
