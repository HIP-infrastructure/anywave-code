#include "vtkPialReader.h"
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkCellArray.h>
#include <QDataStream>
#include <QString>

vtkStandardNewMacro(vtkPialReader);

vtkPialReader::vtkPialReader()
{
	this->FileName = nullptr;
	this->SetNumberOfInputPorts(0);
	this->SetNumberOfOutputPorts(1);
}

vtkPialReader::~vtkPialReader()
{
	//if (this->FileName)
	//	delete[] this->FileName;
}

int vtkPialReader::RequestData(vtkInformation* vtkNotUsed(request),
	vtkInformationVector** vtkNotUsed(inputVector),
	vtkInformationVector* outputVector)
{
	try {
		// Get the info object
		vtkInformation* outInfo = outputVector->GetInformationObject(0);
		// Get the output
		vtkPolyData* output = vtkPolyData::SafeDownCast(
			outInfo->Get(vtkDataObject::DATA_OBJECT()));

		// read the file
		m_file.setFileName(QString::fromUtf8(FileName));
		m_stream.setDevice(&m_file);
		m_stream.setVersion(QDataStream::Qt_4_4);
		constexpr int NEW_QUAD_FILE_MAGIC_NUMBER = 16777213;
		constexpr int TRIANGLE_FILE_MAGIC_NUMBER = 16777214;
		constexpr int QUAD_FILE_MAGIC_NUMBER = 16777215;
		if (!m_file.open(QIODevice::ReadOnly)) {
			vtkErrorMacro("Cannot open file " << this->FileName);
			return 0;
		}
		int magic = read3bytes();
		int nPoints = 0;
		int nQuad = 0;
		int nTriangles = 0;
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
		if (magic == QUAD_FILE_MAGIC_NUMBER || magic == NEW_QUAD_FILE_MAGIC_NUMBER) {
			nPoints = read3bytes();
			nQuad = read3bytes();
			points->SetNumberOfPoints(nPoints);
			if (magic == QUAD_FILE_MAGIC_NUMBER) {
				qint16 xyz[3];
				int count = 0;
				for (int i = 0; i < nPoints ; i++) {
					m_stream >> xyz[0] >> xyz[1] >> xyz[2];
					points->SetPoint(i, xyz[0] / 100., xyz[1] / 100., xyz[2] / 100.);
				}
			}
			else {
				float xyz[3];
				for (int i = 0; i < nPoints; i++) {
					m_stream >> xyz[0] >> xyz[1] >> xyz[2];
					points->SetPoint(i, xyz[0], xyz[1], xyz[2]);
				}
			}
			// read cells (quads)
			for (int i = 0; i < nQuad; i++) {
				vtkIdType indexes[4];
				for (int n = 0; n < 4; n++) 
					indexes[n] = read3bytes();
				vertices->InsertNextCell(4, indexes);
			}
			output->SetPoints(points);
			output->SetPolys(vertices);

			m_file.close();
			return VTK_OK;
		}
		else if (magic == TRIANGLE_FILE_MAGIC_NUMBER) {
			m_file.readLine();
			m_file.readLine();
			m_stream >> nPoints;
			m_stream >> nTriangles;
			if (std::floor(std::abs(nPoints)) != nPoints || std::floor(std::abs(nTriangles)) != nTriangles) {
				m_file.reset();
				read3bytes();
				m_file.readLine();
				m_stream >> nPoints >> nTriangles;
			}
			float xyz[3];
			points->SetNumberOfPoints(nPoints);
			for (int i = 0; i < nPoints; i++) {
				m_stream >> xyz[0] >> xyz[1] >> xyz[2];
				points->SetPoint(i, xyz[0], xyz[1], xyz[2]);
			}
			// read cells (triangles)
			int indexes[3];
			for (int i = 0; i < nTriangles; i++) {
				vertices->InsertNextCell(3);
				m_stream >> indexes[0] >> indexes[1] >> indexes[2];
				vertices->InsertCellPoint(indexes[0]);
				vertices->InsertCellPoint(indexes[1]);
				vertices->InsertCellPoint(indexes[2]);
			}
			output->SetPoints(points);
			output->SetPolys(vertices);
			m_file.close();
			return VTK_OK;
		}
		else {
			vtkErrorMacro("Unknown magic number " << magic);
			return 0;
		}
	}
	catch (const std::exception& e)
	{
		vtkErrorMacro("exception: " << e.what());
	}
	catch (...)
	{
		vtkErrorMacro("Unknown exception");
	}
	return 0;
}

int vtkPialReader::read3bytes()
{
	unsigned char bytes[3];
	m_stream >> bytes[0] >> bytes[1] >> bytes[2];
	int res = (bytes[0] <<16) + (bytes[1] << 8) + bytes[2];
	return res;
}


void vtkPialReader::PrintSelf(ostream& os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
	os << "vtkPialReader" << std::endl;
	os << "Filename: " << this->FileName << std::endl;
}
