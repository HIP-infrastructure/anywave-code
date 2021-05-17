#ifndef AW_TOPOLABELIMAGE_H
#define AW_TOPOLABELIMAGE_H
#include <QLabel>
class QwtMatrixRasterData;

class AwTopoLabelImage : public QLabel
{
	Q_OBJECT
public:
	AwTopoLabelImage(QWidget *parent = 0, Qt::WindowFlags = 0);
	inline void setData(QwtMatrixRasterData *data) { m_data = data; }
	void setPadSize(float pad, float size);
signals:
	void positionChanged(double value);
	void clickedAt(double x, double y);
protected:
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void computeValueAtPosition(const QPointF& pos, double *head_pos);

	float m_pad, m_head_size;
	QwtMatrixRasterData *m_data;	// pointer to current matrix containing interpolated values.
	QRegion m_region;	// region used to map mouse position in the head display.
};

#endif