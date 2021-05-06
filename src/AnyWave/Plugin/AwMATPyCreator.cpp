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

	//// associate flags with radio buttons
	//m_ui.radioAcceptSelection->setProperty("flag", Aw::ProcessIO::modifiers::AcceptChannelSelection);
	//m_buttons << m_ui.radioAcceptSelection;
	//m_ui.radioRequireSelection->setProperty("flag", Aw::ProcessIO::modifiers::RequireChannelSelection);
	//m_buttons << m_ui.radioRequireSelection;
	//m_ui.radioIgnoreSelection->setProperty("flag", Aw::ProcessIO::modifiers::IgnoreChannelSelection);
	//m_buttons << m_ui.radioIgnoreSelection;
	//m_ui.radioGetMontage->setProperty("flag", Aw::ProcessIO::GetCurrentMontage);
	//m_buttons << m_ui.radioGetMontage;
	//m_ui.radioGetRaw->setProperty("flag", Aw::ProcessIO::GetAsRecordedChannels);
	//m_buttons << m_ui.radioGetRaw;
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
	int mod_flags = 0;
	if (m_ui.radioAcceptSelection->isChecked())
		mod_flags |= Aw::ProcessIO::modifiers::AcceptChannelSelection;
	if (m_ui.radioRequireSelection->isChecked())
		mod_flags |= Aw::ProcessIO::modifiers::RequireChannelSelection;
	if (m_ui.radioIgnoreSelection->isChecked())
		mod_flags |= Aw::ProcessIO::modifiers::IgnoreChannelSelection;
	settings["mod_flags"] = mod_flags;
	int input_flags = 0;
	if (m_ui.radioGetMontage->isChecked())
		input_flags |= Aw::ProcessIO::GetCurrentMontage;
	if (m_ui.radioGetRaw->isChecked())
		input_flags |= Aw::ProcessIO::GetAsRecordedChannels;

	settings["input_flags"] = input_flags;


	settings["type"] = QString("Python");
	if (m_ui.radioMATLAB->isChecked())
		settings["type"] = "MATLAB";
	int flags = 0;
	if (m_ui.checkNoData->isChecked())
		flags |= Aw::ProcessFlags::ProcessDoesntRequireData;

	if (m_ui.checkCommandLine->isChecked())
		flags |= Aw::ProcessFlags::CanRunFromCommandLine;

	settings["flags"] = flags;
	makePlugin(settings);
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
	QString name = settings["name"].toString();
	for (const auto& f : folders) {
		if (f.compare(name, Qt::CaseInsensitive))
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
	return AwMATPyCreator::Ok;
}