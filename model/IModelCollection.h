#ifndef IMODELLIST_H
#define IMODELLIST_H

#include <QObject>

class IModelCollection : public QObject
{
	Q_OBJECT
public:
	explicit IModelCollection(QObject *parent = 0);
	
	void load();
	
signals:
	
public slots:
};

#endif // IMODELLIST_H
