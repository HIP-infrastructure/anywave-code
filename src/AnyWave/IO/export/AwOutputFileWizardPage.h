#pragma once
#include <QWizardPage>
#include <QMap>
#include <QLineEdit>

class AwFileIOPlugin;

class AwOutputFileWizardPage : public QWizardPage
{
	Q_OBJECT
public:
	AwOutputFileWizardPage(QWidget* parent = nullptr);
	bool validatePage() override;

	AwFileIOPlugin* m_writerPlugin;
	QMap<QString, AwFileIOPlugin*> m_writersMap;
	QString filePath;
	QString writerName;
public slots:
	void browseOutputFile();
protected:
	QLineEdit *m_filePath;
	QComboBox* m_comboWriters;
	QStringList m_extensions;
};