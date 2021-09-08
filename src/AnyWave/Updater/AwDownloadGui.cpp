#include "AwDownloadGui.h"
#include <cmath>

AwDownloadGui::AwDownloadGui(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

}

AwDownloadGui::~AwDownloadGui()
{
}


void AwDownloadGui::setText(const QString& text)
{
	m_ui.label->setText(text);
}

void AwDownloadGui::updateDownloadProgress(qint64 received, qint64 total)
{
	m_ui.progressBar->setValue((int)std::floor(received * 100. / total));
}

