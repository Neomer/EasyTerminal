#include "IModel.h"

IModel::IModel(QString tablename) :
	_tableName(tablename)
{
	_ready = false;
	_propertyListReady = false;
}

bool IModel::load(quint32 id)
{
	QSqlQuery q(QString("SELECT * FROM %1 WHERE id=%2;").arg(
						getTable(),
						QString::number(id)
					));
	
	if (!q.exec())
		return false;

	if (!q.first())
		return false;
	
	parseQuery(q);
	return true;
}

bool IModel::insert()
{
	if (!_propertyListReady)
	{
		updatePropertyList();
	}
	
	QSqlQuery q(QString("INSERT INTO %1 (%2) VALUES (%3);").arg(
					getTable(),
					_propertyList.join(','),
					prepareValues().join(',')
				));
	
	if (!q.exec())
		return false;
	
	return true;
}

bool IModel::update(quint32 id)
{
	if (!_propertyListReady)
	{
		updatePropertyList();
	}
	
	QStringList v = prepareValues();
	QString upd;
	for (int i = 0; i < _propertyList.count(); i++)
	{
		upd.append(QString("%1=%2,").arg(
					   _propertyList.at(i),
					   v.at(i)));
	}
	upd.left(upd.count() - 1);
	
	QSqlQuery q(QString("UPDATE %1 SET %2 WHERE id=%3;").arg(
					getTable(),
					upd,
					QString::number(id)
				));
	
	qDebug("%s", QString("UPDATE %1 SET %2 WHERE id=%3;").arg(
			   getTable(),
			   upd,
			   QString::number(id)
		   ).toLatin1().constData());
	
	if (!q.exec())
		return false;
	
	return true;
}

void IModel::parseQuery(QSqlQuery &q)
{
	for (int i = 0; i < this->metaObject()->propertyCount(); i++)
	{
		int idx = q.record().indexOf(this->metaObject()->property(i).name());
		if (idx >= 0)
		{
			this->metaObject()->property(i).write(this, q.value(idx));
		}
	}
	_ready = true;
}

void IModel::updatePropertyList()
{
	for (int i = 0; i < this->metaObject()->propertyCount(); i++)
	{
		if ((strcmp(this->metaObject()->property(i).name(), "objectName"))&&((strcmp(this->metaObject()->property(i).name(), "id"))))
		{
			_propertyList.append(this->metaObject()->property(i).name());
		}
	}
	_propertyListReady = true;
}

QStringList IModel::prepareValues()
{
	QStringList ret;
	
	for (int i = 0; i < this->metaObject()->propertyCount(); i++)
	{
		if ((strcmp(this->metaObject()->property(i).name(), "objectName"))&&((strcmp(this->metaObject()->property(i).name(), "id"))))
		{
			QVariant v = this->metaObject()->property(i).read(this);
			switch (v.type())
			{
				case QVariant::String:
					ret.append(QString("'%1'").arg(v.toString()));
					break;
				
				case QVariant::Bool: case QVariant::Int: case QVariant::UInt: case QVariant::LongLong: case QVariant::ULongLong:
					ret.append(QString::number(v.toLongLong()));
					break;
					
				case QVariant::UserType:
					ret.append(QString::number(v.toLongLong()));
					break;
				
				default:
					break;
			}

						
		}
	}
	
	return ret;
}
