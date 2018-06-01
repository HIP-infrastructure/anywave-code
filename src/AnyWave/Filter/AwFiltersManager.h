#pragma once
#include <QObject>
#include <AwFilteringOptions.h>

class AwFilterSettings;

class AwFiltersManager : public QObject
{
	Q_OBJECT
public:
	static AwFiltersManager * instance();
	void closeFile();
	void quit();

	inline AwFilteringOptions& fo() { return m_fo; }
	void update();
	void showUi();
	AwFilterSettings *ui() { return m_ui; }
	void setSourceSettings(int type, float hp, float lp);
	void setICASettings(int type, float hp, float lp);
	/** Turn on/off filters **/
	void switchFilters(bool flag); 
public slots:
	/* called each time a new file is open */
	void setFilename(const QString& path); 
signals:
	void filtersChanged(AwFilteringOptions *fo);
protected:
	void load();
	void save();
	void reset();

	AwFilterSettings *m_ui;
	AwFilteringOptions m_fo, m_copiedFo;
	QString m_filePath;	// path to .flt file.
	// store source or ICA filter settings to warn the user when current filters are over these settings
	QMap<int, QPair<float, float> > m_sourceSettings;
	QMap<int, QPair<float, float> > m_icaSettings;

	static AwFiltersManager *m_instance;
	AwFiltersManager(QObject *parent = 0);
};