#ifndef LAYERPROPERTYODF_H
#define LAYERPROPERTYODF_H

#include "LayerPropertyMRI.h"
#include "vtkSmartPointer.h"
#include <QMap>

class vtkColorTransferFunction;

class LayerPropertyODF : public LayerPropertyMRI
{
  Q_OBJECT
public:
  explicit LayerPropertyODF(QObject *parent = 0);

  int GetOdfSkip()
  {
    return m_nOdfSkip;
  }

  int GetOdfInversion()
  {
    return m_nOdfInversion;
  }

  double GetOdfScale()
  {
    return m_dOdfScale;
  }

  int GetOdfColorCode()
  {
    return m_nOdfColorCode;
  }

  void GetMagnitudeThreshold(double* th);

  void SetMagnitudeThreshold(double* th);

  vtkColorTransferFunction* GetOdfLut();

  enum ODF_COLOR { Directional = 0, Magnitude };

signals:
  void OdfPropertyChanged();
  void ColorCodeChanged();

public slots:
  void SetOdfSkip(int n);
  void SetOdfScale(double val);
  void SetOdfInversion(int n);
  void SetOdfColorCode(int n);

protected:
  void UpdateOdfLut();

  int     m_nOdfSkip;
  double  m_dOdfScale;
  int     m_nOdfInversion;
  int     m_nOdfColorCode;
  double  m_dMagnitudeThreshold[2];
  vtkSmartPointer<vtkColorTransferFunction> m_odfLut;
  QMap<double, QColor> m_spectrum;
};

#endif // LAYERPROPERTYODF_H
