#include "SST.h"

SST::SST()
{
	pdi.addInputParameter(Aw::ProcessInput::SEEGChannels, QString("1-n"));
	pdi.addInputParameter(Aw::ProcessInput::ProcessIgnoresChannelSelection, QString("1-n"));
}

SST::~SST()
{
	delete m_settings;
	delete m_widget;
}

SSTPlugin::SSTPlugin()
{
	name = QString(tr("SEEG Spatio Temporal Representation"));
	description = QString(tr("Show SEEG pads activities"));
	type = AwProcessPlugin::GUI;
}


bool SST::showUi()
{
	// prepare labels
	QStringList labels;
	foreach(AwChannel *c, pdi.input.channels) {
		if (c->isSEEG()) {
			QRegularExpression exp("\\d+$");
			QString label = c->name();
			label.remove(exp);
			if (!labels.contains(label))
				labels.append(label);
		}
	}
	m_settings = new Settings(labels);
	if (m_settings->exec() == QDialog::Accepted) {
		foreach(AwChannel *c, pdi.input.channels) {
			if (c->name().startsWith(m_settings->label)) {
				c->clearRefName();
				m_channels << c;
			}
		}
		return true;
	}
	return false;
}
void SST::run()
{
	if (!showUi())
		return;
	m_widget = new SSTWidget(m_channels, pdi.input.reader()->infos.totalDuration());
	connectClient(m_widget->client());
	m_widget->show();
	m_widget->updateData();
}