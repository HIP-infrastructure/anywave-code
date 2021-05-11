#include "AwMATPyCreator.h"
#include <AwProcess.h>
#include <QMessageBox>
#include "Prefs/AwSettings.h"
#include <QDir>
#include <QTextStream>

AwMATPyCreator::AwMATPyCreator(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
}

AwMATPyCreator::~AwMATPyCreator()
{
}


void AwMATPyCreator::accept()
{
	QString name, description;
	name = m_ui.lineEditName->text();
	description = m_ui.lineEditDesc->text();
	if (name.isEmpty() || description.isEmpty()) {
		QMessageBox::warning(this, "Parameters", "Please fill in name and description.");
		return;
	}
	QVariantMap settings;
	settings["name"] = name;
	settings["desc"] = description;
	settings["type"] = QString("Python");
	if (m_ui.radioMATLAB->isChecked())
		settings["type"] = "MATLAB";

	int status = makePlugin(settings);
	if (status == AwMATPyCreator::FailedtoCreate) {
		QMessageBox::critical(this, "Creating plugin", "Failed to create the folder for plugin.");
		return;
	}
	if (status == AwMATPyCreator::AlreadyExists) {
		QMessageBox::critical(this, "Creating plugin", "A plugin with this name already exists.");
		return;
	}
	QDialog::accept();
}

int AwMATPyCreator::makePlugin(const QVariantMap& settings)
{
	auto aws = AwSettings::getInstance();
	auto matlabPluginDir = aws->value(aws::matlab_plugins_dir).toString();
	auto pythonPluginDir = aws->value(aws::python_plugins_dir).toString();
	QDir dir;
	bool isMatlab = false;
	if (settings.value("type").toString() == "MATLAB") {
		dir.setCurrent(matlabPluginDir);
		isMatlab = true;
	}
	else
		dir.setCurrent(pythonPluginDir);
	auto folders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	QString name = settings.value("name").toString();
	for (const auto& f : folders) {
		if (f.compare(name, Qt::CaseInsensitive) == 0)
			return AwMATPyCreator::AlreadyExists;
	}
	dir.mkdir(name);
	dir.cd(name);

	QString descPath = QString("%1/%2").arg(dir.absolutePath()).arg("desc.txt");
	QFile file(descPath);
	QTextStream stream(&file);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		stream << "name= " << name << endl;
		stream << "description= " << settings["desc"].toString() << endl;
		stream << "input_flags=";
		// input flags
		if (m_ui.radioGetMontage->isChecked())
			stream << "getcurrentmontage" << endl;
		else
			stream << "getasrecordedchannels" << endl;
		stream << "flags=";
		QStringList flags;
		if (m_ui.checkNoData->isChecked())
			flags  << "nodatarequired";
		if (m_ui.checkCommandLine->isChecked())
			flags << "canrunfromcommandline";
		for (int i = 0; i < flags.size(); i++) {
			if (i > 0)
				stream << ":";
			stream << flags.at(i);
		}
		stream << endl;
		stream << "modifiers_flags=";
		if (m_ui.radioAcceptSelection->isChecked())
			stream << "acceptchannelselection" << endl;
		if (m_ui.radioRequireSelection->isChecked())
			stream << "processrequireschannelselection" << endl;
		if (m_ui.radioIgnoreSelection->isChecked())
			stream << "processignoreschannelselection" << endl;

	//	stream << "input_flags=";
		file.close();
	}
	else
		return AwMATPyCreator::FailedtoCreate;
	if (isMatlab) {
		file.setFileName(QString("%1/%2").arg(dir.absolutePath()).arg("main.m"));
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			stream << "function main(varargin)" << endl;
			stream << "anywave('init', varargin);" << endl;
			stream << "end" << endl;
			file.close();
		} 
	}
	else {
		file.setFileName(QString("%1/%2").arg(dir.absolutePath()).arg("__main__.py"));
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			stream << "import sys" << endl;
			stream << "import anywave" << endl;
			stream << "anywave.init(sys.argv)" << endl;
			file.close();
		}
	}
	return AwMATPyCreator::Ok;
}