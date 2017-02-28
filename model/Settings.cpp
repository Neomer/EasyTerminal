#include "Settings.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMetaObject>
#include <QMetaProperty>


Settings::Settings()
{
	QSqlQuery q = QSqlDatabase::database().exec("select * from settings;");
	
	mapValues(q);
}

void Settings::setLocaleName(QString value)
{
	m_locale = QLocale(value);
	updateRecord("locale", value);	
}

void Settings::setLocale(QLocale value)	
{
	m_locale = value; 
}

void Settings::mapValues(QSqlQuery &q)
{
	if (!q.first())
		return;
	
	do
	{
		QString field = q.value(0).toString();

		for (int i = 0; i < this->metaObject()->propertyCount(); i++)
		{
			qDebug("%s %s", this->metaObject()->property(i).name(),
				   field.toUtf8().constData());
			
			if (this->metaObject()->property(i).name() == field)
			{
				if (this->metaObject()->property(i).write(this, q.value(1)))
				{
					qDebug("Failed! Property: %s Value %s",
						   this->metaObject()->property(i).name(),
						   q.value(1).toString().toLatin1().constData());
				}
			}
			
		}
		
	}
	while (q.next());
	
}

bool Settings::updateRecord(QString id, QVariant value)
{
	QSqlQuery q;
	
	if (!q.prepare("update settings set value=:value where id=:id;"))
		return false;
	
	q.bindValue(":value", value);
	q.bindValue(":id", id);
	
	if (!q.exec())
		return false;
	
	return true;
}


