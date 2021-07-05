#pragma once
#include <qfile.h>
#include <QDataStream>
#include <vtkPolyDataAlgorithm.h>

class vtkPialReader : public vtkPolyDataAlgorithm
{
public:
	vtkTypeMacro(vtkPialReader, vtkPolyDataAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent) override;

	static vtkPialReader* New();
	void SetFileName(const char* name)
	{ 
		FileName = name; 
		if (m_file.isOpen()) 
			m_file.close(); 
	}
protected:
	vtkPialReader();
	~vtkPialReader();

	int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
private:
	int read3bytes();
	QFile m_file;
	QDataStream m_stream;
	const char* FileName;
};