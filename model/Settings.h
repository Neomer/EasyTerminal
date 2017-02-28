#ifndef SETTINGS_H
#define SETTINGS_H

#include "IModel.h"
#include <QLocale>

class Settings : public QObject
{
	Q_OBJECT
	
	Q_PROPERTY(QString locale READ getLocaleName WRITE setLocaleName)
	
public:
	Settings();
	
	QLocale getLocale() const {	return m_locale; }
	QString getLocaleName() const {	return m_locale.name(); }
	
public slots:
	void setLocale(QLocale value);
	void setLocaleName(QString value);
	
private:
	void mapValues(QSqlQuery &query);
	bool updateRecord(QString id, QVariant value);
	
	QLocale m_locale;
};

#endif // SETTINGS_H
