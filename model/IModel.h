#ifndef IMODEL_H
#define IMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMetaProperty>
#include <QMap>
#include <QDateTime>

class IModel : public QObject
{
	Q_OBJECT
	
public:
	IModel(QString tablename);
	
	QString getTable() { return _tableName; }
	
	bool load(QVariant id);
	bool load(QMap<QString, QVariant> filter);
	
	bool insert();
	bool update(quint32 id);
	bool remove(quint32 id);
	
	bool isReady() { return _ready; }
	
private:
	void parseQuery(QSqlQuery &q);
	void updatePropertyList();
	QStringList prepareValues();
	QString formatValue(QVariant value);
	
	QStringList _propertyList;
	bool _propertyListReady;
	
	QString _tableName;
	bool _ready;
};

#endif // IMODEL_H
