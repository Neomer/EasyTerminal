#ifndef IMODEL_H
#define IMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMetaProperty>

class IModel : public QObject
{
	Q_OBJECT
	
public:
	IModel(QString tablename);
	
	QString getTable() { return _tableName; }
	
	bool load(quint32 id);
	bool insert();
	bool update(quint32 id);
	
	bool isReady() { return _ready; }
	
private:
	void parseQuery(QSqlQuery &q);
	void updatePropertyList();
	QStringList prepareValues();
	
	QStringList _propertyList;
	bool _propertyListReady;
	
	QString _tableName;
	bool _ready;
};

#endif // IMODEL_H
