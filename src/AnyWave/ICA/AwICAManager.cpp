#include "AwICAManager.h"
#include <hdf5/AwHDF5.h>
#include <AwMATLAB.h>
#include "AwICAComponents.h"
#include <widget/AwMessageBox.h>
#include "Prefs/AwSettings.h"
#include <QFileInfo>

// statics init and definitions
AwICAManager *AwICAManager::m_instance = 0;

AwICAManager *AwICAManager::instance()
{
	if (!m_instance) {
		m_instance = new AwICAManager;
	}
	return m_instance;
}

AwICAManager::AwICAManager(QObject *parent)
	: QObject(parent)
{
	for (int i = 0; i < AW_CHANNEL_TYPES; i++)
		m_comps[i] = NULL;
	m_isFilteringOn = false;	// no ICA filtering by default
}

AwICAManager::~AwICAManager()
{
}

void AwICAManager::closeFile()
{
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		if (m_comps[i])
			delete m_comps[i];
		m_comps[i] = NULL;
	}
}

void AwICAManager::turnICAOff()
{
	emit icaComponentsUnloaded();

	m_isFilteringOn = false;
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		if (m_comps[i]) {
			delete m_comps[i];
			m_comps[i] = NULL;
		}
	}
}

bool AwICAManager::reject(int type)
{
	if (!m_isFilteringOn)
		return false;

	if (m_comps[type])
		return m_comps[type]->isRejectionNeeded();
	return false;
}

void AwICAManager::rejectComponents(int type, const AwChannelList& channels)
{
	m_comps[type]->buildChannels(channels);
}

bool AwICAManager::containsComponents(int type)
{
	if (m_comps[type])
		if (m_comps[type]->components().size())
			return true;
	return false;
}


//
// returns the total number of components (MEG, EEG, EMG)
//
int AwICAManager::numberOfComponents()
{
	int count = 0;
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		if (m_comps[i])
			count += m_comps[i]->components().size();
	}
	return count;
}

int AwICAManager::loadComponents(const QString& icaFile)
{
	int row, col;
	// turn previously ICA components to off
	turnICAOff();
	QString inputFile = icaFile;

	// check for 'old' HDF5 format
	if (inputFile.endsWith(".h5"))
		inputFile = convertToMatlab(inputFile);
	
	if (inputFile.isEmpty()) {
		AwMessageBox::critical(0, tr("ICA Components"), "Failed to convert to .mat");
		return - 1;
	}
	AwMATLABFile file;
	if (file.open(inputFile) != 0) {
		AwMessageBox::critical(0, tr("ICA Components"), file.error());
		return -1;
	}
	QString modality;
	if (file.readString("modality", modality) != 0) {
		AwMessageBox::critical(0, tr("ICA Components"), file.error());
		return -1;
	}
	int type = AwChannel::stringToType(modality);
	AwICAComponents *comp = new AwICAComponents(type);
	try {
		comp->loadComponents(file);
	}
	catch (const AwException& e) {
		AwMessageBox::critical(0, tr("ICA Components"), e.errorString());
		delete comp;
		return -1;
	}
//	if (comp->loadComponents(file) == 0) {
		// delete previous components if any
		if (m_comps[type])
			delete m_comps[type];
		m_comps[type] = comp;
		AwSettings::getInstance()->currentIcaFile = icaFile;
		emit componentsFiltersLoaded(comp->lpFilter(), comp->hpFilter());
//	}
	//else {
	//	AwMessageBox::critical(0, tr("ICA Components"), QString("Unable to load %1 components.").arg(modality));
	//	delete comp;
	//	return -1;
	//}
	emit componentsLoaded();
	
	return 0;
}


AwICAComponents *AwICAManager::getComponents(int type)
{
	return m_comps[type];
}

AwICAComponents **AwICAManager::getAllComponents()
{
	return m_comps;
}

///
/// convert the old HDF5 file to the new .mat format and return the path to the converted matlab file.
/// gain are also converted as HDF5 ICA was computed with MEG values in T and MATLAB ICA uses pT.
QString AwICAManager::convertToMatlab(const QString& fileName)
{
	AwHDF5 file(fileName);
	
	if (file.open(AwHDF5::Read, "/") != AwHDF5::Ok)
		return QString();

	int modality;
	if (file.isGroup("ica/MEG")) {
		file.setGroup("ica/MEG");
		modality = AwChannel::MEG;
	}
	else if (file.isGroup("ica/EEG")) {
		file.setGroup("ica/EEG");
		modality = AwChannel::EEG;
	}
	else if (file.isGroup("ica/EMG")) {
		file.setGroup("ica/EMG");
		modality = AwChannel::EMG;
	}
	else if (file.isGroup("ica/Source")) {
		file.setGroup("ica/Source");
		modality = AwChannel::Source;
	}

	float lf, hf;
	if (file.readFloatAttr("LPF", &lf) != AwHDF5::Ok)
		return QString();
	if (file.readFloatAttr("HPF", &hf) != AwHDF5::Ok)
		return QString();

	int row, col;
	double *tmp = file.readDoubleMatrix("unmixing", &row, &col);
	if (tmp == NULL)
		return QString();
	// data read are row major => fill a col major matrix
	mat unmix(tmp, col, row);
	unmix = unmix.t();
	if (modality == AwChannel::MEG)
		unmix *= 1e-12;
	tmp = file.readDoubleMatrix("mixing", &row, &col);
	if (tmp == NULL)
		return QString();

	mat mix(tmp, col, row);
	mix = mix.t();
	if (modality == AwChannel::MEG)
		mix *= 1e-12;
	QString labels = file.readString("labels");

	if (labels.isEmpty())
		return QString();

	QStringList names = labels.split(",");
	QFileInfo fi(fileName);
	QString path = fi.absolutePath();


	QString newFile = QString("%1/ica_%2.mat").arg(path).arg(AwChannel::typeToString(modality));
	AwMATLABFile matlabFile;
	if (matlabFile.create(newFile) != 0) 
		return QString();
	
	if (matlabFile.writeString(QString("modality"), AwChannel::typeToString(modality)) != 0) {
		return QString();
	}

	if (matlabFile.writeScalar(QString("lpf"), (double)lf) != 0) {
		return QString();
	}
	if (matlabFile.writeScalar(QString("hpf"), (double)hf) != 0) {
		return QString();
	}
	if (matlabFile.writeScalar(QString("sr"), (double)-1) != 0) {
		return QString();
	}
	if (matlabFile.writeMatrix(QString("mixing"), mix) != 0) {
		return QString();
	}
	if (matlabFile.writeMatrix(QString("unmixing"), unmix) != 0) {
		return QString();
	}
	if (matlabFile.writeStringCellArray(QString("labels"), names) != 0) {
		return QString();
	}
	return newFile;
}