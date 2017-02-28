#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include "model/Settings.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(qApp->applicationDirPath().append("/db.sqlite"));
	if (!db.open())
	{
		qDebug("Database not found!");
		return 0;
	}

	MainWindow w;
	Settings s;
	w.installTranslator(s.getLocale());
	
	
	a.installEventFilter(&w);
	w.show();
	
	return a.exec();
}
